
// MFCdemo2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCdemo2.h"
#include "MFCdemo2Dlg.h"
#include "afxdialogex.h"
#include "canny.h"
#include "resource.h"
#include <afxdlgs.h>
#include <minwindef.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
int uncharToMat(uchar* p2, Mat& Src, int flag)
{
	int img_width = Src.cols;
	int img_height = Src.rows;
	//Mat img(Size(img_width, img_height), CV_8UC3);
	for (int i = 0; i < img_width * img_height * 3; i++)
	{
		Src.at<Vec3b>(i / (img_width * 3), (i % (img_width * 3)) / 3)[i % 3] = p2[i];//BGR格式
		//Src.at<Vec3b>(i / (img_width * 3), (i % (img_width * 3)) / 3)[i % 3] = p2[i];//换为RGB使用
	}
	flag = 1;
	return flag;
}
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCdemo2Dlg 对话框



CMFCdemo2Dlg::CMFCdemo2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDEMO2_DIALOG, pParent)
	, m_bRun(FALSE)
	, m_hCam(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCdemo2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCdemo2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FREE_RUN, &CMFCdemo2Dlg::OnBnClickedFreeRun)
	ON_BN_CLICKED(IDC_STOP, &CMFCdemo2Dlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_SAVE, &CMFCdemo2Dlg::OnBnClickedSave)

	ON_BN_CLICKED(IDC_PROPERTIES, &CMFCdemo2Dlg::OnBnClickedProperties)
	ON_BN_CLICKED(IDC_STARTRUN, &CMFCdemo2Dlg::OnBnClickedStartrun)

	//ON_BN_CLICKED(IDC_OpenCam, &CMFCdemo2Dlg::OnBnClickedOpencam)
	//ON_BN_CLICKED(IDC_StartGrab, &CMFCdemo2Dlg::OnBnClickedStartgrab)
	//ON_BN_CLICKED(IDC_CloseCam, &CMFCdemo2Dlg::OnBnClickedClosecam)
END_MESSAGE_MAP()


// CMFCdemo2Dlg 消息处理程序

BOOL CMFCdemo2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码
	MVSTATUS_CODES r;
	r = MVInitLib();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("函数库初始化失败"),_T("提示"), MB_ICONWARNING);
		return TRUE;
	}
	r = MVUpdateCameraList();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("查找连接计算机的相机失败！"),_T( "提示"), MB_ICONWARNING);
		return TRUE;
	}
	// 打开相机
	OpenCamera();

	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_DISPLAY_IMAGE);//参数为控件ID
	pWnd->GetClientRect(&rc);//rc为控件的大小。
	int py = rc.Height();
	int px = rc.Width();
	//获取窗口大小
	CRect _GetGroup;
	this->GetClientRect(_GetGroup);
	int y = _GetGroup.Height();
	int x = _GetGroup.Width();

	namedWindow("result", WINDOW_AUTOSIZE);
	HWND hwnd = static_cast<HWND>(cvGetWindowHandle("result"));
	HWND parent = ::GetParent(hwnd);
	::SetParent(hwnd, GetDlgItem(IDC_DISPLAY_IMAGE)->GetSafeHwnd());
	::ShowWindow(parent, SW_HIDE);
	//设置控件位置
	pWnd->MoveWindow(x - 900, y - 800, 800, 600);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCdemo2Dlg::OpenCamera()
{
	CWaitCursor cursor;

	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		MessageBox(_T(" 没 有 找 到 相 机 , 请确认连接和相机 IP 设 置 "), _T(" 提 示"), MB_ICONWARNING);
		return;
	}

	//打开连接到电脑上的第一个相机
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			AfxMessageBox(IDS_ERR_ACCESS_DENY);
		else
		{
			AfxMessageBox(IDS_ERR_OPEN);
		}
		return;
	}

	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		//设置为连续非触发模式
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
	}

	GetDlgItem(IDC_PROPERTIES)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE)->EnableWindow(TRUE);

	if (m_hPropDlg == NULL)
	{
		//创建及初始化属性页对话框
		CString strCaption(MAKEINTRESOURCE(IDS_PROPERTY));
		MVCamProptySheetInit(&m_hPropDlg, m_hCam, this, strCaption);
	}

	EnableButton();
}

void CMFCdemo2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCdemo2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCdemo2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CMFCdemo2Dlg::EnableButton()
{
	GetDlgItem(IDC_FREE_RUN)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_STOP)->EnableWindow(m_bRun);
	GetDlgItem(IDC_STARTRUN)->EnableWindow(m_bRun);
}

int __stdcall StreamCB(MV_IMAGE_INFO* pInfo, ULONG_PTR nUserVal)
{
	CMFCdemo2Dlg* pDlg = (CMFCdemo2Dlg*)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}

void CMFCdemo2Dlg::OnBnClickedFreeRun()
{
	int w, h;
	MV_PixelFormatEnums PixelFormat;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &PixelFormat);

	//根据相机的宽、高、像素格式创建图像，用于接收和绘制采集到的图像
	m_image.CreateByPixelFormat(w, h, PixelFormat);

	//开始连续采集
	MVStartGrab(m_hCam, StreamCB, (ULONG_PTR)this);
	m_bRun = TRUE;

	if (m_hPropDlg != NULL)
	{
		MVCamProptySheetCameraRun(m_hPropDlg, MVCameraRun_ON);
	}


	EnableButton();
}
void CMFCdemo2Dlg::OnBnClickedStop()
{
	//停止连续采集
	MVStopGrab(m_hCam);
	m_bRun = FALSE;

	if (m_hPropDlg != NULL)
	{
		MVCamProptySheetCameraRun(m_hPropDlg, MVCameraRun_OFF);
	}
	EnableButton();
}
void CMFCdemo2Dlg::OnBnClickedSave()
{
	MVImage tImage;

	tImage.Create(m_image.GetWidth(), m_image.GetHeight(), m_image.GetBPP());
	memcpy(tImage.GetBits(), m_image.GetBits(), tImage.GetPitch() * tImage.GetHeight());

	CString strName(MAKEINTRESOURCE(IDS_UNNAMED));
	CFileDialog dlg(FALSE,_T("BMP"), strName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap (*.bmp)|*.bmp|Jpeg (*.jpg)|*.jpg|TIFF (*.tif)|*.tif|");
	if (dlg.DoModal() == IDOK)
	{
		tImage.Save(dlg.GetPathName());
	}
	tImage.Destroy();
}
void CMFCdemo2Dlg::OnBnClickedProperties()
{
	//显示属性页对话框，非模式框
	if (m_hPropDlg != NULL)
	{
		MVCamProptySheetShow(m_hPropDlg, SW_SHOW);
	}
}
void CMFCdemo2Dlg::OnBnClickedStartrun()
{
	int w, h, i, j, flag = 0;
	//Mat cdst;
	//string area;
	string area, time;
	MVSTATUS_CODES r;
	unsigned char* p;
	//Mat dstImg, proImg;
	vector<Feather> featherList;                    // 存放连通域特征
	MV_PixelFormatEnums PixelFormat;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &PixelFormat);
	//根据相机的宽、高、像素格式创建图像，用于接收和绘制采集到的图像
	m_image.CreateByPixelFormat(w, h, PixelFormat);
	r = MVSingleGrab(m_hCam, &m_image, 1500);
	p = (unsigned char*)m_image.GetBits();
	Mat Image(h, w, CV_8UC3, Scalar(0, 0, 0));
	Mat res;
	//Mat Src = imread("D:/dataset/screen_detection/test6.bmp");
	uncharToMat(p, Image, 0);
	
	Mat dstImg = Image.clone();  //原图备份
	Mat cdst;
	int height = Image.rows;
	int width = Image.cols;

	Mat gray;
	cv::cvtColor(Image, gray, cv::COLOR_BGRA2GRAY);
	//cvtColor(Image, gray, CV_BGR2GRAY);	//转灰度图xz
	Mat grayimg = gray.clone();
	Mat proImg; //= Image.clone();

	int64 t0 = getTickCount();

	//medianBlur(Image, proImg, 3);
	fastMedianBlur(gray, proImg, 3);

	Canny(proImg, proImg, 6, 18);
	cv::cvtColor(proImg, cdst, cv::COLOR_GRAY2BGR);
	//cvtColor(proImg, cdst, CV_GRAY2BGR);

	uchar** ptr = (uchar**)malloc(height * sizeof(uchar*));       //二维数组ptr[][]
	for (i = 0; i < height; i++)
		ptr[i] = (uchar*)malloc(width * sizeof(uchar*));
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			ptr[i][j] = proImg.at<uchar>(i, j);                       //img的矩阵数据传给二维数组ptr[][]
		}
	}
	//float t00 = getTickCount();
	vector<struct line> lines = houghLine(ptr, height, width, 90);//90
	vector<struct line> sorted_lines = SortLines(lines);
	vector<struct line> correct_lines = Filter(sorted_lines);

	proImg = drawLines(ptr, cdst, proImg, correct_lines, flag);

	if (flag == 1)
	{
		Mat mage = imread("D:/dataset/screen_detection/erroe.jpeg"); //
		imshow("result", mage);
		waitKey(1000000);
	}

	else
	{

		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		//Dilate(proImg, Image, element);

		fastDilate(proImg, Image); //膨胀操作

		bwLabel(Image, proImg, grayimg, featherList);//标记连通区域 计算面积
		cv::cvtColor(Image, Image, cv::COLOR_GRAY2BGR);
		vector<vector<Point>> contours;

		for (vector<Feather>::iterator it = featherList.begin(); it < featherList.end(); it++)
		{
			if (it->flag == 0)
			{
				area = to_string(it->area);
				rectangle(Image, it->boundingbox, Scalar(0, 255, 0), 2, 2);
				putText(Image, area, it->boundingbox.br(), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 1, 2);
			}
			else
			{
				area = to_string(it->area);
				rectangle(Image, it->boundingbox, Scalar(255, 0, 0), 2, 2);
				putText(Image, area, it->boundingbox.br(), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 1, 2);
			}//tl 


		}
	}

	cv::resize(Image, res, cv::Size(800, 600));
	
	imshow("result", res);
	
	EnableButton();

	MVStartGrab(m_hCam, StreamCB, (ULONG_PTR)this);
}
/*
void CMFCdemo2Dlg::OnBnClickedOpencam()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		MessageBox(_T(" 没 有 找 到 相 机 , 请确认连接和相机 IP 设 置 "), _T(" 提 示"),MB_ICONWARNING);
			return;
	}
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox(_T(" 无 法 打 开 相 机 ， 可 能 正 被 别 的 软 件 控 制 "),_T( " 提 示 "),
				MB_ICONWARNING);
		else
			MessageBox(_T("无法打开相机"),_T("提示"), MB_ICONWARNING);
		return;
	}
	int w, h;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &m_PixelFormat);
	m_image.CreateByPixelFormat(w, h, m_PixelFormat);
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(true);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);

}


void CMFCdemo2Dlg::OnBnClickedStartgrab()
{
	// TODO: 在此添加控件通知处理程序代码
	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}
	MVStartGrab(m_hCam, StreamCB, (long)this);
	m_bRun = true;
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(true);
}

void CMFCdemo2Dlg::OnBnClickedClosecam()
{
	// TODO: 在此添加控件通知处理程序代码
	MVStopGrab(m_hCam);
	MVCloseCam(m_hCam);
	m_bRun = false;
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);

}
*/
void CMFCdemo2Dlg::DrawImage()
{
	int w = m_image.GetWidth();
	int h = m_image.GetHeight();
	const int xDest = 180;
	const int yDest = 240;

	CDC* pDC = GetDC();

	//1:1 显示全图
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest);

	//1:2 显示全图, zoom 50%
	//pDC->SetStretchBltMode(COLORONCOLOR);
	m_image.Draw(pDC->GetSafeHdc(), xDest, yDest, w / 2, h / 2);

	//1:1 显示中间部分图
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2,w/4,h/4,w/2,h/2);
	ReleaseDC(pDC);
}
int CMFCdemo2Dlg::OnStreamCB(MV_IMAGE_INFO* pInfo)
{
	MVInfo2Image(m_hCam, pInfo, &m_image);
	DrawImage();
	return 0;
}
int __stdcall StreamCB(MV_IMAGE_INFO* pInfo, long nUserVal)
{
	CMFCdemo2Dlg* pDlg = (CMFCdemo2Dlg*)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}
void CMFCdemo2Dlg::OnClose()
{
	if (m_bRun != false)
	{
		MVStopGrab(m_hCam);
	}
	MVTerminateLib();
	CDialog::OnClose();
}





