#include "canny.h"

Point2i getcrosspoint(Point point1, Point point2, Point point3, Point point4, int height, int width)
{
	double ka, kb;
	Point2i crossPoint;
	ka = (double)(point2.y - point1.y) / (double)(point2.x - point1.x); //求出LineA斜率
	kb = (double)(point4.y - point3.y) / (double)(point4.x - point3.x); //求出LineB斜率

	crossPoint.x = (ka*point1.x - point1.y - kb * point3.x + point3.y) / (ka - kb);
	crossPoint.y = (ka*kb*(point1.x - point3.x) + ka * point3.y - kb * point1.y) / (ka - kb);
	//crosspoint.x = (ka*LineA[0] - LineA[1] - kb * LineB[0] + LineB[1]) / (ka - kb);
	//crossPoint.y = (ka*kb*(LineA[0] - LineB[0]) + ka * LineB[1] - kb * LineA[1]) / (ka - kb);
	if (-500 > crossPoint.x || crossPoint.x > width+500 || -500 > crossPoint.y || crossPoint.y > height+500)
	{
		crossPoint.x = 0;
		crossPoint.y = 0;
		return crossPoint;
	}
	else
	{
		
		return crossPoint;
	}
}

bool sortcrosspoint(Point point1, Point point2, Point point3, Point point4, int height, int width)
{
	double ka, kb;
	Point2i crossPoint;
	ka = (double)(point2.y - point1.y) / (double)(point2.x - point1.x); //求出LineA斜率
	kb = (double)(point4.y - point3.y) / (double)(point4.x - point3.x); //求出LineB斜率

	crossPoint.x = (ka*point1.x - point1.y - kb * point3.x + point3.y) / (ka - kb);
	crossPoint.y = (ka*kb*(point1.x - point3.x) + ka * point3.y - kb * point1.y) / (ka - kb);
	if (0 > crossPoint.x || crossPoint.x > width  || 0 > crossPoint.y || crossPoint.y > height )
	{
		return false;
	}
	else
	{
		return true;
	}
}

int judgepoint(uchar** ptr,Mat &img1,Point point, int height, int width)
{
	//Mat img1 = imread("D:/hough.png", 0);
	//int height = img1.rows;
	//int width = img1.cols;
	int i, j;
	int result;
	/*
	int i, j;
	int result;
	uchar **ptr = (uchar **)malloc(height * sizeof(uchar *));       //二维数组ptr[][]
	for (i = 0; i < height; i++)
		ptr[i] = (uchar *)malloc(width * sizeof(uchar *));
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			ptr[i][j] = img1.at<uchar>(i, j);                       //img的矩阵数据传给二维数组ptr[][]
		}
	}
	*/
	if (point.x == 0)           ////即prt中的y为0 point.x=0 与左边界的交点
	{
		int mins = minn(point.y, height - point.y, 20);                  //正常情况下左右各20的像素范围内 但是要是靠近图像顶点就会越界，所以增加min()函数
		for (i = -mins; i < mins; i++)
		{
			for (j = 0; j < 5; j++)
			{
				if ((ptr[point.y +i][point.x + j]) == 255)
				{
					result = 1;
					return result;
				}
				if (i == 2 && j == 5)
				{
					result = 0;
					return result;
				}
			}
		}
	}
	else if (point.x == width - 1)  //即prt中的y为width - 1  与右边界的交点
	{
		int mins = minn(point.y , height - point.y , 20);
		for (i = -mins; i < mins; i++)
		{
			for (j = 0; j < 5; j++)
			{
				if ((ptr[point.y + i][point.x - j]) == 255)
				{
					result = 1;
					return result;
				}
				if (i == 2 && j == 4)
				{
					result = 0;
					return result;
				}
			}
		}
	}
	else if (point.y == 0)             //即prt中的x为0  与上边界的交点
	{
		int mins = minn(point.x, width - point.x, 20);
		for (i = 0; i < 5; i++)
		{
			for (j = -mins; j < mins; j++)
			{
				if ((ptr[point.y + i][point.x + j]) == 255)
				{
					result = 1;
					return result;
				}
				if (i == 4 && j == 2)
				{
					result = 0;
					return result;
				}
			}
		}
	}
	else if (point.y == height - 1)      //即prt中的x为height-1 与下边界的交点
	{
		int mins = minn(point.x, width - point.x, 20);
		for (i = 0; i < 5; i++)
		{
			for (j = -mins; j < mins; j++)
			{
				if ((ptr[point.y - i][point.x + j]) == 255)
				{
					result = 1;
					return result;
				}
				if (i == 4 && j == 2)
				{
					result = 0;
					return result;
				}
			}
		}
	}
	else
	{
		result = 0;
		return result;
	}
}
vector<Point> sort(vector<Point> &result_points,int height,int width)       //xy之和最小为左上   最大不一定为右下！！！
{                                                      //确定左上后根据交点来确定右下
	int i, j,e;
	double ka, kb;
	Point2i crossPoint;
	
	Point temp;
	i = 0;
	for (i = 0; i < result_points.size() - 1; i++)
	{
		for (j = 0; j < result_points.size() - 1 - i; j++)
		{
			if ((abs(result_points[j].x) + abs(result_points[j].y)) > (abs(result_points[j + 1].x) + abs(result_points[j + 1].y)))
			{
				temp = result_points[j];
				result_points[j] = result_points[j + 1];
				result_points[j + 1] = temp;
			}
		}
	}
	
	if (sortcrosspoint(result_points[0], result_points[1], result_points[2], result_points[3], height, width)) //第二个点在右下
	{
		temp = result_points[1];
		result_points[1] = result_points[3];
		result_points[3] = temp;
	}
	if (sortcrosspoint(result_points[0], result_points[2], result_points[1], result_points[3], height, width)) //第三个点在右下 
	{
		temp = result_points[2];
		result_points[2] = result_points[3];
		result_points[3] = temp;
	}
	//否则 第四个点在右下 即正确的位置
	//int ans = (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);//
	int ans = (result_points[1].y - result_points[0].y)*(result_points[2].x - result_points[0].x) -
		 (result_points[1].x - result_points[0].x)*(result_points[2].y - result_points[0].y);
	if (ans < 0)
	{
		return result_points;
	}
	else        //根据顺时针来确定左下点和右上点
	{
		temp = result_points[1];
		result_points[1] = result_points[2];
		result_points[2] = temp;
	}
	return result_points;
}
//传入参数：四个点坐标  返回参数：图像 Mat 
//将不在这四个点坐标围成的四边形的像素值全部变为0（消除边界的联通问题）
Mat frame(int count,uchar** ptr,Mat &img,vector<Point> &sorted_points)
{
	//Mat img = imread("D:/hough.png", 0);
	
	int height = img.rows;
	int width = img.cols;
	Mat image = Mat(height, width, CV_8UC1);
	int i, j;
	uchar *ptmp = NULL;//这是关键的指针！！
	//uchar **ptr = (uchar **)malloc(height * sizeof(uchar *));       //二维数组ptr[][]
	//for (i = 0; i < height; i++)
		//ptr[i] = (uchar *)malloc(width * sizeof(uchar *));
	for (i = 0; i < height; i++)
	{
		ptmp = image.ptr<uchar>(i);
		for (j = 0; j < width; j++)
		{
			//ptr[i][j] = img.at<uchar>(i, j);                       //img的矩阵数据传给二维数组ptr[][]
			if (ptr[i][j] == 255&&judgeinout(count,i, j, sorted_points) == 1)  
			{
				ptmp[j] = ptr[i][j];
			}
			else
			{
				ptmp[j] = 0;
			}
		}
	}
	return image;
	/*
	float k1, k2, k3, b1, b2, b3;            //三条直线的斜率和截距【xy互换】
	k1 = float(sorted_points[1].x - sorted_points[0].x) / (sorted_points[1].y - sorted_points[0].y);
	k2 = float(sorted_points[2].x - sorted_points[0].x) / (sorted_points[3].y - sorted_points[0].y);
	k3 = float(sorted_points[3].x - sorted_points[2].x) / (sorted_points[3].y - sorted_points[2].y);
	b1 = float(sorted_points[1].x - k1 * sorted_points[1].y);
	b2 = float(sorted_points[2].x - k2 * sorted_points[2].y);
	b3 = float(sorted_points[3].x - k3 * sorted_points[3].y);
	*/

}
int judgeinout(int count,int x, int y, vector<Point> &sorted_points)  //判断一个点是否在该四边形内部 点向量法   四个点顺时针或逆时针！！！不是左上右上左下右下
{
	if (count == 4)
	{
		int a, b, c, d;                             //0123 顺时针为 0231 0->1 2->2 3->3 1->4
		a = (sorted_points[2].y - sorted_points[0].y)*(y - sorted_points[0].x) - (sorted_points[2].x - sorted_points[0].x)*(x - sorted_points[0].y);
		b = (sorted_points[3].y - sorted_points[2].y)*(y - sorted_points[2].x) - (sorted_points[3].x - sorted_points[2].x)*(x - sorted_points[2].y);
		c = (sorted_points[1].y - sorted_points[3].y)*(y - sorted_points[3].x) - (sorted_points[1].x - sorted_points[3].x)*(x - sorted_points[3].y);
		d = (sorted_points[0].y - sorted_points[1].y)*(y - sorted_points[1].x) - (sorted_points[0].x - sorted_points[1].x)*(x - sorted_points[1].y);
		if ((a > 0 && b > 0 && c > 0 && d > 0) || (a < 0 && b < 0 && c < 0 && d < 0))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	if (count == 3)
	{
		int a, b, c;                             //012 顺时针为 0231 0->1 2->2 3->3 1->4
		a = (sorted_points[1].y - sorted_points[0].y)*(y - sorted_points[0].x) - (sorted_points[1].x - sorted_points[0].x)*(x - sorted_points[0].y);
		b = (sorted_points[2].y - sorted_points[1].y)*(y - sorted_points[1].x) - (sorted_points[2].x - sorted_points[1].x)*(x - sorted_points[1].y);
		c = (sorted_points[0].y - sorted_points[2].y)*(y - sorted_points[2].x) - (sorted_points[0].x - sorted_points[2].x)*(x - sorted_points[2].y);
		if ((a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0 ))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
}

vector<struct line> SortLines( vector<struct line> &lines )  //先theta后r 
{
	int i,j;
	struct line temp;
	for (i = 0; i < lines.size(); i++)
	{
		for (j = 0; j < lines.size() - i - 1; j++)
		{
			if (lines[j].theta < lines[j + 1].theta)
			{
				temp = lines[j];
				lines[j] = lines[j + 1];
				lines[j + 1] = temp;
			}
		}
	}
	int based = FindBase(lines);                              //众数
	Point base(lines[based].theta, lines[based].r);
	for (i = 0; i < lines.size(); i++)
	{
		for (j = 0; j < lines.size() - i - 1; j++)
		{
			if (lines[j].r < lines[j + 1].r&&abs(lines[j].theta- lines[j + 1].theta)<6)
			{
				temp = lines[j];
				lines[j] = lines[j + 1];
				lines[j + 1] = temp;
			}
		}
	}
	struct line newline;
	newline.theta = base.x;
	newline.r = base.y;
	lines.push_back(newline);
	return lines;
}

vector<struct line> Filter(vector<struct line> &lines)
{
	int i, j;
	struct line base;
	vector<struct line> correct_lines;
	base = lines[lines.size()-1];
	/*
	for (i = 0; i < lines.size() - 2; i++)
	{
		if ((lines[i].theta - lines[i + 1].theta < 5 && abs(lines[i].r - lines[i + 1].r) < 150)
			&& (lines[i].theta - lines[i + 2].theta < 5 && abs(lines[i].r - lines[i + 2].r) < 150)
			&& (lines[i].theta - lines[i + 3].theta < 5 && abs(lines[i].r - lines[i + 3].r) < 150)
			&& (lines[i].theta - lines[i + 4].theta < 5 && abs(lines[i].r - lines[i + 4].r) < 150)
			&& (lines[i].theta - lines[i + 5].theta < 5 && abs(lines[i].r - lines[i + 5].r) < 150))
		{
			base=lines[i];
			break;
		}
	}
	*/
	for (i = 0; i < lines.size()-1; i++)
	{
		if (abs(lines[i].theta - base.theta) < 3|| abs(abs(base.theta - lines[i].theta) - 90) < 3
			|| abs(abs(base.theta - lines[i].theta) - 180) < 3 || abs(abs(base.theta - lines[i].theta) - 270) < 3
			|| abs(abs(base.theta - lines[i].theta) - 360) < 3)
		{
			correct_lines.push_back(lines[i]);
		}
	}
	return correct_lines;
}
int FindBase(vector<struct line> &lines)
{ 
	int many = 1, less = 1;	
	int temp;
	int value = 0; 	
	for (int i = 0; i < lines.size()-1; i++) 
	{
		for (int j = i; j < lines.size()-1; j++) 
		{
			 if (lines[j].theta == lines[j + 1].theta) 
			//if(abs(lines[j].theta-lines[j+1].theta)<=3)
			{
				less++; 
			} 
			else 
			{ 
				if (many < less) 
				{ 
					temp = many;
					many = less;
					less = temp;
					value = j; 
				}				
				    less = 1;				
					break; 
			}
		}
	}	 	 
	return value; 
}
int minn(int a,int b,int c)
{
	if (a <= b && a <= c)
	{
		return a;
	}
	else if (b <= a && b <= c)
	{
		return b;
	}
	else
	{
		return c;
	}
}
bool contain(Point point,vector<Point> &result_points)
{
	for (int i = 0; i < result_points.size(); i++)
	{
		if (result_points[i] == point)
		{
			return true;
		}
	}
	return false;
}