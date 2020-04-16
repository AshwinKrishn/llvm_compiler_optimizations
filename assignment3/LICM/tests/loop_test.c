#include <stdio.h>

int main() {

        /* local variable definition */
        int i, j, k;
        int inv_a = 20, inv_b = 10;
        inv_a++;
        inv_b--;
        for (i = 2; i < 100; i++) {

                for (j = 2; j <= (i / j); j++) {
                        if (!(i % j))
                                break; // if factor found, not prime
                        for (k = 0; k < 100; k++) {
                                int inv_c, inv_d;
                                inv_c = inv_a * inv_b;
                                inv_d = inv_c + 1;
                                k = k + i;
                        }
                }
                if (j > (i / j)) {

                        printf("%d is prime\n", i);
                }
        }

        return 0;
}
