// NewProject.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyCaffe.h"
#include "NewProject.h"
#include "afxdialogex.h"
#include "DirFileOption.h"


// CNewProject �Ի���

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


// CNewProject ��Ϣ�������


void CNewProject::OnBnClickedChooseFdir()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("��ѡ��·��");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;
	sInfo.lpfn = NULL;

	// ��ʾ�ļ���ѡ��Ի���  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// ȡ���ļ�����  
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	std::string str1 = CW2A(m_father_path);
	/*std::wstring wstr1 = m_father_path;
	std::string str1 = DirFile::wstring2string(wstr1);*/
	std::string str2 = CW2A(m_project_name);
	if (str1.length() < 1 || str2.length() < 1)
	{
		AfxMessageBox(L"�������");
		return;
	}
	m_full_path = DirFile::DirAddSubdir(str1, str2);
	CDialogEx::OnOK();
}


void CNewProject::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
