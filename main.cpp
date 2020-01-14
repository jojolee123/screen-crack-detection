#include "canny.h"

int main()
{
	uint16 i, j;
	int flag=0;       
	string area, time;
	vector<Feather> featherList;                    // 存放连通域特征
	vector<Point> pointss;
	Mat Image = imread("D:/detection/hough14.bmp");
	Mat dstImg = Image.clone();  //原图备份
	Mat cdst;
	int height = Image.rows;
	int width = Image.cols;
	
	Mat gray;
	cvtColor(Image, gray, CV_BGR2GRAY);	//转灰度图
	Mat grayimg=gray.clone();
	Mat proImg; //= Image.clone();

	int64 t0 = getTickCount();

	//medianBlur(Image, proImg, 3);
	fastMedianBlur(gray, proImg, 3);

	Canny(proImg, proImg, 6, 18);
	cvtColor(proImg, cdst, CV_GRAY2BGR);

	uchar **ptr = (uchar **)malloc(height * sizeof(uchar *));       //二维数组ptr[][]
	for (i = 0; i < height; i++)
		ptr[i] = (uchar *)malloc(width * sizeof(uchar *));
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			ptr[i][j] = proImg.at<uchar>(i, j);                       //img的矩阵数据传给二维数组ptr[][]
		}
	}
	//float t00 = getTickCount();
	vector<struct line> lines = houghLine(ptr, height, width, 90);//90
	vector<struct line> sorted_lines= SortLines(lines);
	vector<struct line> correct_lines = Filter(sorted_lines);

	proImg = drawLines(ptr, cdst, proImg, correct_lines,flag);
	if (flag == 1)
	{
		Mat mage = imread("D:/detection/error.bmp");
		imshow("result", mage);
		waitKey(1000000);
	}
	else
	{
		//float t11 = getTickCount();
		//float timee = (t11 - t00) / cv::getTickFrequency();
		//printf("%2f", timee);
		//system("pause");
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		//Dilate(proImg, Image, element);

		fastDilate(proImg, Image); //膨胀操作

		bwLabel(Image, proImg, grayimg, featherList);//标记连通区域 计算面积

		vector<vector<Point>> contours;

		for (vector<Feather>::iterator it = featherList.begin(); it < featherList.end(); it++)
		{
			if (it->flag == 0)
			{
				area = to_string(it->area);
				rectangle(dstImg, it->boundingbox, Scalar(0, 255, 0), 2, 2);
				putText(dstImg, area, it->boundingbox.br(), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 1, 2);
			}
			else
			{
				area = to_string(it->area);
				rectangle(dstImg, it->boundingbox, Scalar(255, 0, 0), 2, 2);
				putText(dstImg, area, it->boundingbox.br(), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 1, 2);
			}//tl 
			


			/*	area = to_string(it->area);
				rectangle(dstImg, it->boundingbox, Scalar(0, 255, 0), 2, 2);
				putText(dstImg, area, it->boundingbox.br(), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 1, 2);*/
		}
		int64 t1 = getTickCount();
		time = to_string((t1 - t0) / getTickFrequency());
		putText(dstImg, time, Point(10, 10), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 1, 2);
		imshow("Image", Image);
		imshow("dstImg", dstImg);

		waitKey(1000000);
		destroyAllWindows();

		imwrite("F:/detect/canny15.bmp", proImg);

		return 0;
	}
	
	//Mat Image, cdst, edge;
	//int i, j;
	//Image = imread("F:/detect/canny10.bmp", 0);
	//int height = Image.rows;
	//int width = Image.cols;
	//uchar **ptr = (uchar **)malloc(height * sizeof(uchar *));       //二维数组ptr[][]
	//for (i = 0; i < height; i++)
	//	ptr[i] = (uchar *)malloc(width * sizeof(uchar *));
	//for (i = 0; i < height; i++)
	//{
	//	for (j = 0; j < width; j++)
	//	{
	//		ptr[i][j] = Image.at<uchar>(i, j);                       //img的矩阵数据传给二维数组ptr[][]
	//	}
	//}
	//cvtColor(Image, cdst, CV_GRAY2BGR);
	//float t0 = getTickCount();
	//vector<struct line> lines = houghLine(ptr, height, width, 250);
	//float t1 = getTickCount();
	//float time = (t1 - t0) / cv::getTickFrequency();
	//printf("%2f", time);

	//Image = drawLines(ptr, cdst, Image, lines);
	//float t2 = getTickCount();

	//float time0 = (t2 - t1) / cv::getTickFrequency();

	//imwrite("F:/detect/hough16.bmp", Image);

	//printf("\n");
	//printf("%2f", time0);
	//namedWindow("result", 1);
	//imshow("result", Image);
	//waitKey();
	//return 0;
}
