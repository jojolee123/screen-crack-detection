#include "canny.h"

/*
* Comments : Mat转矩阵
* Param mat: Mat类型图像
* Return: 二维矩阵
*/
void Mat2Vec(Mat mat, uint8** ptr)
{
	for (uint16 i = 0; i < mat.rows; ++i)
	{
		for (uint16 j = 0; j < mat.cols; ++j)
		{
			ptr[i][j] = mat.at<uint8>(i, j);
		}
	}
}

/*
* Comments : 矩阵转Mat
* Param array: 二维矩阵
* Param row: 图像行大小
* Param col: 图像列大小
* Return: Mat类图像
*/
Mat Vec2Mat(uint8 **array, uint16 row, uint16 col)
{

	Mat img(row, col, CV_8UC1);
	uint8 *ptmp = NULL;
	for (uint16 i = 0; i < row; ++i)
	{
		ptmp = img.ptr<uint8>(i);
		for (uint16 j = 0; j < col; ++j)
		{
			ptmp[j] = array[i][j];
		}
	}
	return img;
}

void Mat2Array(Mat mat, unsigned char *array)
{
	if (mat.isContinuous())
		array = mat.data;
}




