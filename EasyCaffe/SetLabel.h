#pragma once


// CSetLabel �Ի���

class CSetLabel : public CDialogEx
{
	DECLARE_DYNAMIC(CSetLabel)

public:
	CSetLabel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetLabel();

// �Ի�������
	enum { IDD = IDD_SET_LABEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_label;
	afx_msg void OnBnClickedOk();
};
