#pragma once
#include "afxwin.h"
#include <iostream>
#include <opencv2\opencv.hpp>

// Param �Ի���

class CEParam : public CDialogEx
{
	DECLARE_DYNAMIC(CEParam)

public:
	CEParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEParam();

// �Ի�������
	enum { IDD = IDD_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//��֤����С
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
	//��ת�Ƕ�
	int m_Rstep;
	// ��С����
	double m_crop_scale;
	// �������Ȳ���
	int m_CStep;
	// ������������
	int m_CMax;
	// �������Ȳ���
	int m_BStep;
	// ������������
	int m_BMax;
	// ����ֵ
	double m_Sharpen_Thre;
	// ������
	double m_Sharpen_Amount;
	// �м�Ŀ¼
	CString m_temp_path;
	std::string m_other_path;
	//xml�ļ�
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
