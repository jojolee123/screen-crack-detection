#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <stack>
#include <basetsd.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#define Gourd 256
#define pi 3.1415926535

typedef unsigned char uint8;
typedef unsigned short uint16;

using namespace cv;
using namespace std;

typedef struct _Feather
{
	Rect boundingbox;       // 连通域的外接矩形框
	int label;              // 连通域的label值
	int area;               // 连通域的面积
	int flag=0;
} Feather;

struct line
{
	int theta;
	int r;
};

const double RADIAN = 180.0 / pi;

/* 转换函数 */
void Mat2Vec(Mat mat, uint8** ptr);
Mat Vec2Mat(uint8 **array, uint16 row, uint16 col);

/* 80x faster中值滤波, + 0.5s compared with opencv*/
void fastMedianBlur(Mat& srcImg, Mat& dstImg, int diameter);
int calMediaValue(const int histogram[], int thresh);

/* canny算子 */
void Gauss_blur(uint8** ptr, float sigma, uint8 size, uint16 height, uint16 width);
void Graient(uint8** ptr, float** gra, float** theta, uint16 height, uint16 width);
void Limeting(uint8**ptr, float** gra, float** theta, uint16 height, uint16 width);
void Double_threshold(uint8** ptr, float** gra, uint16 height, uint16 width);
void Trace_edge(uint8** ptr, float** gra, uint16 height, uint16 width);
void canny(Mat& image, Mat& src, uint8 lowThreshold, uint8 highThreshold);

/* 数学形态学处理 */
void Dilate(Mat &Src, Mat &Dst, Mat& Tem);
void Erode(Mat &Src, Mat &Dst, Mat& Tem);

/* 2x faster膨胀, + 0.1s compared with opencv */
void fastDilate(Mat &src, Mat &dst);

/* 标记连通区域并计算面积 */
int bwLabel(Mat & src, Mat & dst, Mat & grayImg, vector<Feather> & featherList);


Point2i getcrosspoint(Point point1, Point point2, Point point3, Point point4, int height, int width); //得到两条直线的交点，要是在图像范围之外 返回0
int judgepoint(uchar** ptr, Mat &img1, Point point, int height, int width);              //判断是否为与边界正确的交点
vector<Point> sort(vector<Point> &result_points,int height,int width);                //对四个点进行排序
Mat frame(int count,uchar** ptr, Mat &img, vector<Point> &sorted_points);                         //矩阵操作 四边形外面全部变黑 0
int judgeinout(int count,int x, int y, vector<Point> &sorted_points);      //判断是否在四边形里面
vector<struct line> SortLines(vector<struct line> &lines);   //lines排序
vector<struct line> Filter(vector<struct line> &lines);  //lines筛选
int FindBase(vector<struct line> &lines);  //依据众数找基准直线
int minn(int a, int b, int c);             //寻找三个正整数中的最小值，
bool contain(Point point, vector<Point> &result_points);//判断一个vector中是否有某个point
bool sortcrosspoint(Point point1, Point point2, Point point3, Point point4, int height, int width);//判断交点是否在图像范围内
vector<struct line> houghLine(uchar** ptr, int rows, int cols, int threshold);
Mat drawLines(uchar** ptr, Mat &img, Mat &img1, const vector<struct line> &lines,int &flag);




