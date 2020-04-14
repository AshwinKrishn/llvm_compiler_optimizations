#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100
#define NUMBER '0'
#define MAXVAL 100
#define BUFSIZE 100

// int getop(char);
// void push(double);
// double pop(void);

// int global_var = 0;
//
// int main() {
//        int a, b, c, d;
//        a = 1;
//        b = 2;
//        c = a + b;
//        d = c - b;
//        return a;
//}

int main() {
        char type = '0';
        double op2;
        int a, b, c, d, e, f, g, h;
        bool i, j, k = true;
        char s[MAXOP];
        d = 0;
        b = 1;
        c = b - d;
        do {
                switch (type) {
                case NUMBER:
                        a = 1;
                        b = 2;
                        c = 25;
                        if (k) {
                                c = a + b;
                                e = c - a;
                        } else {
                                d = 8;
                                h = 6;
                                f = 24;
                                e = f + h;
                                if (e) {
                                        e -= 1;
                                }
                        }
                        break;
                case '+':
                        break;
                case '*':
                        f = c - d;

                        break;
                case '-':
                        break;
                case '/':
                        break;
                case '\n':
                        break;
                default:
                        break;
                }
        } while (a);

        e = d + f;

        return 0;
}

// int getop(char[]);
// void push(double);
// double pop(void);
// int main() {
//        int type;
//        double op2;
//        char s[MAXOP];
//
//        while ((type = getop(s)) != EOF) {
//                switch (type) {
//                case NUMBER:
//                        push(atof(s));
//                        break;
//                case '+':
//                        push(pop() + pop());
//                        break;
//                case '*':
//                        push(pop() * pop());
//                        break;
//                case '-':
//                        op2 = pop();
//                        push(pop() - op2);
//                        break;
//                case '/':
//                        op2 = pop();
//                        if (op2 != 0.0)
//                                push(pop() / op2);
//                        else
//                                printf("error: zero divisor\n");
//                        break;
//                case '\n':
//                        printf("\t%.8g\n", pop());
//                        break;
//                default:
//                        printf("error: unknown command %s\n", s);
//                        break;
//                }
//        }
//        return 0;
//}
//
// int sp = 0;
// double val[MAXVAL];
//
///* push f onto value stack */
// void push(double f) {
//	if (sp < MAXVAL)
//		val[sp++] = f;
//	else
//		printf("error: stack full\n");
//}
//
///* pop and return top value from stack */
// double pop(void) {
//	if (sp > 0)
//		return val[--sp];
//	else {
//		printf("error: stack empty\n");
//		return 0.0;
//	}
//}
//
// int getch(void);
// void ungetch(int);
//
///* get next operator or numberic operand */
// int getop(char s[]) {
//        int i, c;
//
//        while ((s[0] = c = getch()) == ' ' || c == '\t')
//                ;
//        s[1] = '\0';
//        if (!isdigit(c) && c != '.')
//                return c;
//        i = 0;
//        if (isdigit(c))
//                while (isdigit(s[++i] = c = getch()))
//                        ;
//        if (c == '.')
//                while (isdigit(s[++i] = c = getch()))
//                        ;
//        s[i] = '\0';
//        if (c != EOF)
//                ungetch(c);
//        return NUMBER;
//}
//
// char buf[BUFSIZE];
// int bufp = 0;
//
// int getch(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
//
// void ungetch(int c) {
//	if (bufp >= BUFSIZE)
//		printf("ungetch: toomany characters\n");
//	else
//		buf[bufp++] = c;
//}

