#ifndef FAKE_STDIO_H
#define FAKE_STDIO_H
typedef unsigned long size_t;
#ifndef NULL
#define NULL ((void*)0)
#endif
#define EOF (-1)
int printf(const char*, ...);
int snprintf(char*, size_t, const char*, ...);
int puts(const char*);
int putchar(int);
int getchar(void);
#endif
