// The web build is your actual terminal program: it pulls in main.c unchanged
// and exports a tiny entry so the page can call main.c's own find().
#include "../terminal/main.c"

static char OUTBUF[128];   // find() writes the expression here (like main.c's answer[])

// solve(a,b,c,d): runs main.c's find(); returns the solution length, or -1
__attribute__((export_name("solve")))
int solve(int a, int b, int c, int d){
    int card[4] = { a, b, c, d };
    if(!find(card, OUTBUF, sizeof OUTBUF)) return -1;
    int n = 0; while(OUTBUF[n]) n++;
    return n;
}

__attribute__((export_name("answer")))
int answer(){ return (int)(unsigned long)OUTBUF; }   // address of the buffer
