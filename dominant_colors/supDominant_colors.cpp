#include "dominant_colors.h"
#define CLAMP(value)			(value < 0 ? 0 : value > 255 ? 255 : (value))

void  topEdge(unsigned char* image, int width, int height, int depth, int stride, int color) {
	int i , j;
	int radius = height / 3;
	int dist = radius * radius;
	float rate;
	unsigned char red, green , blue;
	unsigned char* pBuf;
	int r, g, b;
	getcolor(color, red, green, blue);
	//LOG("r = %d, g = %d, b= %d", (int)red, (int)green, (int)blue);
	//green = 0;
	for(j = 0; j < radius; j++) {
		pBuf = image + j * stride;
		rate = ((float)j) / radius;
		for(i = 0; i < width; i++, pBuf += depth) {
			PR(pBuf)	= (unsigned char)CLAMP(red + (PR(pBuf) - red) * rate);
			PG(pBuf)	= (unsigned char)CLAMP(green + (PG(pBuf) - green) * rate);
			PB(pBuf)	= (unsigned char)CLAMP(blue + (PB(pBuf) - blue) * rate);

		/*	r = PR(pBuf);
			g = PG(pBuf);
			b = PB(pBuf);
			
			r = red * (1 - rate);
			g = green * (1 - rate);
			b = blue * (1 - rate);
			if(r <= 128) {
			//	PR(pBuf)	= (unsigned char)CLAMP(r * red * (1 - rate) / 128);
			}else {
				PR(pBuf)	= (unsigned char)CLAMP(255 - (255 - red * (1 - rate) ) * (255 - r) / 128);
			}
			if(g <= 128) {
			//	PG(pBuf)	= (unsigned char)CLAMP(g * green * (1 - rate) / 128);
			}else {
			//	PG(pBuf)	= (unsigned char)CLAMP(255 - (255 - green * (1 - rate) ) * (255 - g) / 128);
			}
			if(b <= 128) {
				PB(pBuf)	= (unsigned char)CLAMP(b * blue * (1 - rate) / 128);
			}else {
				PB(pBuf)	= (unsigned char)CLAMP(255 - (255 - blue * (1 - rate) ) * (255 - b) / 128);
			}

	/*		PR(pBuf)	= (unsigned char)CLAMP(red *(1 - rate));
			PG(pBuf)	= (unsigned char)CLAMP(green *(1 - rate));
			PB(pBuf)	= (unsigned char)CLAMP(blue *(1 - rate));
		/*				
		/*	
	/*		PR(pBuf)	= (unsigned char)CLAMP(PR(pBuf) * rate);
			PG(pBuf)	= (unsigned char)CLAMP(PG(pBuf) * rate);
			PB(pBuf)	= (unsigned char)CLAMP(PB(pBuf) * rate);

/*

			*
			//*/
/*

			r = PR(pBuf);
			g = PG(pBuf);
			b = PB(pBuf);
			PR(pBuf)	= (unsigned char)CLAMP(r + r * red * (1 - rate) / (255 - red * (1-rate)));
			PG(pBuf)	= (unsigned char)CLAMP(g +g * green * (1 - rate) / (255 - red * (1-rate)));
			PB(pBuf)	= (unsigned char)CLAMP(b + b * blue * (1 - rate) / (255 - red * (1-rate)));
/*
			if(r <= 128) {
				PR(pBuf)	= (unsigned char)CLAMP(r * red * (1 - rate) / 128);
			}else {
				PR(pBuf)	= (unsigned char)CLAMP(255 - (255 - red * (1 - rate) ) * (255 - r) / 128);
			}
			if(g <= 128) {
				PG(pBuf)	= (unsigned char)CLAMP(g * green * (1 - rate) / 128);
			}else {
				PG(pBuf)	= (unsigned char)CLAMP(255 - (255 - green * (1 - rate) ) * (255 - g) / 128);
			}
			if(b <= 128) {
				PB(pBuf)	= (unsigned char)CLAMP(b * blue * (1 - rate) / 128);
			}else {
				PB(pBuf)	= (unsigned char)CLAMP(255 - (255 - blue * (1 - rate) ) * (255 - b) / 128);
			}
*/

		}
	}
}

/*
void  fillcolor(unsigned char* rgba, int width, int height, int color) {
	int i = width * height;
	unsigned char red, green, blue;
	unsigned char* pBuf;

	getcolor(color, red, green, blue);
	pBuf = rgba;
	while(i-- > 0) {
		if(PA(pBuf) == 255) {
		//	LOG("nativeProcess :r = %d, g = %d, b= %d", (int)PR(pBuf), (int)PG(pBuf), (int)PB(pBuf));
			PR(pBuf)	= red;
			PG(pBuf)	= green;
			PB(pBuf)	= blue;
		}
		pBuf += 4;
	}

}
*/
