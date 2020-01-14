#include "canny.h"


void BublleSort(vector<uint16> &arr) 
{
	bool flag = true;
	for (uint16 i = 0; i < arr.size() - 1; ++i) 
	{
		while (flag) 
		{
			flag = false;
			for (uint16 j = 0; j < arr.size() - 1 - i; ++j)
			{
				if (arr[j] > arr[j + 1]) 
				{
					uint16 tmp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = tmp;
					flag = true;
				}
			}
		}
	}
}

void MedianFilter(Mat& src, Mat& dst, uint16 size) 
{
	//ͼ��߽�����
	if (size % 2 == 0) 
	{
		fprintf(stderr, "Please enter odd size!");
		exit(-1);
	}
	uint16 hh = (size - 1) / 2;
	uint16 hw = (size - 1) / 2;
	Mat Newsrc;
	copyMakeBorder(src, Newsrc, hh, hh, hw, hw, BORDER_REFLECT);//��Ե����
	dst = Mat::zeros(src.rows, src.cols, src.type());

	//��ֵ�˲�
	for (uint16 i = hh; i < src.rows + hh - 1; i++) 
	{
		uint8* ptrdst = dst.ptr(i - hh);
		for (uint16 j = hw; j < src.cols + hw - 1; j++) 
		{
			vector<uint16> pix;
			for (uint16 r = i - hh; r <= i + hh; r++) 
			{
				const uint8* ptrsrc = Newsrc.ptr(r);
				for (uint16 c = j - hw; c <= j + hw; c++) 
					pix.push_back(ptrsrc[c]);
			}
			BublleSort(pix);//ð������
			ptrdst[j - hw] = pix[(pow(size, 2) - 1) / 2];//����ֵӳ�䵽���ͼ��
		}
	}
}	