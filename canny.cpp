#include "canny.h"

void canny(Mat& image, Mat& src, uint8 lowThreshold, uint8 highThreshold)
{
	uint16 i, j;
	uint8 size = 3;
	float sigma = 1.4;
	uint16 height = image.rows;
	uint16 width = image.cols;

	float** gra = (float **)malloc(height * sizeof(float*));
	for (i = 0; i < height; i++)
	{
		gra[i] = (float*)malloc(width * sizeof(float));
	}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			gra[i][j] = 0;

	float** theta = (float **)malloc(height * sizeof(float*));
	for (i = 0; i < height; i++)
	{
		theta[i] = (float*)malloc(width * sizeof(float));
	}
	
	uint8** ptr = (uint8 **)malloc(height * sizeof(uint8*));
	for (i = 0; i < height; i++)
	{
		ptr[i] = (uint8*)malloc(width * sizeof(uint8));
	}
	Mat2Vec(image, ptr);

	Gauss_blur(ptr, sigma, size, height, width);
	Graient(ptr, gra, theta, height, width);
	Limeting(ptr, gra, theta, height, width);
	Double_threshold(ptr, gra, height, width);
	Trace_edge(ptr, gra, height, width);

	src = Vec2Mat(ptr, height, width);

	for (i = 0; i < height; i++)
	{
		free(ptr[i]);
		free(gra[i]);
		free(theta[i]);
	}
	free(ptr);
	free(gra);
	free(theta);
}