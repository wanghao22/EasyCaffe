
// EasyCaffeDlg.h : 头文件
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

// CEasyCaffeDlg 对话框
class CEasyCaffeDlg : public CDialogEx
{
// 构造
public:
	CEasyCaffeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EASYCAFFE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	//新建工程——实体
	void DoNewProject();
	bool m_project_open_flag;
	//UI按钮变化——实体
	void DoProjectChange(bool flag);
	bool m_run_flag;
	//获取EXE的安装目录
	void GetThisDir();
	//复制caffemodel文件夹下的文件
	void CopyCaffeModelFile(std::string filename);
	//重写训练解决方案文件
	void WriteSolverProto();
	//重写训练文件
	void WriteTrainProto(int cnt);
	//重写解析文件
	void WriteDeployProto(int cnt);
	//保存工程文件
	int WriteXML();
	//加载工程文件
	int ReadXML(std::string file);
	//种类数目
	int m_labels_cnt;
	//关联MFC窗口和OPENCV窗口
	void SetImgWindow(int Dlgitem, const char* str);
	//显示MAT图像到MFC窗口上
	void ShowMatImg(cv::Mat src, int Dlgitem, const char* str);
	//打开项目——实体
	void OpenProject(std::string file);
	//显示空白图像
	//void ShowNoneImg();
	//删除文件夹及其子文件
	void DeleteDirectory(CString sDirName);
	//删除标签——实体
	void DeleteLabel(std::string label);
	//替换'\\'为'/'
	void DoWithStr(std::string &str);
	cv::Mat m_src;
	cv::Mat m_white;
public:
	//图像Size
	int m_size_w, m_size_h;
	//线程调整按钮功能
	void DuringThreadBtn(bool flag);
	ImgEnhance train_tools;
	int GetSize(cv::Mat src);
	std::string m_project_path;
	std::string m_data_path;
	//标签种类
	std::vector<std::string> m_labels;
	std::vector<std::string> m_full_path;
	std::vector<std::string> m_path;
	std::vector<std::string> m_name;
	std::vector<std::string> m_type;
	//每个图像对应的标签
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
	//选择标签
	afx_msg void OnLbnSelchangeLabelsBox();
	afx_msg void OnBnClickedAddLabel();
	afx_msg void OnBnClickedChgLabel();
	afx_msg void OnBnClickedDelLabel();
	//遍历所有文件，会查找子目录
	void findFile(std::string path, std::vector<std::string>& res);
	//文件全名分割
	int PathDevision(std::string str, std::string & path, std::string & name, std::string & type);
	CListCtrl m_imgnames_list;
	//开始加载所有图像
	void GetImgLists();
	//刷新标签箱子——实体
	void ReflashBox();
	//UI——列表显示图像
	afx_msg void OnLvnItemchangedLableList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddImage();
	afx_msg void OnBnClickedDelImage();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedEnhanceAndTrain();
	afx_msg void OnBnClickedSaveXml();
	afx_msg void OnBnClickedMakeLabelFile();
};
