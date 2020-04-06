

int main() {
        volatile int d = 0;
        int a = 4;
        int c = 6;
        int b = 0;

        if (d) {
                b = c - a;
        } else {
                int g = 9;
        }
        b = a + c;

        return 0;
}
