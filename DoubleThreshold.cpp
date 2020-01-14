#include "canny.h"


/*
* Comments : ˫��ֵ�㷨��Ⲣ���ӱ�Ե��
* Param ptr : ԭͼ��ĻҶ�ֵ
* Param gradient_value : ԭͼ����ݶ�ֵ
* Param height : ԭͼ��ĸ߶�
* Param width : ԭͼ��Ŀ��
*/
void Double_threshold(uint8** ptr, float** gradient_value, uint16 height, uint16 width)
{
	uint16 i, j;
	uint8 threshold_high = 0, threshold_low = 0;

	//uint16 Histogram[500];//ֱ��ͼhistogram
	//for (i = 0;i < 500; i++)
	//{
	//	Histogram[i] = 0;//��������
	//}
	//

	//for (i = 1; i < height - 1; i++)
	//{
	//	for (j = 1; j < width - 1; j++)
	//	{
	//		if (gradient_value[i][j] != -1)
	//		{
	//			Histogram[(uint8)gradient_value[i][j]] += 1;//����ÿ�����أ�����ÿ���Ҷȼ������ظ�����
	//		}
	//	}

	//}
	//for (i = 1; i < 500; i++)
	//{
	//	Histogram[i] += Histogram[i - 1];//�����ۼ�ֱ��ͼ	
	//}

	//for (i = 1; i < height; i++)
	//	if (0.7*height*width < Histogram[i])
	//	{
	//		threshold_high = (uint8)i;//����ֵѡ��Ϊ�ۼ�ֱ��ͼ��70%
	//		break;
	//	}
	threshold_high = 50;
	threshold_low = threshold_high / 2;//����ֵΪ����ֵ��һ��

	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++)
		{
			if (gradient_value[i][j] != -1)
			{
				if (gradient_value[i][j] >= threshold_high)//���ڸ���ֵΪǿ��Ե��
				{
					ptr[i][j] = 255;
					gradient_value[i][j] = -2;//ǿ��Ե����Ϊ-2
				}
				else if (gradient_value[i][j] < threshold_low)//С�ڵ���ֵ����
				{
					ptr[i][j] = 0;
					gradient_value[i][j] = -1;//�Ǳ�Ե����Ϊ-1
				}
				else//�ڶ����м�Ϊ����Ե�㣬������
					continue;
			}
		}
	}
}