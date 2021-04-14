#pragma once
#include <iostream>

// CNewProject 对话框

class CNewProject : public CDialogEx
{
	DECLARE_DYNAMIC(CNewProject)

public:
	CNewProject(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewProject();

// 对话框数据
	enum { IDD = IDD_CHOOSE_DIR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_father_path;
	CString m_project_name;
	afx_msg void OnBnClickedChooseFdir();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	std::string m_full_path;
};
