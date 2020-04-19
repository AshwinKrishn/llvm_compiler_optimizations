#include <stdio.h>

int main() {

        /* local variable definition */
        int i, j = 0, k = 0;
        int inv_a = 20, inv_b = 10;
        inv_a++;
        inv_b--;
        for (i = 2; i < j; i++) {

                for (j = 2; j <= ((i / j) + 10); j++) {
                        // if (!(i % j))
                        //        break; // if factor found, not prime
                        k = k + 100;
                        k = (long)j;
                        for (k = 0; k < 100; k++) {
                                int inv_c, inv_d;
                                inv_c = inv_a * inv_b;
                                inv_d = inv_c + 1;
                                k = k + i;
                        }
                        j = 100;
                }
                if (j > (i / j)) {
                        k++;
                        // printf("%d is prime\n", i);
                }
        }

        printf("%d, %d, %d, %d, %d \n", j, i, k, inv_a, inv_b);
        return 0;
}
