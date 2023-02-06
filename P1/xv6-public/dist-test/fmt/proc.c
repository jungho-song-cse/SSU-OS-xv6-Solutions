2100 #include "types.h"
2101 #include "defs.h"
2102 #include "param.h"
2103 #include "memlayout.h"
2104 #include "mmu.h"
2105 #include "x86.h"
2106 #include "proc.h"
2107 #include "spinlock.h"
2108 
2109 struct {
2110   struct spinlock lock;
2111   struct proc proc[NPROC];
2112 } ptable;
2113 
2114 static struct proc *initproc;
2115 
2116 int nextpid = 1;
2117 extern void forkret(void);
2118 extern void trapret(void);
2119 
2120 static void wakeup1(void *chan);
2121 
2122 void
2123 pinit(void)
2124 {
2125   initlock(&ptable.lock, "ptable");
2126 }
2127 
2128 // Must be called with interrupts disabled
2129 int
2130 cpuid() {
2131   return mycpu()-cpus;
2132 }
2133 
2134 // Must be called with interrupts disabled to avoid the caller being
2135 // rescheduled between reading lapicid and running through the loop.
2136 struct cpu*
2137 mycpu(void)
2138 {
2139   int apicid, i;
2140 
2141   if(readeflags()&FL_IF)
2142     panic("mycpu called with interrupts enabled\n");
2143 
2144   apicid = lapicid();
2145   // APIC IDs are not guaranteed to be contiguous. Maybe we should have
2146   // a reverse map, or reserve a register to store &cpus[i].
2147   for (i = 0; i < ncpu; ++i) {
2148     if (cpus[i].apicid == apicid)
2149       return &cpus[i];
2150   }
2151   panic("unknown apicid\n");
2152 }
2153 
2154 // Disable interrupts so that we are not rescheduled
2155 // while reading proc from the cpu structure
2156 struct proc*
2157 myproc(void) {
2158   struct cpu *c;
2159   struct proc *p;
2160   pushcli();
2161   c = mycpu();
2162   p = c->proc;
2163   popcli();
2164   return p;
2165 }
2166 
2167 // Look in the process table for an UNUSED proc.
2168 // If found, change state to EMBRYO and initialize
2169 // state required to run in the kernel.
2170 // Otherwise return 0.
2171 static struct proc*
2172 allocproc(void)
2173 {
2174   struct proc *p;
2175   char *sp;
2176 
2177   acquire(&ptable.lock);
2178 
2179   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
2180     if(p->state == UNUSED)
2181       goto found;
2182 
2183   release(&ptable.lock);
2184   return 0;
2185 
2186 found:
2187   p->state = EMBRYO;
2188   p->pid = nextpid++;
2189 
2190   release(&ptable.lock);
2191 
2192   // Allocate kernel stack.
2193   if((p->kstack = kalloc()) == 0){
2194     p->state = UNUSED;
2195     return 0;
2196   }
2197   sp = p->kstack + KSTACKSIZE;
2198 
2199 
2200   // Leave room for trap frame.
2201   sp -= sizeof *p->tf;
2202   p->tf = (struct trapframe*)sp;
2203 
2204   // Set up new context to start executing at forkret,
2205   // which returns to trapret.
2206   sp -= 4;
2207   *(uint*)sp = (uint)trapret;
2208 
2209   sp -= sizeof *p->context;
2210   p->context = (struct context*)sp;
2211   memset(p->context, 0, sizeof *p->context);
2212   p->context->eip = (uint)forkret;
2213 
2214   return p;
2215 }
2216 
2217 // Set up first user process.
2218 void
2219 userinit(void)
2220 {
2221   struct proc *p;
2222   extern char _binary_initcode_start[], _binary_initcode_size[];
2223 
2224   p = allocproc();
2225 
2226   initproc = p;
2227   if((p->pgdir = setupkvm()) == 0)
2228     panic("userinit: out of memory?");
2229   inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
2230   p->sz = PGSIZE;
2231   memset(p->tf, 0, sizeof(*p->tf));
2232   p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
2233   p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
2234   p->tf->es = p->tf->ds;
2235   p->tf->ss = p->tf->ds;
2236   p->tf->eflags = FL_IF;
2237   p->tf->esp = PGSIZE;
2238   p->tf->eip = 0;  // beginning of initcode.S
2239 
2240   safestrcpy(p->name, "initcode", sizeof(p->name));
2241   p->cwd = namei("/");
2242 
2243   // this assignment to p->state lets other cores
2244   // run this process. the acquire forces the above
2245   // writes to be visible, and the lock is also needed
2246   // because the assignment might not be atomic.
2247   acquire(&ptable.lock);
2248 
2249   p->state = RUNNABLE;
2250   release(&ptable.lock);
2251 }
2252 
2253 // Grow current process's memory by n bytes.
2254 // Return 0 on success, -1 on failure.
2255 int
2256 growproc(int n)
2257 {
2258   uint sz;
2259   struct proc *curproc = myproc();
2260 
2261   sz = curproc->sz;
2262   if(n > 0){
2263     if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
2264       return -1;
2265   } else if(n < 0){
2266     if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
2267       return -1;
2268   }
2269   curproc->sz = sz;
2270   switchuvm(curproc);
2271   return 0;
2272 }
2273 
2274 // Create a new process copying p as the parent.
2275 // Sets up stack to return as if from system call.
2276 // Caller must set state of returned proc to RUNNABLE.
2277 int
2278 fork(void)
2279 {
2280   int i, pid;
2281   struct proc *np;
2282   struct proc *curproc = myproc();
2283 
2284   // Allocate process.
2285   if((np = allocproc()) == 0){
2286     return -1;
2287   }
2288 
2289   // Copy process state from proc.
2290   if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
2291     kfree(np->kstack);
2292     np->kstack = 0;
2293     np->state = UNUSED;
2294     return -1;
2295   }
2296   np->sz = curproc->sz;
2297   np->parent = curproc;
2298   *np->tf = *curproc->tf;
2299 
2300   // Clear %eax so that fork returns 0 in the child.
2301   np->tf->eax = 0;
2302 
2303   for(i = 0; i < NOFILE; i++)
2304     if(curproc->ofile[i])
2305       np->ofile[i] = filedup(curproc->ofile[i]);
2306   np->cwd = idup(curproc->cwd);
2307 
2308   safestrcpy(np->name, curproc->name, sizeof(curproc->name));
2309 
2310   pid = np->pid;
2311 
2312   acquire(&ptable.lock);
2313 
2314   np->state = RUNNABLE;
2315 
2316   release(&ptable.lock);
2317 
2318   return pid;
2319 }
2320 
2321 // Exit the current process.  Does not return.
2322 // An exited process remains in the zombie state
2323 // until its parent calls wait() to find out it exited.
2324 void
2325 exit(void)
2326 {
2327   struct proc *curproc = myproc();
2328   struct proc *p;
2329   int fd;
2330 
2331   if(curproc == initproc)
2332     panic("init exiting");
2333 
2334   // Close all open files.
2335   for(fd = 0; fd < NOFILE; fd++){
2336     if(curproc->ofile[fd]){
2337       fileclose(curproc->ofile[fd]);
2338       curproc->ofile[fd] = 0;
2339     }
2340   }
2341 
2342   begin_op();
2343   iput(curproc->cwd);
2344   end_op();
2345   curproc->cwd = 0;
2346 
2347   acquire(&ptable.lock);
2348 
2349 
2350   // Parent might be sleeping in wait().
2351   wakeup1(curproc->parent);
2352 
2353   // Pass abandoned children to init.
2354   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2355     if(p->parent == curproc){
2356       p->parent = initproc;
2357       if(p->state == ZOMBIE)
2358         wakeup1(initproc);
2359     }
2360   }
2361 
2362   // Jump into the scheduler, never to return.
2363   curproc->state = ZOMBIE;
2364   sched();
2365   panic("zombie exit");
2366 }
2367 
2368 // Wait for a child process to exit and return its pid.
2369 // Return -1 if this process has no children.
2370 int
2371 wait(void)
2372 {
2373   struct proc *p;
2374   int havekids, pid;
2375   struct proc *curproc = myproc();
2376 
2377   acquire(&ptable.lock);
2378   for(;;){
2379     // Scan through table looking for exited children.
2380     havekids = 0;
2381     for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2382       if(p->parent != curproc)
2383         continue;
2384       havekids = 1;
2385       if(p->state == ZOMBIE){
2386         // Found one.
2387         pid = p->pid;
2388         kfree(p->kstack);
2389         p->kstack = 0;
2390         freevm(p->pgdir);
2391         p->pid = 0;
2392         p->parent = 0;
2393         p->name[0] = 0;
2394         p->killed = 0;
2395         p->state = UNUSED;
2396         release(&ptable.lock);
2397         return pid;
2398       }
2399     }
2400     // No point waiting if we don't have any children.
2401     if(!havekids || curproc->killed){
2402       release(&ptable.lock);
2403       return -1;
2404     }
2405 
2406     // Wait for children to exit.  (See wakeup1 call in proc_exit.)
2407     sleep(curproc, &ptable.lock);  //DOC: wait-sleep
2408   }
2409 }
2410 
2411 // Per-CPU process scheduler.
2412 // Each CPU calls scheduler() after setting itself up.
2413 // Scheduler never returns.  It loops, doing:
2414 //  - choose a process to run
2415 //  - swtch to start running that process
2416 //  - eventually that process transfers control
2417 //      via swtch back to the scheduler.
2418 void
2419 scheduler(void)
2420 {
2421   struct proc *p;
2422   struct cpu *c = mycpu();
2423   c->proc = 0;
2424 
2425   for(;;){
2426     // Enable interrupts on this processor.
2427     sti();
2428 
2429     // Loop over process table looking for process to run.
2430     acquire(&ptable.lock);
2431     for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2432       if(p->state != RUNNABLE)
2433         continue;
2434 
2435       // Switch to chosen process.  It is the process's job
2436       // to release ptable.lock and then reacquire it
2437       // before jumping back to us.
2438       c->proc = p;
2439       switchuvm(p);
2440       p->state = RUNNING;
2441 
2442       swtch(&(c->scheduler), p->context);
2443       switchkvm();
2444 
2445       // Process is done running for now.
2446       // It should have changed its p->state before coming back.
2447       c->proc = 0;
2448     }
2449     release(&ptable.lock);
2450   }
2451 }
2452 
2453 // Enter scheduler.  Must hold only ptable.lock
2454 // and have changed proc->state. Saves and restores
2455 // intena because intena is a property of this
2456 // kernel thread, not this CPU. It should
2457 // be proc->intena and proc->ncli, but that would
2458 // break in the few places where a lock is held but
2459 // there's no process.
2460 void
2461 sched(void)
2462 {
2463   int intena;
2464   struct proc *p = myproc();
2465 
2466   if(!holding(&ptable.lock))
2467     panic("sched ptable.lock");
2468   if(mycpu()->ncli != 1)
2469     panic("sched locks");
2470   if(p->state == RUNNING)
2471     panic("sched running");
2472   if(readeflags()&FL_IF)
2473     panic("sched interruptible");
2474   intena = mycpu()->intena;
2475   swtch(&p->context, mycpu()->scheduler);
2476   mycpu()->intena = intena;
2477 }
2478 
2479 // Give up the CPU for one scheduling round.
2480 void
2481 yield(void)
2482 {
2483   acquire(&ptable.lock);  //DOC: yieldlock
2484   myproc()->state = RUNNABLE;
2485   sched();
2486   release(&ptable.lock);
2487 }
2488 
2489 
2490 
2491 
2492 
2493 
2494 
2495 
2496 
2497 
2498 
2499 
2500 // A fork child's very first scheduling by scheduler()
2501 // will swtch here.  "Return" to user space.
2502 void
2503 forkret(void)
2504 {
2505   static int first = 1;
2506   // Still holding ptable.lock from scheduler.
2507   release(&ptable.lock);
2508 
2509   if (first) {
2510     // Some initialization functions must be run in the context
2511     // of a regular process (e.g., they call sleep), and thus cannot
2512     // be run from main().
2513     first = 0;
2514     iinit(ROOTDEV);
2515     initlog(ROOTDEV);
2516   }
2517 
2518   // Return to "caller", actually trapret (see allocproc).
2519 }
2520 
2521 // Atomically release lock and sleep on chan.
2522 // Reacquires lock when awakened.
2523 void
2524 sleep(void *chan, struct spinlock *lk)
2525 {
2526   struct proc *p = myproc();
2527 
2528   if(p == 0)
2529     panic("sleep");
2530 
2531   if(lk == 0)
2532     panic("sleep without lk");
2533 
2534   // Must acquire ptable.lock in order to
2535   // change p->state and then call sched.
2536   // Once we hold ptable.lock, we can be
2537   // guaranteed that we won't miss any wakeup
2538   // (wakeup runs with ptable.lock locked),
2539   // so it's okay to release lk.
2540   if(lk != &ptable.lock){  //DOC: sleeplock0
2541     acquire(&ptable.lock);  //DOC: sleeplock1
2542     release(lk);
2543   }
2544   // Go to sleep.
2545   p->chan = chan;
2546   p->state = SLEEPING;
2547 
2548   sched();
2549 
2550   // Tidy up.
2551   p->chan = 0;
2552 
2553   // Reacquire original lock.
2554   if(lk != &ptable.lock){  //DOC: sleeplock2
2555     release(&ptable.lock);
2556     acquire(lk);
2557   }
2558 }
2559 
2560 // Wake up all processes sleeping on chan.
2561 // The ptable lock must be held.
2562 static void
2563 wakeup1(void *chan)
2564 {
2565   struct proc *p;
2566 
2567   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
2568     if(p->state == SLEEPING && p->chan == chan)
2569       p->state = RUNNABLE;
2570 }
2571 
2572 // Wake up all processes sleeping on chan.
2573 void
2574 wakeup(void *chan)
2575 {
2576   acquire(&ptable.lock);
2577   wakeup1(chan);
2578   release(&ptable.lock);
2579 }
2580 
2581 // Kill the process with the given pid.
2582 // Process won't exit until it returns
2583 // to user space (see trap in trap.c).
2584 int
2585 kill(int pid)
2586 {
2587   struct proc *p;
2588 
2589   acquire(&ptable.lock);
2590   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2591     if(p->pid == pid){
2592       p->killed = 1;
2593       // Wake process from sleep if necessary.
2594       if(p->state == SLEEPING)
2595         p->state = RUNNABLE;
2596       release(&ptable.lock);
2597       return 0;
2598     }
2599   }
2600   release(&ptable.lock);
2601   return -1;
2602 }
2603 
2604 // Print a process listing to console.  For debugging.
2605 // Runs when user types ^P on console.
2606 // No lock to avoid wedging a stuck machine further.
2607 void
2608 procdump(void)
2609 {
2610   static char *states[] = {
2611   [UNUSED]    "unused",
2612   [EMBRYO]    "embryo",
2613   [SLEEPING]  "sleep ",
2614   [RUNNABLE]  "runble",
2615   [RUNNING]   "run   ",
2616   [ZOMBIE]    "zombie"
2617   };
2618   int i;
2619   struct proc *p;
2620   char *state;
2621   uint pc[10];
2622 
2623   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2624     if(p->state == UNUSED)
2625       continue;
2626     if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
2627       state = states[p->state];
2628     else
2629       state = "???";
2630     cprintf("%d %s %s", p->pid, state, p->name);
2631     if(p->state == SLEEPING){
2632       getcallerpcs((uint*)p->context->ebp+2, pc);
2633       for(i=0; i<10 && pc[i] != 0; i++)
2634         cprintf(" %p", pc[i]);
2635     }
2636     cprintf("\n");
2637   }
2638 }
2639 
2640 
2641 
2642 
2643 
2644 
2645 
2646 
2647 
2648 
2649 
