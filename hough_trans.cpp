#include"canny.h"
vector<struct line> houghLine(uchar** ptr, int rows, int cols, int threshold)
{
	int i, j, x, y, y1, y2;
	vector<struct line> lines;
	//vector<struct line> addup;
	vector<struct line> candidate_lines;
	//int diagonal = floor(sqrt(img.rows*img.rows + img.cols*img.cols));  //对角线
	int diagonal = floor(sqrt(rows*rows + cols * cols));
	vector< vector<int> >p(360, vector<int>(diagonal, 0));
	//int count = 0;
	//float tt0 = getTickCount();
	for (j = 0; j < rows; j++)       //构造累加数组	
	{
		for (i = 0; i < cols; i++)
		{
			//if (img.at<unsigned char>(j, i) > 0)

			if (ptr[j][i] == 255&&(i+j)%2==0)
			{
				for (int theta = 0; theta < 360; theta++)
				{
					int r = floor(i*cos(theta / RADIAN) + j * sin(theta / RADIAN));    //!!!!!!!!!!!!!!!
					/*
					if (p[theta][r] >= threshold)
					{
						for (x = j + 1; x < rows; x++)   //删掉这条直线上的点
						{
							y2 = 0;
							/*  越界问题
							y = floor((float(r) - x * sin(theta / RADIAN)) / cos(theta / RADIAN));
							y1 = floor((float(r) - x * sin(theta / RADIAN)) / cos(theta / RADIAN)) + 1;
							y2 = floor((float(r) - x * sin(theta / RADIAN)) / cos(theta / RADIAN)) - 1;
							if (y > 0 && y < cols)
							{
								ptr[x][y] = 0;
							}
							*/
							/*
							if (y1 > 0 && y1 < cols)
							{
								ptr[x][y1] = 0;
							}
							if (y2 > 0 && y2 < cols)
							{
								ptr[x][y2] = 0;
							}
							*/
						//}	
					//}
			
					if (r < 0)
						continue;
					p[theta][r]++;
				}
			}
		}
	}

	//float tt1 = getTickCount();
	//float time = (tt1 - tt0) / cv::getTickFrequency();
	//printf("%2f\n", time);
	//printf("%2f\n", time);

	//system("pause");
	//get local maximum

	for (int theta = 0; theta < 360; theta++)
	{
		for (int r = 0; r < diagonal; r++)
		{
			int thetaLeft = max(0, theta - 1);
			int thetaRight = min(179, theta + 1);
			int rLeft = max(0, r - 1);
			int rRight = min(diagonal - 1, r + 1);
			int tmp = p[theta][r];
			//if (tmp > threshold)
			
			if (tmp > threshold
				&& tmp > p[thetaLeft][rLeft] && tmp > p[thetaLeft][r] && tmp > p[thetaLeft][rRight]
				&& tmp > p[theta][rLeft] && tmp > p[theta][rRight]
				&& tmp > p[thetaRight][rLeft] && tmp > p[thetaRight][r] && tmp > p[thetaRight][rRight])    //tmp大于阈值且区部最大*/
			{
				struct line newline;
				newline.theta = theta;
				newline.r = r;
				lines.push_back(newline);

			}
		}
	}
	
	return lines;
}

Mat drawLines(uchar** ptr,Mat &img, Mat &img1,const vector<struct line> &lines,int &flag)        //横向为x方向 竖直为y方向
{                                    //img是三通道 在上面划线  img1是单通道
	int i,j, count= 0;
	vector<Point> resultlines;      //三条结果直线
	vector<Point> candidate_points;  //三个个候选结果点  三条直线三个交点  其中有一个为0，0
	vector<Point> result_points;     //四个正确结果点
	vector<Point> prefour;   //三条直线的两个交点 超出图像范围一点亦无妨
	vector<Point> sorted_points;     //正确顺序 左上 右上 左下 右下
	//Mat img1 = imread("D:/hough.png", 0);
	int height = img1.rows;
	int width = img1.cols;
	int vertex_x, vertex_y;
	Point temp;

	
	
	for (int i = 0; i < lines.size(); i++)
	{
		vector<Point> points;
		int theta = lines[i].theta;
		int r = lines[i].r;
		double ct = cos(theta / RADIAN);
		double st = sin(theta / RADIAN);
		
		//r = x*ct + y*st

		//if ((i == 0) || (i > 0 && (int(abs(lines[i].theta - lines[i - 1].theta)) > 10 || int(abs(lines[i].r - lines[i - 1].r)) > 150)))


	if ((i == 0)|| ((int(abs(lines[i].theta - lines[i - 1].theta)) > 10 || int(abs(lines[i].r - lines[i - 1].r)) > 250)&&(lines[i].theta>3||lines[0].theta<355)))
			//lines中的数据是按照角度从0到360，所以当前后角度变化超过10°即认定为下一类的直线，
			//前后两类直线平行，则依据他们的r值
		{
			int y0 = int(r / st);           //一条直线的延长线与图像的四条边的延长线都有交点 
			if (y0 >= 0 && y0 < img.rows) { //只保留在图像边界上的两个交点去掉图像边界延长线上的两个交点
				Point p(0, y0);
				points.push_back(p);
			}
			//right
			int y1 = int((r - ct * (img.cols - 1)) / st);
			if (y1 >= 0 && y1 < img.rows)
			{
				Point p(img.cols - 1, y1);
				points.push_back(p);
			}
			//top
			int x0 = int(r / ct);
			if (x0 >= 0 && x0 < img.cols)
			{
				Point p(x0, 0);
				points.push_back(p);
			}
			//down
			int x1 = int((r - st * (img.rows - 1)) / ct);   //确定该直线和图像边界的两个交点points[0], points[1]
			if (x1 >= 0 && x1 < img.cols)
			{
				Point p(x1, img.rows - 1);
				points.push_back(p);
			}
			
			resultlines.push_back(points[0]);
			resultlines.push_back(points[1]);
			if (judgepoint(ptr,img1,points[0], height, width) == 1) //要是判断成功，则加入result_points 只有两个点会判断成功 正确的交点
			{
				result_points.push_back(points[0]);
			}
			if (judgepoint(ptr,img1,points[1], height, width) == 1)
			{
				result_points.push_back(points[1]);
			}
			
			cv::line(img, points[0], points[1], Scalar(0, 0, 255), 1, CV_AA);
			count += 1;
			
			//Point p(lines[i].theta, lines[i].r);
			//resultlines.push_back(p);
		}
	}                                               //三条直线的三个交点 其中一个为0，0
	if (count == 3&&result_points.size()==2)  //两个直线交点+两个有效边界交点
	{
		candidate_points.push_back(getcrosspoint(resultlines[0], resultlines[1], resultlines[2], resultlines[3], height, width)); //分情况 三条直线 
		candidate_points.push_back(getcrosspoint(resultlines[0], resultlines[1], resultlines[4], resultlines[5], height, width));
		candidate_points.push_back(getcrosspoint(resultlines[4], resultlines[5], resultlines[2], resultlines[3], height, width));
		for (count = 0; count < 3; count++)
		{
			if (candidate_points[count].x != 0 || candidate_points[count].y != 0)
			{
				result_points.push_back(candidate_points[count]);
			}
		}
	}
	else if (count == 3 && result_points.size() != 2)
	{
		candidate_points.push_back(getcrosspoint(resultlines[0], resultlines[1], resultlines[2], resultlines[3], height, width)); //分情况 三条直线 
		candidate_points.push_back(getcrosspoint(resultlines[0], resultlines[1], resultlines[4], resultlines[5], height, width));
		candidate_points.push_back(getcrosspoint(resultlines[4], resultlines[5], resultlines[2], resultlines[3], height, width));
		
		for (count = 0; count < 3; count++)                     //先存两个直线交点
		{
			if (candidate_points[count].x != 0 || candidate_points[count].y != 0)
			{
				result_points.push_back(candidate_points[count]);
			}
		}

		for (i = 0; i < result_points.size()-1; i++) //找最右边的点 和最左边的点
		{
			for (j=0;j< result_points.size()-1-i;j++)
			if (result_points[j].x < result_points[j + 1].x)
			{
				temp = result_points[j];
				result_points[j]=result_points[j+ 1];
				result_points[j+ 1] = temp;
			}
		}
		prefour.push_back(result_points[result_points.size() - 1]); //可能最右面的点也是最上面的点
		if (result_points[result_points.size() - 1].x == result_points[result_points.size() - 2].x)
		{
			prefour.push_back(result_points[result_points.size() - 2]);
		}
		prefour.push_back(result_points[0]);
		if (result_points[1].x == result_points[0].x)
		{
			prefour.push_back(result_points[1]);
		}
		
	
		for (i = 0; i < result_points.size()-1; i++) //找最下边的点和最上边的点
		{
			for (j = 0; j < result_points.size() - 1 - i; j++)
				if (result_points[j].y < result_points[j + 1].y)
				{
					temp = result_points[j];
					result_points[j] = result_points[j+ 1];
					result_points[j+ 1] = temp;
				}
		}
		prefour.push_back(result_points[result_points.size() - 1]);
		if (result_points[result_points.size() - 1].y == result_points[result_points.size() - 2].y)
		{
			prefour.push_back(result_points[result_points.size() - 2]);
		}
		prefour.push_back(result_points[0]);
		if (result_points[1].y == result_points[0].y)
		{
			prefour.push_back(result_points[1]);
		}
		
        
		result_points.clear();  //清空


		for (i = 0; i < prefour.size(); i++)             //prefour里面可能有重复的点
		{
			if (!contain(prefour[i],result_points))      //要是结果点集中没有 则加入
			{
				result_points.push_back(prefour[i]);
			}
		}
		
	}
	else if (count == 4)   //四个直线的交点
	{
		candidate_points.push_back(getcrosspoint(resultlines[0], resultlines[1], resultlines[2], resultlines[3], height, width)); //分情况  四条直线
		candidate_points.push_back(getcrosspoint(resultlines[0], resultlines[1], resultlines[4], resultlines[5], height, width));
		candidate_points.push_back(getcrosspoint(resultlines[0], resultlines[1], resultlines[6], resultlines[7], height, width));
		candidate_points.push_back(getcrosspoint(resultlines[2], resultlines[3], resultlines[4], resultlines[5], height, width)); 
		candidate_points.push_back(getcrosspoint(resultlines[2], resultlines[3], resultlines[6], resultlines[7], height, width));
		candidate_points.push_back(getcrosspoint(resultlines[4], resultlines[5], resultlines[6], resultlines[7], height, width));
		for (count = 0; count < 6; count++)
		{
			if (candidate_points[count].x != 0 || candidate_points[count].y != 0)
			{
				result_points.push_back(candidate_points[count]);
			}
		}
	}                            
	//一条直线交点+两个边界有效点（两条直线交于同一条图像边界）
	else if (count == 2 && result_points.size() == 2 &&
		(result_points[0].x == result_points[1].x || result_points[0].y == result_points[1].y))
	{
	    result_points.push_back(getcrosspoint(resultlines[0], resultlines[1], resultlines[2], resultlines[3], height, width));
		
		Mat unconnected1 = frame(3, ptr, img1, result_points);
		return unconnected1;


	}
	//一个直线交点+两个有效边界交点+一个图像顶点（两条直线交于两条边界）
	else if (count == 2 && result_points.size() == 2 && 
	    result_points[0].x != result_points[1].x && result_points[0].y != result_points[1].y)    
	{                                                    
		result_points.push_back(getcrosspoint(resultlines[0], resultlines[1], resultlines[2], resultlines[3], height, width));
		if (result_points[0].x == (img.cols-1)|| result_points[0].x==0)
		{
			vertex_x = result_points[0].x;
		}
		if (result_points[1].x == 0 || result_points[1].x == (img.cols-1))
		{
			vertex_x = result_points[1].x;
		}
		if (result_points[0].y == 0 || result_points[0].y == (img.rows-1))
		{
			vertex_y = result_points[0].y;
		}
		if (result_points[1].y == 0 || result_points[1].y == (img.rows-1))
		{
			vertex_y = result_points[1].y;
		}
		Point vertex(vertex_x, vertex_y);
		result_points.push_back(vertex);
	}
	else if (count == 2 && result_points.size() == 4) //大手机两天平行边框横跨屏幕
	{
	    count = 2;
    }
	else
	{
	    flag = 1;
		return img;
	}
	//else if (count==1)
	int flagg;
	Mat unconnected2;
	sorted_points = sort(result_points,height,width);   //没有阴影！！按照斜率来缩进
	float detay1 = sorted_points[3].y - sorted_points[0].y;
	float detax1 = sorted_points[3].x - sorted_points[0].x;
	float k1 = abs(detay1/detax1);
	float detay2 = sorted_points[1].y - sorted_points[2].y;
	float detax2 = sorted_points[1].x - sorted_points[2].x;
	float k2 = abs(detay2/detax2);
	if (k1 > 10.0 || k1 < 0.15 || k2>10.0 || k2 < 0.15)
	{
		flagg = 0;
	}
	else
	{
		flagg = 1;
	}
	if (flagg == 0)
	{
		if (sorted_points[0].x > sorted_points[0].y)
		{
			sorted_points[0].y += 50;
			sorted_points[1].x -= 50;
			sorted_points[2].x += 50;
			sorted_points[3].y -= 50;
			unconnected2 = frame(4, ptr, img1, sorted_points);
			return unconnected2;
		}
		else
		{
			sorted_points[0].x += 50;
			sorted_points[1].y += 50;
			sorted_points[2].y -= 50;
			sorted_points[3].x -= 50;
			unconnected2 = frame(4, ptr, img1, sorted_points);
			return unconnected2;
		}
	}
	else
	{
		sorted_points[0].x += 30;
		sorted_points[0].y += floor(30 * k1);
		sorted_points[3].x -= 30;
		sorted_points[3].y -= floor(30 * k1);
		sorted_points[2].x += 30;
		sorted_points[2].y -= floor(30 * k2);
		sorted_points[1].x -= 30;
		sorted_points[1].y += floor(30 * k2);
		unconnected2 = frame(4, ptr, img1, sorted_points);
		i = 1;
		return unconnected2;
	}
	
	/*
	sorted_points[0].x += 30; sorted_points[0].y += 100;
	sorted_points[1].x -= 150; sorted_points[1].y += 100;
	sorted_points[2].x += 30; sorted_points[2].y -= 100;
	sorted_points[3].x -= 150; sorted_points[3].y -= 100;
	*/
	//按照斜率关系来缩进
	


	
	//return result_points；
	//printf("%d", sorted_points.size());
	//printf("%5d%5d%5d%5d%5d%5d%5d%5d", sorted_points[0].x, sorted_points[0].y, sorted_points[1].x, sorted_points[1].y, sorted_points[2].x, sorted_points[2].y, sorted_points[3].x, sorted_points[3].y);
	//system("pause");

	//imshow("resultgggggggggg", img);
	//imshow("reggg", success);
	//printf("hhh");

}