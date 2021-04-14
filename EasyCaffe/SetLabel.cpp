// SetLabel.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyCaffe.h"
#include "SetLabel.h"
#include "afxdialogex.h"


// CSetLabel 对话框

IMPLEMENT_DYNAMIC(CSetLabel, CDialogEx)

CSetLabel::CSetLabel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetLabel::IDD, pParent)
	, m_label(_T(""))
{

}

CSetLabel::~CSetLabel()
{
}

void CSetLabel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LABEL, m_label);
}


BEGIN_MESSAGE_MAP(CSetLabel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetLabel::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetLabel 消息处理程序


void CSetLabel::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
