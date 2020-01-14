#include "canny.h"

/*
* Comments : Matת����
* Param mat: Mat����ͼ��
* Return: ��ά����
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
* Comments : ����תMat
* Param array: ��ά����
* Param row: ͼ���д�С
* Param col: ͼ���д�С
* Return: Mat��ͼ��
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




