#pragma once
#include "afxwin.h"
#include <iostream>
#include <opencv2\opencv.hpp>

// Param 对话框

class CEParam : public CDialogEx
{
	DECLARE_DYNAMIC(CEParam)

public:
	CEParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEParam();

// 对话框数据
	enum { IDD = IDD_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//验证集大小
	int m_Val_Scale;
	CButton Btn_Rotate;
	CButton Btn_Rotate90;
	CButton Btn_FlipXY;
	CButton Btn_FlipX;
	CButton Btn_FlipY;
	CButton Btn_Crop;
	CButton Btn_Blur;
	CButton Btn_Noise;
	CButton Btn_Contrast;
	CButton Btn_Brightness;
	CButton Btn_Sharpen;
	BOOL m_Flag_Rotate;
	BOOL m_Flag_Rotate90;
	BOOL m_Flag_FlipXY;
	BOOL m_Flag_FlipX;
	BOOL m_Flag_FlipY;
	BOOL m_Flag_Crop;
	BOOL m_Flag_Blur;
	BOOL m_Flag_Noise;
	BOOL m_Flag_Contrast;
	BOOL m_Flag_Brightness;
	BOOL m_Flag_Sharpen;
	//旋转角度
	int m_Rstep;
	// 缩小因子
	double m_crop_scale;
	// 降低亮度步数
	int m_CStep;
	// 降低亮度总数
	int m_CMax;
	// 提升亮度步数
	int m_BStep;
	// 提升亮度总数
	int m_BMax;
	// 锐化阈值
	double m_Sharpen_Thre;
	// 锐化总数
	double m_Sharpen_Amount;
	// 中间目录
	CString m_temp_path;
	std::string m_other_path;
	//xml文件
	std::string m_xml_file;
	int SaveXML(std::string file);
	int LoadXML(std::string file);
	bool m_other_path_flag;
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGetPath();
};
