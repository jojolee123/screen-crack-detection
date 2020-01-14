#include "canny.h"

/*
* Comments : 计算亮度中值和灰度<=中值的像素点个数
* Param histogram : 原图像
* Param thresh : 结构元素
* Return : 灰度值255
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
* Comments : 直方图快速中值滤波
* Param src: 原图
* Param dst: 结果图
* Param diameter: 卷积核大小
*/
void fastMedianBlur(Mat &src, Mat &dst, int diameter)
{
	int radius = (diameter - 1) / 2;
	int imgW = src.cols;
	int imgH = src.rows; 
	int channels = src.channels();
	dst = Mat::zeros(imgH, imgW, CV_8UC1);
	int windowSize = (diameter << 1) + 1; 

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
			//直方图求中值
			uchar filterValue = calMediaValue(Hist, thresholdValue);
			pDstData[j*imgW + i] = filterValue;
		}
	}

	//边界直接赋原始值，不做滤波处理
	pSrcData = src.data;
	pDstData = dst.data;
	//上下边界
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
	//左右边界
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

