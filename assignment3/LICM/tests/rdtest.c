#include <stdbool.h>

int main() {
        bool firstBranch = true;
        bool secondBranch = false;
        int a, b, c;
        if (firstBranch) {
                b = 1;
                c = 2;
        } else {
                b = 0;
                c = 1;
        }

        a = b - c;

        if (secondBranch) {
                a = a + 1;
        }
        b = a;
        return 0;
}
