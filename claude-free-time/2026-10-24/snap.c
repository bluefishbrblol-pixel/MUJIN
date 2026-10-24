// dump colors of the painted region as PGM-ish raw: W H then bytes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define W 8192
static unsigned char g[W*W];
int main(int c,char**v){const char*r=v[1];long N=atol(v[2]);int n=strlen(r);
int DX[4]={0,1,0,-1},DY[4]={-1,0,1,0};int x=W/2,y=W/2,d=0,x0=x,x1=x,y0=y,y1=y;
for(long t=0;t<N;t++){unsigned char*p=&g[(long)y*W+x];d=(d+(r[*p]=='R'?1:3))&3;*p=(*p+1)%n;
 x+=DX[d];y+=DY[d];if(x<x0)x0=x;if(x>x1)x1=x;if(y<y0)y0=y;if(y>y1)y1=y;if(x<2||y<2||x>W-3||y>W-3)break;}
FILE*f=fopen(v[3],"wb");int w=x1-x0+1,h=y1-y0+1;fprintf(f,"P5\n%d %d\n255\n",w,h);
for(int yy=y0;yy<=y1;yy++)for(int xx=x0;xx<=x1;xx++){unsigned char q=g[(long)yy*W+xx];fputc(q?40+215*q/(n-1):0,f);}
fclose(f);return 0;}
