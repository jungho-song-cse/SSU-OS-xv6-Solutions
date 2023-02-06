3150 #include "types.h"
3151 #include "defs.h"
3152 #include "param.h"
3153 #include "memlayout.h"
3154 #include "mmu.h"
3155 #include "proc.h"
3156 #include "x86.h"
3157 #include "syscall.h"
3158 
3159 // User code makes a system call with INT T_SYSCALL.
3160 // System call number in %eax.
3161 // Arguments on the stack, from the user call to the C
3162 // library system call function. The saved user %esp points
3163 // to a saved program counter, and then the first argument.
3164 
3165 // Fetch the int at addr from the current process.
3166 int
3167 fetchint(uint addr, int *ip)
3168 {
3169   struct proc *curproc = myproc();
3170 
3171   if(addr >= curproc->sz || addr+4 > curproc->sz)
3172     return -1;
3173   *ip = *(int*)(addr);
3174   return 0;
3175 }
3176 
3177 // Fetch the nul-terminated string at addr from the current process.
3178 // Doesn't actually copy the string - just sets *pp to point at it.
3179 // Returns length of string, not including nul.
3180 int
3181 fetchstr(uint addr, char **pp)
3182 {
3183   char *s, *ep;
3184   struct proc *curproc = myproc();
3185 
3186   if(addr >= curproc->sz)
3187     return -1;
3188   *pp = (char*)addr;
3189   ep = (char*)curproc->sz;
3190   for(s = *pp; s < ep; s++){
3191     if(*s == 0)
3192       return s - *pp;
3193   }
3194   return -1;
3195 }
3196 
3197 
3198 
3199 
3200 // Fetch the nth 32-bit system call argument.
3201 int
3202 argint(int n, int *ip)
3203 {
3204   return fetchint((myproc()->tf->esp) + 4 + 4*n, ip);
3205 }
3206 
3207 // Fetch the nth word-sized system call argument as a pointer
3208 // to a block of memory of size bytes.  Check that the pointer
3209 // lies within the process address space.
3210 int
3211 argptr(int n, char **pp, int size)
3212 {
3213   int i;
3214   struct proc *curproc = myproc();
3215 
3216   if(argint(n, &i) < 0)
3217     return -1;
3218   if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz)
3219     return -1;
3220   *pp = (char*)i;
3221   return 0;
3222 }
3223 
3224 // Fetch the nth word-sized system call argument as a string pointer.
3225 // Check that the pointer is valid and the string is nul-terminated.
3226 // (There is no shared writable memory, so the string can't change
3227 // between this check and being used by the kernel.)
3228 int
3229 argstr(int n, char **pp)
3230 {
3231   int addr;
3232   if(argint(n, &addr) < 0)
3233     return -1;
3234   return fetchstr(addr, pp);
3235 }
3236 
3237 
3238 
3239 
3240 
3241 
3242 
3243 
3244 
3245 
3246 
3247 
3248 
3249 
3250 extern int sys_chdir(void);
3251 extern int sys_close(void);
3252 extern int sys_dup(void);
3253 extern int sys_exec(void);
3254 extern int sys_exit(void);
3255 extern int sys_fork(void);
3256 extern int sys_fstat(void);
3257 extern int sys_getpid(void);
3258 extern int sys_kill(void);
3259 extern int sys_link(void);
3260 extern int sys_mkdir(void);
3261 extern int sys_mknod(void);
3262 extern int sys_open(void);
3263 extern int sys_pipe(void);
3264 extern int sys_read(void);
3265 extern int sys_sbrk(void);
3266 extern int sys_sleep(void);
3267 extern int sys_unlink(void);
3268 extern int sys_wait(void);
3269 extern int sys_write(void);
3270 extern int sys_uptime(void);
3271 
3272 static int (*syscalls[])(void) = {
3273 [SYS_fork]    sys_fork,
3274 [SYS_exit]    sys_exit,
3275 [SYS_wait]    sys_wait,
3276 [SYS_pipe]    sys_pipe,
3277 [SYS_read]    sys_read,
3278 [SYS_kill]    sys_kill,
3279 [SYS_exec]    sys_exec,
3280 [SYS_fstat]   sys_fstat,
3281 [SYS_chdir]   sys_chdir,
3282 [SYS_dup]     sys_dup,
3283 [SYS_getpid]  sys_getpid,
3284 [SYS_sbrk]    sys_sbrk,
3285 [SYS_sleep]   sys_sleep,
3286 [SYS_uptime]  sys_uptime,
3287 [SYS_open]    sys_open,
3288 [SYS_write]   sys_write,
3289 [SYS_mknod]   sys_mknod,
3290 [SYS_unlink]  sys_unlink,
3291 [SYS_link]    sys_link,
3292 [SYS_mkdir]   sys_mkdir,
3293 [SYS_close]   sys_close,
3294 };
3295 
3296 
3297 
3298 
3299 
3300 void
3301 syscall(void)
3302 {
3303   int num;
3304   struct proc *curproc = myproc();
3305 
3306   num = curproc->tf->eax;
3307   if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
3308     curproc->tf->eax = syscalls[num]();
3309   } else {
3310     cprintf("%d %s: unknown sys call %d\n",
3311             curproc->pid, curproc->name, num);
3312     curproc->tf->eax = -1;
3313   }
3314 }
3315 
3316 
3317 
3318 
3319 
3320 
3321 
3322 
3323 
3324 
3325 
3326 
3327 
3328 
3329 
3330 
3331 
3332 
3333 
3334 
3335 
3336 
3337 
3338 
3339 
3340 
3341 
3342 
3343 
3344 
3345 
3346 
3347 
3348 
3349 
