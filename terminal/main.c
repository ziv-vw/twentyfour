// 24 points: deal four cards, make 24 with + - * /.
// A=1, J=11, Q=12, K=13.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define EPS 1e-6

struct card { int num; const char *suit; };

static void build_deck(struct card deck[52]) {
    const char *suits[4] = { "SPADE", "HEART", "DIAMOND", "CLUB" };
    int i = 0;
    for (int s = 0; s < 4; s++)
        for (int n = 1; n <= 13; n++)
            deck[i++] = (struct card){ n, suits[s] };
}

// four different cards
static void deal(const struct card deck[52], struct card hand[4]) {
    int picked[4], n = 0;
    while (n < 4) {
        int idx = rand() % 52, dup = 0;
        for (int k = 0; k < n; k++) if (picked[k] == idx) dup = 1;
        if (!dup) { picked[n] = idx; hand[n] = deck[idx]; n++; }
    }
}

static void print_card(struct card c) {
    char buf[3];
    const char *rank = buf;
    switch (c.num) {
        case 1:  rank = "A"; break;
        case 11: rank = "J"; break;
        case 12: rank = "Q"; break;
        case 13: rank = "K"; break;
        default: snprintf(buf, sizeof buf, "%d", c.num);
    }
    printf("  %-7s %s\n", c.suit, rank);
}

static double apply(double a, char op, double b, int *ok) {
    *ok = 1;
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': if (fabs(b) < EPS) { *ok = 0; return 0; } return a / b;
    }
    *ok = 0;
    return 0;
}

// one of the five bracketings, evaluated
static double eval(int shape, double a, double b, double c, double d,
                   char o0, char o1, char o2, int *ok) {
    int k; double x, y; *ok = 1;
    switch (shape) {
        case 0: x=apply(a,o0,b,&k);if(!k)break; x=apply(x,o1,c,&k);if(!k)break; x=apply(x,o2,d,&k);if(!k)break; return x;
        case 1: y=apply(b,o1,c,&k);if(!k)break; x=apply(a,o0,y,&k);if(!k)break; x=apply(x,o2,d,&k);if(!k)break; return x;
        case 2: x=apply(a,o0,b,&k);if(!k)break; y=apply(c,o2,d,&k);if(!k)break; x=apply(x,o1,y,&k);if(!k)break; return x;
        case 3: y=apply(b,o1,c,&k);if(!k)break; y=apply(y,o2,d,&k);if(!k)break; x=apply(a,o0,y,&k);if(!k)break; return x;
        case 4: y=apply(c,o2,d,&k);if(!k)break; y=apply(b,o1,y,&k);if(!k)break; x=apply(a,o0,y,&k);if(!k)break; return x;
    }
    *ok = 0;
    return 0;
}

static const char *FMT[5] = {
    "((%d %c %d) %c %d) %c %d",
    "(%d %c (%d %c %d)) %c %d",
    "(%d %c %d) %c (%d %c %d)",
    "%d %c ((%d %c %d) %c %d)",
    "%d %c (%d %c (%d %c %d))"
};

// look for a way to make 24; write it into `out` and return 1 if found
static int find(const int card[4], char *out, size_t cap) {
    char ops[4] = { '+', '-', '*', '/' };
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
        if (j == i) continue;
        for (int k = 0; k < 4; k++) {
            if (k == i || k == j) continue;
            int l = 6 - i - j - k;
            int a = card[i], b = card[j], c = card[k], d = card[l];
            for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
            for (int z = 0; z < 4; z++)
            for (int s = 0; s < 5; s++) {
                int ok;
                double r = eval(s, a, b, c, d, ops[x], ops[y], ops[z], &ok);
                if (ok && fabs(r - 24.0) < EPS) {
                    snprintf(out, cap, FMT[s], a, ops[x], b, ops[y], c, ops[z], d);
                    return 1;
                }
            }
        }
    }
    return 0;
}

// first non-space char of the line
static int choice(void) {
    int ch;
    do { ch = getchar(); } while (ch == ' ' || ch == '\t' || ch == '\n');
    int first = ch;
    while (ch != '\n' && ch != EOF) ch = getchar();
    return first;
}

int main(void) {
    struct card deck[52], hand[4];
    char answer[128];

    srand((unsigned) time(NULL));
    build_deck(deck);

    puts("24 POINTS");
    puts("Make 24 from the four cards with + - * /. A=1, J=11, Q=12, K=13.\n");

    int played = 0, makeable = 0;
    for (;;) {
        deal(deck, hand);
        played++;

        int card[4];
        puts("Your cards:");
        for (int i = 0; i < 4; i++) { print_card(hand[i]); card[i] = hand[i].num; }

        int solvable = find(card, answer, sizeof answer);
        makeable += solvable;

        printf("\n[y] show a solution  [n] new hand  [q] quit: ");
        int c = choice();
        if (c == 'q' || c == 'Q' || c == EOF) break;
        if (c == 'y' || c == 'Y')
            printf(solvable ? "24 = %s\n\n" : "%s\n\n", solvable ? answer : "This hand can't make 24.");
        else
            putchar('\n');
    }

    printf("\n%d hands, %d of them could make 24. Thanks for playing!\n", played, makeable);
    return 0;
}
