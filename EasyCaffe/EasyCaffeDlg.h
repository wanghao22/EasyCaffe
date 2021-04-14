
// EasyCaffeDlg.h : ͷ�ļ�
//

#pragma once
#include <opencv2\opencv.hpp>
#include "NewProject.h"
#include "DirFileOption.h"
#include <fstream>
#include "afxwin.h"
#include <vector>
#include "SetLabel.h"
#include <map>
#include "ImgEnhance.h"
#include "afxcmn.h"
#include "Param.h"

// CEasyCaffeDlg �Ի���
class CEasyCaffeDlg : public CDialogEx
{
// ����
public:
	CEasyCaffeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EASYCAFFE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	const char* m_win1 = "win1";
	CRect m_wndRect;
	void __init__();
	void __exit__();
	//Project Path
	std::string m_caffe_model_path;
	std::string m_lmdb_path;
	std::string m_img_enhance_xml;
	//Exe Path
	std::string this_dir;
	std::string m_caffe_model_path_ori;
	std::string m_caffe_tools_path;
	//�½����̡���ʵ��
	void DoNewProject();
	bool m_project_open_flag;
	//UI��ť�仯����ʵ��
	void DoProjectChange(bool flag);
	bool m_run_flag;
	//��ȡEXE�İ�װĿ¼
	void GetThisDir();
	//����caffemodel�ļ����µ��ļ�
	void CopyCaffeModelFile(std::string filename);
	//��дѵ����������ļ�
	void WriteSolverProto();
	//��дѵ���ļ�
	void WriteTrainProto(int cnt);
	//��д�����ļ�
	void WriteDeployProto(int cnt);
	//���湤���ļ�
	int WriteXML();
	//���ع����ļ�
	int ReadXML(std::string file);
	//������Ŀ
	int m_labels_cnt;
	//����MFC���ں�OPENCV����
	void SetImgWindow(int Dlgitem, const char* str);
	//��ʾMATͼ��MFC������
	void ShowMatImg(cv::Mat src, int Dlgitem, const char* str);
	//����Ŀ����ʵ��
	void OpenProject(std::string file);
	//��ʾ�հ�ͼ��
	//void ShowNoneImg();
	//ɾ���ļ��м������ļ�
	void DeleteDirectory(CString sDirName);
	//ɾ����ǩ����ʵ��
	void DeleteLabel(std::string label);
	//�滻'\\'Ϊ'/'
	void DoWithStr(std::string &str);
	cv::Mat m_src;
	cv::Mat m_white;
public:
	//ͼ��Size
	int m_size_w, m_size_h;
	//�̵߳�����ť����
	void DuringThreadBtn(bool flag);
	ImgEnhance train_tools;
	int GetSize(cv::Mat src);
	std::string m_project_path;
	std::string m_data_path;
	//��ǩ����
	std::vector<std::string> m_labels;
	std::vector<std::string> m_full_path;
	std::vector<std::string> m_path;
	std::vector<std::string> m_name;
	std::vector<std::string> m_type;
	//ÿ��ͼ���Ӧ�ı�ǩ
	std::vector<std::string> m_img_labels;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedNewWork();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCloseWork();
	bool getRunStatus() { return m_run_flag; };
	afx_msg void OnClose();
	afx_msg void OnBnClickedOpenWork();
	CListBox m_labels_box;
	//ѡ���ǩ
	afx_msg void OnLbnSelchangeLabelsBox();
	afx_msg void OnBnClickedAddLabel();
	afx_msg void OnBnClickedChgLabel();
	afx_msg void OnBnClickedDelLabel();
	//���������ļ����������Ŀ¼
	void findFile(std::string path, std::vector<std::string>& res);
	//�ļ�ȫ���ָ�
	int PathDevision(std::string str, std::string & path, std::string & name, std::string & type);
	CListCtrl m_imgnames_list;
	//��ʼ��������ͼ��
	void GetImgLists();
	//ˢ�±�ǩ���ӡ���ʵ��
	void ReflashBox();
	//UI�����б���ʾͼ��
	afx_msg void OnLvnItemchangedLableList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddImage();
	afx_msg void OnBnClickedDelImage();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedEnhanceAndTrain();
	afx_msg void OnBnClickedSaveXml();
	afx_msg void OnBnClickedMakeLabelFile();
};
