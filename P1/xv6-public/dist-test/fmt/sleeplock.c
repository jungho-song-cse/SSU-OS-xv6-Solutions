4150 // Sleeping locks
4151 
4152 #include "types.h"
4153 #include "defs.h"
4154 #include "param.h"
4155 #include "x86.h"
4156 #include "memlayout.h"
4157 #include "mmu.h"
4158 #include "proc.h"
4159 #include "spinlock.h"
4160 #include "sleeplock.h"
4161 
4162 void
4163 initsleeplock(struct sleeplock *lk, char *name)
4164 {
4165   initlock(&lk->lk, "sleep lock");
4166   lk->name = name;
4167   lk->locked = 0;
4168   lk->pid = 0;
4169 }
4170 
4171 void
4172 acquiresleep(struct sleeplock *lk)
4173 {
4174   acquire(&lk->lk);
4175   while (lk->locked) {
4176     sleep(lk, &lk->lk);
4177   }
4178   lk->locked = 1;
4179   lk->pid = myproc()->pid;
4180   release(&lk->lk);
4181 }
4182 
4183 void
4184 releasesleep(struct sleeplock *lk)
4185 {
4186   acquire(&lk->lk);
4187   lk->locked = 0;
4188   lk->pid = 0;
4189   wakeup(lk);
4190   release(&lk->lk);
4191 }
4192 
4193 
4194 
4195 
4196 
4197 
4198 
4199 
4200 int
4201 holdingsleep(struct sleeplock *lk)
4202 {
4203   int r;
4204 
4205   acquire(&lk->lk);
4206   r = lk->locked && (lk->pid == myproc()->pid);
4207   release(&lk->lk);
4208   return r;
4209 }
4210 
4211 
4212 
4213 
4214 
4215 
4216 
4217 
4218 
4219 
4220 
4221 
4222 
4223 
4224 
4225 
4226 
4227 
4228 
4229 
4230 
4231 
4232 
4233 
4234 
4235 
4236 
4237 
4238 
4239 
4240 
4241 
4242 
4243 
4244 
4245 
4246 
4247 
4248 
4249 
