
_ssu_login:     file format elf32-i386


Disassembly of section .text:

00000000 <main>:
	}

	return i;
}

int main(int argc, char *argv[]){
   0:	f3 0f 1e fb          	endbr32 
   4:	8d 4c 24 04          	lea    0x4(%esp),%ecx
   8:	83 e4 f0             	and    $0xfffffff0,%esp
   b:	ff 71 fc             	pushl  -0x4(%ecx)
   e:	55                   	push   %ebp
   f:	89 e5                	mov    %esp,%ebp
  11:	56                   	push   %esi
  12:	53                   	push   %ebx
  13:	51                   	push   %ecx
  14:	83 ec 0c             	sub    $0xc,%esp
  17:	8b 71 04             	mov    0x4(%ecx),%esi
	int pid, wpid;
	get_user_list();
  1a:	e8 71 00 00 00       	call   90 <get_user_list>

	check_idpw();
  1f:	e8 cc 01 00 00       	call   1f0 <check_idpw>
  24:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

	for(;;){
		pid = fork();
  28:	e8 1e 05 00 00       	call   54b <fork>
  2d:	89 c3                	mov    %eax,%ebx
		if(pid < 0){
  2f:	85 c0                	test   %eax,%eax
  31:	78 26                	js     59 <main+0x59>
			printf(1, "init: fork failed\n");
			exit();
		}
		if(pid == 0){
  33:	74 37                	je     6c <main+0x6c>
  35:	8d 76 00             	lea    0x0(%esi),%esi
			exec("sh", argv);
			printf(1, "init: exec sh failed\n");
			exit();
		}
		while((wpid=wait()) >= 0 && wpid != pid)
  38:	e8 1e 05 00 00       	call   55b <wait>
  3d:	85 c0                	test   %eax,%eax
  3f:	78 e7                	js     28 <main+0x28>
  41:	39 c3                	cmp    %eax,%ebx
  43:	74 e3                	je     28 <main+0x28>
			printf(1, "zombie!\n");
  45:	83 ec 08             	sub    $0x8,%esp
  48:	68 70 0a 00 00       	push   $0xa70
  4d:	6a 01                	push   $0x1
  4f:	e8 5c 06 00 00       	call   6b0 <printf>
  54:	83 c4 10             	add    $0x10,%esp
  57:	eb df                	jmp    38 <main+0x38>
			printf(1, "init: fork failed\n");
  59:	53                   	push   %ebx
  5a:	53                   	push   %ebx
  5b:	68 44 0a 00 00       	push   $0xa44
  60:	6a 01                	push   $0x1
  62:	e8 49 06 00 00       	call   6b0 <printf>
			exit();
  67:	e8 e7 04 00 00       	call   553 <exit>
			exec("sh", argv);
  6c:	50                   	push   %eax
  6d:	50                   	push   %eax
  6e:	56                   	push   %esi
  6f:	68 57 0a 00 00       	push   $0xa57
  74:	e8 12 05 00 00       	call   58b <exec>
			printf(1, "init: exec sh failed\n");
  79:	5a                   	pop    %edx
  7a:	59                   	pop    %ecx
  7b:	68 5a 0a 00 00       	push   $0xa5a
  80:	6a 01                	push   $0x1
  82:	e8 29 06 00 00       	call   6b0 <printf>
			exit();
  87:	e8 c7 04 00 00       	call   553 <exit>
  8c:	66 90                	xchg   %ax,%ax
  8e:	66 90                	xchg   %ax,%ax

00000090 <get_user_list>:
void get_user_list(){
  90:	f3 0f 1e fb          	endbr32 
  94:	55                   	push   %ebp
  95:	89 e5                	mov    %esp,%ebp
  97:	57                   	push   %edi
  98:	56                   	push   %esi
  99:	53                   	push   %ebx
  9a:	83 ec 34             	sub    $0x34,%esp
	if((fd = open("list.txt", O_RDONLY))<0){
  9d:	6a 00                	push   $0x0
  9f:	68 18 0a 00 00       	push   $0xa18
  a4:	e8 ea 04 00 00       	call   593 <open>
  a9:	83 c4 10             	add    $0x10,%esp
  ac:	85 c0                	test   %eax,%eax
  ae:	0f 88 23 01 00 00    	js     1d7 <get_user_list+0x147>
	for(i=0; i<16; i++){
  b4:	c7 45 d0 00 00 00 00 	movl   $0x0,-0x30(%ebp)
  bb:	89 c3                	mov    %eax,%ebx
  bd:	31 ff                	xor    %edi,%edi
  bf:	8d 75 e7             	lea    -0x19(%ebp),%esi
  c2:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
		for(j=0; j<32; j++){
  c8:	31 d2                	xor    %edx,%edx
  ca:	eb 0c                	jmp    d8 <get_user_list+0x48>
  cc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  d0:	83 c2 01             	add    $0x1,%edx
  d3:	83 fa 20             	cmp    $0x20,%edx
  d6:	74 40                	je     118 <get_user_list+0x88>
			if((n=read(fd, buf, 1))<0){
  d8:	83 ec 04             	sub    $0x4,%esp
  db:	89 55 d4             	mov    %edx,-0x2c(%ebp)
  de:	6a 01                	push   $0x1
  e0:	56                   	push   %esi
  e1:	53                   	push   %ebx
  e2:	e8 84 04 00 00       	call   56b <read>
  e7:	83 c4 10             	add    $0x10,%esp
  ea:	8b 55 d4             	mov    -0x2c(%ebp),%edx
  ed:	85 c0                	test   %eax,%eax
  ef:	0f 88 ce 00 00 00    	js     1c3 <get_user_list+0x133>
			userID[i][j] = buf[0];
  f5:	0f b6 45 e7          	movzbl -0x19(%ebp),%eax
  f9:	88 84 17 60 0e 00 00 	mov    %al,0xe60(%edi,%edx,1)
			if(userID[i][j]==' '){
 100:	3c 20                	cmp    $0x20,%al
 102:	75 cc                	jne    d0 <get_user_list+0x40>
				userID[i][j]='\n';
 104:	8b 45 d0             	mov    -0x30(%ebp),%eax
 107:	c1 e0 05             	shl    $0x5,%eax
 10a:	c6 84 02 60 0e 00 00 	movb   $0xa,0xe60(%edx,%eax,1)
 111:	0a 
				break;
 112:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
			if((n=read(fd, buf, 1))<0){
 118:	83 ec 04             	sub    $0x4,%esp
 11b:	6a 01                	push   $0x1
 11d:	56                   	push   %esi
 11e:	53                   	push   %ebx
 11f:	e8 47 04 00 00       	call   56b <read>
 124:	83 c4 10             	add    $0x10,%esp
 127:	85 c0                	test   %eax,%eax
 129:	0f 88 94 00 00 00    	js     1c3 <get_user_list+0x133>
			if(buf[0]!=' '){
 12f:	0f b6 45 e7          	movzbl -0x19(%ebp),%eax
 133:	3c 20                	cmp    $0x20,%al
 135:	74 e1                	je     118 <get_user_list+0x88>
				pwdID[i][0] = buf[0];
 137:	88 87 60 10 00 00    	mov    %al,0x1060(%edi)
		for(j=1; j<32; j++){
 13d:	ba 01 00 00 00       	mov    $0x1,%edx
 142:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
			if((n=read(fd, buf, 1))<0){
 148:	83 ec 04             	sub    $0x4,%esp
 14b:	89 55 d4             	mov    %edx,-0x2c(%ebp)
 14e:	6a 01                	push   $0x1
 150:	56                   	push   %esi
 151:	53                   	push   %ebx
 152:	e8 14 04 00 00       	call   56b <read>
 157:	83 c4 10             	add    $0x10,%esp
 15a:	85 c0                	test   %eax,%eax
 15c:	78 65                	js     1c3 <get_user_list+0x133>
			pwdID[i][j] = buf[0];
 15e:	0f b6 45 e7          	movzbl -0x19(%ebp),%eax
 162:	8b 55 d4             	mov    -0x2c(%ebp),%edx
 165:	88 84 17 60 10 00 00 	mov    %al,0x1060(%edi,%edx,1)
			if(pwdID[i][j]=='\n'||pwdID[i][j]==' '){
 16c:	3c 0a                	cmp    $0xa,%al
 16e:	74 30                	je     1a0 <get_user_list+0x110>
 170:	3c 20                	cmp    $0x20,%al
 172:	74 2c                	je     1a0 <get_user_list+0x110>
		for(j=1; j<32; j++){
 174:	83 c2 01             	add    $0x1,%edx
 177:	83 fa 20             	cmp    $0x20,%edx
 17a:	75 cc                	jne    148 <get_user_list+0xb8>
	for(i=0; i<16; i++){
 17c:	83 45 d0 01          	addl   $0x1,-0x30(%ebp)
 180:	8b 45 d0             	mov    -0x30(%ebp),%eax
 183:	83 c7 20             	add    $0x20,%edi
 186:	83 f8 10             	cmp    $0x10,%eax
 189:	0f 85 39 ff ff ff    	jne    c8 <get_user_list+0x38>
}
 18f:	8d 65 f4             	lea    -0xc(%ebp),%esp
 192:	5b                   	pop    %ebx
 193:	5e                   	pop    %esi
 194:	5f                   	pop    %edi
 195:	5d                   	pop    %ebp
 196:	c3                   	ret    
 197:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 19e:	66 90                	xchg   %ax,%ax
				pwdID[i][j]='\n';
 1a0:	8b 45 d0             	mov    -0x30(%ebp),%eax
	for(i=0; i<16; i++){
 1a3:	83 45 d0 01          	addl   $0x1,-0x30(%ebp)
 1a7:	83 c7 20             	add    $0x20,%edi
				pwdID[i][j]='\n';
 1aa:	c1 e0 05             	shl    $0x5,%eax
 1ad:	c6 84 02 60 10 00 00 	movb   $0xa,0x1060(%edx,%eax,1)
 1b4:	0a 
	for(i=0; i<16; i++){
 1b5:	8b 45 d0             	mov    -0x30(%ebp),%eax
 1b8:	83 f8 10             	cmp    $0x10,%eax
 1bb:	0f 85 07 ff ff ff    	jne    c8 <get_user_list+0x38>
 1c1:	eb cc                	jmp    18f <get_user_list+0xff>
				printf(2, "read ERROR!\n");
 1c3:	83 ec 08             	sub    $0x8,%esp
 1c6:	68 21 0a 00 00       	push   $0xa21
 1cb:	6a 02                	push   $0x2
 1cd:	e8 de 04 00 00       	call   6b0 <printf>
				exit();
 1d2:	e8 7c 03 00 00       	call   553 <exit>
		printf(2, "file open ERROR! check list.txt file\n");
 1d7:	50                   	push   %eax
 1d8:	50                   	push   %eax
 1d9:	68 7c 0a 00 00       	push   $0xa7c
 1de:	6a 02                	push   $0x2
 1e0:	e8 cb 04 00 00       	call   6b0 <printf>
		exit();
 1e5:	e8 69 03 00 00       	call   553 <exit>
 1ea:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

000001f0 <check_idpw>:
int check_idpw(){
 1f0:	f3 0f 1e fb          	endbr32 
 1f4:	55                   	push   %ebp
 1f5:	89 e5                	mov    %esp,%ebp
 1f7:	57                   	push   %edi
 1f8:	56                   	push   %esi
 1f9:	53                   	push   %ebx
 1fa:	81 ec dc 00 00 00    	sub    $0xdc,%esp
	if(count==13){
 200:	83 3d 40 0e 00 00 0d 	cmpl   $0xd,0xe40
 207:	0f 84 c1 00 00 00    	je     2ce <check_idpw+0xde>
	printf(1,"Username: ");
 20d:	83 ec 08             	sub    $0x8,%esp
	gets(usernameInput, 31);
 210:	8d bd 20 ff ff ff    	lea    -0xe0(%ebp),%edi
	for(i=0; i<16; i++){
 216:	31 db                	xor    %ebx,%ebx
	printf(1,"Username: ");
 218:	68 2e 0a 00 00       	push   $0xa2e
 21d:	6a 01                	push   $0x1
 21f:	e8 8c 04 00 00       	call   6b0 <printf>
	gets(usernameInput, 31);
 224:	58                   	pop    %eax
 225:	5a                   	pop    %edx
 226:	6a 1f                	push   $0x1f
 228:	57                   	push   %edi
 229:	e8 e2 01 00 00       	call   410 <gets>
 22e:	83 c4 10             	add    $0x10,%esp
 231:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
		if(strcmp(usernameInput, userID[i])==0){
 238:	89 de                	mov    %ebx,%esi
 23a:	83 ec 08             	sub    $0x8,%esp
 23d:	c1 e6 05             	shl    $0x5,%esi
 240:	8d 86 60 0e 00 00    	lea    0xe60(%esi),%eax
 246:	50                   	push   %eax
 247:	57                   	push   %edi
 248:	e8 d3 00 00 00       	call   320 <strcmp>
 24d:	83 c4 10             	add    $0x10,%esp
 250:	85 c0                	test   %eax,%eax
 252:	74 34                	je     288 <check_idpw+0x98>
	for(i=0; i<16; i++){
 254:	83 c3 01             	add    $0x1,%ebx
 257:	83 fb 10             	cmp    $0x10,%ebx
 25a:	75 dc                	jne    238 <check_idpw+0x48>
		printf(2,"##There is no Such name You've Entered!!!##\n\n");
 25c:	83 ec 08             	sub    $0x8,%esp
 25f:	68 f8 0a 00 00       	push   $0xaf8
 264:	6a 02                	push   $0x2
		return -1;
 266:	bb ff ff ff ff       	mov    $0xffffffff,%ebx
		printf(2,"##There is no Such name You've Entered!!!##\n\n");
 26b:	e8 40 04 00 00       	call   6b0 <printf>
		count++;
 270:	83 05 40 0e 00 00 01 	addl   $0x1,0xe40
		check_idpw();
 277:	e8 74 ff ff ff       	call   1f0 <check_idpw>
		return -1;
 27c:	83 c4 10             	add    $0x10,%esp
 27f:	eb 39                	jmp    2ba <check_idpw+0xca>
 281:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
		printf(1,"Password: ");
 288:	83 ec 08             	sub    $0x8,%esp
		gets(passwordInput, 31);
 28b:	8d 7d 84             	lea    -0x7c(%ebp),%edi
		if(strcmp(passwordInput, pwdID[i])!=0){
 28e:	81 c6 60 10 00 00    	add    $0x1060,%esi
		printf(1,"Password: ");
 294:	68 39 0a 00 00       	push   $0xa39
 299:	6a 01                	push   $0x1
 29b:	e8 10 04 00 00       	call   6b0 <printf>
		gets(passwordInput, 31);
 2a0:	58                   	pop    %eax
 2a1:	5a                   	pop    %edx
 2a2:	6a 1f                	push   $0x1f
 2a4:	57                   	push   %edi
 2a5:	e8 66 01 00 00       	call   410 <gets>
		if(strcmp(passwordInput, pwdID[i])!=0){
 2aa:	59                   	pop    %ecx
 2ab:	58                   	pop    %eax
 2ac:	56                   	push   %esi
 2ad:	57                   	push   %edi
 2ae:	e8 6d 00 00 00       	call   320 <strcmp>
 2b3:	83 c4 10             	add    $0x10,%esp
 2b6:	85 c0                	test   %eax,%eax
 2b8:	75 0a                	jne    2c4 <check_idpw+0xd4>
}
 2ba:	8d 65 f4             	lea    -0xc(%ebp),%esp
 2bd:	89 d8                	mov    %ebx,%eax
 2bf:	5b                   	pop    %ebx
 2c0:	5e                   	pop    %esi
 2c1:	5f                   	pop    %edi
 2c2:	5d                   	pop    %ebp
 2c3:	c3                   	ret    
			printf(2,"Username / Password does not MATCH!!!\n\n");
 2c4:	83 ec 08             	sub    $0x8,%esp
 2c7:	68 d0 0a 00 00       	push   $0xad0
 2cc:	eb 96                	jmp    264 <check_idpw+0x74>
		printf(2,"Too much input.. Reinitializing shell..\n");
 2ce:	51                   	push   %ecx
 2cf:	51                   	push   %ecx
 2d0:	68 a4 0a 00 00       	push   $0xaa4
 2d5:	6a 02                	push   $0x2
 2d7:	e8 d4 03 00 00       	call   6b0 <printf>
		exit();
 2dc:	e8 72 02 00 00       	call   553 <exit>
 2e1:	66 90                	xchg   %ax,%ax
 2e3:	66 90                	xchg   %ax,%ax
 2e5:	66 90                	xchg   %ax,%ax
 2e7:	66 90                	xchg   %ax,%ax
 2e9:	66 90                	xchg   %ax,%ax
 2eb:	66 90                	xchg   %ax,%ax
 2ed:	66 90                	xchg   %ax,%ax
 2ef:	90                   	nop

000002f0 <strcpy>:
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, const char *t)
{
 2f0:	f3 0f 1e fb          	endbr32 
 2f4:	55                   	push   %ebp
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 2f5:	31 c0                	xor    %eax,%eax
{
 2f7:	89 e5                	mov    %esp,%ebp
 2f9:	53                   	push   %ebx
 2fa:	8b 4d 08             	mov    0x8(%ebp),%ecx
 2fd:	8b 5d 0c             	mov    0xc(%ebp),%ebx
  while((*s++ = *t++) != 0)
 300:	0f b6 14 03          	movzbl (%ebx,%eax,1),%edx
 304:	88 14 01             	mov    %dl,(%ecx,%eax,1)
 307:	83 c0 01             	add    $0x1,%eax
 30a:	84 d2                	test   %dl,%dl
 30c:	75 f2                	jne    300 <strcpy+0x10>
    ;
  return os;
}
 30e:	89 c8                	mov    %ecx,%eax
 310:	5b                   	pop    %ebx
 311:	5d                   	pop    %ebp
 312:	c3                   	ret    
 313:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 31a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

00000320 <strcmp>:

int
strcmp(const char *p, const char *q)
{
 320:	f3 0f 1e fb          	endbr32 
 324:	55                   	push   %ebp
 325:	89 e5                	mov    %esp,%ebp
 327:	53                   	push   %ebx
 328:	8b 4d 08             	mov    0x8(%ebp),%ecx
 32b:	8b 55 0c             	mov    0xc(%ebp),%edx
  while(*p && *p == *q)
 32e:	0f b6 01             	movzbl (%ecx),%eax
 331:	0f b6 1a             	movzbl (%edx),%ebx
 334:	84 c0                	test   %al,%al
 336:	75 19                	jne    351 <strcmp+0x31>
 338:	eb 26                	jmp    360 <strcmp+0x40>
 33a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 340:	0f b6 41 01          	movzbl 0x1(%ecx),%eax
    p++, q++;
 344:	83 c1 01             	add    $0x1,%ecx
 347:	83 c2 01             	add    $0x1,%edx
  while(*p && *p == *q)
 34a:	0f b6 1a             	movzbl (%edx),%ebx
 34d:	84 c0                	test   %al,%al
 34f:	74 0f                	je     360 <strcmp+0x40>
 351:	38 d8                	cmp    %bl,%al
 353:	74 eb                	je     340 <strcmp+0x20>
  return (uchar)*p - (uchar)*q;
 355:	29 d8                	sub    %ebx,%eax
}
 357:	5b                   	pop    %ebx
 358:	5d                   	pop    %ebp
 359:	c3                   	ret    
 35a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 360:	31 c0                	xor    %eax,%eax
  return (uchar)*p - (uchar)*q;
 362:	29 d8                	sub    %ebx,%eax
}
 364:	5b                   	pop    %ebx
 365:	5d                   	pop    %ebp
 366:	c3                   	ret    
 367:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 36e:	66 90                	xchg   %ax,%ax

00000370 <strlen>:

uint
strlen(const char *s)
{
 370:	f3 0f 1e fb          	endbr32 
 374:	55                   	push   %ebp
 375:	89 e5                	mov    %esp,%ebp
 377:	8b 55 08             	mov    0x8(%ebp),%edx
  int n;

  for(n = 0; s[n]; n++)
 37a:	80 3a 00             	cmpb   $0x0,(%edx)
 37d:	74 21                	je     3a0 <strlen+0x30>
 37f:	31 c0                	xor    %eax,%eax
 381:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 388:	83 c0 01             	add    $0x1,%eax
 38b:	80 3c 02 00          	cmpb   $0x0,(%edx,%eax,1)
 38f:	89 c1                	mov    %eax,%ecx
 391:	75 f5                	jne    388 <strlen+0x18>
    ;
  return n;
}
 393:	89 c8                	mov    %ecx,%eax
 395:	5d                   	pop    %ebp
 396:	c3                   	ret    
 397:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 39e:	66 90                	xchg   %ax,%ax
  for(n = 0; s[n]; n++)
 3a0:	31 c9                	xor    %ecx,%ecx
}
 3a2:	5d                   	pop    %ebp
 3a3:	89 c8                	mov    %ecx,%eax
 3a5:	c3                   	ret    
 3a6:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 3ad:	8d 76 00             	lea    0x0(%esi),%esi

000003b0 <memset>:

void*
memset(void *dst, int c, uint n)
{
 3b0:	f3 0f 1e fb          	endbr32 
 3b4:	55                   	push   %ebp
 3b5:	89 e5                	mov    %esp,%ebp
 3b7:	57                   	push   %edi
 3b8:	8b 55 08             	mov    0x8(%ebp),%edx
}

static inline void
stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
 3bb:	8b 4d 10             	mov    0x10(%ebp),%ecx
 3be:	8b 45 0c             	mov    0xc(%ebp),%eax
 3c1:	89 d7                	mov    %edx,%edi
 3c3:	fc                   	cld    
 3c4:	f3 aa                	rep stos %al,%es:(%edi)
  stosb(dst, c, n);
  return dst;
}
 3c6:	89 d0                	mov    %edx,%eax
 3c8:	5f                   	pop    %edi
 3c9:	5d                   	pop    %ebp
 3ca:	c3                   	ret    
 3cb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 3cf:	90                   	nop

000003d0 <strchr>:

char*
strchr(const char *s, char c)
{
 3d0:	f3 0f 1e fb          	endbr32 
 3d4:	55                   	push   %ebp
 3d5:	89 e5                	mov    %esp,%ebp
 3d7:	8b 45 08             	mov    0x8(%ebp),%eax
 3da:	0f b6 4d 0c          	movzbl 0xc(%ebp),%ecx
  for(; *s; s++)
 3de:	0f b6 10             	movzbl (%eax),%edx
 3e1:	84 d2                	test   %dl,%dl
 3e3:	75 16                	jne    3fb <strchr+0x2b>
 3e5:	eb 21                	jmp    408 <strchr+0x38>
 3e7:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 3ee:	66 90                	xchg   %ax,%ax
 3f0:	0f b6 50 01          	movzbl 0x1(%eax),%edx
 3f4:	83 c0 01             	add    $0x1,%eax
 3f7:	84 d2                	test   %dl,%dl
 3f9:	74 0d                	je     408 <strchr+0x38>
    if(*s == c)
 3fb:	38 d1                	cmp    %dl,%cl
 3fd:	75 f1                	jne    3f0 <strchr+0x20>
      return (char*)s;
  return 0;
}
 3ff:	5d                   	pop    %ebp
 400:	c3                   	ret    
 401:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  return 0;
 408:	31 c0                	xor    %eax,%eax
}
 40a:	5d                   	pop    %ebp
 40b:	c3                   	ret    
 40c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

00000410 <gets>:

char*
gets(char *buf, int max)
{
 410:	f3 0f 1e fb          	endbr32 
 414:	55                   	push   %ebp
 415:	89 e5                	mov    %esp,%ebp
 417:	57                   	push   %edi
 418:	56                   	push   %esi
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 419:	31 f6                	xor    %esi,%esi
{
 41b:	53                   	push   %ebx
 41c:	89 f3                	mov    %esi,%ebx
 41e:	83 ec 1c             	sub    $0x1c,%esp
 421:	8b 7d 08             	mov    0x8(%ebp),%edi
  for(i=0; i+1 < max; ){
 424:	eb 33                	jmp    459 <gets+0x49>
 426:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 42d:	8d 76 00             	lea    0x0(%esi),%esi
    cc = read(0, &c, 1);
 430:	83 ec 04             	sub    $0x4,%esp
 433:	8d 45 e7             	lea    -0x19(%ebp),%eax
 436:	6a 01                	push   $0x1
 438:	50                   	push   %eax
 439:	6a 00                	push   $0x0
 43b:	e8 2b 01 00 00       	call   56b <read>
    if(cc < 1)
 440:	83 c4 10             	add    $0x10,%esp
 443:	85 c0                	test   %eax,%eax
 445:	7e 1c                	jle    463 <gets+0x53>
      break;
    buf[i++] = c;
 447:	0f b6 45 e7          	movzbl -0x19(%ebp),%eax
 44b:	83 c7 01             	add    $0x1,%edi
 44e:	88 47 ff             	mov    %al,-0x1(%edi)
    if(c == '\n' || c == '\r')
 451:	3c 0a                	cmp    $0xa,%al
 453:	74 23                	je     478 <gets+0x68>
 455:	3c 0d                	cmp    $0xd,%al
 457:	74 1f                	je     478 <gets+0x68>
  for(i=0; i+1 < max; ){
 459:	83 c3 01             	add    $0x1,%ebx
 45c:	89 fe                	mov    %edi,%esi
 45e:	3b 5d 0c             	cmp    0xc(%ebp),%ebx
 461:	7c cd                	jl     430 <gets+0x20>
 463:	89 f3                	mov    %esi,%ebx
      break;
  }
  buf[i] = '\0';
  return buf;
}
 465:	8b 45 08             	mov    0x8(%ebp),%eax
  buf[i] = '\0';
 468:	c6 03 00             	movb   $0x0,(%ebx)
}
 46b:	8d 65 f4             	lea    -0xc(%ebp),%esp
 46e:	5b                   	pop    %ebx
 46f:	5e                   	pop    %esi
 470:	5f                   	pop    %edi
 471:	5d                   	pop    %ebp
 472:	c3                   	ret    
 473:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 477:	90                   	nop
 478:	8b 75 08             	mov    0x8(%ebp),%esi
 47b:	8b 45 08             	mov    0x8(%ebp),%eax
 47e:	01 de                	add    %ebx,%esi
 480:	89 f3                	mov    %esi,%ebx
  buf[i] = '\0';
 482:	c6 03 00             	movb   $0x0,(%ebx)
}
 485:	8d 65 f4             	lea    -0xc(%ebp),%esp
 488:	5b                   	pop    %ebx
 489:	5e                   	pop    %esi
 48a:	5f                   	pop    %edi
 48b:	5d                   	pop    %ebp
 48c:	c3                   	ret    
 48d:	8d 76 00             	lea    0x0(%esi),%esi

00000490 <stat>:

int
stat(const char *n, struct stat *st)
{
 490:	f3 0f 1e fb          	endbr32 
 494:	55                   	push   %ebp
 495:	89 e5                	mov    %esp,%ebp
 497:	56                   	push   %esi
 498:	53                   	push   %ebx
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 499:	83 ec 08             	sub    $0x8,%esp
 49c:	6a 00                	push   $0x0
 49e:	ff 75 08             	pushl  0x8(%ebp)
 4a1:	e8 ed 00 00 00       	call   593 <open>
  if(fd < 0)
 4a6:	83 c4 10             	add    $0x10,%esp
 4a9:	85 c0                	test   %eax,%eax
 4ab:	78 2b                	js     4d8 <stat+0x48>
    return -1;
  r = fstat(fd, st);
 4ad:	83 ec 08             	sub    $0x8,%esp
 4b0:	ff 75 0c             	pushl  0xc(%ebp)
 4b3:	89 c3                	mov    %eax,%ebx
 4b5:	50                   	push   %eax
 4b6:	e8 f0 00 00 00       	call   5ab <fstat>
  close(fd);
 4bb:	89 1c 24             	mov    %ebx,(%esp)
  r = fstat(fd, st);
 4be:	89 c6                	mov    %eax,%esi
  close(fd);
 4c0:	e8 b6 00 00 00       	call   57b <close>
  return r;
 4c5:	83 c4 10             	add    $0x10,%esp
}
 4c8:	8d 65 f8             	lea    -0x8(%ebp),%esp
 4cb:	89 f0                	mov    %esi,%eax
 4cd:	5b                   	pop    %ebx
 4ce:	5e                   	pop    %esi
 4cf:	5d                   	pop    %ebp
 4d0:	c3                   	ret    
 4d1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
    return -1;
 4d8:	be ff ff ff ff       	mov    $0xffffffff,%esi
 4dd:	eb e9                	jmp    4c8 <stat+0x38>
 4df:	90                   	nop

000004e0 <atoi>:

int
atoi(const char *s)
{
 4e0:	f3 0f 1e fb          	endbr32 
 4e4:	55                   	push   %ebp
 4e5:	89 e5                	mov    %esp,%ebp
 4e7:	53                   	push   %ebx
 4e8:	8b 55 08             	mov    0x8(%ebp),%edx
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
 4eb:	0f be 02             	movsbl (%edx),%eax
 4ee:	8d 48 d0             	lea    -0x30(%eax),%ecx
 4f1:	80 f9 09             	cmp    $0x9,%cl
  n = 0;
 4f4:	b9 00 00 00 00       	mov    $0x0,%ecx
  while('0' <= *s && *s <= '9')
 4f9:	77 1a                	ja     515 <atoi+0x35>
 4fb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 4ff:	90                   	nop
    n = n*10 + *s++ - '0';
 500:	83 c2 01             	add    $0x1,%edx
 503:	8d 0c 89             	lea    (%ecx,%ecx,4),%ecx
 506:	8d 4c 48 d0          	lea    -0x30(%eax,%ecx,2),%ecx
  while('0' <= *s && *s <= '9')
 50a:	0f be 02             	movsbl (%edx),%eax
 50d:	8d 58 d0             	lea    -0x30(%eax),%ebx
 510:	80 fb 09             	cmp    $0x9,%bl
 513:	76 eb                	jbe    500 <atoi+0x20>
  return n;
}
 515:	89 c8                	mov    %ecx,%eax
 517:	5b                   	pop    %ebx
 518:	5d                   	pop    %ebp
 519:	c3                   	ret    
 51a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

00000520 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 520:	f3 0f 1e fb          	endbr32 
 524:	55                   	push   %ebp
 525:	89 e5                	mov    %esp,%ebp
 527:	57                   	push   %edi
 528:	8b 45 10             	mov    0x10(%ebp),%eax
 52b:	8b 55 08             	mov    0x8(%ebp),%edx
 52e:	56                   	push   %esi
 52f:	8b 75 0c             	mov    0xc(%ebp),%esi
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
 532:	85 c0                	test   %eax,%eax
 534:	7e 0f                	jle    545 <memmove+0x25>
 536:	01 d0                	add    %edx,%eax
  dst = vdst;
 538:	89 d7                	mov    %edx,%edi
 53a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
    *dst++ = *src++;
 540:	a4                   	movsb  %ds:(%esi),%es:(%edi)
  while(n-- > 0)
 541:	39 f8                	cmp    %edi,%eax
 543:	75 fb                	jne    540 <memmove+0x20>
  return vdst;
}
 545:	5e                   	pop    %esi
 546:	89 d0                	mov    %edx,%eax
 548:	5f                   	pop    %edi
 549:	5d                   	pop    %ebp
 54a:	c3                   	ret    

0000054b <fork>:
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
 54b:	b8 01 00 00 00       	mov    $0x1,%eax
 550:	cd 40                	int    $0x40
 552:	c3                   	ret    

00000553 <exit>:
SYSCALL(exit)
 553:	b8 02 00 00 00       	mov    $0x2,%eax
 558:	cd 40                	int    $0x40
 55a:	c3                   	ret    

0000055b <wait>:
SYSCALL(wait)
 55b:	b8 03 00 00 00       	mov    $0x3,%eax
 560:	cd 40                	int    $0x40
 562:	c3                   	ret    

00000563 <pipe>:
SYSCALL(pipe)
 563:	b8 04 00 00 00       	mov    $0x4,%eax
 568:	cd 40                	int    $0x40
 56a:	c3                   	ret    

0000056b <read>:
SYSCALL(read)
 56b:	b8 05 00 00 00       	mov    $0x5,%eax
 570:	cd 40                	int    $0x40
 572:	c3                   	ret    

00000573 <write>:
SYSCALL(write)
 573:	b8 10 00 00 00       	mov    $0x10,%eax
 578:	cd 40                	int    $0x40
 57a:	c3                   	ret    

0000057b <close>:
SYSCALL(close)
 57b:	b8 15 00 00 00       	mov    $0x15,%eax
 580:	cd 40                	int    $0x40
 582:	c3                   	ret    

00000583 <kill>:
SYSCALL(kill)
 583:	b8 06 00 00 00       	mov    $0x6,%eax
 588:	cd 40                	int    $0x40
 58a:	c3                   	ret    

0000058b <exec>:
SYSCALL(exec)
 58b:	b8 07 00 00 00       	mov    $0x7,%eax
 590:	cd 40                	int    $0x40
 592:	c3                   	ret    

00000593 <open>:
SYSCALL(open)
 593:	b8 0f 00 00 00       	mov    $0xf,%eax
 598:	cd 40                	int    $0x40
 59a:	c3                   	ret    

0000059b <mknod>:
SYSCALL(mknod)
 59b:	b8 11 00 00 00       	mov    $0x11,%eax
 5a0:	cd 40                	int    $0x40
 5a2:	c3                   	ret    

000005a3 <unlink>:
SYSCALL(unlink)
 5a3:	b8 12 00 00 00       	mov    $0x12,%eax
 5a8:	cd 40                	int    $0x40
 5aa:	c3                   	ret    

000005ab <fstat>:
SYSCALL(fstat)
 5ab:	b8 08 00 00 00       	mov    $0x8,%eax
 5b0:	cd 40                	int    $0x40
 5b2:	c3                   	ret    

000005b3 <link>:
SYSCALL(link)
 5b3:	b8 13 00 00 00       	mov    $0x13,%eax
 5b8:	cd 40                	int    $0x40
 5ba:	c3                   	ret    

000005bb <mkdir>:
SYSCALL(mkdir)
 5bb:	b8 14 00 00 00       	mov    $0x14,%eax
 5c0:	cd 40                	int    $0x40
 5c2:	c3                   	ret    

000005c3 <chdir>:
SYSCALL(chdir)
 5c3:	b8 09 00 00 00       	mov    $0x9,%eax
 5c8:	cd 40                	int    $0x40
 5ca:	c3                   	ret    

000005cb <dup>:
SYSCALL(dup)
 5cb:	b8 0a 00 00 00       	mov    $0xa,%eax
 5d0:	cd 40                	int    $0x40
 5d2:	c3                   	ret    

000005d3 <getpid>:
SYSCALL(getpid)
 5d3:	b8 0b 00 00 00       	mov    $0xb,%eax
 5d8:	cd 40                	int    $0x40
 5da:	c3                   	ret    

000005db <sbrk>:
SYSCALL(sbrk)
 5db:	b8 0c 00 00 00       	mov    $0xc,%eax
 5e0:	cd 40                	int    $0x40
 5e2:	c3                   	ret    

000005e3 <sleep>:
SYSCALL(sleep)
 5e3:	b8 0d 00 00 00       	mov    $0xd,%eax
 5e8:	cd 40                	int    $0x40
 5ea:	c3                   	ret    

000005eb <uptime>:
SYSCALL(uptime)
 5eb:	b8 0e 00 00 00       	mov    $0xe,%eax
 5f0:	cd 40                	int    $0x40
 5f2:	c3                   	ret    
 5f3:	66 90                	xchg   %ax,%ax
 5f5:	66 90                	xchg   %ax,%ax
 5f7:	66 90                	xchg   %ax,%ax
 5f9:	66 90                	xchg   %ax,%ax
 5fb:	66 90                	xchg   %ax,%ax
 5fd:	66 90                	xchg   %ax,%ax
 5ff:	90                   	nop

00000600 <printint>:
  write(fd, &c, 1);
}

static void
printint(int fd, int xx, int base, int sgn)
{
 600:	55                   	push   %ebp
 601:	89 e5                	mov    %esp,%ebp
 603:	57                   	push   %edi
 604:	56                   	push   %esi
 605:	53                   	push   %ebx
 606:	83 ec 3c             	sub    $0x3c,%esp
 609:	89 4d c4             	mov    %ecx,-0x3c(%ebp)
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
 60c:	89 d1                	mov    %edx,%ecx
{
 60e:	89 45 b8             	mov    %eax,-0x48(%ebp)
  if(sgn && xx < 0){
 611:	85 d2                	test   %edx,%edx
 613:	0f 89 7f 00 00 00    	jns    698 <printint+0x98>
 619:	f6 45 08 01          	testb  $0x1,0x8(%ebp)
 61d:	74 79                	je     698 <printint+0x98>
    neg = 1;
 61f:	c7 45 bc 01 00 00 00 	movl   $0x1,-0x44(%ebp)
    x = -xx;
 626:	f7 d9                	neg    %ecx
  } else {
    x = xx;
  }

  i = 0;
 628:	31 db                	xor    %ebx,%ebx
 62a:	8d 75 d7             	lea    -0x29(%ebp),%esi
 62d:	8d 76 00             	lea    0x0(%esi),%esi
  do{
    buf[i++] = digits[x % base];
 630:	89 c8                	mov    %ecx,%eax
 632:	31 d2                	xor    %edx,%edx
 634:	89 cf                	mov    %ecx,%edi
 636:	f7 75 c4             	divl   -0x3c(%ebp)
 639:	0f b6 92 30 0b 00 00 	movzbl 0xb30(%edx),%edx
 640:	89 45 c0             	mov    %eax,-0x40(%ebp)
 643:	89 d8                	mov    %ebx,%eax
 645:	8d 5b 01             	lea    0x1(%ebx),%ebx
  }while((x /= base) != 0);
 648:	8b 4d c0             	mov    -0x40(%ebp),%ecx
    buf[i++] = digits[x % base];
 64b:	88 14 1e             	mov    %dl,(%esi,%ebx,1)
  }while((x /= base) != 0);
 64e:	39 7d c4             	cmp    %edi,-0x3c(%ebp)
 651:	76 dd                	jbe    630 <printint+0x30>
  if(neg)
 653:	8b 4d bc             	mov    -0x44(%ebp),%ecx
 656:	85 c9                	test   %ecx,%ecx
 658:	74 0c                	je     666 <printint+0x66>
    buf[i++] = '-';
 65a:	c6 44 1d d8 2d       	movb   $0x2d,-0x28(%ebp,%ebx,1)
    buf[i++] = digits[x % base];
 65f:	89 d8                	mov    %ebx,%eax
    buf[i++] = '-';
 661:	ba 2d 00 00 00       	mov    $0x2d,%edx

  while(--i >= 0)
 666:	8b 7d b8             	mov    -0x48(%ebp),%edi
 669:	8d 5c 05 d7          	lea    -0x29(%ebp,%eax,1),%ebx
 66d:	eb 07                	jmp    676 <printint+0x76>
 66f:	90                   	nop
 670:	0f b6 13             	movzbl (%ebx),%edx
 673:	83 eb 01             	sub    $0x1,%ebx
  write(fd, &c, 1);
 676:	83 ec 04             	sub    $0x4,%esp
 679:	88 55 d7             	mov    %dl,-0x29(%ebp)
 67c:	6a 01                	push   $0x1
 67e:	56                   	push   %esi
 67f:	57                   	push   %edi
 680:	e8 ee fe ff ff       	call   573 <write>
  while(--i >= 0)
 685:	83 c4 10             	add    $0x10,%esp
 688:	39 de                	cmp    %ebx,%esi
 68a:	75 e4                	jne    670 <printint+0x70>
    putc(fd, buf[i]);
}
 68c:	8d 65 f4             	lea    -0xc(%ebp),%esp
 68f:	5b                   	pop    %ebx
 690:	5e                   	pop    %esi
 691:	5f                   	pop    %edi
 692:	5d                   	pop    %ebp
 693:	c3                   	ret    
 694:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  neg = 0;
 698:	c7 45 bc 00 00 00 00 	movl   $0x0,-0x44(%ebp)
 69f:	eb 87                	jmp    628 <printint+0x28>
 6a1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 6a8:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 6af:	90                   	nop

000006b0 <printf>:

// Print to the given fd. Only understands %d, %x, %p, %s.
void
printf(int fd, const char *fmt, ...)
{
 6b0:	f3 0f 1e fb          	endbr32 
 6b4:	55                   	push   %ebp
 6b5:	89 e5                	mov    %esp,%ebp
 6b7:	57                   	push   %edi
 6b8:	56                   	push   %esi
 6b9:	53                   	push   %ebx
 6ba:	83 ec 2c             	sub    $0x2c,%esp
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 6bd:	8b 75 0c             	mov    0xc(%ebp),%esi
 6c0:	0f b6 1e             	movzbl (%esi),%ebx
 6c3:	84 db                	test   %bl,%bl
 6c5:	0f 84 b4 00 00 00    	je     77f <printf+0xcf>
  ap = (uint*)(void*)&fmt + 1;
 6cb:	8d 45 10             	lea    0x10(%ebp),%eax
 6ce:	83 c6 01             	add    $0x1,%esi
  write(fd, &c, 1);
 6d1:	8d 7d e7             	lea    -0x19(%ebp),%edi
  state = 0;
 6d4:	31 d2                	xor    %edx,%edx
  ap = (uint*)(void*)&fmt + 1;
 6d6:	89 45 d0             	mov    %eax,-0x30(%ebp)
 6d9:	eb 33                	jmp    70e <printf+0x5e>
 6db:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 6df:	90                   	nop
 6e0:	89 55 d4             	mov    %edx,-0x2c(%ebp)
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
 6e3:	ba 25 00 00 00       	mov    $0x25,%edx
      if(c == '%'){
 6e8:	83 f8 25             	cmp    $0x25,%eax
 6eb:	74 17                	je     704 <printf+0x54>
  write(fd, &c, 1);
 6ed:	83 ec 04             	sub    $0x4,%esp
 6f0:	88 5d e7             	mov    %bl,-0x19(%ebp)
 6f3:	6a 01                	push   $0x1
 6f5:	57                   	push   %edi
 6f6:	ff 75 08             	pushl  0x8(%ebp)
 6f9:	e8 75 fe ff ff       	call   573 <write>
 6fe:	8b 55 d4             	mov    -0x2c(%ebp),%edx
      } else {
        putc(fd, c);
 701:	83 c4 10             	add    $0x10,%esp
  for(i = 0; fmt[i]; i++){
 704:	0f b6 1e             	movzbl (%esi),%ebx
 707:	83 c6 01             	add    $0x1,%esi
 70a:	84 db                	test   %bl,%bl
 70c:	74 71                	je     77f <printf+0xcf>
    c = fmt[i] & 0xff;
 70e:	0f be cb             	movsbl %bl,%ecx
 711:	0f b6 c3             	movzbl %bl,%eax
    if(state == 0){
 714:	85 d2                	test   %edx,%edx
 716:	74 c8                	je     6e0 <printf+0x30>
      }
    } else if(state == '%'){
 718:	83 fa 25             	cmp    $0x25,%edx
 71b:	75 e7                	jne    704 <printf+0x54>
      if(c == 'd'){
 71d:	83 f8 64             	cmp    $0x64,%eax
 720:	0f 84 9a 00 00 00    	je     7c0 <printf+0x110>
        printint(fd, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
 726:	81 e1 f7 00 00 00    	and    $0xf7,%ecx
 72c:	83 f9 70             	cmp    $0x70,%ecx
 72f:	74 5f                	je     790 <printf+0xe0>
        printint(fd, *ap, 16, 0);
        ap++;
      } else if(c == 's'){
 731:	83 f8 73             	cmp    $0x73,%eax
 734:	0f 84 d6 00 00 00    	je     810 <printf+0x160>
          s = "(null)";
        while(*s != 0){
          putc(fd, *s);
          s++;
        }
      } else if(c == 'c'){
 73a:	83 f8 63             	cmp    $0x63,%eax
 73d:	0f 84 8d 00 00 00    	je     7d0 <printf+0x120>
        putc(fd, *ap);
        ap++;
      } else if(c == '%'){
 743:	83 f8 25             	cmp    $0x25,%eax
 746:	0f 84 b4 00 00 00    	je     800 <printf+0x150>
  write(fd, &c, 1);
 74c:	83 ec 04             	sub    $0x4,%esp
 74f:	c6 45 e7 25          	movb   $0x25,-0x19(%ebp)
 753:	6a 01                	push   $0x1
 755:	57                   	push   %edi
 756:	ff 75 08             	pushl  0x8(%ebp)
 759:	e8 15 fe ff ff       	call   573 <write>
        putc(fd, c);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
 75e:	88 5d e7             	mov    %bl,-0x19(%ebp)
  write(fd, &c, 1);
 761:	83 c4 0c             	add    $0xc,%esp
 764:	6a 01                	push   $0x1
 766:	83 c6 01             	add    $0x1,%esi
 769:	57                   	push   %edi
 76a:	ff 75 08             	pushl  0x8(%ebp)
 76d:	e8 01 fe ff ff       	call   573 <write>
  for(i = 0; fmt[i]; i++){
 772:	0f b6 5e ff          	movzbl -0x1(%esi),%ebx
        putc(fd, c);
 776:	83 c4 10             	add    $0x10,%esp
      }
      state = 0;
 779:	31 d2                	xor    %edx,%edx
  for(i = 0; fmt[i]; i++){
 77b:	84 db                	test   %bl,%bl
 77d:	75 8f                	jne    70e <printf+0x5e>
    }
  }
}
 77f:	8d 65 f4             	lea    -0xc(%ebp),%esp
 782:	5b                   	pop    %ebx
 783:	5e                   	pop    %esi
 784:	5f                   	pop    %edi
 785:	5d                   	pop    %ebp
 786:	c3                   	ret    
 787:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 78e:	66 90                	xchg   %ax,%ax
        printint(fd, *ap, 16, 0);
 790:	83 ec 0c             	sub    $0xc,%esp
 793:	b9 10 00 00 00       	mov    $0x10,%ecx
 798:	6a 00                	push   $0x0
 79a:	8b 5d d0             	mov    -0x30(%ebp),%ebx
 79d:	8b 45 08             	mov    0x8(%ebp),%eax
 7a0:	8b 13                	mov    (%ebx),%edx
 7a2:	e8 59 fe ff ff       	call   600 <printint>
        ap++;
 7a7:	89 d8                	mov    %ebx,%eax
 7a9:	83 c4 10             	add    $0x10,%esp
      state = 0;
 7ac:	31 d2                	xor    %edx,%edx
        ap++;
 7ae:	83 c0 04             	add    $0x4,%eax
 7b1:	89 45 d0             	mov    %eax,-0x30(%ebp)
 7b4:	e9 4b ff ff ff       	jmp    704 <printf+0x54>
 7b9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
        printint(fd, *ap, 10, 1);
 7c0:	83 ec 0c             	sub    $0xc,%esp
 7c3:	b9 0a 00 00 00       	mov    $0xa,%ecx
 7c8:	6a 01                	push   $0x1
 7ca:	eb ce                	jmp    79a <printf+0xea>
 7cc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
        putc(fd, *ap);
 7d0:	8b 5d d0             	mov    -0x30(%ebp),%ebx
  write(fd, &c, 1);
 7d3:	83 ec 04             	sub    $0x4,%esp
        putc(fd, *ap);
 7d6:	8b 03                	mov    (%ebx),%eax
  write(fd, &c, 1);
 7d8:	6a 01                	push   $0x1
        ap++;
 7da:	83 c3 04             	add    $0x4,%ebx
  write(fd, &c, 1);
 7dd:	57                   	push   %edi
 7de:	ff 75 08             	pushl  0x8(%ebp)
        putc(fd, *ap);
 7e1:	88 45 e7             	mov    %al,-0x19(%ebp)
  write(fd, &c, 1);
 7e4:	e8 8a fd ff ff       	call   573 <write>
        ap++;
 7e9:	89 5d d0             	mov    %ebx,-0x30(%ebp)
 7ec:	83 c4 10             	add    $0x10,%esp
      state = 0;
 7ef:	31 d2                	xor    %edx,%edx
 7f1:	e9 0e ff ff ff       	jmp    704 <printf+0x54>
 7f6:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 7fd:	8d 76 00             	lea    0x0(%esi),%esi
        putc(fd, c);
 800:	88 5d e7             	mov    %bl,-0x19(%ebp)
  write(fd, &c, 1);
 803:	83 ec 04             	sub    $0x4,%esp
 806:	e9 59 ff ff ff       	jmp    764 <printf+0xb4>
 80b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 80f:	90                   	nop
        s = (char*)*ap;
 810:	8b 45 d0             	mov    -0x30(%ebp),%eax
 813:	8b 18                	mov    (%eax),%ebx
        ap++;
 815:	83 c0 04             	add    $0x4,%eax
 818:	89 45 d0             	mov    %eax,-0x30(%ebp)
        if(s == 0)
 81b:	85 db                	test   %ebx,%ebx
 81d:	74 17                	je     836 <printf+0x186>
        while(*s != 0){
 81f:	0f b6 03             	movzbl (%ebx),%eax
      state = 0;
 822:	31 d2                	xor    %edx,%edx
        while(*s != 0){
 824:	84 c0                	test   %al,%al
 826:	0f 84 d8 fe ff ff    	je     704 <printf+0x54>
 82c:	89 75 d4             	mov    %esi,-0x2c(%ebp)
 82f:	89 de                	mov    %ebx,%esi
 831:	8b 5d 08             	mov    0x8(%ebp),%ebx
 834:	eb 1a                	jmp    850 <printf+0x1a0>
          s = "(null)";
 836:	bb 26 0b 00 00       	mov    $0xb26,%ebx
        while(*s != 0){
 83b:	89 75 d4             	mov    %esi,-0x2c(%ebp)
 83e:	b8 28 00 00 00       	mov    $0x28,%eax
 843:	89 de                	mov    %ebx,%esi
 845:	8b 5d 08             	mov    0x8(%ebp),%ebx
 848:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 84f:	90                   	nop
  write(fd, &c, 1);
 850:	83 ec 04             	sub    $0x4,%esp
          s++;
 853:	83 c6 01             	add    $0x1,%esi
 856:	88 45 e7             	mov    %al,-0x19(%ebp)
  write(fd, &c, 1);
 859:	6a 01                	push   $0x1
 85b:	57                   	push   %edi
 85c:	53                   	push   %ebx
 85d:	e8 11 fd ff ff       	call   573 <write>
        while(*s != 0){
 862:	0f b6 06             	movzbl (%esi),%eax
 865:	83 c4 10             	add    $0x10,%esp
 868:	84 c0                	test   %al,%al
 86a:	75 e4                	jne    850 <printf+0x1a0>
 86c:	8b 75 d4             	mov    -0x2c(%ebp),%esi
      state = 0;
 86f:	31 d2                	xor    %edx,%edx
 871:	e9 8e fe ff ff       	jmp    704 <printf+0x54>
 876:	66 90                	xchg   %ax,%ax
 878:	66 90                	xchg   %ax,%ax
 87a:	66 90                	xchg   %ax,%ax
 87c:	66 90                	xchg   %ax,%ax
 87e:	66 90                	xchg   %ax,%ax

00000880 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 880:	f3 0f 1e fb          	endbr32 
 884:	55                   	push   %ebp
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 885:	a1 44 0e 00 00       	mov    0xe44,%eax
{
 88a:	89 e5                	mov    %esp,%ebp
 88c:	57                   	push   %edi
 88d:	56                   	push   %esi
 88e:	53                   	push   %ebx
 88f:	8b 5d 08             	mov    0x8(%ebp),%ebx
 892:	8b 10                	mov    (%eax),%edx
  bp = (Header*)ap - 1;
 894:	8d 4b f8             	lea    -0x8(%ebx),%ecx
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 897:	39 c8                	cmp    %ecx,%eax
 899:	73 15                	jae    8b0 <free+0x30>
 89b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 89f:	90                   	nop
 8a0:	39 d1                	cmp    %edx,%ecx
 8a2:	72 14                	jb     8b8 <free+0x38>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8a4:	39 d0                	cmp    %edx,%eax
 8a6:	73 10                	jae    8b8 <free+0x38>
{
 8a8:	89 d0                	mov    %edx,%eax
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 8aa:	8b 10                	mov    (%eax),%edx
 8ac:	39 c8                	cmp    %ecx,%eax
 8ae:	72 f0                	jb     8a0 <free+0x20>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8b0:	39 d0                	cmp    %edx,%eax
 8b2:	72 f4                	jb     8a8 <free+0x28>
 8b4:	39 d1                	cmp    %edx,%ecx
 8b6:	73 f0                	jae    8a8 <free+0x28>
      break;
  if(bp + bp->s.size == p->s.ptr){
 8b8:	8b 73 fc             	mov    -0x4(%ebx),%esi
 8bb:	8d 3c f1             	lea    (%ecx,%esi,8),%edi
 8be:	39 fa                	cmp    %edi,%edx
 8c0:	74 1e                	je     8e0 <free+0x60>
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
 8c2:	89 53 f8             	mov    %edx,-0x8(%ebx)
  if(p + p->s.size == bp){
 8c5:	8b 50 04             	mov    0x4(%eax),%edx
 8c8:	8d 34 d0             	lea    (%eax,%edx,8),%esi
 8cb:	39 f1                	cmp    %esi,%ecx
 8cd:	74 28                	je     8f7 <free+0x77>
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
 8cf:	89 08                	mov    %ecx,(%eax)
  freep = p;
}
 8d1:	5b                   	pop    %ebx
  freep = p;
 8d2:	a3 44 0e 00 00       	mov    %eax,0xe44
}
 8d7:	5e                   	pop    %esi
 8d8:	5f                   	pop    %edi
 8d9:	5d                   	pop    %ebp
 8da:	c3                   	ret    
 8db:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 8df:	90                   	nop
    bp->s.size += p->s.ptr->s.size;
 8e0:	03 72 04             	add    0x4(%edx),%esi
 8e3:	89 73 fc             	mov    %esi,-0x4(%ebx)
    bp->s.ptr = p->s.ptr->s.ptr;
 8e6:	8b 10                	mov    (%eax),%edx
 8e8:	8b 12                	mov    (%edx),%edx
 8ea:	89 53 f8             	mov    %edx,-0x8(%ebx)
  if(p + p->s.size == bp){
 8ed:	8b 50 04             	mov    0x4(%eax),%edx
 8f0:	8d 34 d0             	lea    (%eax,%edx,8),%esi
 8f3:	39 f1                	cmp    %esi,%ecx
 8f5:	75 d8                	jne    8cf <free+0x4f>
    p->s.size += bp->s.size;
 8f7:	03 53 fc             	add    -0x4(%ebx),%edx
  freep = p;
 8fa:	a3 44 0e 00 00       	mov    %eax,0xe44
    p->s.size += bp->s.size;
 8ff:	89 50 04             	mov    %edx,0x4(%eax)
    p->s.ptr = bp->s.ptr;
 902:	8b 53 f8             	mov    -0x8(%ebx),%edx
 905:	89 10                	mov    %edx,(%eax)
}
 907:	5b                   	pop    %ebx
 908:	5e                   	pop    %esi
 909:	5f                   	pop    %edi
 90a:	5d                   	pop    %ebp
 90b:	c3                   	ret    
 90c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

00000910 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 910:	f3 0f 1e fb          	endbr32 
 914:	55                   	push   %ebp
 915:	89 e5                	mov    %esp,%ebp
 917:	57                   	push   %edi
 918:	56                   	push   %esi
 919:	53                   	push   %ebx
 91a:	83 ec 1c             	sub    $0x1c,%esp
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 91d:	8b 45 08             	mov    0x8(%ebp),%eax
  if((prevp = freep) == 0){
 920:	8b 3d 44 0e 00 00    	mov    0xe44,%edi
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 926:	8d 70 07             	lea    0x7(%eax),%esi
 929:	c1 ee 03             	shr    $0x3,%esi
 92c:	83 c6 01             	add    $0x1,%esi
  if((prevp = freep) == 0){
 92f:	85 ff                	test   %edi,%edi
 931:	0f 84 a9 00 00 00    	je     9e0 <malloc+0xd0>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 937:	8b 07                	mov    (%edi),%eax
    if(p->s.size >= nunits){
 939:	8b 48 04             	mov    0x4(%eax),%ecx
 93c:	39 f1                	cmp    %esi,%ecx
 93e:	73 6d                	jae    9ad <malloc+0x9d>
 940:	81 fe 00 10 00 00    	cmp    $0x1000,%esi
 946:	bb 00 10 00 00       	mov    $0x1000,%ebx
 94b:	0f 43 de             	cmovae %esi,%ebx
  p = sbrk(nu * sizeof(Header));
 94e:	8d 0c dd 00 00 00 00 	lea    0x0(,%ebx,8),%ecx
 955:	89 4d e4             	mov    %ecx,-0x1c(%ebp)
 958:	eb 17                	jmp    971 <malloc+0x61>
 95a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 960:	8b 10                	mov    (%eax),%edx
    if(p->s.size >= nunits){
 962:	8b 4a 04             	mov    0x4(%edx),%ecx
 965:	39 f1                	cmp    %esi,%ecx
 967:	73 4f                	jae    9b8 <malloc+0xa8>
 969:	8b 3d 44 0e 00 00    	mov    0xe44,%edi
 96f:	89 d0                	mov    %edx,%eax
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 971:	39 c7                	cmp    %eax,%edi
 973:	75 eb                	jne    960 <malloc+0x50>
  p = sbrk(nu * sizeof(Header));
 975:	83 ec 0c             	sub    $0xc,%esp
 978:	ff 75 e4             	pushl  -0x1c(%ebp)
 97b:	e8 5b fc ff ff       	call   5db <sbrk>
  if(p == (char*)-1)
 980:	83 c4 10             	add    $0x10,%esp
 983:	83 f8 ff             	cmp    $0xffffffff,%eax
 986:	74 1b                	je     9a3 <malloc+0x93>
  hp->s.size = nu;
 988:	89 58 04             	mov    %ebx,0x4(%eax)
  free((void*)(hp + 1));
 98b:	83 ec 0c             	sub    $0xc,%esp
 98e:	83 c0 08             	add    $0x8,%eax
 991:	50                   	push   %eax
 992:	e8 e9 fe ff ff       	call   880 <free>
  return freep;
 997:	a1 44 0e 00 00       	mov    0xe44,%eax
      if((p = morecore(nunits)) == 0)
 99c:	83 c4 10             	add    $0x10,%esp
 99f:	85 c0                	test   %eax,%eax
 9a1:	75 bd                	jne    960 <malloc+0x50>
        return 0;
  }
}
 9a3:	8d 65 f4             	lea    -0xc(%ebp),%esp
        return 0;
 9a6:	31 c0                	xor    %eax,%eax
}
 9a8:	5b                   	pop    %ebx
 9a9:	5e                   	pop    %esi
 9aa:	5f                   	pop    %edi
 9ab:	5d                   	pop    %ebp
 9ac:	c3                   	ret    
    if(p->s.size >= nunits){
 9ad:	89 c2                	mov    %eax,%edx
 9af:	89 f8                	mov    %edi,%eax
 9b1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
      if(p->s.size == nunits)
 9b8:	39 ce                	cmp    %ecx,%esi
 9ba:	74 54                	je     a10 <malloc+0x100>
        p->s.size -= nunits;
 9bc:	29 f1                	sub    %esi,%ecx
 9be:	89 4a 04             	mov    %ecx,0x4(%edx)
        p += p->s.size;
 9c1:	8d 14 ca             	lea    (%edx,%ecx,8),%edx
        p->s.size = nunits;
 9c4:	89 72 04             	mov    %esi,0x4(%edx)
      freep = prevp;
 9c7:	a3 44 0e 00 00       	mov    %eax,0xe44
}
 9cc:	8d 65 f4             	lea    -0xc(%ebp),%esp
      return (void*)(p + 1);
 9cf:	8d 42 08             	lea    0x8(%edx),%eax
}
 9d2:	5b                   	pop    %ebx
 9d3:	5e                   	pop    %esi
 9d4:	5f                   	pop    %edi
 9d5:	5d                   	pop    %ebp
 9d6:	c3                   	ret    
 9d7:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 9de:	66 90                	xchg   %ax,%ax
    base.s.ptr = freep = prevp = &base;
 9e0:	c7 05 44 0e 00 00 48 	movl   $0xe48,0xe44
 9e7:	0e 00 00 
    base.s.size = 0;
 9ea:	bf 48 0e 00 00       	mov    $0xe48,%edi
    base.s.ptr = freep = prevp = &base;
 9ef:	c7 05 48 0e 00 00 48 	movl   $0xe48,0xe48
 9f6:	0e 00 00 
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 9f9:	89 f8                	mov    %edi,%eax
    base.s.size = 0;
 9fb:	c7 05 4c 0e 00 00 00 	movl   $0x0,0xe4c
 a02:	00 00 00 
    if(p->s.size >= nunits){
 a05:	e9 36 ff ff ff       	jmp    940 <malloc+0x30>
 a0a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
        prevp->s.ptr = p->s.ptr;
 a10:	8b 0a                	mov    (%edx),%ecx
 a12:	89 08                	mov    %ecx,(%eax)
 a14:	eb b1                	jmp    9c7 <malloc+0xb7>
