#include <Imlib2.h>
#include <stdio.h>
#include <stdlib.h>

static inline void
die(const char* message) {
	fputs(message, stderr);
	exit(1);
}

int
main (int argc, char** argv) {
	Imlib_Image file;
	Imlib_Image scaled_image;
	int size = 0;
	float threshold = 0.5;
	unsigned int i, j;
	unsigned int w, h;
	float hs, v;
	int a;
	unsigned char group;
	char dither = 0;

	if (argc < 2) die("USAGE: broil IMAGE_FILE [-t THRESHOLD] [-s SIZE] [-d <for dithering>]");

	if (!(file = imlib_load_image(argv[1]))) die ("Unable to open image!");

	imlib_context_set_image(file);

	w = imlib_image_get_width();
	h = imlib_image_get_height();

	for (i = 2; i < argc; i++) {
		if (*argv[i] == '-') {
			switch (argv[i][1]) {
				case 't':
					if (++i >= argc)
						die("Please specify a threshold");
					else
						sscanf(argv[i], "%f", &threshold);
					break;
				case 's':
					if (++i >= argc)
						die("Please specify a size");
					else
						sscanf(argv[i], "%d", &size);
					break;
				case 'd':
					dither = 1;
					break;
			}
		}
	}

	if (size) {
		if (!(scaled_image = imlib_create_cropped_scaled_image(0, 0, w, h, size, size*h/w)))
			die("Couldn't create scaled image!");
		imlib_context_set_image(scaled_image);
	}
	w = imlib_image_get_width();
	h = imlib_image_get_height();

	for (i = 0; i < h; i+=4) {
		for (j = 0; j < w; j+=2) {
			group = 0;

			if (dither) {
				imlib_image_query_pixel_hsva(j  , i  , &hs, &hs, &v, &a); group |= ((int)(100*v) > random()%100)     ;
				imlib_image_query_pixel_hsva(j  , i+1, &hs, &hs, &v, &a); group |= ((int)(100*v) > random()%100) << 1;
				imlib_image_query_pixel_hsva(j  , i+2, &hs, &hs, &v, &a); group |= ((int)(100*v) > random()%100) << 2;
				imlib_image_query_pixel_hsva(j+1, i  , &hs, &hs, &v, &a); group |= ((int)(100*v) > random()%100) << 3;
				imlib_image_query_pixel_hsva(j+1, i+1, &hs, &hs, &v, &a); group |= ((int)(100*v) > random()%100) << 4;
				imlib_image_query_pixel_hsva(j+1, i+2, &hs, &hs, &v, &a); group |= ((int)(100*v) > random()%100) << 5;
				imlib_image_query_pixel_hsva(j  , i+3, &hs, &hs, &v, &a); group |= ((int)(100*v) > random()%100) << 6;
				imlib_image_query_pixel_hsva(j+1, i+3, &hs, &hs, &v, &a); group |= ((int)(100*v) > random()%100) << 7;
			} else {
				imlib_image_query_pixel_hsva(j  , i  , &hs, &hs, &v, &a); group |= (v > threshold)     ;
				imlib_image_query_pixel_hsva(j  , i+1, &hs, &hs, &v, &a); group |= (v > threshold) << 1;
				imlib_image_query_pixel_hsva(j  , i+2, &hs, &hs, &v, &a); group |= (v > threshold) << 2;
				imlib_image_query_pixel_hsva(j+1, i  , &hs, &hs, &v, &a); group |= (v > threshold) << 3;
				imlib_image_query_pixel_hsva(j+1, i+1, &hs, &hs, &v, &a); group |= (v > threshold) << 4;
				imlib_image_query_pixel_hsva(j+1, i+2, &hs, &hs, &v, &a); group |= (v > threshold) << 5;
				imlib_image_query_pixel_hsva(j  , i+3, &hs, &hs, &v, &a); group |= (v > threshold) << 6;
				imlib_image_query_pixel_hsva(j+1, i+3, &hs, &hs, &v, &a); group |= (v > threshold) << 7;
			}

			putchar(0xe2);
			putchar(0xa0 + ((group >> 6) & 0x3));
			putchar(0x80 + (group & 0x3f));
		}
		putchar('\n');
	}
}
