#include <stdio.h>

int main() {
	int f;
	int samid[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int a[10] = { 1374,248364,3840,3764,0,-1,-2,-3,-4,0 };
	int b[10] = { 3144,2342,29743,4382,0,-1,-2,-3,-4,0 };
	int c[10] = { 13155,27573,3474,45083,0,-1,-2,-3,-4,0 };
	int LOWF[40]={0}, MIDF[40] = { 0 }, HIGHF[40] = { 0 };
	int lowpos = 0, midpos = 0, highpos = 0;
	for (int i = 0; i < 10; i++) {
		f = (5 * a[i] + b[i] - c[i] + 100) / 128;
		if (f < 100) {
			LOWF[lowpos++] = samid[i];
			LOWF[lowpos++] = a[i];
			LOWF[lowpos++] = b[i];
			LOWF[lowpos++] = c[i];
		}
		else if (f > 100) {
			HIGHF[lowpos++] = samid[i];
			HIGHF[lowpos++] = a[i];
			HIGHF[lowpos++] = b[i];
			HIGHF[lowpos++] = c[i];
		}
		else {
			MIDF[lowpos++] = samid[i];
			MIDF[lowpos++] = a[i];
			MIDF[lowpos++] = b[i];
			MIDF[lowpos++] = c[i];
		}
	}
	return 0;
}