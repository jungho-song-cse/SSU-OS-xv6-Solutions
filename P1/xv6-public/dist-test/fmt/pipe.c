6100 #include "types.h"
6101 #include "defs.h"
6102 #include "param.h"
6103 #include "mmu.h"
6104 #include "proc.h"
6105 #include "fs.h"
6106 #include "spinlock.h"
6107 #include "sleeplock.h"
6108 #include "file.h"
6109 
6110 #define PIPESIZE 512
6111 
6112 struct pipe {
6113   struct spinlock lock;
6114   char data[PIPESIZE];
6115   uint nread;     // number of bytes read
6116   uint nwrite;    // number of bytes written
6117   int readopen;   // read fd is still open
6118   int writeopen;  // write fd is still open
6119 };
6120 
6121 int
6122 pipealloc(struct file **f0, struct file **f1)
6123 {
6124   struct pipe *p;
6125 
6126   p = 0;
6127   *f0 = *f1 = 0;
6128   if((*f0 = filealloc()) == 0 || (*f1 = filealloc()) == 0)
6129     goto bad;
6130   if((p = (struct pipe*)kalloc()) == 0)
6131     goto bad;
6132   p->readopen = 1;
6133   p->writeopen = 1;
6134   p->nwrite = 0;
6135   p->nread = 0;
6136   initlock(&p->lock, "pipe");
6137   (*f0)->type = FD_PIPE;
6138   (*f0)->readable = 1;
6139   (*f0)->writable = 0;
6140   (*f0)->pipe = p;
6141   (*f1)->type = FD_PIPE;
6142   (*f1)->readable = 0;
6143   (*f1)->writable = 1;
6144   (*f1)->pipe = p;
6145   return 0;
6146 
6147 
6148 
6149 
6150  bad:
6151   if(p)
6152     kfree((char*)p);
6153   if(*f0)
6154     fileclose(*f0);
6155   if(*f1)
6156     fileclose(*f1);
6157   return -1;
6158 }
6159 
6160 void
6161 pipeclose(struct pipe *p, int writable)
6162 {
6163   acquire(&p->lock);
6164   if(writable){
6165     p->writeopen = 0;
6166     wakeup(&p->nread);
6167   } else {
6168     p->readopen = 0;
6169     wakeup(&p->nwrite);
6170   }
6171   if(p->readopen == 0 && p->writeopen == 0){
6172     release(&p->lock);
6173     kfree((char*)p);
6174   } else
6175     release(&p->lock);
6176 }
6177 
6178 int
6179 pipewrite(struct pipe *p, char *addr, int n)
6180 {
6181   int i;
6182 
6183   acquire(&p->lock);
6184   for(i = 0; i < n; i++){
6185     while(p->nwrite == p->nread + PIPESIZE){  //DOC: pipewrite-full
6186       if(p->readopen == 0 || myproc()->killed){
6187         release(&p->lock);
6188         return -1;
6189       }
6190       wakeup(&p->nread);
6191       sleep(&p->nwrite, &p->lock);  //DOC: pipewrite-sleep
6192     }
6193     p->data[p->nwrite++ % PIPESIZE] = addr[i];
6194   }
6195   wakeup(&p->nread);  //DOC: pipewrite-wakeup1
6196   release(&p->lock);
6197   return n;
6198 }
6199 
6200 int
6201 piperead(struct pipe *p, char *addr, int n)
6202 {
6203   int i;
6204 
6205   acquire(&p->lock);
6206   while(p->nread == p->nwrite && p->writeopen){  //DOC: pipe-empty
6207     if(myproc()->killed){
6208       release(&p->lock);
6209       return -1;
6210     }
6211     sleep(&p->nread, &p->lock); //DOC: piperead-sleep
6212   }
6213   for(i = 0; i < n; i++){  //DOC: piperead-copy
6214     if(p->nread == p->nwrite)
6215       break;
6216     addr[i] = p->data[p->nread++ % PIPESIZE];
6217   }
6218   wakeup(&p->nwrite);  //DOC: piperead-wakeup
6219   release(&p->lock);
6220   return i;
6221 }
6222 
6223 
6224 
6225 
6226 
6227 
6228 
6229 
6230 
6231 
6232 
6233 
6234 
6235 
6236 
6237 
6238 
6239 
6240 
6241 
6242 
6243 
6244 
6245 
6246 
6247 
6248 
6249 
