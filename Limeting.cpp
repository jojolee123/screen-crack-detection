#include "canny.h"

/*
* Comments : 非极大值抑制算法细化边缘
* Param ptr : 原图像的灰度值
* Param gra : 原图像的梯度值
* Param height : 原图像的高度
* Param width : 原图像的宽度
*/
void Limeting(uint8** ptr, float** gra, float** theta, uint16 height, uint16 width)
{
	uint16 i, j;
	for (i = 1; i < height - 1; i++)
	{
		for (j = 1; j < width - 1; j++)
		{
			if ((0 < theta[i][j]) && (theta[i][j] <= pi / 4))//0 < theta <= pi/4
			{
				float Gp1 = (float)(gra[i - 1][j + 1] * tan(theta[i][j]) + (1 - tan(theta[i][j]))*gra[i][j + 1]);//插值方法
				float Gp2 = (float)(gra[i + 1][j - 1] * tan(theta[i][j]) + (1 - tan(theta[i][j]))*gra[i][j - 1]);
				if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
				{
					gra[i][j] = -1;//非边缘点标记为-1
					ptr[i][j] = 0;//抑制非边缘点
				}
			}
			else if ((pi / 4 < theta[i][j]) && (theta[i][j] <= pi / 2))//pi/4 < theta <= pi/2
			{
				float Gp1 = (float)(gra[i - 1][j] * tan(theta[i][j]) + (1 - tan(theta[i][j]))*gra[i - 1][j + 1]);//插值方法
				float Gp2 = (float)(gra[i + 1][j] * tan(theta[i][j]) + (1 - tan(theta[i][j]))*gra[i + 1][j - 1]);
				if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
				{
					gra[i][j] = -1;//非边缘点标记为-1
					ptr[i][j] = 0;//抑制非边缘点
				}
			}
			else if ((-pi / 4 < theta[i][j]) && (theta[i][j] <= 0))//-pi/4 < theta <= 0
			{
				float Gp1 = (float)(gra[i + 1][j + 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j + 1]);//插值方法
				float Gp2 = (float)(gra[i - 1][j - 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j - 1]);
				if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
				{
					gra[i][j] = -1;//非边缘点标记为-1
					ptr[i][j] = 0;//抑制非边缘点
				}
			}
			else//-pi/2 < theta <= -pi/4
			{
				float Gp1 = (float)(gra[i + 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i + 1][j + 1]);//插值方法
				float Gp2 = (float)(gra[i - 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i - 1][j - 1]);
				if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
				{
					gra[i][j] = -1;//非边缘点标记为-1
					ptr[i][j] = 0;//抑制非边缘点
				}
			}
		}
	}
	//for (i = 1; i < height - 1; i++)
	//{
	//	for (j = 1; j < width - 1; j++)
	//	{
	//		if ((0 < theta[i][j]) && (theta[i][j] <= pi / 4))//0 < theta <= pi/4
	//		{
	//			float Gp1 = (float)(gra[i - 1][j + 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j + 1]);//插值方法
	//			float Gp2 = (float)(gra[i + 1][j - 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j - 1]);
	//			if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
	//			{
	//				gra[i][j] = -1;//非边缘点标记为-1
	//				ptr[i][j] = 0;//抑制非边缘点
	//			}
	//		}
	//		else if ((pi / 4 < theta[i][j]) && (theta[i][j] <= pi / 2))//pi/4 < theta <= pi/2
	//		{
	//			float Gp1 = (float)(gra[i - 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i - 1][j + 1]);//插值方法
	//			float Gp2 = (float)(gra[i + 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i + 1][j - 1]);
	//			if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
	//			{
	//				gra[i][j] = -1;//非边缘点标记为-1
	//				ptr[i][j] = 0;//抑制非边缘点
	//			}
	//		}
	//		else if ((-pi / 4 < theta[i][j]) && (theta[i][j] <= 0))//-pi/4 < theta <= 0
	//		{
	//			float Gp1 = (float)(gra[i + 1][j + 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j + 1]);//插值方法
	//			float Gp2 = (float)(gra[i - 1][j - 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j - 1]);
	//			if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
	//			{
	//				gra[i][j] = -1;//非边缘点标记为-1
	//				ptr[i][j] = 0;//抑制非边缘点
	//			}
	//		}
	//		else//-pi/2 < theta <= -pi/4
	//		{
	//			float Gp1 = (float)(gra[i + 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i + 1][j + 1]);//插值方法
	//			float Gp2 = (float)(gra[i - 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i - 1][j - 1]);
	//			if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
	//			{
	//				gra[i][j] = -1;//非边缘点标记为-1
	//				ptr[i][j] = 0;//抑制非边缘点
	//			}
	//		}
	//	}
	//}
}