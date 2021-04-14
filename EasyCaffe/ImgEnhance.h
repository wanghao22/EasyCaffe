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
	//��ʼѵ��
	void TrainStart(std::string out_dir);
	void CmdTrainStart(std::string out_dir);
	void SetCaffeToolsDir(std::string caffe_tools)
	{
		caffe_tools_dir = caffe_tools;
	}
	BOOL m_Flag_Rotate;//��ת
	BOOL m_Flag_Rotate90;//��ת90��
	BOOL m_Flag_FlipX;//X�ᷭת
	BOOL m_Flag_FlipY;//Y�ᷭת
	BOOL m_Flag_FlipXY;//XY�ᷭת
	BOOL m_Flag_Crop;//�޼�
	BOOL m_Flag_Blur;//�˲�
	BOOL m_Flag_Contrast;//��������
	BOOL m_Flag_Brightness;//��������
	BOOL m_Flag_Noise;//�����
	BOOL m_Flag_Sharpen;//��
	int m_Rotate_Step;
	double m_Crop_Scale;//�������ӣ�0.5~0.9��
	int m_Contrast_Max;
	int m_Contrast_Step;
	int m_Brightness_Max;
	int m_Brightness_Step;
	bool m_Other_Path_Flag;//�Ƿ񱣴�������λ��
	double m_Sharpen_Thre;
	double m_Sharpen_Amount;
	int m_Val_Scale;//��֤���Ĵ�С��ǧ��֮��
	std::string m_Other_Path;//����λ��
	std::vector<std::string> m_fullpath;//Դͼ���ȫ·��
	//std::vector<cv::Mat> m_imgs;//Դͼ��
	std::vector<std::string> path;//·��
	std::vector<std::string> name;//����
	std::vector<std::string> type;//��׺
	//��ǩ
	std::vector<std::string> labels;
	//std::vector<cv::Mat> m_enhance_imgs;//��ǿ��ͼ��
	std::vector<std::string> m_enhance_fpath;//��ǿ��ͼ���ȫ·��
	std::string this_dir;
	bool m_open_thread_flag;
	//�������е��ļ�
	void findFile(std::string path, std::vector<std::string>& res);
	//��������·���ָ�ɣ�·�����ļ������ļ���׺��
	int PathDevision(std::string str, std::string &path, std::string &name, std::string &type);
	//BOOL m_is_color_flag;
	//����txt�ļ�
	void ClassifyTrainVal();
	std::map<std::string, int> m_label_map;
private:
	std::string caffe_tools_dir;//Caffe����·��
	//��ת�������
	void Do_rotate(cv::Mat src, cv::Mat &dst, double angle);
	//�����Ⱦ������
	void Do_Vintensity(cv::Mat src, cv::Mat &dst, int value);
	//�񻯾������
	void Do_Sharpen(cv::Mat src, cv::Mat &dst, double threshold, double amount);
	//��ת
	//int IRotate();
	//int IRotate90();
	int IRotate(cv::Mat src, int idx);
	int IRotate90(cv::Mat src, int idx);
	//��ת
	//int IFlipX();
	//int IFlipY();
	//int IFlipXY();
	int IFlipX(cv::Mat src, int idx);
	int IFlipY(cv::Mat src, int idx);
	int IFlipXY(cv::Mat src, int idx);
	//�޼�
	//int ICrop();
	int ICrop(cv::Mat src, int idx);
	//�˲�
	//int IBlur();
	int IBlur(cv::Mat src, int idx);
	//�����ȵ���
	//int IContrast();
	//int IBrightness();
	int IContrast(cv::Mat src, int idx);
	int IBrightness(cv::Mat src, int idx);
	//�������
	//int INoise();
	int INoise(cv::Mat src, int idx);
	//��
	//int ISharpen();
	int ISharpen(cv::Mat src, int idx);
	//�����ȡ���ļ���
	std::string ClassifyFloder(int idx);
	std::string ClassifyFloder(std::string str) = delete;
	//
	void GiveTxtInfo(std::string fullpath, bool & train_flag, int & info, std::string &filename);
};

