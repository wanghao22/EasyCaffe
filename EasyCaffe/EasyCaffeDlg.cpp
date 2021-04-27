
// EasyCaffeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyCaffe.h"
#include "EasyCaffeDlg.h"
#include "afxdialogex.h"

using namespace cv;
using namespace DirFile;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEasyCaffeDlg �Ի���
//����ͼ���߳�
UINT LoadImage_Thread(PVOID pParam)
{
	CEasyCaffeDlg* pWnd = (CEasyCaffeDlg*)pParam;
	pWnd->DuringThreadBtn(false);
	pWnd->m_full_path.clear();
	pWnd->m_path.clear();
	pWnd->m_name.clear();
	pWnd->m_type.clear();
	pWnd->m_img_labels.clear();
	std::vector<std::string> temp;
	pWnd->findFile(pWnd->m_data_path, temp);
	Mat src;
	std::string paths, names, types;

	CProgressCtrl myProCtrl;
	CRect rect, proRect;
	GetClientRect(pWnd->m_hWnd, &rect);
	proRect.left = rect.left + rect.Width() / 2 - 100;
	proRect.top = rect.top + rect.Height() / 2 - 20;
	proRect.right = rect.right - rect.Width() / 2 + 100;
	proRect.bottom = rect.bottom - rect.Height() / 2 + 20;
	//WS_CHILD|WS_VISIBLE|PBS_SMOOTHREVERSE
	myProCtrl.Create(WS_VISIBLE, proRect, pWnd, 99); //����λ�á���С
	myProCtrl.SetRange(0, 100);

	for (int i = 0; i < (int)temp.size(); i++)
	{
		src = imread(temp[i], -1);
		if (src.empty())
			continue;
		if (pWnd->PathDevision(temp[i], paths, names, types) != 0)
			continue;
		if (pWnd->GetSize(src) == 1)
		{
			resize(src, src, cv::Size(pWnd->m_size_w, pWnd->m_size_h));
			imwrite(temp[i], src);
		}
		int rc = pWnd->GetChannels(src);
		if (rc != -1)
			cv::cvtColor(src, src, rc);
		pWnd->m_full_path.push_back(temp[i]);
		pWnd->m_path.push_back(paths);
		pWnd->m_name.push_back(names);
		pWnd->m_type.push_back(types);
		std::string filename = names + types;
		int intm = pWnd->m_imgnames_list.InsertItem(i, CString(filename.c_str()), i);

		std::string str = GetFatherDirName(paths);
		pWnd->m_img_labels.push_back(str);
		bool no_flag = false;
		if (str != "data"){
			pWnd->m_imgnames_list.SetItemText(intm, 1, (CString)(str.c_str()));
			for (auto i : pWnd->m_labels){
				if (i == str){
					no_flag = true;
				}
			}
			if (!no_flag)
				pWnd->m_labels.push_back(str);
		}
		int sz = (int)temp.size();
		int pos = (i + 1) * 100 / sz;
		myProCtrl.SetPos(pos);
	}
	pWnd->ReflashBox();
	pWnd->DuringThreadBtn(true);
	return 0;
}

UINT EnhanceImage_Thread(PVOID pParam)
{
	CEasyCaffeDlg* pWnd = (CEasyCaffeDlg*)pParam;
	pWnd->DuringThreadBtn(false);
	pWnd->train_tools.m_enhance_fpath.clear();
	CProgressCtrl myProCtrl;
	CRect rect, proRect;
	GetClientRect(pWnd->m_hWnd, &rect);
	proRect.left = rect.left + rect.Width() / 2 - 100;
	proRect.top = rect.top + rect.Height() / 2 - 20;
	proRect.right = rect.right - rect.Width() / 2 + 100;
	proRect.bottom = rect.bottom - rect.Height() / 2 + 20;
	//WS_CHILD|WS_VISIBLE|PBS_SMOOTHREVERSE
	myProCtrl.Create(WS_VISIBLE, proRect, pWnd, 99); //����λ�á���С
	myProCtrl.SetRange(0, 100);

	int maxz = pWnd->train_tools.m_fullpath.size();
	for (int i = 0; i < maxz; i++)
	{
		pWnd->train_tools.Enhance(i);
		int pos = (i + 1) * 100 / maxz;
		myProCtrl.SetPos(pos);
	}
	if (pWnd->train_tools.m_Other_Path_Flag)
	{
		pWnd->train_tools.ClassifyTrainVal();
	}
	pWnd->train_tools.CmdTrainStart(pWnd->m_project_path);
	pWnd->DuringThreadBtn(true);
	return 0;
}

CEasyCaffeDlg::CEasyCaffeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEasyCaffeDlg::IDD, pParent)
	, m_project_open_flag(false)
	, m_run_flag(true)
	, m_labels_cnt(0)
	, m_size_w(0)
	, m_size_h(0)
	, m_channel(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEasyCaffeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABELS_BOX, m_labels_box);
	DDX_Control(pDX, IDC_LABLE_LIST, m_imgnames_list);
}

BEGIN_MESSAGE_MAP(CEasyCaffeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CEasyCaffeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEasyCaffeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NEW_WORK, &CEasyCaffeDlg::OnBnClickedNewWork)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CLOSE_WORK, &CEasyCaffeDlg::OnBnClickedCloseWork)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OPEN_WORK, &CEasyCaffeDlg::OnBnClickedOpenWork)
	ON_LBN_SELCHANGE(IDC_LABELS_BOX, &CEasyCaffeDlg::OnLbnSelchangeLabelsBox)
	ON_BN_CLICKED(IDC_ADD_LABEL, &CEasyCaffeDlg::OnBnClickedAddLabel)
	ON_BN_CLICKED(IDC_CHG_LABEL, &CEasyCaffeDlg::OnBnClickedChgLabel)
	ON_BN_CLICKED(IDC_DEL_LABEL, &CEasyCaffeDlg::OnBnClickedDelLabel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LABLE_LIST, &CEasyCaffeDlg::OnLvnItemchangedLableList)
	ON_BN_CLICKED(IDC_ADD_IMAGE, &CEasyCaffeDlg::OnBnClickedAddImage)
	ON_BN_CLICKED(IDC_DEL_IMAGE, &CEasyCaffeDlg::OnBnClickedDelImage)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_ENHANCE_AND_TRAIN, &CEasyCaffeDlg::OnBnClickedEnhanceAndTrain)
	ON_BN_CLICKED(IDC_SAVE_XML, &CEasyCaffeDlg::OnBnClickedSaveXml)
	ON_BN_CLICKED(IDC_MAKE_LABEL_FILE, &CEasyCaffeDlg::OnBnClickedMakeLabelFile)
END_MESSAGE_MAP()


// CEasyCaffeDlg ��Ϣ�������

BOOL CEasyCaffeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	__init__();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEasyCaffeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEasyCaffeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEasyCaffeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEasyCaffeDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void CEasyCaffeDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


//�½����̡�����ť
void CEasyCaffeDlg::OnBnClickedNewWork()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ���������� 
	OnBnClickedCloseWork();
	CNewProject newprownd;
	auto rc = newprownd.DoModal();
	if (rc == IDOK){
		//TODO: AfxMessageBox(L"IDOK"); 
		m_project_path = newprownd.m_full_path;
		//AfxMessageBox((CString)(m_project_path.c_str()));
		DoNewProject();
	}
}

//�½����̡���ʵ��
void CEasyCaffeDlg::DoNewProject()
{
	if (IsExisteDir(m_project_path)){
		AfxMessageBox(L"��Ŀ�Ѿ����ڣ������裡");
		return;
	}
	CreateDir(m_project_path);
	m_data_path = DirAddSubdir(m_project_path, "data");
	m_caffe_model_path = DirAddSubdir(m_project_path, "caffemodel");
	m_lmdb_path = DirAddSubdir(m_project_path, "lmdb");
	m_img_enhance_xml = DirAddSubdir(m_caffe_model_path, "Enhance.xml");

	CreateDir(m_data_path);
	CreateDir(m_caffe_model_path);
	CreateDir(m_lmdb_path);

	CopyCaffeModelFile("Enhance.xml");
	CopyCaffeModelFile("train_val.prototxt");
	//CopyCaffeModelFile("deploy.prototxt");
	WriteSolverProto();

	std::string other_path = DirAddSubdir(m_project_path, "temp");
	train_tools.LoadXML(m_img_enhance_xml);
	train_tools.m_Other_Path = other_path;
	train_tools.SaveXML(m_img_enhance_xml);
	CreateDir(other_path);


	WriteXML();
	m_project_open_flag = true;
	DoProjectChange(m_project_open_flag);
}

//����caffemodel�ļ����µ��ļ�
void CEasyCaffeDlg::CopyCaffeModelFile(std::string filename)
{
	std::string tmp1 = DirAddSubdir(m_caffe_model_path_ori, filename);
	std::string tmp2 = DirAddSubdir(m_caffe_model_path, filename);
	std::wstring wtmp1 = string2wstring(tmp1);
	std::wstring wtmp2 = string2wstring(tmp2);
	CopyFile(wtmp1.c_str(), wtmp2.c_str(), FALSE);
}

//�滻'\\'Ϊ'/'
void CEasyCaffeDlg::DoWithStr(std::string &str)
{
	for (auto &i : str)
		if (i == '\\')
			i = '/';
}

//��дѵ����������ļ�
void CEasyCaffeDlg::WriteSolverProto()
{
	std::string tmp1 = DirAddSubdir(m_caffe_model_path, "solver.prototxt");
	std::ofstream ofs;
	ofs.open(tmp1);
	std::string str1 = DirAddSubdir(m_caffe_model_path, "train_val.prototxt");
	std::string str2 = DirAddSubdir(m_caffe_model_path, "caffenet_train");
	DoWithStr(str1);
	DoWithStr(str2);
	if (ofs.is_open()){
		ofs << "net: \"" << str1 << "\"" << std::endl;
		ofs << "test_iter: 100" << std::endl;
		ofs << "test_interval: 500" << std::endl;
		ofs << "base_lr: 0.001" << std::endl;
		ofs << "lr_policy: \"step\"" << std::endl;
		ofs << "gamma: 0.2" << std::endl;
		ofs << "stepsize: 5000" << std::endl;
		ofs << "display: 1000" << std::endl;
		ofs << "max_iter: 10000" << std::endl;
		ofs << "momentum: 0.9" << std::endl;
		ofs << "weight_decay: 0.0005" << std::endl;
		ofs << "snapshot: 1000" << std::endl;
		ofs << "snapshot_prefix: \"" << str2 << "\"" << std::endl;
		ofs << "solver_mode: GPU" << std::endl;
	}
	ofs.close();
}

//��дѵ���ļ�
void CEasyCaffeDlg::WriteTrainProto(int cnt)
{
	std::string tmp1 = DirAddSubdir(m_caffe_model_path_ori, "train_val.prototxt");
	std::string tmp2 = DirAddSubdir(m_caffe_model_path, "train_val.prototxt");
	std::ifstream ifs;
	std::ofstream ofs;
	ifs.open(tmp1);
	ofs.open(tmp2);
	int rows = 1;
	if (ifs.is_open() && ofs.is_open()){
		while (true)
		{
			if (ifs.eof())
				break;
			const int LINE_LENGTH = 100;
			char data[LINE_LENGTH];
			ifs.getline(data, LINE_LENGTH);
			if (rows == 13 || rows == 45)
			{
				std::string str = DirAddSubdir(m_caffe_model_path, "mean.binaryproto");
				DoWithStr(str);
				ofs << "    mean_file: \"" << str << "\"" << std::endl;
			}
			else if (rows == 29)
			{
				std::string str = DirAddSubdir(m_lmdb_path, "train");
				DoWithStr(str);
				ofs << "    source: \"" << str << "\"" << std::endl;
			}
			else if (rows == 56)
			{
				std::string str = DirAddSubdir(m_lmdb_path, "val");
				DoWithStr(str);
				ofs << "    source: \"" << str << "\"" << std::endl;
			}
			else if (rows == 378)
				ofs << "    num_output: " << cnt << "  #�������" << std::endl;
			else
				ofs << data << std::endl;
			rows++;
		}
	}
	ifs.close();
	ofs.close();
}

//��д�����ļ�
void CEasyCaffeDlg::WriteDeployProto(int cnt)
{
	std::string tmp1 = DirAddSubdir(m_caffe_model_path_ori, "deploy.prototxt");
	std::string tmp2 = DirAddSubdir(m_project_path, "deploy.prototxt");
	std::ifstream ifs;
	std::ofstream ofs;
	ifs.open(tmp1);
	ofs.open(tmp2);
	int rows = 1;
	if (ifs.is_open() && ofs.is_open()){
		while (true)
		{
			if (ifs.eof())
				break;
			const int LINE_LENGTH = 100;
			char data[LINE_LENGTH];
			ifs.getline(data, LINE_LENGTH);
			/*if (rows == 4)
				ofs << "input_dim:" << m_channel << std::endl;
			else */
			if (rows == 5)
				ofs << "input_dim:" << m_size_w << std::endl;
			else if (rows == 6)
				ofs << "input_dim:" << m_size_h << std::endl;
			else if (rows == 204)
				ofs << "    num_output: " << cnt << "  #�������" << std::endl;
			else
				ofs << data << std::endl;
			rows++;
		}
	}
	ifs.close();
	ofs.close();
}

//UI��ť�仯����ʵ��
void CEasyCaffeDlg::DoProjectChange(bool flag)
{
	GetDlgItem(IDC_CLOSE_WORK)->EnableWindow(flag);
	GetDlgItem(IDC_SAVE_XML)->EnableWindow(flag);
	GetDlgItem(IDC_ENHANCE_AND_TRAIN)->EnableWindow(flag);
	GetDlgItem(IDC_MAKE_LABEL_FILE)->EnableWindow(flag);
	GetDlgItem(IDC_ADD_IMAGE)->EnableWindow(flag);
	GetDlgItem(IDC_DEL_IMAGE)->EnableWindow(flag);
	GetDlgItem(IDC_ADD_LABEL)->EnableWindow(flag);
	GetDlgItem(IDC_DEL_LABEL)->EnableWindow(flag);
	GetDlgItem(IDC_CHG_LABEL)->EnableWindow(flag);
}

void CEasyCaffeDlg::DuringThreadBtn(bool flag)
{
	GetDlgItem(IDC_CLOSE_WORK)->EnableWindow(flag);
	GetDlgItem(IDC_NEW_WORK)->EnableWindow(flag);
	GetDlgItem(IDC_OPEN_WORK)->EnableWindow(flag);
	GetDlgItem(IDC_ENHANCE_AND_TRAIN)->EnableWindow(flag);
	GetDlgItem(IDC_MAKE_LABEL_FILE)->EnableWindow(flag);
	GetDlgItem(IDC_SAVE_XML)->EnableWindow(flag);
	GetDlgItem(IDC_ADD_IMAGE)->EnableWindow(flag);
	GetDlgItem(IDC_DEL_IMAGE)->EnableWindow(flag);
	GetDlgItem(IDC_ADD_LABEL)->EnableWindow(flag);
	GetDlgItem(IDC_DEL_LABEL)->EnableWindow(flag);
	GetDlgItem(IDC_CHG_LABEL)->EnableWindow(flag);
}

void CEasyCaffeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	for (int i = 1000; i <1050; i++)//��Ϊ�Ƕ���ؼ���������������ѭ��
	{
		CWnd *pWnd = GetDlgItem(i);
		if (pWnd && nType != 1 && m_wndRect.Width() && m_wndRect.Height())  //�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
		{
			CRect rect;   //��ȡ�ؼ��仯ǰ�Ĵ�С 
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
			rect.left = rect.left*cx / m_wndRect.Width();//�����ؼ���С
			rect.right = rect.right*cx / m_wndRect.Width();
			rect.top = rect.top*cy / m_wndRect.Height();
			rect.bottom = rect.bottom*cy / m_wndRect.Height();
			pWnd->MoveWindow(rect);//���ÿؼ���С 
		}
	}
	GetClientRect(&m_wndRect);
	//�ı�ߴ�ʱ����listCtrl���п�
	if (m_imgnames_list.m_hWnd)
	{
		CRect listRect;
		m_imgnames_list.GetClientRect(&listRect);
		/*m_imgnames_list.SetExtendedStyle(m_imgnames_list.GetExtendedStyle() | LVS_EX_GRIDLINES
			| LVS_EX_FULLROWSELECT);*/
		m_imgnames_list.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_imgnames_list.SetColumnWidth(0, 2 * listRect.Width() / 3);
		m_imgnames_list.SetColumnWidth(1, listRect.Width() / 3);
	}
	if (m_project_open_flag){
		if (!m_src.empty()){
			ShowMatImg(m_src, IDC_LABLEIMG_SHOW, m_win1);
		}
	}
	else
	{
		ShowMatImg(m_white, IDC_LABLEIMG_SHOW, m_win1);
	}
}

void CEasyCaffeDlg::DeleteDirectory(CString sDirName)
{
	static int s_cnt = 0;
	s_cnt++;
	CFileFind   ff;
	BOOL   bFound;
	CString sTempFileFind;
	sTempFileFind.Format(_T("%s\\*.*"), sDirName);

	bFound = ff.FindFile(sTempFileFind);

	while (bFound)
	{
		bFound = ff.FindNextFile();
		CString  sFilePath = ff.GetFilePath();

		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				DeleteDirectory(sFilePath);
			}
		}
		else
		{
			if (ff.IsReadOnly())
			{
				SetFileAttributes(sFilePath, FILE_ATTRIBUTE_NORMAL);
			}
			DeleteFile(sFilePath);
		}
	}
	ff.Close();
	SetFileAttributes(sDirName, FILE_ATTRIBUTE_NORMAL);
	s_cnt--;
	if (s_cnt == 0)
		return;
	RemoveDirectory(sDirName);
}

void CEasyCaffeDlg::__init__(){
	srand((int)time(NULL));
	ShowWindow(SW_MAXIMIZE);
	GetClientRect(&m_wndRect);

	DoProjectChange(m_project_open_flag);
	GetThisDir();

	SetImgWindow(IDC_LABLEIMG_SHOW, m_win1);

	CRect Listrect;
	m_imgnames_list.GetClientRect(&Listrect);
	m_imgnames_list.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_imgnames_list.InsertColumn(0, L"�ļ���", LVCFMT_CENTER, Listrect.Width() * 2 / 3);
	m_imgnames_list.InsertColumn(1, L"����", LVCFMT_CENTER, Listrect.Width() / 3);

	//AfxMessageBox((CString)(this_dir.c_str()));
	m_white = Mat(512, 512, CV_8UC3, cvScalarAll(0xff));
	ShowMatImg(m_white, IDC_LABLEIMG_SHOW, m_win1);
	//OpenProject("D:/1122/Classify/test/project.xml");
	//DeleteDirectory(L"D:/1122/Classify/test/temp");
}

void CEasyCaffeDlg::__exit__()
{
	if (m_run_flag)
	{
		OnBnClickedCloseWork();
		m_run_flag = false;
	}
}

//�رչ��̡�����ť&&ʵ��
void CEasyCaffeDlg::OnBnClickedCloseWork()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_project_open_flag)
	{
		WriteXML();
		m_project_path = "";
		m_data_path = "";
		m_caffe_model_path = "";
		m_lmdb_path = "";
		m_img_enhance_xml = "";
		m_size_w = 0;
		m_size_h = 0;
		m_imgnames_list.DeleteAllItems();
		m_labels.clear();
		m_src.release();
		ReflashBox();
		ShowMatImg(m_white, IDC_LABLEIMG_SHOW, m_win1);

		m_project_open_flag = false;
		DoProjectChange(m_project_open_flag);
	}
}


void CEasyCaffeDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	__exit__();
	CDialogEx::OnCancel();
}

//��ȡEXE�İ�װĿ¼
void CEasyCaffeDlg::GetThisDir()
{
	TCHAR dd[256];
	GetModuleFileName(NULL, dd, 256);
	if (dd == NULL)
	{
		this_dir = "D:\\EasyCaffe";
		return;
	}
	this_dir = CW2A(dd);
	this_dir = GetFatherDir(this_dir);
	this_dir = GetFatherDir(this_dir);
	this_dir = GetFatherDir(this_dir);
	m_caffe_tools_path = DirAddSubdir(this_dir, "caffe_tools");
	m_caffe_model_path_ori = DirAddSubdir(this_dir, "caffemodel");
}

//�򿪹��̡�����ť
void CEasyCaffeDlg::OnBnClickedOpenWork()
{
	OnBnClickedCloseWork();
	CString picPath;   //����ͼƬ·������  
	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//	NULL, this);   //ѡ���ļ��Ի���  
	CFileDialog  dlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"XML Files(*.xml)|*.xml|All Files(*.*)|*.*||");
	if (dlg.DoModal() == IDOK)
	{
		picPath = dlg.GetPathName();  //��ȡͼƬ·��  
	}
	else
	{
		return;
	}
	std::string file = CW2A(picPath);
	OpenProject(file);
}

//�򿪹��̡���ʵ��
void CEasyCaffeDlg::OpenProject(std::string file){
	if (ReadXML(file) == 0){


		m_project_open_flag = true;
		DoProjectChange(m_project_open_flag);
		GetImgLists();
		ReflashBox();
	}
}

//���湤���ļ�
int CEasyCaffeDlg::WriteXML(){
	std::string xmlf = DirAddSubdir(m_project_path, "project.xml");
	FileStorage fs(xmlf, FileStorage::WRITE);
	if (!fs.isOpened())
	{
		return -2;
	}
	fs << "Project_Path" << m_project_path;
	fs << "Data_Path" << m_data_path;
	fs << "Caffe_Model_Path" << m_caffe_model_path;
	fs << "LMDB_Path" << m_lmdb_path;
	fs << "Image_Enhance_XML" << m_img_enhance_xml;
	fs << "Mat_Channel" << m_channel;
	fs << "SIZE_Width" << m_size_w;
	fs << "SIZE_Height" << m_size_h;
	m_labels.clear();
	for (int i = 0; i < m_labels_box.GetCount(); i++){
		CString cstr;
		m_labels_box.GetText(i, cstr);
		std::string str = CW2A(cstr);
		m_labels.push_back(str);
	}
	fs << "LABELS" << m_labels;
	fs.release();
	return 0;
}

//���ع����ļ�
int CEasyCaffeDlg::ReadXML(std::string file)
{
	FileStorage fs(file, FileStorage::READ);
	if (!fs.isOpened())
	{
		return -2;
	}
	fs["Project_Path"] >> m_project_path;
	fs["Data_Path"] >> m_data_path;
	fs["Caffe_Model_Path"] >> m_caffe_model_path;
	fs["LMDB_Path"] >> m_lmdb_path;
	fs["Image_Enhance_XML"] >> m_img_enhance_xml;
	fs["Mat_Channel"] >> m_channel;
	fs["SIZE_Width"] >> m_size_w;
	fs["SIZE_Height"] >> m_size_h;
	m_labels.clear();
	auto ffs = fs["LABELS"];
	for (auto i : ffs)
		m_labels.push_back(i);
	if (m_project_path.length() < 1 || m_data_path.length() < 1 || 
		m_caffe_model_path.length() < 1 || m_lmdb_path.length() < 1 || 
		m_img_enhance_xml.length() < 1)
		return -1;
	fs.release();
	return 0;
}

//ѡ���ǩ
void CEasyCaffeDlg::OnLbnSelchangeLabelsBox()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int row = m_labels_box.GetCurSel();
	if (row < 0)
		return;
	CString cstr; 
	m_labels_box.GetText(row, cstr);
	std::string new_label = CW2A(cstr);
	std::string new_path = DirAddSubdir(m_data_path, new_label);

	POSITION pos = m_imgnames_list.GetFirstSelectedItemPosition();
	while (true)
	{
		if (pos == NULL)
			break;
		int nld = (int)m_imgnames_list.GetNextSelectedItem(pos);
		CString staticc = m_imgnames_list.GetItemText(nld, 1);
		if (staticc != cstr)
		{
			std::string new_file = DirAddSubdir(new_path, m_name[nld] + m_type[nld]);
			if (MoveFileA(m_full_path[nld].c_str(), new_file.c_str())){
				m_full_path[nld] = new_file;
				m_path[nld] = new_path + "\\";
				m_img_labels[nld] = new_label;
				m_imgnames_list.SetItemText(nld, 1, cstr);
			}
		}
	}
}

//��ӱ�ǩ������ť
void CEasyCaffeDlg::OnBnClickedAddLabel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CSetLabel wnd;
	auto rc = wnd.DoModal();
	if (rc == IDOK){
		std::string str = CW2A(wnd.m_label);
		for (auto i : m_labels){
			if (i == str){
				AfxMessageBox(L"��ǩ�Ѵ��ڣ�");
				return;
			}
		}
		m_labels.push_back(str);
		CreateDir(DirAddSubdir(m_data_path, str));
		ReflashBox();
	}
}

//�޸ı�ǩ������ť
void CEasyCaffeDlg::OnBnClickedChgLabel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int row = m_labels_box.GetCurSel();
	if (row < 0){
		AfxMessageBox(L"����ѡ��һ����ǩ");
		return;
	}
	CString ccc;
	m_labels_box.GetText(row, ccc);
	std::string oldstr = CW2A(ccc);
	CSetLabel wnd;
	auto rc = wnd.DoModal();
	if (rc == IDOK){
		std::string str = CW2A(wnd.m_label);
		for (auto i : m_labels){
			if (i == str){
				AfxMessageBox(L"��ǩ�Ѵ��ڣ�");
				return;
			}
		}
		for (auto &i : m_labels){
			if (i == oldstr)
				i = str;
		}
		std::string old_path = DirAddSubdir(m_data_path, oldstr);
		std::string new_path = DirAddSubdir(m_data_path, str);
		/*RenameDirFile(old_path, new_path);*/
		MoveFileA(old_path.c_str(), new_path.c_str());
		ReflashBox();

		//ˢ�½���
		WriteXML();
		std::string xmlfile = DirAddSubdir(m_project_path, "project.xml");
		OpenProject(xmlfile);
	}
}

//ˢ�±�ǩ���ӡ���ʵ��
void CEasyCaffeDlg::ReflashBox()
{
	while (m_labels_box.GetCount()>0)
		m_labels_box.DeleteString(0);
	for (auto i : m_labels){
		m_labels_box.AddString((CString)(i.c_str()));
	}
}

//ɾ����ǩ������ť
void CEasyCaffeDlg::OnBnClickedDelLabel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int row = m_labels_box.GetCurSel();
	if (row < 0){
		AfxMessageBox(L"����ѡ��һ����ǩ");
		return;
	}
	CString ccc;
	m_labels_box.GetText(row, ccc);
	std::string oldstr = CW2A(ccc);
	DeleteLabel(oldstr);
}

//ɾ����ǩ����ʵ��
void CEasyCaffeDlg::DeleteLabel(std::string label)
{
	int sz = (int)m_labels.size();
	int idx = -1;
	for (int i = sz - 1; i > -1; i--)
		if (m_labels[i] == label)
			idx = i;
	if (idx >= 0)
		m_labels.erase(m_labels.begin() + idx);
	ReflashBox();

	for (int i = 0; i < (int)m_full_path.size(); i++){
		if (label == m_img_labels[i]){
			std::string new_file = DirAddSubdir(m_data_path, m_name[i] + m_type[i]);
			MoveFileA(m_full_path[i].c_str(), new_file.c_str());
		}
	}
	DeleteDir(m_data_path, label);
	//ˢ�½���
	WriteXML();
	std::string xmlfile = DirAddSubdir(m_project_path, "project.xml");
	OpenProject(xmlfile);
}

//��ʼ���ã�ȷ�����ڿؼ��ʹ�������
void CEasyCaffeDlg::SetImgWindow(int Dlgitem, const char* str)
{
	cv::namedWindow(str, cv::WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle(str);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(Dlgitem)->m_hWnd);  //picture�ؼ�
	::ShowWindow(hParent, SW_HIDE);
}

//��ʾͼ��
void CEasyCaffeDlg::ShowMatImg(cv::Mat src, int Dlgitem, const char* str)
{
	if (src.empty())
	{
		return;
	}
	cv::Mat bgr;
	if (src.channels() == 3)
		bgr = src.clone();
	else if (src.channels() == 1)
		cvtColor(src, bgr, CV_GRAY2BGR);
	else
		return;
	CRect m_rect;
	cv::Mat temp;
	GetDlgItem(Dlgitem)->GetClientRect(&m_rect);
	CvSize window_size = cvSize(m_rect.Width(), m_rect.Height());
	double width = window_size.width;
	double scale = (double)src.rows / (double)src.cols;
	double height = width*scale;
	if (width > window_size.width || height > window_size.height)
	{
		height = window_size.height;
		scale = (double)src.cols / (double)src.rows;
		width = height*scale;
		if (width > window_size.width || height > window_size.height)
		{
			return;
		}
	}
	cv::resize(bgr, temp, cvSize((int)width, (int)height));
	cv::Mat dst = cv::Mat::Mat(window_size, CV_8UC3, cvScalarAll(0xff));
	int x = (dst.cols - temp.cols) / 2;
	int y = (dst.rows - temp.rows) / 2;
	cv::Mat roi = dst(cvRect(x, y, temp.cols, temp.rows));
	temp.copyTo(roi);
	cv::imshow(str, dst);
}

//���������ļ����������Ŀ¼
void CEasyCaffeDlg::findFile(std::string path, std::vector<std::string>& res)
{
	std::string p;
	intptr_t hFile = 0;                //�ļ����
	struct _finddata_t fileinfo;    //�ļ���Ϣ
	//�����ļ����еĵ�һ���ļ���Ȼ�����þ�����б���
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != 0)
	{
		do
		{
			std::string path_s = p.assign(path).append("\\").append(fileinfo.name);
			if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
				continue;
			if (fileinfo.attrib == _A_SUBDIR)
			{
				//������Ŀ¼
				std::vector<std::string> temp;
				findFile(path_s, temp);
				for (size_t i = 0; i < temp.size(); i++)
					res.push_back(temp[i]);
				continue;
			}
			res.push_back(path_s);            //��������
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);    //�ر��ļ�
	}
}

//�ļ�ȫ���ָ�
int CEasyCaffeDlg::PathDevision(std::string str, std::string & path, std::string & name, std::string & type)
{
	size_t i = str.find_last_of("/");
	size_t j = str.find_last_of("\\");
	size_t k = str.find_last_of(".");
	size_t d = 0;
	size_t l = str.length();
	if (i >= l&&j >= l)
	{
		return -1;
	}
	else if (i >= l)
		d = j;
	else if (j > l)
		d = i;
	else
		d = MAX(i, j);
	path = str.substr(0, d + 1);
	name = str.substr(d + 1, k - d - 1);
	type = str.substr(k, l - k);
	return 0;
}

//��ʼ��������ͼ��
void CEasyCaffeDlg::GetImgLists()
{
	m_imgnames_list.DeleteAllItems();
	AfxBeginThread(LoadImage_Thread, this);
}

//UI�����б���ʾͼ��
void CEasyCaffeDlg::OnLvnItemchangedLableList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_imgnames_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	int nld = (int)m_imgnames_list.GetNextSelectedItem(pos);
	m_src = imread(m_full_path[nld], -1);
	ShowMatImg(m_src, IDC_LABLEIMG_SHOW, m_win1);
	*pResult = 0;
}


void CEasyCaffeDlg::OnBnClickedAddImage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog  dlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, 
		L"jpg Files(*.jpg)|*.jpg|png Files(*.png)|*.png|bmp Files(*.bmp)|*.bmp|All Files(*.*)|*.*||");
	dlg.m_ofn.lStructSize = 88;
	dlg.m_ofn.lpstrFile = new TCHAR[2048];
	memset(dlg.m_ofn.lpstrFile, 0, 2048);
	dlg.m_ofn.nMaxFile = 2048;
	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();
		while (pos)
		{
			CString szFileName = dlg.GetNextPathName(pos);
			std::string file = CW2A(szFileName);
			cv::Mat src = imread(file, -1);
			if (src.empty())
				continue;
			std::string spath, sname, stype;
			if (PathDevision(file, spath, sname, stype) != 0)
				continue;
			std::string flpath = DirAddSubdir(m_data_path, sname + stype);
			if (GetSize(src) == 1)
				resize(src, src, cv::Size(m_size_w, m_size_h));
			int rc = GetChannels(src);
			if (rc != -1)
				cv::cvtColor(src, src, rc);
			imwrite(flpath, src);
		}

		//ˢ�½���
		WriteXML();
		std::string xmlfile = DirAddSubdir(m_project_path, "project.xml");
		OpenProject(xmlfile);
	}
}


void CEasyCaffeDlg::OnBnClickedDelImage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_imgnames_list.GetFirstSelectedItemPosition();
	while (true)
	{
		if (pos == NULL)
			break;
		int nld = (int)m_imgnames_list.GetNextSelectedItem(pos);
		_DeleteFile(m_full_path[nld]);
	}

	//ˢ�½���
	WriteXML();
	std::string xmlfile = DirAddSubdir(m_project_path, "project.xml");
	OpenProject(xmlfile);
}

int CEasyCaffeDlg::GetSize(cv::Mat src)
{
	if (m_size_w == 0 || m_size_h == 0){
		if (src.cols > 512 || src.rows > 512){
			m_size_w = 512;
			m_size_h = 512;
			return 1;
		}
		else
		{
			m_size_w = src.cols;
			m_size_h = src.rows;
			return 0;
		}
	}
	else{
		if (src.cols != m_size_w || src.rows > m_size_h){
			return 1;
		}
	}
	return 0;
}


int CEasyCaffeDlg::GetChannels(cv::Mat src)
{
	int channel = src.channels();
	if (m_channel == 0){
		m_channel = channel;
		return -1;
	}
	else{
		if (m_channel == channel)
			return -1;
		else{
			if (channel == 4 && m_channel == 1)
				return cv::COLOR_BGRA2GRAY;
			else if (channel == 4 && m_channel == 3)
				return cv::COLOR_BGRA2BGR;
			else if (channel == 3 && m_channel == 1)
				return cv::COLOR_BGR2GRAY;
			else if (channel == 3 && m_channel == 4)
				return cv::COLOR_BGR2BGRA;
			else if (channel == 1 && m_channel == 3)
				return cv::COLOR_GRAY2BGR;
			else if (channel == 1 && m_channel == 4)
				return cv::COLOR_GRAY2BGRA;
		}
	}
	return -1;
}

void CEasyCaffeDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_project_open_flag)
	{
		UINT i;
		UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
		for (i = 0; i < nFiles; i++)
		{
			TCHAR szFileName[_MAX_PATH];
			::DragQueryFile(hDropInfo, i, szFileName, _MAX_PATH);
			//char str[_MAX_PATH]; sprintf_s(str, "%hs", szFileName);
			std::string file = CW2A(szFileName);
			cv::Mat src = imread(file, -1);
			if (src.empty())
				continue;
			std::string spath, sname, stype;
			if (PathDevision(file, spath, sname, stype) != 0)
				continue;
			std::string flpath = DirAddSubdir(m_data_path, sname + stype);
			if (GetSize(src) == 1)
				resize(src, src, cv::Size(m_size_w, m_size_h));
			int rc = GetChannels(src);
			if (rc != -1)
				cv::cvtColor(src, src, rc);
			imwrite(flpath, src);
		}

		//ˢ�½���
		WriteXML();
		std::string xmlfile = DirAddSubdir(m_project_path, "project.xml");
		OpenProject(xmlfile);
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

//��ǿѵ����ť
void CEasyCaffeDlg::OnBnClickedEnhanceAndTrain()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CEParam wnd;
	wnd.m_xml_file = m_img_enhance_xml;
	auto rc = wnd.DoModal();
	if (rc == IDOK){
		train_tools.m_Flag_Rotate = wnd.m_Flag_Rotate;
		train_tools.m_Flag_Rotate90 = wnd.m_Flag_Rotate90;
		train_tools.m_Flag_FlipXY = wnd.m_Flag_FlipXY;
		train_tools.m_Flag_FlipX = wnd.m_Flag_FlipX;
		train_tools.m_Flag_FlipY = wnd.m_Flag_FlipY;
		train_tools.m_Flag_Crop = wnd.m_Flag_Crop;
		train_tools.m_Flag_Blur = wnd.m_Flag_Blur;
		train_tools.m_Flag_Noise = wnd.m_Flag_Noise;
		train_tools.m_Flag_Contrast = wnd.m_Flag_Contrast;
		train_tools.m_Flag_Brightness = wnd.m_Flag_Brightness;
		train_tools.m_Other_Path_Flag = wnd.m_other_path_flag;
		train_tools.m_Flag_Sharpen = wnd.m_Flag_Sharpen;
		train_tools.m_Rotate_Step = wnd.m_Rstep;
		train_tools.m_Crop_Scale = wnd.m_crop_scale;
		train_tools.m_Contrast_Step = wnd.m_CStep;
		train_tools.m_Brightness_Step = wnd.m_BStep;
		train_tools.m_Contrast_Max = wnd.m_CMax;
		train_tools.m_Brightness_Max = wnd.m_BMax;
		train_tools.m_Sharpen_Thre = wnd.m_Sharpen_Thre;
		train_tools.m_Sharpen_Amount = wnd.m_Sharpen_Amount;
		train_tools.m_Val_Scale = wnd.m_Val_Scale;
		train_tools.m_Other_Path = wnd.m_other_path;

		DeleteDirectory(wnd.m_temp_path);

		train_tools.m_fullpath = m_full_path;
		train_tools.SetCaffeToolsDir(m_caffe_tools_path);
		train_tools.path = m_path;
		train_tools.name = m_name;
		train_tools.type = m_type;
		train_tools.labels = m_img_labels;

		std::string train_path = DirAddSubdir(wnd.m_other_path, "train");
		std::string val_path = DirAddSubdir(wnd.m_other_path, "val");
		CreateDir(train_path);
		CreateDir(val_path);
		for (int i = 0; i < m_labels_box.GetCount(); i++){
			CString cstr;
			m_labels_box.GetText(i, cstr);
			std::string str = CW2A(cstr);
			train_tools.m_label_map[str] = i;
			CreateDir(train_path, str);
			CreateDir(val_path, str);
		}

		int sz = (int)m_labels.size();
		WriteTrainProto(sz);
		WriteDeployProto(sz);
		OnBnClickedMakeLabelFile();
		WriteXML();

		AfxBeginThread(EnhanceImage_Thread, this);
	}
}


void CEasyCaffeDlg::OnBnClickedSaveXml()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	WriteXML();
}


void CEasyCaffeDlg::OnBnClickedMakeLabelFile()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	std::vector<BOOL> have_flag;
	for (auto i : m_labels)
		have_flag.push_back(FALSE);
	int label_len = (int)m_labels.size();
	for (auto it : m_img_labels){
		for (int i = 0; i < label_len; i++)
			if (it == m_labels[i])
				have_flag[i] = TRUE;
	}
	for (int i = label_len - 1; i > -1; i--){
		if (!have_flag[i])
		{
			std::string spath = DirAddSubdir(m_data_path, m_labels[i]);
			RemoveDirectoryA(spath.c_str());
			m_labels.erase(m_labels.begin() + i);
		}
	}
	ReflashBox();
	OnBnClickedSaveXml();

	std::string label_file = DirAddSubdir(m_project_path, "label.txt");
	std::ofstream ofs;
	ofs.open(label_file);
	if (ofs.is_open()){
		for (int i = 0; i < m_labels_box.GetCount(); i++){
			CString cstr;
			m_labels_box.GetText(i, cstr);
			std::string str = CW2A(cstr);
			ofs << str << std::endl;
		}
	}
	ofs.close();
}
