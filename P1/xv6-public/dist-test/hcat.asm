
_hcat:     file format elf32-i386


Disassembly of section .text:

00000000 <main>:

}

	int
main(int argc, char *argv[])
{
   0:	f3 0f 1e fb          	endbr32 
   4:	8d 4c 24 04          	lea    0x4(%esp),%ecx
   8:	83 e4 f0             	and    $0xfffffff0,%esp
   b:	ff 71 fc             	pushl  -0x4(%ecx)
   e:	55                   	push   %ebp
   f:	89 e5                	mov    %esp,%ebp
  11:	57                   	push   %edi
  12:	56                   	push   %esi
  13:	53                   	push   %ebx
  14:	51                   	push   %ecx
  15:	83 ec 18             	sub    $0x18,%esp
  18:	8b 01                	mov    (%ecx),%eax
  1a:	8b 59 04             	mov    0x4(%ecx),%ebx
  1d:	89 45 e4             	mov    %eax,-0x1c(%ebp)
	int fd, i;

	if(argc <= 1){
  20:	83 f8 01             	cmp    $0x1,%eax
  23:	7e 73                	jle    98 <main+0x98>
		cat(0);
		exit();
	}


	lines = atoi(argv[1]);
  25:	83 ec 0c             	sub    $0xc,%esp
  28:	ff 73 04             	pushl  0x4(%ebx)
  2b:	e8 a0 03 00 00       	call   3d0 <atoi>

	if(lines<=0){
  30:	83 c4 10             	add    $0x10,%esp
	lines = atoi(argv[1]);
  33:	a3 e0 0e 00 00       	mov    %eax,0xee0
	if(lines<=0){
  38:	85 c0                	test   %eax,%eax
  3a:	7e 70                	jle    ac <main+0xac>
		printf(2, "ERROR!!Please enter more than 1 after hcat!!\n");
		exit();
	}

	for(i = 2; i < argc; i++){
  3c:	83 c3 08             	add    $0x8,%ebx
  3f:	83 7d e4 02          	cmpl   $0x2,-0x1c(%ebp)
  43:	be 02 00 00 00       	mov    $0x2,%esi
  48:	75 25                	jne    6f <main+0x6f>
  4a:	eb 5b                	jmp    a7 <main+0xa7>
  4c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
		if((fd = open(argv[i], 0)) < 0){
			printf(1, "cat: cannot open %s\n", argv[i]);
			exit();
		}
		cat(fd);
  50:	83 ec 0c             	sub    $0xc,%esp
	for(i = 2; i < argc; i++){
  53:	83 c6 01             	add    $0x1,%esi
  56:	83 c3 04             	add    $0x4,%ebx
		cat(fd);
  59:	50                   	push   %eax
  5a:	e8 61 00 00 00       	call   c0 <cat>
		close(fd);
  5f:	89 3c 24             	mov    %edi,(%esp)
  62:	e8 04 04 00 00       	call   46b <close>
	for(i = 2; i < argc; i++){
  67:	83 c4 10             	add    $0x10,%esp
  6a:	39 75 e4             	cmp    %esi,-0x1c(%ebp)
  6d:	74 38                	je     a7 <main+0xa7>
		if((fd = open(argv[i], 0)) < 0){
  6f:	83 ec 08             	sub    $0x8,%esp
  72:	6a 00                	push   $0x0
  74:	ff 33                	pushl  (%ebx)
  76:	e8 08 04 00 00       	call   483 <open>
  7b:	83 c4 10             	add    $0x10,%esp
  7e:	89 c7                	mov    %eax,%edi
  80:	85 c0                	test   %eax,%eax
  82:	79 cc                	jns    50 <main+0x50>
			printf(1, "cat: cannot open %s\n", argv[i]);
  84:	50                   	push   %eax
  85:	ff 33                	pushl  (%ebx)
  87:	68 2b 09 00 00       	push   $0x92b
  8c:	6a 01                	push   $0x1
  8e:	e8 0d 05 00 00       	call   5a0 <printf>
			exit();
  93:	e8 ab 03 00 00       	call   443 <exit>
		cat(0);
  98:	83 ec 0c             	sub    $0xc,%esp
  9b:	6a 00                	push   $0x0
  9d:	e8 1e 00 00 00       	call   c0 <cat>
		exit();
  a2:	e8 9c 03 00 00       	call   443 <exit>
	}
	exit();
  a7:	e8 97 03 00 00       	call   443 <exit>
		printf(2, "ERROR!!Please enter more than 1 after hcat!!\n");
  ac:	52                   	push   %edx
  ad:	52                   	push   %edx
  ae:	68 94 09 00 00       	push   $0x994
  b3:	6a 02                	push   $0x2
  b5:	e8 e6 04 00 00       	call   5a0 <printf>
		exit();
  ba:	e8 84 03 00 00       	call   443 <exit>
  bf:	90                   	nop

000000c0 <cat>:
{
  c0:	f3 0f 1e fb          	endbr32 
  c4:	55                   	push   %ebp
  c5:	89 e5                	mov    %esp,%ebp
  c7:	57                   	push   %edi
  c8:	56                   	push   %esi
  c9:	53                   	push   %ebx
  ca:	83 ec 0c             	sub    $0xc,%esp
	tempLines = lines;
  cd:	8b 3d e0 0e 00 00    	mov    0xee0,%edi
{
  d3:	8b 75 08             	mov    0x8(%ebp),%esi
	if(tempLines==0){
  d6:	85 ff                	test   %edi,%edi
  d8:	74 75                	je     14f <cat+0x8f>
  da:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
		while((n = read(fd, buf, 1))>0){
  e0:	83 ec 04             	sub    $0x4,%esp
  e3:	6a 01                	push   $0x1
  e5:	68 e0 0c 00 00       	push   $0xce0
  ea:	56                   	push   %esi
  eb:	e8 6b 03 00 00       	call   45b <read>
  f0:	83 c4 10             	add    $0x10,%esp
  f3:	89 c3                	mov    %eax,%ebx
  f5:	85 c0                	test   %eax,%eax
  f7:	0f 8e ab 00 00 00    	jle    1a8 <cat+0xe8>
			if(buf[0]=='\n'){
  fd:	80 3d e0 0c 00 00 0a 	cmpb   $0xa,0xce0
 104:	74 7a                	je     180 <cat+0xc0>
			if (write(1, buf, n) != n) {
 106:	83 ec 04             	sub    $0x4,%esp
 109:	53                   	push   %ebx
 10a:	68 e0 0c 00 00       	push   $0xce0
 10f:	6a 01                	push   $0x1
 111:	e8 4d 03 00 00       	call   463 <write>
 116:	83 c4 10             	add    $0x10,%esp
 119:	39 c3                	cmp    %eax,%ebx
 11b:	74 c3                	je     e0 <cat+0x20>
				printf(1, "cat: write error\n");
 11d:	83 ec 08             	sub    $0x8,%esp
 120:	68 08 09 00 00       	push   $0x908
 125:	6a 01                	push   $0x1
 127:	e8 74 04 00 00       	call   5a0 <printf>
				exit();
 12c:	e8 12 03 00 00       	call   443 <exit>
 131:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
			if (write(1, buf, n) != n) {
 138:	83 ec 04             	sub    $0x4,%esp
 13b:	53                   	push   %ebx
 13c:	68 e0 0c 00 00       	push   $0xce0
 141:	6a 01                	push   $0x1
 143:	e8 1b 03 00 00       	call   463 <write>
 148:	83 c4 10             	add    $0x10,%esp
 14b:	39 d8                	cmp    %ebx,%eax
 14d:	75 ce                	jne    11d <cat+0x5d>
		while((n = read(fd, buf, sizeof(buf))) > 0) {
 14f:	83 ec 04             	sub    $0x4,%esp
 152:	68 00 02 00 00       	push   $0x200
 157:	68 e0 0c 00 00       	push   $0xce0
 15c:	56                   	push   %esi
 15d:	e8 f9 02 00 00       	call   45b <read>
 162:	83 c4 10             	add    $0x10,%esp
 165:	89 c3                	mov    %eax,%ebx
 167:	85 c0                	test   %eax,%eax
 169:	7f cd                	jg     138 <cat+0x78>
	if(n < 0){
 16b:	85 db                	test   %ebx,%ebx
 16d:	75 50                	jne    1bf <cat+0xff>
}
 16f:	8d 65 f4             	lea    -0xc(%ebp),%esp
 172:	5b                   	pop    %ebx
 173:	5e                   	pop    %esi
 174:	5f                   	pop    %edi
 175:	5d                   	pop    %ebp
 176:	c3                   	ret    
 177:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 17e:	66 90                	xchg   %ax,%ax
			if (write(1, buf, n) != n) {
 180:	83 ec 04             	sub    $0x4,%esp
				tempLines--;
 183:	83 ef 01             	sub    $0x1,%edi
			if (write(1, buf, n) != n) {
 186:	53                   	push   %ebx
 187:	68 e0 0c 00 00       	push   $0xce0
 18c:	6a 01                	push   $0x1
 18e:	e8 d0 02 00 00       	call   463 <write>
 193:	83 c4 10             	add    $0x10,%esp
 196:	39 d8                	cmp    %ebx,%eax
 198:	75 83                	jne    11d <cat+0x5d>
			if(tempLines==0){
 19a:	85 ff                	test   %edi,%edi
 19c:	0f 85 3e ff ff ff    	jne    e0 <cat+0x20>
 1a2:	eb cb                	jmp    16f <cat+0xaf>
 1a4:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
		if(tempLines>0){
 1a8:	85 ff                	test   %edi,%edi
 1aa:	7e bf                	jle    16b <cat+0xab>
			printf(2, "\n\nWARNING MESSAGE: Printed every lines, you've typed more than exsisting lines!!\n\n");
 1ac:	52                   	push   %edx
 1ad:	52                   	push   %edx
 1ae:	68 40 09 00 00       	push   $0x940
 1b3:	6a 02                	push   $0x2
 1b5:	e8 e6 03 00 00       	call   5a0 <printf>
			exit();
 1ba:	e8 84 02 00 00       	call   443 <exit>
		printf(1, "cat: read error\n");
 1bf:	50                   	push   %eax
 1c0:	50                   	push   %eax
 1c1:	68 1a 09 00 00       	push   $0x91a
 1c6:	6a 01                	push   $0x1
 1c8:	e8 d3 03 00 00       	call   5a0 <printf>
		exit();
 1cd:	e8 71 02 00 00       	call   443 <exit>
 1d2:	66 90                	xchg   %ax,%ax
 1d4:	66 90                	xchg   %ax,%ax
 1d6:	66 90                	xchg   %ax,%ax
 1d8:	66 90                	xchg   %ax,%ax
 1da:	66 90                	xchg   %ax,%ax
 1dc:	66 90                	xchg   %ax,%ax
 1de:	66 90                	xchg   %ax,%ax

000001e0 <strcpy>:
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, const char *t)
{
 1e0:	f3 0f 1e fb          	endbr32 
 1e4:	55                   	push   %ebp
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 1e5:	31 c0                	xor    %eax,%eax
{
 1e7:	89 e5                	mov    %esp,%ebp
 1e9:	53                   	push   %ebx
 1ea:	8b 4d 08             	mov    0x8(%ebp),%ecx
 1ed:	8b 5d 0c             	mov    0xc(%ebp),%ebx
  while((*s++ = *t++) != 0)
 1f0:	0f b6 14 03          	movzbl (%ebx,%eax,1),%edx
 1f4:	88 14 01             	mov    %dl,(%ecx,%eax,1)
 1f7:	83 c0 01             	add    $0x1,%eax
 1fa:	84 d2                	test   %dl,%dl
 1fc:	75 f2                	jne    1f0 <strcpy+0x10>
    ;
  return os;
}
 1fe:	89 c8                	mov    %ecx,%eax
 200:	5b                   	pop    %ebx
 201:	5d                   	pop    %ebp
 202:	c3                   	ret    
 203:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 20a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

00000210 <strcmp>:

int
strcmp(const char *p, const char *q)
{
 210:	f3 0f 1e fb          	endbr32 
 214:	55                   	push   %ebp
 215:	89 e5                	mov    %esp,%ebp
 217:	53                   	push   %ebx
 218:	8b 4d 08             	mov    0x8(%ebp),%ecx
 21b:	8b 55 0c             	mov    0xc(%ebp),%edx
  while(*p && *p == *q)
 21e:	0f b6 01             	movzbl (%ecx),%eax
 221:	0f b6 1a             	movzbl (%edx),%ebx
 224:	84 c0                	test   %al,%al
 226:	75 19                	jne    241 <strcmp+0x31>
 228:	eb 26                	jmp    250 <strcmp+0x40>
 22a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 230:	0f b6 41 01          	movzbl 0x1(%ecx),%eax
    p++, q++;
 234:	83 c1 01             	add    $0x1,%ecx
 237:	83 c2 01             	add    $0x1,%edx
  while(*p && *p == *q)
 23a:	0f b6 1a             	movzbl (%edx),%ebx
 23d:	84 c0                	test   %al,%al
 23f:	74 0f                	je     250 <strcmp+0x40>
 241:	38 d8                	cmp    %bl,%al
 243:	74 eb                	je     230 <strcmp+0x20>
  return (uchar)*p - (uchar)*q;
 245:	29 d8                	sub    %ebx,%eax
}
 247:	5b                   	pop    %ebx
 248:	5d                   	pop    %ebp
 249:	c3                   	ret    
 24a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 250:	31 c0                	xor    %eax,%eax
  return (uchar)*p - (uchar)*q;
 252:	29 d8                	sub    %ebx,%eax
}
 254:	5b                   	pop    %ebx
 255:	5d                   	pop    %ebp
 256:	c3                   	ret    
 257:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 25e:	66 90                	xchg   %ax,%ax

00000260 <strlen>:

uint
strlen(const char *s)
{
 260:	f3 0f 1e fb          	endbr32 
 264:	55                   	push   %ebp
 265:	89 e5                	mov    %esp,%ebp
 267:	8b 55 08             	mov    0x8(%ebp),%edx
  int n;

  for(n = 0; s[n]; n++)
 26a:	80 3a 00             	cmpb   $0x0,(%edx)
 26d:	74 21                	je     290 <strlen+0x30>
 26f:	31 c0                	xor    %eax,%eax
 271:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 278:	83 c0 01             	add    $0x1,%eax
 27b:	80 3c 02 00          	cmpb   $0x0,(%edx,%eax,1)
 27f:	89 c1                	mov    %eax,%ecx
 281:	75 f5                	jne    278 <strlen+0x18>
    ;
  return n;
}
 283:	89 c8                	mov    %ecx,%eax
 285:	5d                   	pop    %ebp
 286:	c3                   	ret    
 287:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 28e:	66 90                	xchg   %ax,%ax
  for(n = 0; s[n]; n++)
 290:	31 c9                	xor    %ecx,%ecx
}
 292:	5d                   	pop    %ebp
 293:	89 c8                	mov    %ecx,%eax
 295:	c3                   	ret    
 296:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 29d:	8d 76 00             	lea    0x0(%esi),%esi

000002a0 <memset>:

void*
memset(void *dst, int c, uint n)
{
 2a0:	f3 0f 1e fb          	endbr32 
 2a4:	55                   	push   %ebp
 2a5:	89 e5                	mov    %esp,%ebp
 2a7:	57                   	push   %edi
 2a8:	8b 55 08             	mov    0x8(%ebp),%edx
}

static inline void
stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
 2ab:	8b 4d 10             	mov    0x10(%ebp),%ecx
 2ae:	8b 45 0c             	mov    0xc(%ebp),%eax
 2b1:	89 d7                	mov    %edx,%edi
 2b3:	fc                   	cld    
 2b4:	f3 aa                	rep stos %al,%es:(%edi)
  stosb(dst, c, n);
  return dst;
}
 2b6:	89 d0                	mov    %edx,%eax
 2b8:	5f                   	pop    %edi
 2b9:	5d                   	pop    %ebp
 2ba:	c3                   	ret    
 2bb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 2bf:	90                   	nop

000002c0 <strchr>:

char*
strchr(const char *s, char c)
{
 2c0:	f3 0f 1e fb          	endbr32 
 2c4:	55                   	push   %ebp
 2c5:	89 e5                	mov    %esp,%ebp
 2c7:	8b 45 08             	mov    0x8(%ebp),%eax
 2ca:	0f b6 4d 0c          	movzbl 0xc(%ebp),%ecx
  for(; *s; s++)
 2ce:	0f b6 10             	movzbl (%eax),%edx
 2d1:	84 d2                	test   %dl,%dl
 2d3:	75 16                	jne    2eb <strchr+0x2b>
 2d5:	eb 21                	jmp    2f8 <strchr+0x38>
 2d7:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 2de:	66 90                	xchg   %ax,%ax
 2e0:	0f b6 50 01          	movzbl 0x1(%eax),%edx
 2e4:	83 c0 01             	add    $0x1,%eax
 2e7:	84 d2                	test   %dl,%dl
 2e9:	74 0d                	je     2f8 <strchr+0x38>
    if(*s == c)
 2eb:	38 d1                	cmp    %dl,%cl
 2ed:	75 f1                	jne    2e0 <strchr+0x20>
      return (char*)s;
  return 0;
}
 2ef:	5d                   	pop    %ebp
 2f0:	c3                   	ret    
 2f1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  return 0;
 2f8:	31 c0                	xor    %eax,%eax
}
 2fa:	5d                   	pop    %ebp
 2fb:	c3                   	ret    
 2fc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

00000300 <gets>:

char*
gets(char *buf, int max)
{
 300:	f3 0f 1e fb          	endbr32 
 304:	55                   	push   %ebp
 305:	89 e5                	mov    %esp,%ebp
 307:	57                   	push   %edi
 308:	56                   	push   %esi
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 309:	31 f6                	xor    %esi,%esi
{
 30b:	53                   	push   %ebx
 30c:	89 f3                	mov    %esi,%ebx
 30e:	83 ec 1c             	sub    $0x1c,%esp
 311:	8b 7d 08             	mov    0x8(%ebp),%edi
  for(i=0; i+1 < max; ){
 314:	eb 33                	jmp    349 <gets+0x49>
 316:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 31d:	8d 76 00             	lea    0x0(%esi),%esi
    cc = read(0, &c, 1);
 320:	83 ec 04             	sub    $0x4,%esp
 323:	8d 45 e7             	lea    -0x19(%ebp),%eax
 326:	6a 01                	push   $0x1
 328:	50                   	push   %eax
 329:	6a 00                	push   $0x0
 32b:	e8 2b 01 00 00       	call   45b <read>
    if(cc < 1)
 330:	83 c4 10             	add    $0x10,%esp
 333:	85 c0                	test   %eax,%eax
 335:	7e 1c                	jle    353 <gets+0x53>
      break;
    buf[i++] = c;
 337:	0f b6 45 e7          	movzbl -0x19(%ebp),%eax
 33b:	83 c7 01             	add    $0x1,%edi
 33e:	88 47 ff             	mov    %al,-0x1(%edi)
    if(c == '\n' || c == '\r')
 341:	3c 0a                	cmp    $0xa,%al
 343:	74 23                	je     368 <gets+0x68>
 345:	3c 0d                	cmp    $0xd,%al
 347:	74 1f                	je     368 <gets+0x68>
  for(i=0; i+1 < max; ){
 349:	83 c3 01             	add    $0x1,%ebx
 34c:	89 fe                	mov    %edi,%esi
 34e:	3b 5d 0c             	cmp    0xc(%ebp),%ebx
 351:	7c cd                	jl     320 <gets+0x20>
 353:	89 f3                	mov    %esi,%ebx
      break;
  }
  buf[i] = '\0';
  return buf;
}
 355:	8b 45 08             	mov    0x8(%ebp),%eax
  buf[i] = '\0';
 358:	c6 03 00             	movb   $0x0,(%ebx)
}
 35b:	8d 65 f4             	lea    -0xc(%ebp),%esp
 35e:	5b                   	pop    %ebx
 35f:	5e                   	pop    %esi
 360:	5f                   	pop    %edi
 361:	5d                   	pop    %ebp
 362:	c3                   	ret    
 363:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 367:	90                   	nop
 368:	8b 75 08             	mov    0x8(%ebp),%esi
 36b:	8b 45 08             	mov    0x8(%ebp),%eax
 36e:	01 de                	add    %ebx,%esi
 370:	89 f3                	mov    %esi,%ebx
  buf[i] = '\0';
 372:	c6 03 00             	movb   $0x0,(%ebx)
}
 375:	8d 65 f4             	lea    -0xc(%ebp),%esp
 378:	5b                   	pop    %ebx
 379:	5e                   	pop    %esi
 37a:	5f                   	pop    %edi
 37b:	5d                   	pop    %ebp
 37c:	c3                   	ret    
 37d:	8d 76 00             	lea    0x0(%esi),%esi

00000380 <stat>:

int
stat(const char *n, struct stat *st)
{
 380:	f3 0f 1e fb          	endbr32 
 384:	55                   	push   %ebp
 385:	89 e5                	mov    %esp,%ebp
 387:	56                   	push   %esi
 388:	53                   	push   %ebx
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 389:	83 ec 08             	sub    $0x8,%esp
 38c:	6a 00                	push   $0x0
 38e:	ff 75 08             	pushl  0x8(%ebp)
 391:	e8 ed 00 00 00       	call   483 <open>
  if(fd < 0)
 396:	83 c4 10             	add    $0x10,%esp
 399:	85 c0                	test   %eax,%eax
 39b:	78 2b                	js     3c8 <stat+0x48>
    return -1;
  r = fstat(fd, st);
 39d:	83 ec 08             	sub    $0x8,%esp
 3a0:	ff 75 0c             	pushl  0xc(%ebp)
 3a3:	89 c3                	mov    %eax,%ebx
 3a5:	50                   	push   %eax
 3a6:	e8 f0 00 00 00       	call   49b <fstat>
  close(fd);
 3ab:	89 1c 24             	mov    %ebx,(%esp)
  r = fstat(fd, st);
 3ae:	89 c6                	mov    %eax,%esi
  close(fd);
 3b0:	e8 b6 00 00 00       	call   46b <close>
  return r;
 3b5:	83 c4 10             	add    $0x10,%esp
}
 3b8:	8d 65 f8             	lea    -0x8(%ebp),%esp
 3bb:	89 f0                	mov    %esi,%eax
 3bd:	5b                   	pop    %ebx
 3be:	5e                   	pop    %esi
 3bf:	5d                   	pop    %ebp
 3c0:	c3                   	ret    
 3c1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
    return -1;
 3c8:	be ff ff ff ff       	mov    $0xffffffff,%esi
 3cd:	eb e9                	jmp    3b8 <stat+0x38>
 3cf:	90                   	nop

000003d0 <atoi>:

int
atoi(const char *s)
{
 3d0:	f3 0f 1e fb          	endbr32 
 3d4:	55                   	push   %ebp
 3d5:	89 e5                	mov    %esp,%ebp
 3d7:	53                   	push   %ebx
 3d8:	8b 55 08             	mov    0x8(%ebp),%edx
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
 3db:	0f be 02             	movsbl (%edx),%eax
 3de:	8d 48 d0             	lea    -0x30(%eax),%ecx
 3e1:	80 f9 09             	cmp    $0x9,%cl
  n = 0;
 3e4:	b9 00 00 00 00       	mov    $0x0,%ecx
  while('0' <= *s && *s <= '9')
 3e9:	77 1a                	ja     405 <atoi+0x35>
 3eb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 3ef:	90                   	nop
    n = n*10 + *s++ - '0';
 3f0:	83 c2 01             	add    $0x1,%edx
 3f3:	8d 0c 89             	lea    (%ecx,%ecx,4),%ecx
 3f6:	8d 4c 48 d0          	lea    -0x30(%eax,%ecx,2),%ecx
  while('0' <= *s && *s <= '9')
 3fa:	0f be 02             	movsbl (%edx),%eax
 3fd:	8d 58 d0             	lea    -0x30(%eax),%ebx
 400:	80 fb 09             	cmp    $0x9,%bl
 403:	76 eb                	jbe    3f0 <atoi+0x20>
  return n;
}
 405:	89 c8                	mov    %ecx,%eax
 407:	5b                   	pop    %ebx
 408:	5d                   	pop    %ebp
 409:	c3                   	ret    
 40a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

00000410 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 410:	f3 0f 1e fb          	endbr32 
 414:	55                   	push   %ebp
 415:	89 e5                	mov    %esp,%ebp
 417:	57                   	push   %edi
 418:	8b 45 10             	mov    0x10(%ebp),%eax
 41b:	8b 55 08             	mov    0x8(%ebp),%edx
 41e:	56                   	push   %esi
 41f:	8b 75 0c             	mov    0xc(%ebp),%esi
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
 422:	85 c0                	test   %eax,%eax
 424:	7e 0f                	jle    435 <memmove+0x25>
 426:	01 d0                	add    %edx,%eax
  dst = vdst;
 428:	89 d7                	mov    %edx,%edi
 42a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
    *dst++ = *src++;
 430:	a4                   	movsb  %ds:(%esi),%es:(%edi)
  while(n-- > 0)
 431:	39 f8                	cmp    %edi,%eax
 433:	75 fb                	jne    430 <memmove+0x20>
  return vdst;
}
 435:	5e                   	pop    %esi
 436:	89 d0                	mov    %edx,%eax
 438:	5f                   	pop    %edi
 439:	5d                   	pop    %ebp
 43a:	c3                   	ret    

0000043b <fork>:
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
 43b:	b8 01 00 00 00       	mov    $0x1,%eax
 440:	cd 40                	int    $0x40
 442:	c3                   	ret    

00000443 <exit>:
SYSCALL(exit)
 443:	b8 02 00 00 00       	mov    $0x2,%eax
 448:	cd 40                	int    $0x40
 44a:	c3                   	ret    

0000044b <wait>:
SYSCALL(wait)
 44b:	b8 03 00 00 00       	mov    $0x3,%eax
 450:	cd 40                	int    $0x40
 452:	c3                   	ret    

00000453 <pipe>:
SYSCALL(pipe)
 453:	b8 04 00 00 00       	mov    $0x4,%eax
 458:	cd 40                	int    $0x40
 45a:	c3                   	ret    

0000045b <read>:
SYSCALL(read)
 45b:	b8 05 00 00 00       	mov    $0x5,%eax
 460:	cd 40                	int    $0x40
 462:	c3                   	ret    

00000463 <write>:
SYSCALL(write)
 463:	b8 10 00 00 00       	mov    $0x10,%eax
 468:	cd 40                	int    $0x40
 46a:	c3                   	ret    

0000046b <close>:
SYSCALL(close)
 46b:	b8 15 00 00 00       	mov    $0x15,%eax
 470:	cd 40                	int    $0x40
 472:	c3                   	ret    

00000473 <kill>:
SYSCALL(kill)
 473:	b8 06 00 00 00       	mov    $0x6,%eax
 478:	cd 40                	int    $0x40
 47a:	c3                   	ret    

0000047b <exec>:
SYSCALL(exec)
 47b:	b8 07 00 00 00       	mov    $0x7,%eax
 480:	cd 40                	int    $0x40
 482:	c3                   	ret    

00000483 <open>:
SYSCALL(open)
 483:	b8 0f 00 00 00       	mov    $0xf,%eax
 488:	cd 40                	int    $0x40
 48a:	c3                   	ret    

0000048b <mknod>:
SYSCALL(mknod)
 48b:	b8 11 00 00 00       	mov    $0x11,%eax
 490:	cd 40                	int    $0x40
 492:	c3                   	ret    

00000493 <unlink>:
SYSCALL(unlink)
 493:	b8 12 00 00 00       	mov    $0x12,%eax
 498:	cd 40                	int    $0x40
 49a:	c3                   	ret    

0000049b <fstat>:
SYSCALL(fstat)
 49b:	b8 08 00 00 00       	mov    $0x8,%eax
 4a0:	cd 40                	int    $0x40
 4a2:	c3                   	ret    

000004a3 <link>:
SYSCALL(link)
 4a3:	b8 13 00 00 00       	mov    $0x13,%eax
 4a8:	cd 40                	int    $0x40
 4aa:	c3                   	ret    

000004ab <mkdir>:
SYSCALL(mkdir)
 4ab:	b8 14 00 00 00       	mov    $0x14,%eax
 4b0:	cd 40                	int    $0x40
 4b2:	c3                   	ret    

000004b3 <chdir>:
SYSCALL(chdir)
 4b3:	b8 09 00 00 00       	mov    $0x9,%eax
 4b8:	cd 40                	int    $0x40
 4ba:	c3                   	ret    

000004bb <dup>:
SYSCALL(dup)
 4bb:	b8 0a 00 00 00       	mov    $0xa,%eax
 4c0:	cd 40                	int    $0x40
 4c2:	c3                   	ret    

000004c3 <getpid>:
SYSCALL(getpid)
 4c3:	b8 0b 00 00 00       	mov    $0xb,%eax
 4c8:	cd 40                	int    $0x40
 4ca:	c3                   	ret    

000004cb <sbrk>:
SYSCALL(sbrk)
 4cb:	b8 0c 00 00 00       	mov    $0xc,%eax
 4d0:	cd 40                	int    $0x40
 4d2:	c3                   	ret    

000004d3 <sleep>:
SYSCALL(sleep)
 4d3:	b8 0d 00 00 00       	mov    $0xd,%eax
 4d8:	cd 40                	int    $0x40
 4da:	c3                   	ret    

000004db <uptime>:
SYSCALL(uptime)
 4db:	b8 0e 00 00 00       	mov    $0xe,%eax
 4e0:	cd 40                	int    $0x40
 4e2:	c3                   	ret    
 4e3:	66 90                	xchg   %ax,%ax
 4e5:	66 90                	xchg   %ax,%ax
 4e7:	66 90                	xchg   %ax,%ax
 4e9:	66 90                	xchg   %ax,%ax
 4eb:	66 90                	xchg   %ax,%ax
 4ed:	66 90                	xchg   %ax,%ax
 4ef:	90                   	nop

000004f0 <printint>:
  write(fd, &c, 1);
}

static void
printint(int fd, int xx, int base, int sgn)
{
 4f0:	55                   	push   %ebp
 4f1:	89 e5                	mov    %esp,%ebp
 4f3:	57                   	push   %edi
 4f4:	56                   	push   %esi
 4f5:	53                   	push   %ebx
 4f6:	83 ec 3c             	sub    $0x3c,%esp
 4f9:	89 4d c4             	mov    %ecx,-0x3c(%ebp)
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
 4fc:	89 d1                	mov    %edx,%ecx
{
 4fe:	89 45 b8             	mov    %eax,-0x48(%ebp)
  if(sgn && xx < 0){
 501:	85 d2                	test   %edx,%edx
 503:	0f 89 7f 00 00 00    	jns    588 <printint+0x98>
 509:	f6 45 08 01          	testb  $0x1,0x8(%ebp)
 50d:	74 79                	je     588 <printint+0x98>
    neg = 1;
 50f:	c7 45 bc 01 00 00 00 	movl   $0x1,-0x44(%ebp)
    x = -xx;
 516:	f7 d9                	neg    %ecx
  } else {
    x = xx;
  }

  i = 0;
 518:	31 db                	xor    %ebx,%ebx
 51a:	8d 75 d7             	lea    -0x29(%ebp),%esi
 51d:	8d 76 00             	lea    0x0(%esi),%esi
  do{
    buf[i++] = digits[x % base];
 520:	89 c8                	mov    %ecx,%eax
 522:	31 d2                	xor    %edx,%edx
 524:	89 cf                	mov    %ecx,%edi
 526:	f7 75 c4             	divl   -0x3c(%ebp)
 529:	0f b6 92 cc 09 00 00 	movzbl 0x9cc(%edx),%edx
 530:	89 45 c0             	mov    %eax,-0x40(%ebp)
 533:	89 d8                	mov    %ebx,%eax
 535:	8d 5b 01             	lea    0x1(%ebx),%ebx
  }while((x /= base) != 0);
 538:	8b 4d c0             	mov    -0x40(%ebp),%ecx
    buf[i++] = digits[x % base];
 53b:	88 14 1e             	mov    %dl,(%esi,%ebx,1)
  }while((x /= base) != 0);
 53e:	39 7d c4             	cmp    %edi,-0x3c(%ebp)
 541:	76 dd                	jbe    520 <printint+0x30>
  if(neg)
 543:	8b 4d bc             	mov    -0x44(%ebp),%ecx
 546:	85 c9                	test   %ecx,%ecx
 548:	74 0c                	je     556 <printint+0x66>
    buf[i++] = '-';
 54a:	c6 44 1d d8 2d       	movb   $0x2d,-0x28(%ebp,%ebx,1)
    buf[i++] = digits[x % base];
 54f:	89 d8                	mov    %ebx,%eax
    buf[i++] = '-';
 551:	ba 2d 00 00 00       	mov    $0x2d,%edx

  while(--i >= 0)
 556:	8b 7d b8             	mov    -0x48(%ebp),%edi
 559:	8d 5c 05 d7          	lea    -0x29(%ebp,%eax,1),%ebx
 55d:	eb 07                	jmp    566 <printint+0x76>
 55f:	90                   	nop
 560:	0f b6 13             	movzbl (%ebx),%edx
 563:	83 eb 01             	sub    $0x1,%ebx
  write(fd, &c, 1);
 566:	83 ec 04             	sub    $0x4,%esp
 569:	88 55 d7             	mov    %dl,-0x29(%ebp)
 56c:	6a 01                	push   $0x1
 56e:	56                   	push   %esi
 56f:	57                   	push   %edi
 570:	e8 ee fe ff ff       	call   463 <write>
  while(--i >= 0)
 575:	83 c4 10             	add    $0x10,%esp
 578:	39 de                	cmp    %ebx,%esi
 57a:	75 e4                	jne    560 <printint+0x70>
    putc(fd, buf[i]);
}
 57c:	8d 65 f4             	lea    -0xc(%ebp),%esp
 57f:	5b                   	pop    %ebx
 580:	5e                   	pop    %esi
 581:	5f                   	pop    %edi
 582:	5d                   	pop    %ebp
 583:	c3                   	ret    
 584:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  neg = 0;
 588:	c7 45 bc 00 00 00 00 	movl   $0x0,-0x44(%ebp)
 58f:	eb 87                	jmp    518 <printint+0x28>
 591:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 598:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 59f:	90                   	nop

000005a0 <printf>:

// Print to the given fd. Only understands %d, %x, %p, %s.
void
printf(int fd, const char *fmt, ...)
{
 5a0:	f3 0f 1e fb          	endbr32 
 5a4:	55                   	push   %ebp
 5a5:	89 e5                	mov    %esp,%ebp
 5a7:	57                   	push   %edi
 5a8:	56                   	push   %esi
 5a9:	53                   	push   %ebx
 5aa:	83 ec 2c             	sub    $0x2c,%esp
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 5ad:	8b 75 0c             	mov    0xc(%ebp),%esi
 5b0:	0f b6 1e             	movzbl (%esi),%ebx
 5b3:	84 db                	test   %bl,%bl
 5b5:	0f 84 b4 00 00 00    	je     66f <printf+0xcf>
  ap = (uint*)(void*)&fmt + 1;
 5bb:	8d 45 10             	lea    0x10(%ebp),%eax
 5be:	83 c6 01             	add    $0x1,%esi
  write(fd, &c, 1);
 5c1:	8d 7d e7             	lea    -0x19(%ebp),%edi
  state = 0;
 5c4:	31 d2                	xor    %edx,%edx
  ap = (uint*)(void*)&fmt + 1;
 5c6:	89 45 d0             	mov    %eax,-0x30(%ebp)
 5c9:	eb 33                	jmp    5fe <printf+0x5e>
 5cb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 5cf:	90                   	nop
 5d0:	89 55 d4             	mov    %edx,-0x2c(%ebp)
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
 5d3:	ba 25 00 00 00       	mov    $0x25,%edx
      if(c == '%'){
 5d8:	83 f8 25             	cmp    $0x25,%eax
 5db:	74 17                	je     5f4 <printf+0x54>
  write(fd, &c, 1);
 5dd:	83 ec 04             	sub    $0x4,%esp
 5e0:	88 5d e7             	mov    %bl,-0x19(%ebp)
 5e3:	6a 01                	push   $0x1
 5e5:	57                   	push   %edi
 5e6:	ff 75 08             	pushl  0x8(%ebp)
 5e9:	e8 75 fe ff ff       	call   463 <write>
 5ee:	8b 55 d4             	mov    -0x2c(%ebp),%edx
      } else {
        putc(fd, c);
 5f1:	83 c4 10             	add    $0x10,%esp
  for(i = 0; fmt[i]; i++){
 5f4:	0f b6 1e             	movzbl (%esi),%ebx
 5f7:	83 c6 01             	add    $0x1,%esi
 5fa:	84 db                	test   %bl,%bl
 5fc:	74 71                	je     66f <printf+0xcf>
    c = fmt[i] & 0xff;
 5fe:	0f be cb             	movsbl %bl,%ecx
 601:	0f b6 c3             	movzbl %bl,%eax
    if(state == 0){
 604:	85 d2                	test   %edx,%edx
 606:	74 c8                	je     5d0 <printf+0x30>
      }
    } else if(state == '%'){
 608:	83 fa 25             	cmp    $0x25,%edx
 60b:	75 e7                	jne    5f4 <printf+0x54>
      if(c == 'd'){
 60d:	83 f8 64             	cmp    $0x64,%eax
 610:	0f 84 9a 00 00 00    	je     6b0 <printf+0x110>
        printint(fd, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
 616:	81 e1 f7 00 00 00    	and    $0xf7,%ecx
 61c:	83 f9 70             	cmp    $0x70,%ecx
 61f:	74 5f                	je     680 <printf+0xe0>
        printint(fd, *ap, 16, 0);
        ap++;
      } else if(c == 's'){
 621:	83 f8 73             	cmp    $0x73,%eax
 624:	0f 84 d6 00 00 00    	je     700 <printf+0x160>
          s = "(null)";
        while(*s != 0){
          putc(fd, *s);
          s++;
        }
      } else if(c == 'c'){
 62a:	83 f8 63             	cmp    $0x63,%eax
 62d:	0f 84 8d 00 00 00    	je     6c0 <printf+0x120>
        putc(fd, *ap);
        ap++;
      } else if(c == '%'){
 633:	83 f8 25             	cmp    $0x25,%eax
 636:	0f 84 b4 00 00 00    	je     6f0 <printf+0x150>
  write(fd, &c, 1);
 63c:	83 ec 04             	sub    $0x4,%esp
 63f:	c6 45 e7 25          	movb   $0x25,-0x19(%ebp)
 643:	6a 01                	push   $0x1
 645:	57                   	push   %edi
 646:	ff 75 08             	pushl  0x8(%ebp)
 649:	e8 15 fe ff ff       	call   463 <write>
        putc(fd, c);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
 64e:	88 5d e7             	mov    %bl,-0x19(%ebp)
  write(fd, &c, 1);
 651:	83 c4 0c             	add    $0xc,%esp
 654:	6a 01                	push   $0x1
 656:	83 c6 01             	add    $0x1,%esi
 659:	57                   	push   %edi
 65a:	ff 75 08             	pushl  0x8(%ebp)
 65d:	e8 01 fe ff ff       	call   463 <write>
  for(i = 0; fmt[i]; i++){
 662:	0f b6 5e ff          	movzbl -0x1(%esi),%ebx
        putc(fd, c);
 666:	83 c4 10             	add    $0x10,%esp
      }
      state = 0;
 669:	31 d2                	xor    %edx,%edx
  for(i = 0; fmt[i]; i++){
 66b:	84 db                	test   %bl,%bl
 66d:	75 8f                	jne    5fe <printf+0x5e>
    }
  }
}
 66f:	8d 65 f4             	lea    -0xc(%ebp),%esp
 672:	5b                   	pop    %ebx
 673:	5e                   	pop    %esi
 674:	5f                   	pop    %edi
 675:	5d                   	pop    %ebp
 676:	c3                   	ret    
 677:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 67e:	66 90                	xchg   %ax,%ax
        printint(fd, *ap, 16, 0);
 680:	83 ec 0c             	sub    $0xc,%esp
 683:	b9 10 00 00 00       	mov    $0x10,%ecx
 688:	6a 00                	push   $0x0
 68a:	8b 5d d0             	mov    -0x30(%ebp),%ebx
 68d:	8b 45 08             	mov    0x8(%ebp),%eax
 690:	8b 13                	mov    (%ebx),%edx
 692:	e8 59 fe ff ff       	call   4f0 <printint>
        ap++;
 697:	89 d8                	mov    %ebx,%eax
 699:	83 c4 10             	add    $0x10,%esp
      state = 0;
 69c:	31 d2                	xor    %edx,%edx
        ap++;
 69e:	83 c0 04             	add    $0x4,%eax
 6a1:	89 45 d0             	mov    %eax,-0x30(%ebp)
 6a4:	e9 4b ff ff ff       	jmp    5f4 <printf+0x54>
 6a9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
        printint(fd, *ap, 10, 1);
 6b0:	83 ec 0c             	sub    $0xc,%esp
 6b3:	b9 0a 00 00 00       	mov    $0xa,%ecx
 6b8:	6a 01                	push   $0x1
 6ba:	eb ce                	jmp    68a <printf+0xea>
 6bc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
        putc(fd, *ap);
 6c0:	8b 5d d0             	mov    -0x30(%ebp),%ebx
  write(fd, &c, 1);
 6c3:	83 ec 04             	sub    $0x4,%esp
        putc(fd, *ap);
 6c6:	8b 03                	mov    (%ebx),%eax
  write(fd, &c, 1);
 6c8:	6a 01                	push   $0x1
        ap++;
 6ca:	83 c3 04             	add    $0x4,%ebx
  write(fd, &c, 1);
 6cd:	57                   	push   %edi
 6ce:	ff 75 08             	pushl  0x8(%ebp)
        putc(fd, *ap);
 6d1:	88 45 e7             	mov    %al,-0x19(%ebp)
  write(fd, &c, 1);
 6d4:	e8 8a fd ff ff       	call   463 <write>
        ap++;
 6d9:	89 5d d0             	mov    %ebx,-0x30(%ebp)
 6dc:	83 c4 10             	add    $0x10,%esp
      state = 0;
 6df:	31 d2                	xor    %edx,%edx
 6e1:	e9 0e ff ff ff       	jmp    5f4 <printf+0x54>
 6e6:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 6ed:	8d 76 00             	lea    0x0(%esi),%esi
        putc(fd, c);
 6f0:	88 5d e7             	mov    %bl,-0x19(%ebp)
  write(fd, &c, 1);
 6f3:	83 ec 04             	sub    $0x4,%esp
 6f6:	e9 59 ff ff ff       	jmp    654 <printf+0xb4>
 6fb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 6ff:	90                   	nop
        s = (char*)*ap;
 700:	8b 45 d0             	mov    -0x30(%ebp),%eax
 703:	8b 18                	mov    (%eax),%ebx
        ap++;
 705:	83 c0 04             	add    $0x4,%eax
 708:	89 45 d0             	mov    %eax,-0x30(%ebp)
        if(s == 0)
 70b:	85 db                	test   %ebx,%ebx
 70d:	74 17                	je     726 <printf+0x186>
        while(*s != 0){
 70f:	0f b6 03             	movzbl (%ebx),%eax
      state = 0;
 712:	31 d2                	xor    %edx,%edx
        while(*s != 0){
 714:	84 c0                	test   %al,%al
 716:	0f 84 d8 fe ff ff    	je     5f4 <printf+0x54>
 71c:	89 75 d4             	mov    %esi,-0x2c(%ebp)
 71f:	89 de                	mov    %ebx,%esi
 721:	8b 5d 08             	mov    0x8(%ebp),%ebx
 724:	eb 1a                	jmp    740 <printf+0x1a0>
          s = "(null)";
 726:	bb c2 09 00 00       	mov    $0x9c2,%ebx
        while(*s != 0){
 72b:	89 75 d4             	mov    %esi,-0x2c(%ebp)
 72e:	b8 28 00 00 00       	mov    $0x28,%eax
 733:	89 de                	mov    %ebx,%esi
 735:	8b 5d 08             	mov    0x8(%ebp),%ebx
 738:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 73f:	90                   	nop
  write(fd, &c, 1);
 740:	83 ec 04             	sub    $0x4,%esp
          s++;
 743:	83 c6 01             	add    $0x1,%esi
 746:	88 45 e7             	mov    %al,-0x19(%ebp)
  write(fd, &c, 1);
 749:	6a 01                	push   $0x1
 74b:	57                   	push   %edi
 74c:	53                   	push   %ebx
 74d:	e8 11 fd ff ff       	call   463 <write>
        while(*s != 0){
 752:	0f b6 06             	movzbl (%esi),%eax
 755:	83 c4 10             	add    $0x10,%esp
 758:	84 c0                	test   %al,%al
 75a:	75 e4                	jne    740 <printf+0x1a0>
 75c:	8b 75 d4             	mov    -0x2c(%ebp),%esi
      state = 0;
 75f:	31 d2                	xor    %edx,%edx
 761:	e9 8e fe ff ff       	jmp    5f4 <printf+0x54>
 766:	66 90                	xchg   %ax,%ax
 768:	66 90                	xchg   %ax,%ax
 76a:	66 90                	xchg   %ax,%ax
 76c:	66 90                	xchg   %ax,%ax
 76e:	66 90                	xchg   %ax,%ax

00000770 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 770:	f3 0f 1e fb          	endbr32 
 774:	55                   	push   %ebp
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 775:	a1 c0 0c 00 00       	mov    0xcc0,%eax
{
 77a:	89 e5                	mov    %esp,%ebp
 77c:	57                   	push   %edi
 77d:	56                   	push   %esi
 77e:	53                   	push   %ebx
 77f:	8b 5d 08             	mov    0x8(%ebp),%ebx
 782:	8b 10                	mov    (%eax),%edx
  bp = (Header*)ap - 1;
 784:	8d 4b f8             	lea    -0x8(%ebx),%ecx
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 787:	39 c8                	cmp    %ecx,%eax
 789:	73 15                	jae    7a0 <free+0x30>
 78b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 78f:	90                   	nop
 790:	39 d1                	cmp    %edx,%ecx
 792:	72 14                	jb     7a8 <free+0x38>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 794:	39 d0                	cmp    %edx,%eax
 796:	73 10                	jae    7a8 <free+0x38>
{
 798:	89 d0                	mov    %edx,%eax
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 79a:	8b 10                	mov    (%eax),%edx
 79c:	39 c8                	cmp    %ecx,%eax
 79e:	72 f0                	jb     790 <free+0x20>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 7a0:	39 d0                	cmp    %edx,%eax
 7a2:	72 f4                	jb     798 <free+0x28>
 7a4:	39 d1                	cmp    %edx,%ecx
 7a6:	73 f0                	jae    798 <free+0x28>
      break;
  if(bp + bp->s.size == p->s.ptr){
 7a8:	8b 73 fc             	mov    -0x4(%ebx),%esi
 7ab:	8d 3c f1             	lea    (%ecx,%esi,8),%edi
 7ae:	39 fa                	cmp    %edi,%edx
 7b0:	74 1e                	je     7d0 <free+0x60>
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
 7b2:	89 53 f8             	mov    %edx,-0x8(%ebx)
  if(p + p->s.size == bp){
 7b5:	8b 50 04             	mov    0x4(%eax),%edx
 7b8:	8d 34 d0             	lea    (%eax,%edx,8),%esi
 7bb:	39 f1                	cmp    %esi,%ecx
 7bd:	74 28                	je     7e7 <free+0x77>
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
 7bf:	89 08                	mov    %ecx,(%eax)
  freep = p;
}
 7c1:	5b                   	pop    %ebx
  freep = p;
 7c2:	a3 c0 0c 00 00       	mov    %eax,0xcc0
}
 7c7:	5e                   	pop    %esi
 7c8:	5f                   	pop    %edi
 7c9:	5d                   	pop    %ebp
 7ca:	c3                   	ret    
 7cb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 7cf:	90                   	nop
    bp->s.size += p->s.ptr->s.size;
 7d0:	03 72 04             	add    0x4(%edx),%esi
 7d3:	89 73 fc             	mov    %esi,-0x4(%ebx)
    bp->s.ptr = p->s.ptr->s.ptr;
 7d6:	8b 10                	mov    (%eax),%edx
 7d8:	8b 12                	mov    (%edx),%edx
 7da:	89 53 f8             	mov    %edx,-0x8(%ebx)
  if(p + p->s.size == bp){
 7dd:	8b 50 04             	mov    0x4(%eax),%edx
 7e0:	8d 34 d0             	lea    (%eax,%edx,8),%esi
 7e3:	39 f1                	cmp    %esi,%ecx
 7e5:	75 d8                	jne    7bf <free+0x4f>
    p->s.size += bp->s.size;
 7e7:	03 53 fc             	add    -0x4(%ebx),%edx
  freep = p;
 7ea:	a3 c0 0c 00 00       	mov    %eax,0xcc0
    p->s.size += bp->s.size;
 7ef:	89 50 04             	mov    %edx,0x4(%eax)
    p->s.ptr = bp->s.ptr;
 7f2:	8b 53 f8             	mov    -0x8(%ebx),%edx
 7f5:	89 10                	mov    %edx,(%eax)
}
 7f7:	5b                   	pop    %ebx
 7f8:	5e                   	pop    %esi
 7f9:	5f                   	pop    %edi
 7fa:	5d                   	pop    %ebp
 7fb:	c3                   	ret    
 7fc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

00000800 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 800:	f3 0f 1e fb          	endbr32 
 804:	55                   	push   %ebp
 805:	89 e5                	mov    %esp,%ebp
 807:	57                   	push   %edi
 808:	56                   	push   %esi
 809:	53                   	push   %ebx
 80a:	83 ec 1c             	sub    $0x1c,%esp
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 80d:	8b 45 08             	mov    0x8(%ebp),%eax
  if((prevp = freep) == 0){
 810:	8b 3d c0 0c 00 00    	mov    0xcc0,%edi
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 816:	8d 70 07             	lea    0x7(%eax),%esi
 819:	c1 ee 03             	shr    $0x3,%esi
 81c:	83 c6 01             	add    $0x1,%esi
  if((prevp = freep) == 0){
 81f:	85 ff                	test   %edi,%edi
 821:	0f 84 a9 00 00 00    	je     8d0 <malloc+0xd0>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 827:	8b 07                	mov    (%edi),%eax
    if(p->s.size >= nunits){
 829:	8b 48 04             	mov    0x4(%eax),%ecx
 82c:	39 f1                	cmp    %esi,%ecx
 82e:	73 6d                	jae    89d <malloc+0x9d>
 830:	81 fe 00 10 00 00    	cmp    $0x1000,%esi
 836:	bb 00 10 00 00       	mov    $0x1000,%ebx
 83b:	0f 43 de             	cmovae %esi,%ebx
  p = sbrk(nu * sizeof(Header));
 83e:	8d 0c dd 00 00 00 00 	lea    0x0(,%ebx,8),%ecx
 845:	89 4d e4             	mov    %ecx,-0x1c(%ebp)
 848:	eb 17                	jmp    861 <malloc+0x61>
 84a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 850:	8b 10                	mov    (%eax),%edx
    if(p->s.size >= nunits){
 852:	8b 4a 04             	mov    0x4(%edx),%ecx
 855:	39 f1                	cmp    %esi,%ecx
 857:	73 4f                	jae    8a8 <malloc+0xa8>
 859:	8b 3d c0 0c 00 00    	mov    0xcc0,%edi
 85f:	89 d0                	mov    %edx,%eax
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 861:	39 c7                	cmp    %eax,%edi
 863:	75 eb                	jne    850 <malloc+0x50>
  p = sbrk(nu * sizeof(Header));
 865:	83 ec 0c             	sub    $0xc,%esp
 868:	ff 75 e4             	pushl  -0x1c(%ebp)
 86b:	e8 5b fc ff ff       	call   4cb <sbrk>
  if(p == (char*)-1)
 870:	83 c4 10             	add    $0x10,%esp
 873:	83 f8 ff             	cmp    $0xffffffff,%eax
 876:	74 1b                	je     893 <malloc+0x93>
  hp->s.size = nu;
 878:	89 58 04             	mov    %ebx,0x4(%eax)
  free((void*)(hp + 1));
 87b:	83 ec 0c             	sub    $0xc,%esp
 87e:	83 c0 08             	add    $0x8,%eax
 881:	50                   	push   %eax
 882:	e8 e9 fe ff ff       	call   770 <free>
  return freep;
 887:	a1 c0 0c 00 00       	mov    0xcc0,%eax
      if((p = morecore(nunits)) == 0)
 88c:	83 c4 10             	add    $0x10,%esp
 88f:	85 c0                	test   %eax,%eax
 891:	75 bd                	jne    850 <malloc+0x50>
        return 0;
  }
}
 893:	8d 65 f4             	lea    -0xc(%ebp),%esp
        return 0;
 896:	31 c0                	xor    %eax,%eax
}
 898:	5b                   	pop    %ebx
 899:	5e                   	pop    %esi
 89a:	5f                   	pop    %edi
 89b:	5d                   	pop    %ebp
 89c:	c3                   	ret    
    if(p->s.size >= nunits){
 89d:	89 c2                	mov    %eax,%edx
 89f:	89 f8                	mov    %edi,%eax
 8a1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
      if(p->s.size == nunits)
 8a8:	39 ce                	cmp    %ecx,%esi
 8aa:	74 54                	je     900 <malloc+0x100>
        p->s.size -= nunits;
 8ac:	29 f1                	sub    %esi,%ecx
 8ae:	89 4a 04             	mov    %ecx,0x4(%edx)
        p += p->s.size;
 8b1:	8d 14 ca             	lea    (%edx,%ecx,8),%edx
        p->s.size = nunits;
 8b4:	89 72 04             	mov    %esi,0x4(%edx)
      freep = prevp;
 8b7:	a3 c0 0c 00 00       	mov    %eax,0xcc0
}
 8bc:	8d 65 f4             	lea    -0xc(%ebp),%esp
      return (void*)(p + 1);
 8bf:	8d 42 08             	lea    0x8(%edx),%eax
}
 8c2:	5b                   	pop    %ebx
 8c3:	5e                   	pop    %esi
 8c4:	5f                   	pop    %edi
 8c5:	5d                   	pop    %ebp
 8c6:	c3                   	ret    
 8c7:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 8ce:	66 90                	xchg   %ax,%ax
    base.s.ptr = freep = prevp = &base;
 8d0:	c7 05 c0 0c 00 00 c4 	movl   $0xcc4,0xcc0
 8d7:	0c 00 00 
    base.s.size = 0;
 8da:	bf c4 0c 00 00       	mov    $0xcc4,%edi
    base.s.ptr = freep = prevp = &base;
 8df:	c7 05 c4 0c 00 00 c4 	movl   $0xcc4,0xcc4
 8e6:	0c 00 00 
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 8e9:	89 f8                	mov    %edi,%eax
    base.s.size = 0;
 8eb:	c7 05 c8 0c 00 00 00 	movl   $0x0,0xcc8
 8f2:	00 00 00 
    if(p->s.size >= nunits){
 8f5:	e9 36 ff ff ff       	jmp    830 <malloc+0x30>
 8fa:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
        prevp->s.ptr = p->s.ptr;
 900:	8b 0a                	mov    (%edx),%ecx
 902:	89 08                	mov    %ecx,(%eax)
 904:	eb b1                	jmp    8b7 <malloc+0xb7>
