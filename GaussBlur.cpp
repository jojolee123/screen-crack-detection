#include "canny.h"

/*
* Comments : 高斯平滑函数
* Param ptr : 原图像的灰度值
* Param sigma : 高斯函数参数
* Param size : 高斯平滑模板大小
* Param height : 原图像的高度
* Param width : 原图像的宽度
*/
void Gauss_blur(uint8** ptr, float sigma, uint8 size, uint16 height, uint16 width)
{
	uint16 i,j;
	float** H;
	float sum = 0;
	H = (float**)malloc(size * sizeof(float*));
	for(i = 0; i < size; i++)
	{
		H[i] = (float*)malloc(size * sizeof(float));
	}


	for(i = 0; i < size;i++)
		for (j = 0; j < size; j++)
		{
			H[i][j] = (float)((1.0 / (2 * pi * pow(sigma, 2))) * exp(-(pow(i - (size - 1) / 2 - 1, 2) + pow(j - (size - 1) / 2 - 1, 2)) / (2 * pow(sigma, 2))));//根据高斯函数生成卷积模板
			sum += H[i][j];
		}
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			H[i][j] = H[i][j] / sum;//归一化
		}
	for (i = 2; i < height - 2; i++)
		for (j = 2; j < width - 2; j++)
		{
			ptr[i][j] = (int)H[0][0] * ptr[i - 2][j - 2] + H[0][1] * ptr[i - 2][j - 1] + H[0][2] * ptr[i - 2][j] + H[0][3] * ptr[i - 2][j + 1] + H[0][4] * ptr[i - 2][j + 2] +
				H[1][0] * ptr[i - 1][j - 2] + H[1][1] * ptr[i - 1][j - 1] + H[1][2] * ptr[i - 1][j] + H[1][3] * ptr[i - 1][j + 1] + H[1][4] * ptr[i - 1][j + 2] + 
				H[2][0] * ptr[i][j - 2] + H[2][1] * ptr[i][j - 1] + H[2][2] * ptr[i][j] + H[2][3] * ptr[i][j + 1] + H[2][4] * ptr[i][j + 2] + 
				H[3][0] * ptr[i + 1][j - 2] + H[3][1] * ptr[i + 1][j - 1] + H[3][2] * ptr[i + 1][j] + H[3][3] * ptr[i + 1][j + 1] + H[3][4] * ptr[i + 1][j + 2] +
				H[4][0] * ptr[i + 2][j - 2] + H[4][1] * ptr[i + 2][j - 1] + H[4][2] * ptr[i + 2][j] + H[4][3] * ptr[i + 2][j + 1] + H[4][4] * ptr[i + 2][j + 2];//卷积
		}
	for (i = 0; i < size; i++)
		free(H[i]);
	free(H);
}