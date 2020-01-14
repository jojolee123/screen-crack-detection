#include "canny.h"

/*
* Comments : 连通域与面积
* Param src : 原图像
* Param dst : 结果图
* Param featherList : 连通域特征的清单
* Return : 连通域数量
*/
int bwLabel(Mat& src, Mat& dst, Mat & grayImg, vector<Feather> & featherList)
{
	uint16 rows = src.rows;
	uint16 cols = src.cols;

	uint16 labelValue = 0;
	Point seed, neighbor;
	stack<Point> pointStack;    // 堆栈

	uint16 area = 0;               // 用于计算连通域的面积
	uint16 leftBoundary = 0;       // 连通域的左边界，即外接最小矩形的左边框，横坐标值，依此类推
	uint16 rightBoundary = 0;
	uint16 topBoundary = 0;
	uint16 bottomBoundary = 0;
	uint16 k = 0;
	long sum=0;
	Rect box;                   // 外接矩形框
	Feather feather;

	featherList.clear();    // 清除数组

	dst.release();
	dst = src.clone();
	for (uint16 i = 0; i < rows; i++)
	{
		uint8 *pRow = dst.ptr<uint8>(i);
		for (uint16 j = 0; j < cols; j++)
		{
			if (pRow[j] == 255)
			{
				area = 0;
				labelValue++;           // labelValue最大为254，最小为1.
				seed = Point(j, i);     // Point（横坐标，纵坐标）
				// 数组越界 labelvalue 最大值超过了254，连通区域个数超过了254
				if (labelValue >= 255)
					printf_s("%s\n", "error");
				dst.at<uint8>(seed) = labelValue;
				pointStack.push(seed);

				area++;
				leftBoundary = seed.x;
				rightBoundary = seed.x;
				topBoundary = seed.y;
				bottomBoundary = seed.y;

				k += 1;
				while (!pointStack.empty())
				{
					neighbor = Point(seed.x + 1, seed.y);
					if ((seed.x != (cols - 1)) && (dst.at<uint8>(neighbor) == 255))
					{
						dst.at<uint8>(neighbor) = labelValue;
						pointStack.push(neighbor);
						sum += grayImg.at<uint8>(neighbor);

						area++;
						if (rightBoundary < neighbor.x)
							rightBoundary = neighbor.x;
					}

					neighbor = Point(seed.x, seed.y + 1);
					if ((seed.y != (rows - 1)) && (dst.at<uint8>(neighbor) == 255))
					{
						dst.at<uint8>(neighbor) = labelValue;
						pointStack.push(neighbor);
						sum += grayImg.at<uint8>(neighbor);

						area++;
						if (bottomBoundary < neighbor.y)
							bottomBoundary = neighbor.y;

					}

					neighbor = Point(seed.x - 1, seed.y);
					if ((seed.x != 0) && (dst.at<uint8>(neighbor) == 255))
					{
						dst.at<uint8>(neighbor) = labelValue;
						pointStack.push(neighbor);
						sum += grayImg.at<uint8>(neighbor);

						area++;
						if (leftBoundary > neighbor.x)
							leftBoundary = neighbor.x;
					}

					neighbor = Point(seed.x, seed.y - 1);
					if ((seed.y != 0) && (dst.at<uint8>(neighbor) == 255))
					{
						dst.at<uint8>(neighbor) = labelValue;
						pointStack.push(neighbor);
						sum += grayImg.at<uint8>(neighbor);

						area++;
						if (topBoundary > neighbor.y)
							topBoundary = neighbor.y;
					}

					seed = pointStack.top();
					pointStack.pop();
				}
				int averageGray = sum / area;
				if (averageGray > 30 & averageGray < 100 & area < 5000 & area > 2000)
				{
					box = Rect(leftBoundary, topBoundary, rightBoundary - leftBoundary, bottomBoundary - topBoundary);
					rectangle(src, box, 255);
					feather.area = area;
					feather.boundingbox = box;
					feather.label = labelValue;
					feather.flag = 1;
					featherList.push_back(feather);
				}

				else if (area > 200)
				{
					box = Rect(leftBoundary, topBoundary, rightBoundary - leftBoundary, bottomBoundary - topBoundary);
					rectangle(src, box, 255);
					feather.area = area;
					feather.boundingbox = box;
					feather.label = labelValue;
					feather.flag = 0;
					featherList.push_back(feather);

				}
				else
				{
					labelValue--;
				}
			}
		}
	}
	return labelValue;
}