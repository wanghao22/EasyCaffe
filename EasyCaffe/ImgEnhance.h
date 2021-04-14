#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
#include <io.h>
#include <map>
#include "DirFileOption.h"

#define _TRAIN_CMD_ "train_val.cmd"

class ImgEnhance
{
public:
	ImgEnhance();
	~ImgEnhance();
	//void Enhance();
	void Enhance(int idx);
	void LoadImages(std::string path);
	void AddImage(std::vector<std::string> filenames);
	void AddImage(std::string filename);
	int SaveXML(std::string file);
	int LoadXML(std::string file);
	//开始训练
	void TrainStart(std::string out_dir);
	void CmdTrainStart(std::string out_dir);
	void SetCaffeToolsDir(std::string caffe_tools)
	{
		caffe_tools_dir = caffe_tools;
	}
	BOOL m_Flag_Rotate;//旋转
	BOOL m_Flag_Rotate90;//旋转90°
	BOOL m_Flag_FlipX;//X轴翻转
	BOOL m_Flag_FlipY;//Y轴翻转
	BOOL m_Flag_FlipXY;//XY轴翻转
	BOOL m_Flag_Crop;//修剪
	BOOL m_Flag_Blur;//滤波
	BOOL m_Flag_Contrast;//降低亮度
	BOOL m_Flag_Brightness;//提升亮度
	BOOL m_Flag_Noise;//加噪点
	BOOL m_Flag_Sharpen;//锐化
	int m_Rotate_Step;
	double m_Crop_Scale;//缩放因子（0.5~0.9）
	int m_Contrast_Max;
	int m_Contrast_Step;
	int m_Brightness_Max;
	int m_Brightness_Step;
	bool m_Other_Path_Flag;//是否保存在输入位置
	double m_Sharpen_Thre;
	double m_Sharpen_Amount;
	int m_Val_Scale;//验证集的大小（千分之）
	std::string m_Other_Path;//输入位置
	std::vector<std::string> m_fullpath;//源图像的全路径
	//std::vector<cv::Mat> m_imgs;//源图像
	std::vector<std::string> path;//路径
	std::vector<std::string> name;//名称
	std::vector<std::string> type;//后缀
	//标签
	std::vector<std::string> labels;
	//std::vector<cv::Mat> m_enhance_imgs;//增强的图像
	std::vector<std::string> m_enhance_fpath;//增强的图像的全路径
	std::string this_dir;
	bool m_open_thread_flag;
	//遍历所有的文件
	void findFile(std::string path, std::vector<std::string>& res);
	//把完整的路径分割成：路径、文件名、文件后缀。
	int PathDevision(std::string str, std::string &path, std::string &name, std::string &type);
	//BOOL m_is_color_flag;
	//生成txt文件
	void ClassifyTrainVal();
	std::map<std::string, int> m_label_map;
private:
	std::string caffe_tools_dir;//Caffe工具路径
	//旋转具体操作
	void Do_rotate(cv::Mat src, cv::Mat &dst, double angle);
	//明暗度具体操作
	void Do_Vintensity(cv::Mat src, cv::Mat &dst, int value);
	//锐化具体操作
	void Do_Sharpen(cv::Mat src, cv::Mat &dst, double threshold, double amount);
	//旋转
	//int IRotate();
	//int IRotate90();
	int IRotate(cv::Mat src, int idx);
	int IRotate90(cv::Mat src, int idx);
	//翻转
	//int IFlipX();
	//int IFlipY();
	//int IFlipXY();
	int IFlipX(cv::Mat src, int idx);
	int IFlipY(cv::Mat src, int idx);
	int IFlipXY(cv::Mat src, int idx);
	//修剪
	//int ICrop();
	int ICrop(cv::Mat src, int idx);
	//滤波
	//int IBlur();
	int IBlur(cv::Mat src, int idx);
	//明暗度调节
	//int IContrast();
	//int IBrightness();
	int IContrast(cv::Mat src, int idx);
	int IBrightness(cv::Mat src, int idx);
	//添加噪声
	//int INoise();
	int INoise(cv::Mat src, int idx);
	//锐化
	//int ISharpen();
	int ISharpen(cv::Mat src, int idx);
	//随机获取子文件夹
	std::string ClassifyFloder(int idx);
	std::string ClassifyFloder(std::string str) = delete;
	//
	void GiveTxtInfo(std::string fullpath, bool & train_flag, int & info, std::string &filename);
};

