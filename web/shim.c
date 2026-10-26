// The handful of C-library functions main.c needs, provided for the freestanding
// wasm build (the browser has no libc). main.c itself is unchanged.
#include <stdarg.h>
typedef unsigned long size_t;

double fabs(double x){ return x < 0 ? -x : x; }

void *memset(void *p, int v, size_t n){ unsigned char *d=p; while(n--) *d++=(unsigned char)v; return p; }
void *memcpy(void *d, const void *s, size_t n){ unsigned char *a=d; const unsigned char *b=s; while(n--) *a++=*b++; return d; }

// snprintf, enough for main.c's format strings (%d and %c)
static void put(char *buf, size_t cap, size_t *pos, char c){ if(*pos+1 < cap) buf[*pos]=c; (*pos)++; }
static void puti(char *buf, size_t cap, size_t *pos, int v){
    if(v < 0){ put(buf,cap,pos,'-'); v=-v; }
    char t[12]; int n=0;
    if(v==0) t[n++]='0';
    while(v){ t[n++]='0'+v%10; v/=10; }
    while(n--) put(buf,cap,pos,t[n]);
}
int snprintf(char *buf, size_t cap, const char *fmt, ...){
    va_list ap; va_start(ap, fmt); size_t pos=0;
    for(const char *f=fmt; *f; f++){
        if(*f=='%'){ f++;
            if(*f=='d') puti(buf,cap,&pos,va_arg(ap,int));
            else if(*f=='c') put(buf,cap,&pos,(char)va_arg(ap,int));
            else if(*f=='%') put(buf,cap,&pos,'%');
            else put(buf,cap,&pos,*f);
        } else put(buf,cap,&pos,*f);
    }
    if(cap) buf[pos<cap?pos:cap-1]=0;
    va_end(ap); return (int)pos;
}

// stubs for the console/dealing parts of main.c the web build never calls
int printf(const char *f, ...){ (void)f; return 0; }
int puts(const char *s){ (void)s; return 0; }
int putchar(int c){ return c; }
int getchar(void){ return -1; }
static unsigned rngState = 1;
void srand(unsigned s){ rngState = s; }
int rand(void){ rngState = rngState*1103515245u + 12345u; return (int)((rngState>>16)&0x7fff); }
long time(long *p){ (void)p; return 0; }
