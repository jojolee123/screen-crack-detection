#include "canny.h"

/*
* Comments : �Ǽ���ֵ�����㷨ϸ����Ե
* Param ptr : ԭͼ��ĻҶ�ֵ
* Param gra : ԭͼ����ݶ�ֵ
* Param height : ԭͼ��ĸ߶�
* Param width : ԭͼ��Ŀ��
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
				float Gp1 = (float)(gra[i - 1][j + 1] * tan(theta[i][j]) + (1 - tan(theta[i][j]))*gra[i][j + 1]);//��ֵ����
				float Gp2 = (float)(gra[i + 1][j - 1] * tan(theta[i][j]) + (1 - tan(theta[i][j]))*gra[i][j - 1]);
				if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
				{
					gra[i][j] = -1;//�Ǳ�Ե����Ϊ-1
					ptr[i][j] = 0;//���ƷǱ�Ե��
				}
			}
			else if ((pi / 4 < theta[i][j]) && (theta[i][j] <= pi / 2))//pi/4 < theta <= pi/2
			{
				float Gp1 = (float)(gra[i - 1][j] * tan(theta[i][j]) + (1 - tan(theta[i][j]))*gra[i - 1][j + 1]);//��ֵ����
				float Gp2 = (float)(gra[i + 1][j] * tan(theta[i][j]) + (1 - tan(theta[i][j]))*gra[i + 1][j - 1]);
				if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
				{
					gra[i][j] = -1;//�Ǳ�Ե����Ϊ-1
					ptr[i][j] = 0;//���ƷǱ�Ե��
				}
			}
			else if ((-pi / 4 < theta[i][j]) && (theta[i][j] <= 0))//-pi/4 < theta <= 0
			{
				float Gp1 = (float)(gra[i + 1][j + 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j + 1]);//��ֵ����
				float Gp2 = (float)(gra[i - 1][j - 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j - 1]);
				if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
				{
					gra[i][j] = -1;//�Ǳ�Ե����Ϊ-1
					ptr[i][j] = 0;//���ƷǱ�Ե��
				}
			}
			else//-pi/2 < theta <= -pi/4
			{
				float Gp1 = (float)(gra[i + 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i + 1][j + 1]);//��ֵ����
				float Gp2 = (float)(gra[i - 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i - 1][j - 1]);
				if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
				{
					gra[i][j] = -1;//�Ǳ�Ե����Ϊ-1
					ptr[i][j] = 0;//���ƷǱ�Ե��
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
	//			float Gp1 = (float)(gra[i - 1][j + 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j + 1]);//��ֵ����
	//			float Gp2 = (float)(gra[i + 1][j - 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j - 1]);
	//			if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
	//			{
	//				gra[i][j] = -1;//�Ǳ�Ե����Ϊ-1
	//				ptr[i][j] = 0;//���ƷǱ�Ե��
	//			}
	//		}
	//		else if ((pi / 4 < theta[i][j]) && (theta[i][j] <= pi / 2))//pi/4 < theta <= pi/2
	//		{
	//			float Gp1 = (float)(gra[i - 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i - 1][j + 1]);//��ֵ����
	//			float Gp2 = (float)(gra[i + 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i + 1][j - 1]);
	//			if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
	//			{
	//				gra[i][j] = -1;//�Ǳ�Ե����Ϊ-1
	//				ptr[i][j] = 0;//���ƷǱ�Ե��
	//			}
	//		}
	//		else if ((-pi / 4 < theta[i][j]) && (theta[i][j] <= 0))//-pi/4 < theta <= 0
	//		{
	//			float Gp1 = (float)(gra[i + 1][j + 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j + 1]);//��ֵ����
	//			float Gp2 = (float)(gra[i - 1][j - 1] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i][j - 1]);
	//			if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
	//			{
	//				gra[i][j] = -1;//�Ǳ�Ե����Ϊ-1
	//				ptr[i][j] = 0;//���ƷǱ�Ե��
	//			}
	//		}
	//		else//-pi/2 < theta <= -pi/4
	//		{
	//			float Gp1 = (float)(gra[i + 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i + 1][j + 1]);//��ֵ����
	//			float Gp2 = (float)(gra[i - 1][j] * (1 - tan(theta[i][j])) + tan(theta[i][j])*gra[i - 1][j - 1]);
	//			if ((gra[i][j] < Gp1) | (gra[i][j] < Gp2))
	//			{
	//				gra[i][j] = -1;//�Ǳ�Ե����Ϊ-1
	//				ptr[i][j] = 0;//���ƷǱ�Ե��
	//			}
	//		}
	//	}
	//}
}