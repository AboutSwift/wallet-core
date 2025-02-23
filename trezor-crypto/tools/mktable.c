#include <stdio.h>
#include <assert.h>
#include <TrezorCrypto/bignum.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/rand.h>

/*
 * This program prints the contents of the ecdsa_curve.cp array.
 * The entry cp[i][j] contains the number (2*j+1)*16^i*G,
 * where G is the generator of the specified elliptic curve.
 */
int main(int argc, char **argv) {
	int i,j,k;
	if (argc != 2) {
		printf("Usage: %s CURVE_NAME\n", argv[0]);
		return 1;
	}
	const char *name = argv[1];
	const curve_info *info = get_curve_by_name(name);
	const ecdsa_curve *curve = info->params;
	if (curve == 0) {
		printf("Unknown curve '%s'\n", name);
		return 1;
	}

	curve_point ng = curve->G;
	curve_point pow2ig = curve->G;
	for (i = 0; i < 64; i++) {
		// invariants:
		//   pow2ig = 16^i * G
		//   ng     = pow2ig
		printf("\t{\n");
		for (j = 0; j < 8; j++) {
			// invariants:
			//   pow2ig = 16^i * G
			//   ng     = (2*j+1) * 16^i * G
#ifndef NDEBUG
			curve_point checkresult;
			bignum256 a;
			bn_zero(&a);
			a.val[(4*i) / 30] = ((uint32_t) 2*j+1) << ((4*i) % 30);
			bn_normalize(&a);
			go_point_multiply(curve, &a, &curve->G, &checkresult);
			assert(go_point_is_equal(&checkresult, &ng));
#endif
			printf("\t\t/* %2d*16^%d*G: */\n\t\t{{{", 2*j + 1, i);
			// print x coordinate
			for (k = 0; k < 9; k++) {
				printf((k < 8 ? "0x%08x, " : "0x%04x"), ng.x.val[k]);
			}
			printf("}},\n\t\t {{");
			// print y coordinate
			for (k = 0; k < 9; k++) {
				printf((k < 8 ? "0x%08x, " : "0x%04x"), ng.y.val[k]);
			}
			if (j == 7) {
				printf("}}}\n\t},\n");
			} else {
				printf("}}},\n");
				go_point_add(curve, &pow2ig, &ng);
			}
			go_point_add(curve, &pow2ig, &ng);
		}
		pow2ig = ng;
	}
	return 0;
}
