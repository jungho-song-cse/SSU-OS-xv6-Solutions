7700 // init: The initial user-level program
7701 
7702 #include "types.h"
7703 #include "stat.h"
7704 #include "user.h"
7705 #include "fcntl.h"
7706 
7707 char *argv[] = { "sh", 0 };
7708 
7709 int
7710 main(void)
7711 {
7712   int pid, wpid;
7713 
7714   if(open("console", O_RDWR) < 0){
7715     mknod("console", 1, 1);
7716     open("console", O_RDWR);
7717   }
7718   dup(0);  // stdout
7719   dup(0);  // stderr
7720 
7721   for(;;){
7722     //printf(1, "init: starting sh\n");
7723     printf(1, "init: starting login\n");
7724     pid = fork();
7725     if(pid < 0){
7726       printf(1, "init: fork failed\n");
7727       exit();
7728     }
7729     if(pid == 0){
7730       exec("ssu_login",argv);
7731       //exec("sh", argv);
7732       //printf(1, "init: exec sh failed\n");
7733       printf(1, "init: exec ssu_login failed\n");
7734       exit();
7735     }
7736     while((wpid=wait()) >= 0 && wpid != pid)
7737       printf(1, "zombie!\n");
7738   }
7739 }
7740 
7741 
7742 
7743 
7744 
7745 
7746 
7747 
7748 
7749 
