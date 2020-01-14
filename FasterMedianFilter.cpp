#include "canny.h"

/*
* Comments : ����������ֵ�ͻҶ�<=��ֵ�����ص����
* Param histogram : ԭͼ��
* Param thresh : �ṹԪ��
* Return : �Ҷ�ֵ255
*/
int calMediaValue(const int histogram[], int thresh)
{
	int sum = 0;
	for (int i = 0; i < 256; ++i)
	{
		sum += histogram[i];
		if (sum >= thresh)
		{
			return i;
		}
	}
	return 255;
}


/*
* Comments : ֱ��ͼ������ֵ�˲�
* Param src: ԭͼ
* Param dst: ���ͼ
* Param diameter: ����˴�С
*/
void fastMedianBlur(Mat &src, Mat &dst, int diameter)
{
	int radius = (diameter - 1) / 2;
	int imgW = src.cols;
	int imgH = src.rows; 
	int channels = src.channels();
	dst = Mat::zeros(imgH, imgW, CV_8UC1);
	int windowSize = (diameter << 1) + 1; 

	//ֱ��ͼ
	int Hist[256] = { 0 };
	int histogramSize = 256;
	int thresholdValue = windowSize / 2 + 1; 

	uchar *pSrcData = src.data;
	uchar *pDstData = dst.data;

	int right = imgW - radius;   
	int bot = imgH - radius;    
	for (int j = radius; j < bot; j++)   // ��  
	{

		for (int i = radius; i < right; i++)   //��
		{
			//ÿһ�е�һ�����˲����ؽ���ֱ��ͼ
			if (i == radius)
			{
				memset(Hist, 0, histogramSize * sizeof(int));
				for (int y = j - radius; y <= min(j + radius, imgH); y++)
				{
					for (int x = i - radius; x <= min(i + radius, imgW); x++)
					{
						uchar value = pSrcData[y*imgW + x];
						Hist[value]++;
					}
				}
			}
			else//����ֱ��ͼ
			{
				int left = i - radius - 1;
				int right = i + radius;
				for (int y = j - radius; y <= min(j + radius, imgH); y++)
				{
					//��ȥ���һ��
					int leftIdx = y * imgW + left;
					uchar leftValue = pSrcData[leftIdx];
					Hist[leftValue]--;

					//�����ұ�һ��
					int rightIdx = y * imgW + right;
					uchar rightValue = pSrcData[rightIdx];
					Hist[rightValue]++;
				}
			}
			//ֱ��ͼ����ֵ
			uchar filterValue = calMediaValue(Hist, thresholdValue);
			pDstData[j*imgW + i] = filterValue;
		}
	}

	//�߽�ֱ�Ӹ�ԭʼֵ�������˲�����
	pSrcData = src.data;
	pDstData = dst.data;
	//���±߽�
	for (int i = 0; i < imgW; i++)
	{
		for (int j = 0; j < radius; j++)
		{
			int idxTop = j * imgW + i;
			pDstData[idxTop] = pSrcData[idxTop];
			int idxBot = (imgH - j - 1)*imgW + i;
			pDstData[idxBot] = pSrcData[idxBot];
		}
	}
	//���ұ߽�
	for (int j = radius; j < imgH - radius - 1; j++)
	{
		for (int i = 0; i < radius; i++)
		{
			int idxLeft = j * imgW + i;
			pDstData[idxLeft] = pSrcData[idxLeft];
			int idxRight = j * imgW + imgW - i - 1;
			pDstData[idxRight] = pSrcData[idxRight];
		}
	}
}

