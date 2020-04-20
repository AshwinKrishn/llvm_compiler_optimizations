int main() {
        int x, z,y = 10,k =100;
        int p = 4;
        for (int i = 0; i < 10000; i++) {
                if (k > 3) {
                        x = y + i;
                } else {
                        x = 3 * p;
                }
                z = x;
		y = k + 1;
		int t = y * 100;
		int p = t /100;	
        }
        return z;
}
