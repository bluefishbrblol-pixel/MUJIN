// visited-cell count A(t) at log-spaced times
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define W 16384
static unsigned char g[W*W];
int main(int c,char**v){const char*r=v[1];long N=atol(v[2]);int n=strlen(r);
int DX[4]={0,1,0,-1},DY[4]={-1,0,1,0};int x=W/2,y=W/2,d=0;long A=0;double next=1000;
for(long t=1;t<=N;t++){unsigned char*p=&g[(long)y*W+x];if(!*p&&!(*p&0x80)){}
 unsigned char col=*p&0x7f; if(!(*p&0x80)){A++;}
 d=(d+(r[col]=='R'?1:3))&3; *p=0x80|((col+1)%n); x+=DX[d];y+=DY[d];
 if(x<1||y<1||x>W-2||y>W-2){printf("%s edge %ld\n",r,t);return 0;}
 if(t>=next){printf("%s %ld %ld\n",r,t,A);next*=1.5;}}
return 0;}
