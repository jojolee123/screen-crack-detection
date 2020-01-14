#include "canny.h"


/*
* Comments : 双阈值算法检测并连接边缘点
* Param ptr : 原图像的灰度值
* Param gradient_value : 原图像的梯度值
* Param height : 原图像的高度
* Param width : 原图像的宽度
*/
void Double_threshold(uint8** ptr, float** gradient_value, uint16 height, uint16 width)
{
	uint16 i, j;
	uint8 threshold_high = 0, threshold_low = 0;

	//uint16 Histogram[500];//直方图histogram
	//for (i = 0;i < 500; i++)
	//{
	//	Histogram[i] = 0;//数组清零
	//}
	//

	//for (i = 1; i < height - 1; i++)
	//{
	//	for (j = 1; j < width - 1; j++)
	//	{
	//		if (gradient_value[i][j] != -1)
	//		{
	//			Histogram[(uint8)gradient_value[i][j]] += 1;//遍历每个像素，计算每个灰度级的像素个数和
	//		}
	//	}

	//}
	//for (i = 1; i < 500; i++)
	//{
	//	Histogram[i] += Histogram[i - 1];//计算累计直方图	
	//}

	//for (i = 1; i < height; i++)
	//	if (0.7*height*width < Histogram[i])
	//	{
	//		threshold_high = (uint8)i;//高阈值选择为累计直方图的70%
	//		break;
	//	}
	threshold_high = 50;
	threshold_low = threshold_high / 2;//低阈值为高阈值的一半

	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++)
		{
			if (gradient_value[i][j] != -1)
			{
				if (gradient_value[i][j] >= threshold_high)//大于高阈值为强边缘点
				{
					ptr[i][j] = 255;
					gradient_value[i][j] = -2;//强边缘点标记为-2
				}
				else if (gradient_value[i][j] < threshold_low)//小于低阈值抑制
				{
					ptr[i][j] = 0;
					gradient_value[i][j] = -1;//非边缘点标记为-1
				}
				else//在二者中间为弱边缘点，仍需检测
					continue;
			}
		}
	}
}