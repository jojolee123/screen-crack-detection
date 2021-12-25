
// MFCdemo2Dlg.h: 头文件
//

#pragma once
#include "MVGigE.h"
#include "MVImage.h"
#include "MVCamProptySheet.h"

//int uncharToMat(uchar* p2, Mat& Src, int flag);
// CMFCdemo2Dlg 对话框
class CMFCdemo2Dlg : public CDialogEx
{

// 构造
public:
	CMFCdemo2Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDEMO2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//定义相机句柄对象
	HANDLE m_hCam;
	HANDLE m_hPropDlg;
	//定义图像处理对象
	MVImage m_image;
	//定义像素格式对象
	MV_PixelFormatEnums m_PixelFormat;
	//采集标识
	bool m_bRun;
	//通过回调函数获取图像数据信息
	int OnStreamCB(MV_IMAGE_INFO* pInfo);
	//画图
	void DrawImage();

	//关闭相机、释放资源
	void OnClose();

	//afx_msg void OnBnClickedOpencam();
	//afx_msg void OnBnClickedStartgrab();
	//afx_msg void OnBnClickedClosecam();
	void EnableButton();
	void OpenCamera();
	afx_msg void OnBnClickedFreeRun();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedProperties();
	afx_msg void OnBnClickedStartrun();

	//afx_msg void DrawImage();




};
