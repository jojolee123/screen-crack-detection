#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <stack>

using namespace cv;
using namespace std;

const double pi = 3.1415926;
const double RADIAN = 180.0 / pi;

struct line
{
	int theta;
	int r;
};

Point2i getcrosspoint(Point point1, Point point2, Point point3, Point point4, int height, int width); //得到两条直线的交点，要是在图像范围之外 返回0
int judgepoint(uchar** ptr, Mat &img1,Point point, int height, int width);              //判断是否为与边界正确的交点
vector<Point> sort(vector<Point> &result_points);                //对四个点进行排序
Mat frame(uchar** ptr,Mat &img,vector<Point> &sorted_points);                         //矩阵操作 四边形外面全部变黑 0
int judgeinout(int x, int y, vector<Point> &sorted_points);      //判断是否在四边形里面

vector<struct line> houghLine(uchar** ptr,int rows, int cols, int threshold);
int drawLines(uchar** ptr, Mat &img, Mat &img1, const vector<struct line> &lines);


