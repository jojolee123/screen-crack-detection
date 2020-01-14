#include "canny.h"

/*
* Comments : ��ʴ
* Param Src : ԭͼ��
* Param Tam : �ṹԪ��
* Param Dst : ���ͼ
*/
void Erode(Mat& Src, Mat& Dst, Mat& Tem)
{
	int m = (Tem.rows - 1) / 2;
	int n = (Tem.cols - 1) / 2;
	for (int i = m; i < Src.rows - m; i++)//i��j�ķ�Χ��֤�ṹԪʼ������չ���ͼ���ڲ�
	{
		for (int j = n; j < Src.cols - n; j++)
		{
			Mat SrcROI = Src(Rect(j - m, i - n, Tem.cols, Tem.rows));
			float sum = 0;
			sum = SrcROI.dot(Tem);//�����Ӧλ����˺����
			if (sum == 9)//�ṹԪ��9��Ԫ�ؾ�Ϊ1����Ϊ9���ܱ�֤�ṹԪ��ȫ��������Ӧ����
				Dst.at<uint8>(i, j) = 255;
			else
				Dst.at<uint8>(i, j) = 0;
		}
	}
}
/*
* Comments : ����
* Param Src : ԭͼ��
* Param Tam : �ṹԪ��
* Param Dst : ���ͼ
*/
void Dilate(Mat& Src, Mat& Dst, Mat& Tem)
{
	int m = (Tem.rows - 1) / 2;
	int n = (Tem.cols - 1) / 2;
	for (int i = m; i < Src.rows - m; i++)//i��j�ķ�Χ��֤�ṹԪʼ������չ���ͼ���ڲ�
	{
		for (int j = n; j < Src.cols - n; j++)
		{
			Mat SrcROI = Src(Rect(j - m, i - n, Tem.cols, Tem.rows));
			float sum = 0;
			sum = SrcROI.dot(Tem);//�����Ӧλ����˺����
			if (sum != 0)//�ṹԪ��9��Ԫ�ؾ�Ϊ1����Ϊ9���ܱ�֤�ṹԪ��ȫ��������Ӧ����
				Dst.at<uint8>(i, j) = 255;
			else
				Dst.at<uint8>(i, j) = 0;
		}
	}
}

/*
* Comments : 2x faster��ʴ, Ĭ�Ͼ����Ϊ3��3����
* Param Src : ԭͼ��
* Param Dst : ���ͼ
*/
void fastDilate(Mat &src, Mat &dst)
{
	int radius = 1;
	int imgW = src.cols;
	int imgH = src.rows;
	dst = Mat::zeros(imgH, imgW, CV_8UC1);
	int windowSize = 9;

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
			//���Ҷ�ֵ255Ԫ�ظ�����Ϊ0
			if (Hist[255] != 0)
				pDstData[j*imgW + i] = 255;
		}
	}
	pDstData = dst.data;
}
