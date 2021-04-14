#pragma once
#include <iostream>

// CNewProject �Ի���

class CNewProject : public CDialogEx
{
	DECLARE_DYNAMIC(CNewProject)

public:
	CNewProject(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewProject();

// �Ի�������
	enum { IDD = IDD_CHOOSE_DIR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_father_path;
	CString m_project_name;
	afx_msg void OnBnClickedChooseFdir();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	std::string m_full_path;
};
