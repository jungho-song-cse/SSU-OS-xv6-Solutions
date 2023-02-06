4250 #include "types.h"
4251 #include "defs.h"
4252 #include "param.h"
4253 #include "spinlock.h"
4254 #include "sleeplock.h"
4255 #include "fs.h"
4256 #include "buf.h"
4257 
4258 // Simple logging that allows concurrent FS system calls.
4259 //
4260 // A log transaction contains the updates of multiple FS system
4261 // calls. The logging system only commits when there are
4262 // no FS system calls active. Thus there is never
4263 // any reasoning required about whether a commit might
4264 // write an uncommitted system call's updates to disk.
4265 //
4266 // A system call should call begin_op()/end_op() to mark
4267 // its start and end. Usually begin_op() just increments
4268 // the count of in-progress FS system calls and returns.
4269 // But if it thinks the log is close to running out, it
4270 // sleeps until the last outstanding end_op() commits.
4271 //
4272 // The log is a physical re-do log containing disk blocks.
4273 // The on-disk log format:
4274 //   header block, containing block #s for block A, B, C, ...
4275 //   block A
4276 //   block B
4277 //   block C
4278 //   ...
4279 // Log appends are synchronous.
4280 
4281 // Contents of the header block, used for both the on-disk header block
4282 // and to keep track in memory of logged block# before commit.
4283 struct logheader {
4284   int n;
4285   int block[LOGSIZE];
4286 };
4287 
4288 struct log {
4289   struct spinlock lock;
4290   int start;
4291   int size;
4292   int outstanding; // how many FS sys calls are executing.
4293   int committing;  // in commit(), please wait.
4294   int dev;
4295   struct logheader lh;
4296 };
4297 
4298 
4299 
4300 struct log log;
4301 
4302 static void recover_from_log(void);
4303 static void commit();
4304 
4305 void
4306 initlog(int dev)
4307 {
4308   if (sizeof(struct logheader) >= BSIZE)
4309     panic("initlog: too big logheader");
4310 
4311   struct superblock sb;
4312   initlock(&log.lock, "log");
4313   readsb(dev, &sb);
4314   log.start = sb.logstart;
4315   log.size = sb.nlog;
4316   log.dev = dev;
4317   recover_from_log();
4318 }
4319 
4320 // Copy committed blocks from log to their home location
4321 static void
4322 install_trans(void)
4323 {
4324   int tail;
4325 
4326   for (tail = 0; tail < log.lh.n; tail++) {
4327     struct buf *lbuf = bread(log.dev, log.start+tail+1); // read log block
4328     struct buf *dbuf = bread(log.dev, log.lh.block[tail]); // read dst
4329     memmove(dbuf->data, lbuf->data, BSIZE);  // copy block to dst
4330     bwrite(dbuf);  // write dst to disk
4331     brelse(lbuf);
4332     brelse(dbuf);
4333   }
4334 }
4335 
4336 // Read the log header from disk into the in-memory log header
4337 static void
4338 read_head(void)
4339 {
4340   struct buf *buf = bread(log.dev, log.start);
4341   struct logheader *lh = (struct logheader *) (buf->data);
4342   int i;
4343   log.lh.n = lh->n;
4344   for (i = 0; i < log.lh.n; i++) {
4345     log.lh.block[i] = lh->block[i];
4346   }
4347   brelse(buf);
4348 }
4349 
4350 // Write in-memory log header to disk.
4351 // This is the true point at which the
4352 // current transaction commits.
4353 static void
4354 write_head(void)
4355 {
4356   struct buf *buf = bread(log.dev, log.start);
4357   struct logheader *hb = (struct logheader *) (buf->data);
4358   int i;
4359   hb->n = log.lh.n;
4360   for (i = 0; i < log.lh.n; i++) {
4361     hb->block[i] = log.lh.block[i];
4362   }
4363   bwrite(buf);
4364   brelse(buf);
4365 }
4366 
4367 static void
4368 recover_from_log(void)
4369 {
4370   read_head();
4371   install_trans(); // if committed, copy from log to disk
4372   log.lh.n = 0;
4373   write_head(); // clear the log
4374 }
4375 
4376 // called at the start of each FS system call.
4377 void
4378 begin_op(void)
4379 {
4380   acquire(&log.lock);
4381   while(1){
4382     if(log.committing){
4383       sleep(&log, &log.lock);
4384     } else if(log.lh.n + (log.outstanding+1)*MAXOPBLOCKS > LOGSIZE){
4385       // this op might exhaust log space; wait for commit.
4386       sleep(&log, &log.lock);
4387     } else {
4388       log.outstanding += 1;
4389       release(&log.lock);
4390       break;
4391     }
4392   }
4393 }
4394 
4395 
4396 
4397 
4398 
4399 
4400 // called at the end of each FS system call.
4401 // commits if this was the last outstanding operation.
4402 void
4403 end_op(void)
4404 {
4405   int do_commit = 0;
4406 
4407   acquire(&log.lock);
4408   log.outstanding -= 1;
4409   if(log.committing)
4410     panic("log.committing");
4411   if(log.outstanding == 0){
4412     do_commit = 1;
4413     log.committing = 1;
4414   } else {
4415     // begin_op() may be waiting for log space,
4416     // and decrementing log.outstanding has decreased
4417     // the amount of reserved space.
4418     wakeup(&log);
4419   }
4420   release(&log.lock);
4421 
4422   if(do_commit){
4423     // call commit w/o holding locks, since not allowed
4424     // to sleep with locks.
4425     commit();
4426     acquire(&log.lock);
4427     log.committing = 0;
4428     wakeup(&log);
4429     release(&log.lock);
4430   }
4431 }
4432 
4433 // Copy modified blocks from cache to log.
4434 static void
4435 write_log(void)
4436 {
4437   int tail;
4438 
4439   for (tail = 0; tail < log.lh.n; tail++) {
4440     struct buf *to = bread(log.dev, log.start+tail+1); // log block
4441     struct buf *from = bread(log.dev, log.lh.block[tail]); // cache block
4442     memmove(to->data, from->data, BSIZE);
4443     bwrite(to);  // write the log
4444     brelse(from);
4445     brelse(to);
4446   }
4447 }
4448 
4449 
4450 static void
4451 commit()
4452 {
4453   if (log.lh.n > 0) {
4454     write_log();     // Write modified blocks from cache to log
4455     write_head();    // Write header to disk -- the real commit
4456     install_trans(); // Now install writes to home locations
4457     log.lh.n = 0;
4458     write_head();    // Erase the transaction from the log
4459   }
4460 }
4461 
4462 // Caller has modified b->data and is done with the buffer.
4463 // Record the block number and pin in the cache with B_DIRTY.
4464 // commit()/write_log() will do the disk write.
4465 //
4466 // log_write() replaces bwrite(); a typical use is:
4467 //   bp = bread(...)
4468 //   modify bp->data[]
4469 //   log_write(bp)
4470 //   brelse(bp)
4471 void
4472 log_write(struct buf *b)
4473 {
4474   int i;
4475 
4476   if (log.lh.n >= LOGSIZE || log.lh.n >= log.size - 1)
4477     panic("too big a transaction");
4478   if (log.outstanding < 1)
4479     panic("log_write outside of trans");
4480 
4481   acquire(&log.lock);
4482   for (i = 0; i < log.lh.n; i++) {
4483     if (log.lh.block[i] == b->blockno)   // log absorbtion
4484       break;
4485   }
4486   log.lh.block[i] = b->blockno;
4487   if (i == log.lh.n)
4488     log.lh.n++;
4489   b->flags |= B_DIRTY; // prevent eviction
4490   release(&log.lock);
4491 }
4492 
4493 
4494 
4495 
4496 
4497 
4498 
4499 
