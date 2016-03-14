#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"

unsigned char* OriginalImageL; // Left image
unsigned char* OriginalImageR; // Right image
unsigned char* Disparity;

unsigned char* ImageL; // Left image
unsigned char* ImageR; // Right image

unsigned Error; // Error code
unsigned Width, Height;

unsigned char* resize16(unsigned char* image, unsigned w, unsigned h)
{
	unsigned char* resized = (unsigned char*) malloc(3*w*h/16);
	resized[0] = image[0];
	resized[1] = image[1];
	resized[2] = image[2];
	int i;

	for (i = 16; i < 4*w*h - 16; i+= 16) {
		resized[i - 16] = image[i];
		resized[i + 1 - 16] = image[i + 1];
		resized[i + 2 - 16] = image[i + 2];
	}

	return resized;

};

//unsigned char* rgb2gray(unsigned char* image, unsigned w, unsigned h) {

//}

int main(int argc, char** argv)
{
	// Chicking whether images names are given
	if (argc != 3){
		printf("Specify images names!\n");
		return -1;
	}

	// Reading the images to memory
	unsigned w1, h1;
	Error = lodepng_decode32_file(&OriginalImageL, &w1, &h1, argv[1]);
	if(Error) {
		printf("Error in loading of the left image %u: %s\n", Error, lodepng_error_text(Error));
		return -1;
	}

	unsigned w2, h2;
	Error = lodepng_decode32_file(&OriginalImageR, &w2, &h2, argv[2]);
	if(Error){
		printf("Error in loading of the right image %u: %s\n", Error, lodepng_error_text(Error));
		return -1;
	}

	// Checking whether the sizes of images correspond to each other
	if ((w1 != w2) || (h1 != h2)) {
		printf("The sizes of the images do not match!\n");
		return -1;
	}

	Width = w1;
	Height = h1;

	// Resizing the left image and deleting the original array form the memory
	ImageL = resize16(OriginalImageL, Width, Height);
	free(OriginalImageL);
	Error = lodepng_encode24_file("resized_left.png", ImageL, Width/4, Height/4);
	if(Error){
		printf("Error in saving of the left image %u: %s\n", Error, lodepng_error_text(Error));
		return -1;
	}
	// Resizing the right image and deleting the original array form the memory
	ImageR = resize16(OriginalImageR, Width, Height);
	free(OriginalImageR);
	
	Error = lodepng_encode24_file("resized_right.png", ImageR, Width/4, Height/4);
	if(Error){
		printf("Error in saving of the right image %u: %s\n", Error, lodepng_error_text(Error));
		return -1;
	}
	free(ImageL);
	free(ImageR);
	return 0;
}
