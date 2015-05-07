#include <stdio.h>
#include "filters.h"

void filters_toy(unsigned char *image, int width, int height) {
	register int i = width * height;
	register byte* dp = NULL;
	register int r = 0, g = 0, b = 0, gray = 0;
	float rr = 0, gg = 0, bb = 0, max = 0;

    dp = image;
	while (i--) {
		if (PA(dp)) {
			r = PR(dp);
			g = PG(dp);
			b = PB(dp);

			gray = GRAY(r,g, b);
			r = gray + ((430 * (r - gray)) >> 8);
			g = gray + ((430 * (g - gray)) >> 8);
			b = gray + ((430 * (b - gray)) >> 8);

			r = CLAMP(r);
			g = CLAMP(g);
			b = CLAMP(b);

			rr = r / 255.0;
			gg = g / 255.0;
			bb = b / 255.0;

			rr = rr + rr * (1.0 - rr) * 0.2;
			bb = bb - bb * (1.0 - bb) * 0.2;
				gg = gg + gg * (1.0 - gg) * 0.05;

			max = rr > gg ? (rr > bb ? rr : bb) : (gg > bb ? gg : bb);
			if (max > 1.0) {
				rr /= max;
				gg /= max;
				bb /= max;
			}
			r = rr * 255;
			g = gg * 255;
			b = bb * 255;

			PR(dp) = r;
			PG(dp) = g;
			PB(dp) = b;
		}
		dp += 4;
	}
}
