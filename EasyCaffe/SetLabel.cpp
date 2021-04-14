// SetLabel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyCaffe.h"
#include "SetLabel.h"
#include "afxdialogex.h"


// CSetLabel �Ի���

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


// CSetLabel ��Ϣ�������


void CSetLabel::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
