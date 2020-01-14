#include "canny.h"

/*
* Comments : Sobel算子计算梯度幅值与相位
* Param ptr : 原图像的灰度值
* Param gra : 原图像的梯度值
* Param theta : 原图像的梯度相位
* Param height : 原图像的高度
* Param width : 原图像的宽度
*/
void Graient(uint8** ptr, float** gra, float** theta, uint16 height, uint16 width)
{
	uint16 i, j;
	float** Gx;
	float** Gy;
	Gx = (float**)malloc(3 * sizeof(float*));
	Gy = (float**)malloc(3 * sizeof(float*));
	for (i = 0; i < 3; i++)
	{
		Gx[i] = (float*)malloc(3 * sizeof(float));
		Gy[i] = (float*)malloc(3 * sizeof(float));
	}
	Gx[0][0] = -1; Gx[0][1] = 0, Gx[0][2] = 1,//X方向Sobel算子
	Gx[1][0] = -2, Gx[1][1] = 0, Gx[1][2] = 2,
	Gx[2][0] = -1, Gx[2][1] = 0, Gx[2][2] = 1,

	Gy[0][0] = -1, Gy[0][1] = -2, Gy[0][2] = -1,//Y方向Sobel算子
	Gy[1][0] = 0, Gy[1][1] = 0, Gy[1][2] = 0,
	Gy[2][0] = 1, Gy[2][1] = 2, Gy[2][2] = 1;

	float sumx, sumy;
	for (i = 1; i < height - 1; i++)
		for (j = 1; j < width - 1; j++)
		{
			sumx  = Gx[0][0] * ptr[i - 1][j - 1] + Gx[0][1] * ptr[i - 1][j] + Gx[0][2] * ptr[i - 1][j + 1] +
					Gx[1][0] * ptr[i][j - 1] + Gx[1][1] * ptr[i][j] + Gx[1][2] * ptr[i][j + 1] +
					Gx[2][0] * ptr[i + 1][j - 1] + Gx[2][1] * ptr[i + 1][j] + Gx[2][2] * ptr[i + 1][j + 1];

			sumy  = Gy[0][0] * ptr[i - 1][j - 1] + Gy[0][1] * ptr[i - 1][j] + Gy[0][2] * ptr[i - 1][j + 1] +
					Gy[1][0] * ptr[i][j - 1] + Gy[1][1] * ptr[i][j] + Gy[1][2] * ptr[i][j + 1] +
					Gy[2][0] * ptr[i + 1][j - 1] + Gy[2][1] * ptr[i + 1][j] + Gy[2][2] * ptr[i + 1][j + 1];
			

			//sumx = ptr[i][j + 1] - ptr[i][j - 1];

			//sumy = ptr[i + 1][j] - ptr[i - 1][j];

			gra[i][j] = sqrt(pow(sumx, 2) + pow(sumy, 2));//梯度幅值
			theta[i][j] = (float)atan(sumy / (sumx + 0.001));//梯度相位
		}
	for (i = 0; i < 3; i++)
		free(Gx[i]);
	for (i = 0; i < 3; i++)
		free(Gy[i]);
	free(Gx);
	free(Gy);
}