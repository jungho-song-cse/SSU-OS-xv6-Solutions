7750 // Shell.
7751 
7752 #include "types.h"
7753 #include "user.h"
7754 #include "fcntl.h"
7755 
7756 // Parsed command representation
7757 #define EXEC  1
7758 #define REDIR 2
7759 #define PIPE  3
7760 #define LIST  4
7761 #define BACK  5
7762 
7763 #define MAXARGS 10
7764 
7765 struct cmd {
7766   int type;
7767 };
7768 
7769 struct execcmd {
7770   int type;
7771   char *argv[MAXARGS];
7772   char *eargv[MAXARGS];
7773 };
7774 
7775 struct redircmd {
7776   int type;
7777   struct cmd *cmd;
7778   char *file;
7779   char *efile;
7780   int mode;
7781   int fd;
7782 };
7783 
7784 struct pipecmd {
7785   int type;
7786   struct cmd *left;
7787   struct cmd *right;
7788 };
7789 
7790 struct listcmd {
7791   int type;
7792   struct cmd *left;
7793   struct cmd *right;
7794 };
7795 
7796 struct backcmd {
7797   int type;
7798   struct cmd *cmd;
7799 };
7800 int fork1(void);  // Fork but panics on failure.
7801 void panic(char*);
7802 struct cmd *parsecmd(char*);
7803 
7804 // Execute cmd.  Never returns.
7805 void
7806 runcmd(struct cmd *cmd)
7807 {
7808   int p[2];
7809   struct backcmd *bcmd;
7810   struct execcmd *ecmd;
7811   struct listcmd *lcmd;
7812   struct pipecmd *pcmd;
7813   struct redircmd *rcmd;
7814 
7815   if(cmd == 0)
7816     exit();
7817 
7818   switch(cmd->type){
7819   default:
7820     panic("runcmd");
7821 
7822   case EXEC:
7823     ecmd = (struct execcmd*)cmd;
7824     if(ecmd->argv[0] == 0)
7825       exit();
7826     exec(ecmd->argv[0], ecmd->argv);
7827     printf(2, "exec %s failed\n", ecmd->argv[0]);
7828     break;
7829 
7830   case REDIR:
7831     rcmd = (struct redircmd*)cmd;
7832     close(rcmd->fd);
7833     if(open(rcmd->file, rcmd->mode) < 0){
7834       printf(2, "open %s failed\n", rcmd->file);
7835       exit();
7836     }
7837     runcmd(rcmd->cmd);
7838     break;
7839 
7840   case LIST:
7841     lcmd = (struct listcmd*)cmd;
7842     if(fork1() == 0)
7843       runcmd(lcmd->left);
7844     wait();
7845     runcmd(lcmd->right);
7846     break;
7847 
7848 
7849 
7850   case PIPE:
7851     pcmd = (struct pipecmd*)cmd;
7852     if(pipe(p) < 0)
7853       panic("pipe");
7854     if(fork1() == 0){
7855       close(1);
7856       dup(p[1]);
7857       close(p[0]);
7858       close(p[1]);
7859       runcmd(pcmd->left);
7860     }
7861     if(fork1() == 0){
7862       close(0);
7863       dup(p[0]);
7864       close(p[0]);
7865       close(p[1]);
7866       runcmd(pcmd->right);
7867     }
7868     close(p[0]);
7869     close(p[1]);
7870     wait();
7871     wait();
7872     break;
7873 
7874   case BACK:
7875     bcmd = (struct backcmd*)cmd;
7876     if(fork1() == 0)
7877       runcmd(bcmd->cmd);
7878     break;
7879   }
7880   exit();
7881 }
7882 
7883 int
7884 getcmd(char *buf, int nbuf)
7885 {
7886   printf(2, "$ ");
7887   memset(buf, 0, nbuf);
7888   gets(buf, nbuf);
7889   if(buf[0] == 0) // EOF
7890     return -1;
7891   return 0;
7892 }
7893 
7894 
7895 
7896 
7897 
7898 
7899 
7900 int
7901 main(void)
7902 {
7903   static char buf[100];
7904   int fd;
7905 
7906   // Ensure that three file descriptors are open.
7907   while((fd = open("console", O_RDWR)) >= 0){
7908     if(fd >= 3){
7909       close(fd);
7910       break;
7911     }
7912   }
7913 
7914   // Read and run input commands.
7915   while(getcmd(buf, sizeof(buf)) >= 0){
7916     if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
7917       // Chdir must be called by the parent, not the child.
7918       buf[strlen(buf)-1] = 0;  // chop \n
7919       if(chdir(buf+3) < 0)
7920         printf(2, "cannot cd %s\n", buf+3);
7921       continue;
7922     }
7923     if(fork1() == 0)
7924       runcmd(parsecmd(buf));
7925     wait();
7926   }
7927   exit();
7928 }
7929 
7930 void
7931 panic(char *s)
7932 {
7933   printf(2, "%s\n", s);
7934   exit();
7935 }
7936 
7937 int
7938 fork1(void)
7939 {
7940   int pid;
7941 
7942   pid = fork();
7943   if(pid == -1)
7944     panic("fork");
7945   return pid;
7946 }
7947 
7948 
7949 
7950 // Constructors
7951 
7952 struct cmd*
7953 execcmd(void)
7954 {
7955   struct execcmd *cmd;
7956 
7957   cmd = malloc(sizeof(*cmd));
7958   memset(cmd, 0, sizeof(*cmd));
7959   cmd->type = EXEC;
7960   return (struct cmd*)cmd;
7961 }
7962 
7963 struct cmd*
7964 redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
7965 {
7966   struct redircmd *cmd;
7967 
7968   cmd = malloc(sizeof(*cmd));
7969   memset(cmd, 0, sizeof(*cmd));
7970   cmd->type = REDIR;
7971   cmd->cmd = subcmd;
7972   cmd->file = file;
7973   cmd->efile = efile;
7974   cmd->mode = mode;
7975   cmd->fd = fd;
7976   return (struct cmd*)cmd;
7977 }
7978 
7979 struct cmd*
7980 pipecmd(struct cmd *left, struct cmd *right)
7981 {
7982   struct pipecmd *cmd;
7983 
7984   cmd = malloc(sizeof(*cmd));
7985   memset(cmd, 0, sizeof(*cmd));
7986   cmd->type = PIPE;
7987   cmd->left = left;
7988   cmd->right = right;
7989   return (struct cmd*)cmd;
7990 }
7991 
7992 
7993 
7994 
7995 
7996 
7997 
7998 
7999 
8000 struct cmd*
8001 listcmd(struct cmd *left, struct cmd *right)
8002 {
8003   struct listcmd *cmd;
8004 
8005   cmd = malloc(sizeof(*cmd));
8006   memset(cmd, 0, sizeof(*cmd));
8007   cmd->type = LIST;
8008   cmd->left = left;
8009   cmd->right = right;
8010   return (struct cmd*)cmd;
8011 }
8012 
8013 struct cmd*
8014 backcmd(struct cmd *subcmd)
8015 {
8016   struct backcmd *cmd;
8017 
8018   cmd = malloc(sizeof(*cmd));
8019   memset(cmd, 0, sizeof(*cmd));
8020   cmd->type = BACK;
8021   cmd->cmd = subcmd;
8022   return (struct cmd*)cmd;
8023 }
8024 // Parsing
8025 
8026 char whitespace[] = " \t\r\n\v";
8027 char symbols[] = "<|>&;()";
8028 
8029 int
8030 gettoken(char **ps, char *es, char **q, char **eq)
8031 {
8032   char *s;
8033   int ret;
8034 
8035   s = *ps;
8036   while(s < es && strchr(whitespace, *s))
8037     s++;
8038   if(q)
8039     *q = s;
8040   ret = *s;
8041   switch(*s){
8042   case 0:
8043     break;
8044   case '|':
8045   case '(':
8046   case ')':
8047   case ';':
8048   case '&':
8049   case '<':
8050     s++;
8051     break;
8052   case '>':
8053     s++;
8054     if(*s == '>'){
8055       ret = '+';
8056       s++;
8057     }
8058     break;
8059   default:
8060     ret = 'a';
8061     while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
8062       s++;
8063     break;
8064   }
8065   if(eq)
8066     *eq = s;
8067 
8068   while(s < es && strchr(whitespace, *s))
8069     s++;
8070   *ps = s;
8071   return ret;
8072 }
8073 
8074 int
8075 peek(char **ps, char *es, char *toks)
8076 {
8077   char *s;
8078 
8079   s = *ps;
8080   while(s < es && strchr(whitespace, *s))
8081     s++;
8082   *ps = s;
8083   return *s && strchr(toks, *s);
8084 }
8085 
8086 
8087 
8088 
8089 
8090 
8091 
8092 
8093 
8094 
8095 
8096 
8097 
8098 
8099 
8100 struct cmd *parseline(char**, char*);
8101 struct cmd *parsepipe(char**, char*);
8102 struct cmd *parseexec(char**, char*);
8103 struct cmd *nulterminate(struct cmd*);
8104 
8105 struct cmd*
8106 parsecmd(char *s)
8107 {
8108   char *es;
8109   struct cmd *cmd;
8110 
8111   es = s + strlen(s);
8112   cmd = parseline(&s, es);
8113   peek(&s, es, "");
8114   if(s != es){
8115     printf(2, "leftovers: %s\n", s);
8116     panic("syntax");
8117   }
8118   nulterminate(cmd);
8119   return cmd;
8120 }
8121 
8122 struct cmd*
8123 parseline(char **ps, char *es)
8124 {
8125   struct cmd *cmd;
8126 
8127   cmd = parsepipe(ps, es);
8128   while(peek(ps, es, "&")){
8129     gettoken(ps, es, 0, 0);
8130     cmd = backcmd(cmd);
8131   }
8132   if(peek(ps, es, ";")){
8133     gettoken(ps, es, 0, 0);
8134     cmd = listcmd(cmd, parseline(ps, es));
8135   }
8136   return cmd;
8137 }
8138 
8139 
8140 
8141 
8142 
8143 
8144 
8145 
8146 
8147 
8148 
8149 
8150 struct cmd*
8151 parsepipe(char **ps, char *es)
8152 {
8153   struct cmd *cmd;
8154 
8155   cmd = parseexec(ps, es);
8156   if(peek(ps, es, "|")){
8157     gettoken(ps, es, 0, 0);
8158     cmd = pipecmd(cmd, parsepipe(ps, es));
8159   }
8160   return cmd;
8161 }
8162 
8163 struct cmd*
8164 parseredirs(struct cmd *cmd, char **ps, char *es)
8165 {
8166   int tok;
8167   char *q, *eq;
8168 
8169   while(peek(ps, es, "<>")){
8170     tok = gettoken(ps, es, 0, 0);
8171     if(gettoken(ps, es, &q, &eq) != 'a')
8172       panic("missing file for redirection");
8173     switch(tok){
8174     case '<':
8175       cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
8176       break;
8177     case '>':
8178       cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE, 1);
8179       break;
8180     case '+':  // >>
8181       cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE, 1);
8182       break;
8183     }
8184   }
8185   return cmd;
8186 }
8187 
8188 
8189 
8190 
8191 
8192 
8193 
8194 
8195 
8196 
8197 
8198 
8199 
8200 struct cmd*
8201 parseblock(char **ps, char *es)
8202 {
8203   struct cmd *cmd;
8204 
8205   if(!peek(ps, es, "("))
8206     panic("parseblock");
8207   gettoken(ps, es, 0, 0);
8208   cmd = parseline(ps, es);
8209   if(!peek(ps, es, ")"))
8210     panic("syntax - missing )");
8211   gettoken(ps, es, 0, 0);
8212   cmd = parseredirs(cmd, ps, es);
8213   return cmd;
8214 }
8215 
8216 struct cmd*
8217 parseexec(char **ps, char *es)
8218 {
8219   char *q, *eq;
8220   int tok, argc;
8221   struct execcmd *cmd;
8222   struct cmd *ret;
8223 
8224   if(peek(ps, es, "("))
8225     return parseblock(ps, es);
8226 
8227   ret = execcmd();
8228   cmd = (struct execcmd*)ret;
8229 
8230   argc = 0;
8231   ret = parseredirs(ret, ps, es);
8232   while(!peek(ps, es, "|)&;")){
8233     if((tok=gettoken(ps, es, &q, &eq)) == 0)
8234       break;
8235     if(tok != 'a')
8236       panic("syntax");
8237     cmd->argv[argc] = q;
8238     cmd->eargv[argc] = eq;
8239     argc++;
8240     if(argc >= MAXARGS)
8241       panic("too many args");
8242     ret = parseredirs(ret, ps, es);
8243   }
8244   cmd->argv[argc] = 0;
8245   cmd->eargv[argc] = 0;
8246   return ret;
8247 }
8248 
8249 
8250 // NUL-terminate all the counted strings.
8251 struct cmd*
8252 nulterminate(struct cmd *cmd)
8253 {
8254   int i;
8255   struct backcmd *bcmd;
8256   struct execcmd *ecmd;
8257   struct listcmd *lcmd;
8258   struct pipecmd *pcmd;
8259   struct redircmd *rcmd;
8260 
8261   if(cmd == 0)
8262     return 0;
8263 
8264   switch(cmd->type){
8265   case EXEC:
8266     ecmd = (struct execcmd*)cmd;
8267     for(i=0; ecmd->argv[i]; i++)
8268       *ecmd->eargv[i] = 0;
8269     break;
8270 
8271   case REDIR:
8272     rcmd = (struct redircmd*)cmd;
8273     nulterminate(rcmd->cmd);
8274     *rcmd->efile = 0;
8275     break;
8276 
8277   case PIPE:
8278     pcmd = (struct pipecmd*)cmd;
8279     nulterminate(pcmd->left);
8280     nulterminate(pcmd->right);
8281     break;
8282 
8283   case LIST:
8284     lcmd = (struct listcmd*)cmd;
8285     nulterminate(lcmd->left);
8286     nulterminate(lcmd->right);
8287     break;
8288 
8289   case BACK:
8290     bcmd = (struct backcmd*)cmd;
8291     nulterminate(bcmd->cmd);
8292     break;
8293   }
8294   return cmd;
8295 }
8296 
8297 
8298 
8299 
