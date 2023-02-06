5950 #include "types.h"
5951 #include "param.h"
5952 #include "memlayout.h"
5953 #include "mmu.h"
5954 #include "proc.h"
5955 #include "defs.h"
5956 #include "x86.h"
5957 #include "elf.h"
5958 
5959 int
5960 exec(char *path, char **argv)
5961 {
5962   char *s, *last;
5963   int i, off;
5964   uint argc, sz, sp, ustack[3+MAXARG+1];
5965   struct elfhdr elf;
5966   struct inode *ip;
5967   struct proghdr ph;
5968   pde_t *pgdir, *oldpgdir;
5969   struct proc *curproc = myproc();
5970 
5971   begin_op();
5972 
5973   if((ip = namei(path)) == 0){
5974     end_op();
5975     cprintf("exec: fail\n");
5976     return -1;
5977   }
5978   ilock(ip);
5979   pgdir = 0;
5980 
5981   // Check ELF header
5982   if(readi(ip, (char*)&elf, 0, sizeof(elf)) != sizeof(elf))
5983     goto bad;
5984   if(elf.magic != ELF_MAGIC)
5985     goto bad;
5986 
5987   if((pgdir = setupkvm()) == 0)
5988     goto bad;
5989 
5990   // Load program into memory.
5991   sz = 0;
5992   for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
5993     if(readi(ip, (char*)&ph, off, sizeof(ph)) != sizeof(ph))
5994       goto bad;
5995     if(ph.type != ELF_PROG_LOAD)
5996       continue;
5997     if(ph.memsz < ph.filesz)
5998       goto bad;
5999     if(ph.vaddr + ph.memsz < ph.vaddr)
6000       goto bad;
6001     if((sz = allocuvm(pgdir, sz, ph.vaddr + ph.memsz)) == 0)
6002       goto bad;
6003     if(ph.vaddr % PGSIZE != 0)
6004       goto bad;
6005     if(loaduvm(pgdir, (char*)ph.vaddr, ip, ph.off, ph.filesz) < 0)
6006       goto bad;
6007   }
6008   iunlockput(ip);
6009   end_op();
6010   ip = 0;
6011 
6012   // Allocate two pages at the next page boundary.
6013   // Make the first inaccessible.  Use the second as the user stack.
6014   sz = PGROUNDUP(sz);
6015   if((sz = allocuvm(pgdir, sz, sz + 2*PGSIZE)) == 0)
6016     goto bad;
6017   clearpteu(pgdir, (char*)(sz - 2*PGSIZE));
6018   sp = sz;
6019 
6020   // Push argument strings, prepare rest of stack in ustack.
6021   for(argc = 0; argv[argc]; argc++) {
6022     if(argc >= MAXARG)
6023       goto bad;
6024     sp = (sp - (strlen(argv[argc]) + 1)) & ~3;
6025     if(copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
6026       goto bad;
6027     ustack[3+argc] = sp;
6028   }
6029   ustack[3+argc] = 0;
6030 
6031   ustack[0] = 0xffffffff;  // fake return PC
6032   ustack[1] = argc;
6033   ustack[2] = sp - (argc+1)*4;  // argv pointer
6034 
6035   sp -= (3+argc+1) * 4;
6036   if(copyout(pgdir, sp, ustack, (3+argc+1)*4) < 0)
6037     goto bad;
6038 
6039   // Save program name for debugging.
6040   for(last=s=path; *s; s++)
6041     if(*s == '/')
6042       last = s+1;
6043   safestrcpy(curproc->name, last, sizeof(curproc->name));
6044 
6045   // Commit to the user image.
6046   oldpgdir = curproc->pgdir;
6047   curproc->pgdir = pgdir;
6048   curproc->sz = sz;
6049   curproc->tf->eip = elf.entry;  // main
6050   curproc->tf->esp = sp;
6051   switchuvm(curproc);
6052   freevm(oldpgdir);
6053   return 0;
6054 
6055  bad:
6056   if(pgdir)
6057     freevm(pgdir);
6058   if(ip){
6059     iunlockput(ip);
6060     end_op();
6061   }
6062   return -1;
6063 }
6064 
6065 
6066 
6067 
6068 
6069 
6070 
6071 
6072 
6073 
6074 
6075 
6076 
6077 
6078 
6079 
6080 
6081 
6082 
6083 
6084 
6085 
6086 
6087 
6088 
6089 
6090 
6091 
6092 
6093 
6094 
6095 
6096 
6097 
6098 
6099 
