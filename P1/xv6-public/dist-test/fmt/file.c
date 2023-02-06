5250 //
5251 // File descriptors
5252 //
5253 
5254 #include "types.h"
5255 #include "defs.h"
5256 #include "param.h"
5257 #include "fs.h"
5258 #include "spinlock.h"
5259 #include "sleeplock.h"
5260 #include "file.h"
5261 
5262 struct devsw devsw[NDEV];
5263 struct {
5264   struct spinlock lock;
5265   struct file file[NFILE];
5266 } ftable;
5267 
5268 void
5269 fileinit(void)
5270 {
5271   initlock(&ftable.lock, "ftable");
5272 }
5273 
5274 // Allocate a file structure.
5275 struct file*
5276 filealloc(void)
5277 {
5278   struct file *f;
5279 
5280   acquire(&ftable.lock);
5281   for(f = ftable.file; f < ftable.file + NFILE; f++){
5282     if(f->ref == 0){
5283       f->ref = 1;
5284       release(&ftable.lock);
5285       return f;
5286     }
5287   }
5288   release(&ftable.lock);
5289   return 0;
5290 }
5291 
5292 
5293 
5294 
5295 
5296 
5297 
5298 
5299 
5300 // Increment ref count for file f.
5301 struct file*
5302 filedup(struct file *f)
5303 {
5304   acquire(&ftable.lock);
5305   if(f->ref < 1)
5306     panic("filedup");
5307   f->ref++;
5308   release(&ftable.lock);
5309   return f;
5310 }
5311 
5312 // Close file f.  (Decrement ref count, close when reaches 0.)
5313 void
5314 fileclose(struct file *f)
5315 {
5316   struct file ff;
5317 
5318   acquire(&ftable.lock);
5319   if(f->ref < 1)
5320     panic("fileclose");
5321   if(--f->ref > 0){
5322     release(&ftable.lock);
5323     return;
5324   }
5325   ff = *f;
5326   f->ref = 0;
5327   f->type = FD_NONE;
5328   release(&ftable.lock);
5329 
5330   if(ff.type == FD_PIPE)
5331     pipeclose(ff.pipe, ff.writable);
5332   else if(ff.type == FD_INODE){
5333     begin_op();
5334     iput(ff.ip);
5335     end_op();
5336   }
5337 }
5338 
5339 
5340 
5341 
5342 
5343 
5344 
5345 
5346 
5347 
5348 
5349 
5350 // Get metadata about file f.
5351 int
5352 filestat(struct file *f, struct stat *st)
5353 {
5354   if(f->type == FD_INODE){
5355     ilock(f->ip);
5356     stati(f->ip, st);
5357     iunlock(f->ip);
5358     return 0;
5359   }
5360   return -1;
5361 }
5362 
5363 // Read from file f.
5364 int
5365 fileread(struct file *f, char *addr, int n)
5366 {
5367   int r;
5368 
5369   if(f->readable == 0)
5370     return -1;
5371   if(f->type == FD_PIPE)
5372     return piperead(f->pipe, addr, n);
5373   if(f->type == FD_INODE){
5374     ilock(f->ip);
5375     if((r = readi(f->ip, addr, f->off, n)) > 0)
5376       f->off += r;
5377     iunlock(f->ip);
5378     return r;
5379   }
5380   panic("fileread");
5381 }
5382 
5383 // Write to file f.
5384 int
5385 filewrite(struct file *f, char *addr, int n)
5386 {
5387   int r;
5388 
5389   if(f->writable == 0)
5390     return -1;
5391   if(f->type == FD_PIPE)
5392     return pipewrite(f->pipe, addr, n);
5393   if(f->type == FD_INODE){
5394     // write a few blocks at a time to avoid exceeding
5395     // the maximum log transaction size, including
5396     // i-node, indirect block, allocation blocks,
5397     // and 2 blocks of slop for non-aligned writes.
5398     // this really belongs lower down, since writei()
5399     // might be writing a device like the console.
5400     int max = ((MAXOPBLOCKS-1-1-2) / 2) * 512;
5401     int i = 0;
5402     while(i < n){
5403       int n1 = n - i;
5404       if(n1 > max)
5405         n1 = max;
5406 
5407       begin_op();
5408       ilock(f->ip);
5409       if ((r = writei(f->ip, addr + i, f->off, n1)) > 0)
5410         f->off += r;
5411       iunlock(f->ip);
5412       end_op();
5413 
5414       if(r < 0)
5415         break;
5416       if(r != n1)
5417         panic("short filewrite");
5418       i += r;
5419     }
5420     return i == n ? n : -1;
5421   }
5422   panic("filewrite");
5423 }
5424 
5425 
5426 
5427 
5428 
5429 
5430 
5431 
5432 
5433 
5434 
5435 
5436 
5437 
5438 
5439 
5440 
5441 
5442 
5443 
5444 
5445 
5446 
5447 
5448 
5449 
