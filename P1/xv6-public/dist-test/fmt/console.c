7200 // Console input and output.
7201 // Input is from the keyboard or serial port.
7202 // Output is written to the screen and serial port.
7203 
7204 #include "types.h"
7205 #include "defs.h"
7206 #include "param.h"
7207 #include "traps.h"
7208 #include "spinlock.h"
7209 #include "sleeplock.h"
7210 #include "fs.h"
7211 #include "file.h"
7212 #include "memlayout.h"
7213 #include "mmu.h"
7214 #include "proc.h"
7215 #include "x86.h"
7216 
7217 static void consputc(int);
7218 
7219 static int panicked = 0;
7220 
7221 static struct {
7222   struct spinlock lock;
7223   int locking;
7224 } cons;
7225 
7226 static void
7227 printint(int xx, int base, int sign)
7228 {
7229   static char digits[] = "0123456789abcdef";
7230   char buf[16];
7231   int i;
7232   uint x;
7233 
7234   if(sign && (sign = xx < 0))
7235     x = -xx;
7236   else
7237     x = xx;
7238 
7239   i = 0;
7240   do{
7241     buf[i++] = digits[x % base];
7242   }while((x /= base) != 0);
7243 
7244   if(sign)
7245     buf[i++] = '-';
7246 
7247   while(--i >= 0)
7248     consputc(buf[i]);
7249 }
7250 // Print to the console. only understands %d, %x, %p, %s.
7251 void
7252 cprintf(char *fmt, ...)
7253 {
7254   int i, c, locking;
7255   uint *argp;
7256   char *s;
7257 
7258   locking = cons.locking;
7259   if(locking)
7260     acquire(&cons.lock);
7261 
7262   if (fmt == 0)
7263     panic("null fmt");
7264 
7265   argp = (uint*)(void*)(&fmt + 1);
7266   for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
7267     if(c != '%'){
7268       consputc(c);
7269       continue;
7270     }
7271     c = fmt[++i] & 0xff;
7272     if(c == 0)
7273       break;
7274     switch(c){
7275     case 'd':
7276       printint(*argp++, 10, 1);
7277       break;
7278     case 'x':
7279     case 'p':
7280       printint(*argp++, 16, 0);
7281       break;
7282     case 's':
7283       if((s = (char*)*argp++) == 0)
7284         s = "(null)";
7285       for(; *s; s++)
7286         consputc(*s);
7287       break;
7288     case '%':
7289       consputc('%');
7290       break;
7291     default:
7292       // Print unknown % sequence to draw attention.
7293       consputc('%');
7294       consputc(c);
7295       break;
7296     }
7297   }
7298 
7299 
7300   if(locking)
7301     release(&cons.lock);
7302 }
7303 
7304 void
7305 panic(char *s)
7306 {
7307   int i;
7308   uint pcs[10];
7309 
7310   cli();
7311   cons.locking = 0;
7312   // use lapiccpunum so that we can call panic from mycpu()
7313   cprintf("lapicid %d: panic: ", lapicid());
7314   cprintf(s);
7315   cprintf("\n");
7316   getcallerpcs(&s, pcs);
7317   for(i=0; i<10; i++)
7318     cprintf(" %p", pcs[i]);
7319   panicked = 1; // freeze other CPU
7320   for(;;)
7321     ;
7322 }
7323 
7324 #define BACKSPACE 0x100
7325 #define CRTPORT 0x3d4
7326 static ushort *crt = (ushort*)P2V(0xb8000);  // CGA memory
7327 
7328 static void
7329 cgaputc(int c)
7330 {
7331   int pos;
7332 
7333   // Cursor position: col + 80*row.
7334   outb(CRTPORT, 14);
7335   pos = inb(CRTPORT+1) << 8;
7336   outb(CRTPORT, 15);
7337   pos |= inb(CRTPORT+1);
7338 
7339   if(c == '\n')
7340     pos += 80 - pos%80;
7341   else if(c == BACKSPACE){
7342     if(pos > 0) --pos;
7343   } else
7344     crt[pos++] = (c&0xff) | 0x0700;  // black on white
7345 
7346   if(pos < 0 || pos > 25*80)
7347     panic("pos under/overflow");
7348 
7349 
7350   if((pos/80) >= 24){  // Scroll up.
7351     memmove(crt, crt+80, sizeof(crt[0])*23*80);
7352     pos -= 80;
7353     memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
7354   }
7355 
7356   outb(CRTPORT, 14);
7357   outb(CRTPORT+1, pos>>8);
7358   outb(CRTPORT, 15);
7359   outb(CRTPORT+1, pos);
7360   crt[pos] = ' ' | 0x0700;
7361 }
7362 
7363 void
7364 consputc(int c)
7365 {
7366   if(panicked){
7367     cli();
7368     for(;;)
7369       ;
7370   }
7371 
7372   if(c == BACKSPACE){
7373     uartputc('\b'); uartputc(' '); uartputc('\b');
7374   } else
7375     uartputc(c);
7376   cgaputc(c);
7377 }
7378 
7379 #define INPUT_BUF 128
7380 struct {
7381   char buf[INPUT_BUF];
7382   uint r;  // Read index
7383   uint w;  // Write index
7384   uint e;  // Edit index
7385 } input;
7386 
7387 #define C(x)  ((x)-'@')  // Control-x
7388 
7389 void
7390 consoleintr(int (*getc)(void))
7391 {
7392   int c, doprocdump = 0;
7393 
7394   acquire(&cons.lock);
7395   while((c = getc()) >= 0){
7396     switch(c){
7397     case C('P'):  // Process listing.
7398       // procdump() locks cons.lock indirectly; invoke later
7399       doprocdump = 1;
7400       break;
7401     case C('U'):  // Kill line.
7402       while(input.e != input.w &&
7403             input.buf[(input.e-1) % INPUT_BUF] != '\n'){
7404         input.e--;
7405         consputc(BACKSPACE);
7406       }
7407       break;
7408     case C('H'): case '\x7f':  // Backspace
7409       if(input.e != input.w){
7410         input.e--;
7411         consputc(BACKSPACE);
7412       }
7413       break;
7414     default:
7415       if(c != 0 && input.e-input.r < INPUT_BUF){
7416         c = (c == '\r') ? '\n' : c;
7417         input.buf[input.e++ % INPUT_BUF] = c;
7418         consputc(c);
7419         if(c == '\n' || c == C('D') || input.e == input.r+INPUT_BUF){
7420           input.w = input.e;
7421           wakeup(&input.r);
7422         }
7423       }
7424       break;
7425     }
7426   }
7427   release(&cons.lock);
7428   if(doprocdump) {
7429     procdump();  // now call procdump() wo. cons.lock held
7430   }
7431 }
7432 
7433 int
7434 consoleread(struct inode *ip, char *dst, int n)
7435 {
7436   uint target;
7437   int c;
7438 
7439   iunlock(ip);
7440   target = n;
7441   acquire(&cons.lock);
7442   while(n > 0){
7443     while(input.r == input.w){
7444       if(myproc()->killed){
7445         release(&cons.lock);
7446         ilock(ip);
7447         return -1;
7448       }
7449       sleep(&input.r, &cons.lock);
7450     }
7451     c = input.buf[input.r++ % INPUT_BUF];
7452     if(c == C('D')){  // EOF
7453       if(n < target){
7454         // Save ^D for next time, to make sure
7455         // caller gets a 0-byte result.
7456         input.r--;
7457       }
7458       break;
7459     }
7460     *dst++ = c;
7461     --n;
7462     if(c == '\n')
7463       break;
7464   }
7465   release(&cons.lock);
7466   ilock(ip);
7467 
7468   return target - n;
7469 }
7470 
7471 int
7472 consolewrite(struct inode *ip, char *buf, int n)
7473 {
7474   int i;
7475 
7476   iunlock(ip);
7477   acquire(&cons.lock);
7478   for(i = 0; i < n; i++)
7479     consputc(buf[i] & 0xff);
7480   release(&cons.lock);
7481   ilock(ip);
7482 
7483   return n;
7484 }
7485 
7486 void
7487 consoleinit(void)
7488 {
7489   initlock(&cons.lock, "console");
7490 
7491   devsw[CONSOLE].write = consolewrite;
7492   devsw[CONSOLE].read = consoleread;
7493   cons.locking = 1;
7494 
7495   ioapicenable(IRQ_KBD, 0);
7496 }
7497 
7498 
7499 
