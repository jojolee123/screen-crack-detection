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
	Rect boundingbox;       // ��ͨ�����Ӿ��ο�
	int label;              // ��ͨ���labelֵ
	int area;               // ��ͨ������
	int flag=0;
} Feather;

struct line
{
	int theta;
	int r;
};

const double RADIAN = 180.0 / pi;

/* ת������ */
void Mat2Vec(Mat mat, uint8** ptr);
Mat Vec2Mat(uint8 **array, uint16 row, uint16 col);

/* 80x faster��ֵ�˲�, + 0.5s compared with opencv*/
void fastMedianBlur(Mat& srcImg, Mat& dstImg, int diameter);
int calMediaValue(const int histogram[], int thresh);

/* canny���� */
void Gauss_blur(uint8** ptr, float sigma, uint8 size, uint16 height, uint16 width);
void Graient(uint8** ptr, float** gra, float** theta, uint16 height, uint16 width);
void Limeting(uint8**ptr, float** gra, float** theta, uint16 height, uint16 width);
void Double_threshold(uint8** ptr, float** gra, uint16 height, uint16 width);
void Trace_edge(uint8** ptr, float** gra, uint16 height, uint16 width);
void canny(Mat& image, Mat& src, uint8 lowThreshold, uint8 highThreshold);

/* ��ѧ��̬ѧ���� */
void Dilate(Mat &Src, Mat &Dst, Mat& Tem);
void Erode(Mat &Src, Mat &Dst, Mat& Tem);

/* 2x faster����, + 0.1s compared with opencv */
void fastDilate(Mat &src, Mat &dst);

/* �����ͨ���򲢼������ */
int bwLabel(Mat & src, Mat & dst, Mat & grayImg, vector<Feather> & featherList);


Point2i getcrosspoint(Point point1, Point point2, Point point3, Point point4, int height, int width); //�õ�����ֱ�ߵĽ��㣬Ҫ����ͼ��Χ֮�� ����0
int judgepoint(uchar** ptr, Mat &img1, Point point, int height, int width);              //�ж��Ƿ�Ϊ��߽���ȷ�Ľ���
vector<Point> sort(vector<Point> &result_points,int height,int width);                //���ĸ����������
Mat frame(int count,uchar** ptr, Mat &img, vector<Point> &sorted_points);                         //������� �ı�������ȫ����� 0
int judgeinout(int count,int x, int y, vector<Point> &sorted_points);      //�ж��Ƿ����ı�������
vector<struct line> SortLines(vector<struct line> &lines);   //lines����
vector<struct line> Filter(vector<struct line> &lines);  //linesɸѡ
int FindBase(vector<struct line> &lines);  //���������һ�׼ֱ��
int minn(int a, int b, int c);             //Ѱ�������������е���Сֵ��
bool contain(Point point, vector<Point> &result_points);//�ж�һ��vector���Ƿ���ĳ��point
bool sortcrosspoint(Point point1, Point point2, Point point3, Point point4, int height, int width);//�жϽ����Ƿ���ͼ��Χ��
vector<struct line> houghLine(uchar** ptr, int rows, int cols, int threshold);
Mat drawLines(uchar** ptr, Mat &img, Mat &img1, const vector<struct line> &lines,int &flag);




