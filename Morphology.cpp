#include "canny.h"

/*
* Comments : 腐蚀
* Param Src : 原图像
* Param Tam : 结构元素
* Param Dst : 结果图
*/
void Erode(Mat& Src, Mat& Dst, Mat& Tem)
{
	int m = (Tem.rows - 1) / 2;
	int n = (Tem.cols - 1) / 2;
	for (int i = m; i < Src.rows - m; i++)//i、j的范围保证结构元始终在扩展后的图像内部
	{
		for (int j = n; j < Src.cols - n; j++)
		{
			Mat SrcROI = Src(Rect(j - m, i - n, Tem.cols, Tem.rows));
			float sum = 0;
			sum = SrcROI.dot(Tem);//矩阵对应位置相乘后求和
			if (sum == 9)//结构元的9个元素均为1，和为9才能保证结构元完全包含于相应集合
				Dst.at<uint8>(i, j) = 255;
			else
				Dst.at<uint8>(i, j) = 0;
		}
	}
}
/*
* Comments : 膨胀
* Param Src : 原图像
* Param Tam : 结构元素
* Param Dst : 结果图
*/
void Dilate(Mat& Src, Mat& Dst, Mat& Tem)
{
	int m = (Tem.rows - 1) / 2;
	int n = (Tem.cols - 1) / 2;
	for (int i = m; i < Src.rows - m; i++)//i、j的范围保证结构元始终在扩展后的图像内部
	{
		for (int j = n; j < Src.cols - n; j++)
		{
			Mat SrcROI = Src(Rect(j - m, i - n, Tem.cols, Tem.rows));
			float sum = 0;
			sum = SrcROI.dot(Tem);//矩阵对应位置相乘后求和
			if (sum != 0)//结构元的9个元素均为1，和为9才能保证结构元完全包含于相应集合
				Dst.at<uint8>(i, j) = 255;
			else
				Dst.at<uint8>(i, j) = 0;
		}
	}
}

/*
* Comments : 2x faster腐蚀, 默认卷积核为3×3矩形
* Param Src : 原图像
* Param Dst : 结果图
*/
void fastDilate(Mat &src, Mat &dst)
{
	int radius = 1;
	int imgW = src.cols;
	int imgH = src.rows;
	dst = Mat::zeros(imgH, imgW, CV_8UC1);
	int windowSize = 9;

	//直方图
	int Hist[256] = { 0 };
	int histogramSize = 256;
	int thresholdValue = windowSize / 2 + 1;

	uchar *pSrcData = src.data;
	uchar *pDstData = dst.data;

	int right = imgW - radius;
	int bot = imgH - radius;
	for (int j = radius; j < bot; j++)   // 行  
	{
		for (int i = radius; i < right; i++)   //列
		{
			//每一行第一个待滤波像素建立直方图
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
			else//更新直方图
			{
				int left = i - radius - 1;
				int right = i + radius;
				for (int y = j - radius; y <= min(j + radius, imgH); y++)
				{
					//减去左边一列
					int leftIdx = y * imgW + left;
					uchar leftValue = pSrcData[leftIdx];
					Hist[leftValue]--;

					//加上右边一列
					int rightIdx = y * imgW + right;
					uchar rightValue = pSrcData[rightIdx];
					Hist[rightValue]++;
				}
			}
			//若灰度值255元素个数不为0
			if (Hist[255] != 0)
				pDstData[j*imgW + i] = 255;
		}
	}
	pDstData = dst.data;
}
