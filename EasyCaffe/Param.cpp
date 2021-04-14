// Param.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyCaffe.h"
#include "Param.h"
#include "afxdialogex.h"


// Param 对话框

IMPLEMENT_DYNAMIC(CEParam, CDialogEx)

CEParam::CEParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEParam::IDD, pParent)
	, m_Val_Scale(0)
	, m_Rstep(0)
	, m_crop_scale(0)
	, m_CStep(0)
	, m_CMax(0)
	, m_BStep(0)
	, m_BMax(0)
	, m_Sharpen_Thre(0)
	, m_Sharpen_Amount(0)
	, m_temp_path(_T(""))
{

}

CEParam::~CEParam()
{
}

void CEParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VAL_SCALE, m_Val_Scale);
	DDX_Control(pDX, IDC_ROTATE_FLAG, Btn_Rotate);
	DDX_Control(pDX, IDC_ROTATE90_FLAG, Btn_Rotate90);
	DDX_Control(pDX, IDC_FLIPXY_FLAG, Btn_FlipXY);
	DDX_Control(pDX, IDC_FLIPX_FLAG, Btn_FlipX);
	DDX_Control(pDX, IDC_FLIPY_FLAG, Btn_FlipY);
	DDX_Control(pDX, IDC_CROP_FLAG, Btn_Crop);
	DDX_Control(pDX, IDC_BLUR_FLAG, Btn_Blur);
	DDX_Control(pDX, IDC_NOISE_FLAG, Btn_Noise);
	DDX_Control(pDX, IDC_CONTRAST_FLAG, Btn_Contrast);
	DDX_Control(pDX, IDC_BRIGHTNESS_FLAG, Btn_Brightness);
	DDX_Control(pDX, IDC_SHARPEN_FLAG, Btn_Sharpen);
	DDX_Text(pDX, IDC_ROTATE_STEP, m_Rstep);
	DDX_Text(pDX, IDC_CROP_SACLE, m_crop_scale);
	DDX_Text(pDX, IDC_CONTRAST_STEP, m_CStep);
	DDX_Text(pDX, IDC_CONTRAST_MAX, m_CMax);
	DDX_Text(pDX, IDC_BRIGHTNESS_STEP, m_BStep);
	DDX_Text(pDX, IDC_BRIGHTNESS_MAX, m_BMax);
	DDX_Text(pDX, IDC_SHARPEN_THRE, m_Sharpen_Thre);
	DDX_Text(pDX, IDC_SHARPEN_AMOUNT, m_Sharpen_Amount);
	DDX_Text(pDX, IDC_TEMP_PATH, m_temp_path);
	DDV_MinMaxInt(pDX, m_Rstep, 0, 360);
	DDV_MinMaxDouble(pDX, m_crop_scale, 0.5, 0.9);
	DDV_MinMaxInt(pDX, m_CStep, 1, 255);
	DDV_MinMaxInt(pDX, m_BStep, 1, 255);
	DDV_MinMaxInt(pDX, m_CMax, 1, 255);
	DDV_MinMaxInt(pDX, m_BMax, 1, 255);
	DDV_MinMaxDouble(pDX, m_Sharpen_Thre, 1, 255);
	DDV_MinMaxDouble(pDX, m_Sharpen_Amount, 1, 255);
}


BEGIN_MESSAGE_MAP(CEParam, CDialogEx)
	ON_BN_CLICKED(IDC_APPLY, &CEParam::OnBnClickedApply)
	ON_BN_CLICKED(IDOK, &CEParam::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEParam::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_GET_PATH, &CEParam::OnBnClickedGetPath)
END_MESSAGE_MAP()


// Param 消息处理程序


int CEParam::SaveXML(std::string file)
{
	UpdateData(TRUE);
	m_other_path = CW2A(m_temp_path);
	cv::FileStorage fs(file, cv::FileStorage::WRITE);
	if (!fs.isOpened())
	{
		return -2;
	}
	m_Flag_Rotate = Btn_Rotate.GetCheck();
	m_Flag_Rotate90 = Btn_Rotate90.GetCheck();
	m_Flag_FlipXY = Btn_FlipXY.GetCheck();
	m_Flag_FlipX = Btn_FlipX.GetCheck();
	m_Flag_FlipY = Btn_FlipY.GetCheck();
	m_Flag_Crop = Btn_Crop.GetCheck();
	m_Flag_Blur = Btn_Blur.GetCheck();
	m_Flag_Noise = Btn_Noise.GetCheck();
	m_Flag_Contrast = Btn_Contrast.GetCheck();
	m_Flag_Brightness = Btn_Brightness.GetCheck();
	m_Flag_Sharpen = Btn_Sharpen.GetCheck();
	fs << "m_Flag_Rotate" << m_Flag_Rotate;
	fs << "m_Flag_Rotate90" << m_Flag_Rotate90;
	fs << "m_Flag_FlipXY" << m_Flag_FlipXY;
	fs << "m_Flag_FlipX" << m_Flag_FlipX;
	fs << "m_Flag_FlipY" << m_Flag_FlipY;
	fs << "m_Flag_Crop" << m_Flag_Crop;
	fs << "m_Flag_Blur" << m_Flag_Blur;
	fs << "m_Flag_Contrast" << m_Flag_Contrast;
	fs << "m_Flag_Brightness" << m_Flag_Brightness;
	fs << "m_Flag_Noise" << m_Flag_Noise;
	fs << "m_Flag_Sharpen" << m_Flag_Sharpen;
	fs << "m_Rotate_Step" << m_Rstep;
	fs << "m_Crop_Scale" << m_crop_scale;
	fs << "m_Contrast_Max" << m_CMax;
	fs << "m_Contrast_Step" << m_CStep;
	fs << "m_Brightness_Max" << m_BMax;
	fs << "m_Brightness_Step" << m_BStep;
	fs << "m_Other_Path_Flag" << m_other_path_flag;
	fs << "m_Sharpen_Thre" << m_Sharpen_Thre;
	fs << "m_Sharpen_Amount" << m_Sharpen_Amount;
	fs << "m_Val_Scale" << m_Val_Scale;
	fs << "m_Other_Path" << m_other_path;
	fs.release();
	return 0;
}

int CEParam::LoadXML(std::string file)
{
	cv::FileStorage fs(file, cv::FileStorage::READ);
	if (!fs.isOpened())
	{
		return -2;
	}
	fs["m_Flag_Rotate"] >> m_Flag_Rotate; 
	Btn_Rotate.SetCheck(m_Flag_Rotate);
	fs["m_Flag_Rotate90"] >> m_Flag_Rotate90;
	Btn_Rotate90.SetCheck(m_Flag_Rotate90);
	fs["m_Flag_FlipX"] >> m_Flag_FlipX;
	Btn_FlipX.SetCheck(m_Flag_FlipX);
	fs["m_Flag_FlipY"] >> m_Flag_FlipY;
	Btn_FlipY.SetCheck(m_Flag_FlipY);
	fs["m_Flag_FlipXY"] >> m_Flag_FlipXY;
	Btn_FlipXY.SetCheck(m_Flag_FlipXY);
	fs["m_Flag_Crop"] >> m_Flag_Crop;
	Btn_Crop.SetCheck(m_Flag_Crop);
	fs["m_Flag_Blur"] >> m_Flag_Blur;
	Btn_Blur.SetCheck(m_Flag_Blur);
	fs["m_Flag_Contrast"] >> m_Flag_Contrast;
	Btn_Contrast.SetCheck(m_Flag_Contrast);
	fs["m_Flag_Brightness"] >> m_Flag_Brightness;
	Btn_Brightness.SetCheck(m_Flag_Brightness);
	fs["m_Flag_Noise"] >> m_Flag_Noise;
	Btn_Noise.SetCheck(m_Flag_Noise);
	fs["m_Flag_Sharpen"] >> m_Flag_Sharpen;
	Btn_Sharpen.SetCheck(m_Flag_Sharpen);
	fs["m_Rotate_Step"] >> m_Rstep;
	fs["m_Crop_Scale"] >> m_crop_scale;
	fs["m_Contrast_Max"] >> m_CMax;
	fs["m_Contrast_Step"] >> m_CStep;
	fs["m_Brightness_Max"] >> m_BMax;
	fs["m_Brightness_Step"] >> m_BStep;
	fs["m_Other_Path_Flag"] >> m_other_path_flag;
	fs["m_Sharpen_Thre"] >> m_Sharpen_Thre;
	fs["m_Sharpen_Amount"] >> m_Sharpen_Amount;
	fs["m_Val_Scale"] >> m_Val_Scale;
	fs["m_Other_Path"] >> m_other_path;
	m_temp_path = (CString)(m_other_path.c_str());
	fs.release();
	UpdateData(FALSE);
	return 0;
}

void CEParam::OnBnClickedApply()
{
	// TODO:  在此添加控件通知处理程序代码
	SaveXML(m_xml_file);
}


void CEParam::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	OnBnClickedApply();
	CDialogEx::OnOK();
}


void CEParam::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CEParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LoadXML(m_xml_file);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEParam::OnBnClickedGetPath()
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
	m_temp_path = strFolderPath;
	m_other_path = CW2A(m_temp_path);
	UpdateData(FALSE);
}
