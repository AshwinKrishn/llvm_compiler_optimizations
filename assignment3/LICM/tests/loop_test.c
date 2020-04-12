#include <stdio.h>

int main() {

        /* local variable definition */
        int i, j, k;

        for (i = 2; i < 100; i++) {

                for (j = 2; j <= (i / j); j++) {
                        if (!(i % j))
                                break; // if factor found, not prime
                        for (k = 0; k < 100; k++) {
                                k = k + i;
                        }
                }
                if (j > (i / j)) {
                        printf("%d is prime\n", i);
                }
        }

        return 0;
}
