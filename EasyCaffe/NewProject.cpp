// NewProject.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyCaffe.h"
#include "NewProject.h"
#include "afxdialogex.h"
#include "DirFileOption.h"


// CNewProject 对话框

IMPLEMENT_DYNAMIC(CNewProject, CDialogEx)

CNewProject::CNewProject(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewProject::IDD, pParent)
	, m_father_path(_T(""))
	, m_project_name(_T(""))
{

}

CNewProject::~CNewProject()
{
}

void CNewProject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FATHER_PATH, m_father_path);
	DDX_Text(pDX, IDC_NAME, m_project_name);
}


BEGIN_MESSAGE_MAP(CNewProject, CDialogEx)
	ON_BN_CLICKED(IDC_CHOOSE_FDIR, &CNewProject::OnBnClickedChooseFdir)
	ON_BN_CLICKED(IDOK, &CNewProject::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNewProject::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNewProject 消息处理程序


void CNewProject::OnBnClickedChooseFdir()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择路径");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			strFolderPath = szFolderPath;
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
	m_father_path = strFolderPath;
	UpdateData(FALSE);
}


void CNewProject::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	std::string str1 = CW2A(m_father_path);
	/*std::wstring wstr1 = m_father_path;
	std::string str1 = DirFile::wstring2string(wstr1);*/
	std::string str2 = CW2A(m_project_name);
	if (str1.length() < 1 || str2.length() < 1)
	{
		AfxMessageBox(L"输入错误！");
		return;
	}
	m_full_path = DirFile::DirAddSubdir(str1, str2);
	CDialogEx::OnOK();
}


void CNewProject::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
