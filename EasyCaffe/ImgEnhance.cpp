#include "stdafx.h"
#include "ImgEnhance.h"

using namespace std;
using namespace cv;
using namespace DirFile;

int getValue(map<std::string, int> labels, std::string key) {
	for (auto it : labels) {
		if (it.first == key)
			return it.second;
	}
	return -1;
}

std::string getKey(map<std::string, int> labels, int value) {
	for (auto it : labels) {
		if (it.second == value)
			return it.first;
	}
	return "";
}

ImgEnhance::ImgEnhance()
{
	//srand((int)time(0));
	m_open_thread_flag = false;
}


ImgEnhance::~ImgEnhance()
{
}

void ImgEnhance::Enhance(int idx)
{
	Mat src = imread(m_fullpath[idx], -1);
	if (m_Flag_Rotate)
		IRotate(src,idx);
	if (m_Flag_Rotate90)
		IRotate90(src, idx);
	if (m_Flag_FlipX)
		IFlipX(src, idx);
	if (m_Flag_FlipY)
		IFlipY(src, idx);
	if (m_Flag_FlipXY)
		IFlipXY(src, idx);
	if (m_Flag_Crop)
		ICrop(src, idx);
	if (m_Flag_Blur)
		IBlur(src, idx);
	if (m_Flag_Contrast)
		IContrast(src, idx);
	if (m_Flag_Brightness)
		IBrightness(src, idx);
	if (m_Flag_Noise)
		INoise(src, idx);
	if (m_Flag_Sharpen)
		ISharpen(src, idx);
}


void ImgEnhance::LoadImages(string fullpath)
{
	m_fullpath.clear();
	path.clear();
	name.clear();
	type.clear();
	std::vector<std::string> temp;
	findFile(fullpath, temp);
	Mat src;
	string paths, names, types;
	for (size_t i = 0; i < temp.size(); i++)
	{
		src = imread(temp[i], -1);
		if (src.empty())
			continue;
		if (PathDevision(temp[i], paths, names, types) != 0)
			continue;
		//this->m_imgs.push_back(src);
		this->m_fullpath.push_back(temp[i]);
		this->path.push_back(paths);
		this->name.push_back(names);
		this->type.push_back(types);
	}
}

void ImgEnhance::AddImage(std::vector<std::string> filenames)
{
	for (size_t i = 0; i < filenames.size(); i++)
		AddImage(filenames[i]);
}

void ImgEnhance::AddImage(std::string filename)
{
	Mat src = imread(filename, -1);
	if (src.empty())
		return;
	string paths, names, types;
	if (PathDevision(filename, paths, names, types) != 0)
		return;
	//this->m_imgs.push_back(src);
	this->m_fullpath.push_back(filename);
	this->path.push_back(paths);
	this->name.push_back(names);
	this->type.push_back(types);
}

int ImgEnhance::SaveXML(std::string file)
{
	FileStorage fs(file, FileStorage::WRITE);
	if (!fs.isOpened())
	{
		return -2;
	}
	fs << "m_Flag_Rotate" << m_Flag_Rotate;
	fs << "m_Flag_Rotate90" << m_Flag_Rotate90;
	fs << "m_Flag_FlipX" << m_Flag_FlipX;
	fs << "m_Flag_FlipY" << m_Flag_FlipY;
	fs << "m_Flag_FlipXY" << m_Flag_FlipXY;
	fs << "m_Flag_Crop" << m_Flag_Crop;
	fs << "m_Flag_Blur" << m_Flag_Blur;
	fs << "m_Flag_Contrast" << m_Flag_Contrast;
	fs << "m_Flag_Brightness" << m_Flag_Brightness;
	fs << "m_Flag_Noise" << m_Flag_Noise;
	fs << "m_Flag_Sharpen" << m_Flag_Sharpen;
	fs << "m_Rotate_Step" << m_Rotate_Step;
	fs << "m_Crop_Scale" << m_Crop_Scale;
	fs << "m_Contrast_Max" << m_Contrast_Max;
	fs << "m_Contrast_Step" << m_Contrast_Step;
	fs << "m_Brightness_Max" << m_Brightness_Max;
	fs << "m_Brightness_Step" << m_Brightness_Step;
	fs << "m_Other_Path_Flag" << m_Other_Path_Flag;
	fs << "m_Sharpen_Thre" << m_Sharpen_Thre;
	fs << "m_Sharpen_Amount" << m_Sharpen_Amount;
	fs << "m_Val_Scale" << m_Val_Scale;
	fs << "m_Other_Path" << m_Other_Path;
	fs.release();
	return 0;
}

int ImgEnhance::LoadXML(std::string file)
{
	FileStorage fs(file, FileStorage::READ);
	if (!fs.isOpened())
	{
		return -2;
	}
	fs["m_Flag_Rotate"] >> m_Flag_Rotate;
	fs["m_Flag_Rotate90"] >> m_Flag_Rotate90;
	fs["m_Flag_FlipX"] >> m_Flag_FlipX;
	fs["m_Flag_FlipY"] >> m_Flag_FlipY;
	fs["m_Flag_FlipXY"] >> m_Flag_FlipXY;
	fs["m_Flag_Crop"] >> m_Flag_Crop;
	fs["m_Flag_Blur"] >> m_Flag_Blur;
	fs["m_Flag_Contrast"] >> m_Flag_Contrast;
	fs["m_Flag_Brightness"] >> m_Flag_Brightness;
	fs["m_Flag_Noise"] >> m_Flag_Noise;
	fs["m_Flag_Sharpen"] >> m_Flag_Sharpen;
	fs["m_Rotate_Step"] >> m_Rotate_Step;
	fs["m_Crop_Scale"] >> m_Crop_Scale;
	fs["m_Contrast_Max"] >> m_Contrast_Max;
	fs["m_Contrast_Step"] >> m_Contrast_Step;
	fs["m_Brightness_Max"] >> m_Brightness_Max;
	fs["m_Brightness_Step"] >> m_Brightness_Step;
	fs["m_Other_Path_Flag"] >> m_Other_Path_Flag;
	fs["m_Sharpen_Thre"] >> m_Sharpen_Thre;
	fs["m_Sharpen_Amount"] >> m_Sharpen_Amount;
	fs["m_Val_Scale"] >> m_Val_Scale;
	fs["m_Other_Path"] >> m_Other_Path;
	fs.release();
	return 0;
}

//private:会查找子目录
void ImgEnhance::findFile(std::string path, std::vector<std::string>& res)
{
	string p;
	intptr_t hFile = 0;                //文件句柄
	struct _finddata_t fileinfo;    //文件信息
									//查找文件夹中的第一个文件，然后利用句柄进行遍历
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != 0)
	{
		do
		{
			string path_s = p.assign(path).append("\\").append(fileinfo.name);
			if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
				continue;
			if (fileinfo.attrib == _A_SUBDIR)
			{
				//遍历子目录
				std::vector<std::string> temp;
				findFile(path_s, temp);
				for (size_t i = 0; i < temp.size(); i++)
					res.push_back(temp[i]);
				continue;
			}
			res.push_back(path_s);            //插入容器
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);    //关闭文件
	}
}

int ImgEnhance::PathDevision(std::string str, std::string & path, std::string & name, std::string & type)
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

void ImgEnhance::Do_rotate(cv::Mat src, cv::Mat & dst, double angle)
{
	cv::Point2f center(src.cols / 2.0f, src.rows / 2.0f);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	warpAffine(src, dst, rot, src.size());
}

void ImgEnhance::Do_Vintensity(cv::Mat src, cv::Mat & dst, int value)
{
	cv::Mat bgr;
	if (src.channels() == 1)
		cvtColor(src, bgr, COLOR_GRAY2BGR);
	else if (src.channels() == 4)
		cvtColor(src, bgr, COLOR_BGRA2BGR);
	else if (src.channels() == 3)
		bgr = src.clone();
	else
		return;
	cv::Mat hsv;
	cvtColor(bgr, hsv, CV_BGR2HSV);
	for (int i = 0; i < hsv.rows; i++)
	{
		for (int j = 0; j < hsv.cols; j++)
		{
			if (value > 0)
				hsv.at<Vec3b>(i, j)[2] = MIN(hsv.at<Vec3b>(i, j)[2] + value, 255);
			else
				hsv.at<Vec3b>(i, j)[2] = MAX(hsv.at<Vec3b>(i, j)[2] + value, 0);
		}
	}
	cvtColor(hsv, bgr, CV_HSV2BGR);
	if (src.channels() == 1)
		cvtColor(bgr, dst, COLOR_BGR2GRAY);
	else if (src.channels() == 4)
		cvtColor(bgr, dst, COLOR_BGR2BGRA);
	else if (src.channels() == 3)
		dst = bgr.clone();
}

void ImgEnhance::Do_Sharpen(cv::Mat src, cv::Mat & dst, double threshold, double amount)
{
	Mat blurred;
	double sigma = 1;
	GaussianBlur(src, blurred, Size(), sigma, sigma);
	//int depth = CV_MAT_DEPTH(imageSrc.type());
	Mat lowContrastMask = abs(src - blurred) < threshold;
	dst = src*(1 + amount) + blurred*(-amount);      //original + (original - blurred) * amount
	src.copyTo(dst, lowContrastMask);//将imageSrc中lowContrastMask对应的非0部分复制到dst中
	//lowContrastMask等于1时，说明高频分量比较小，有可能是噪声，此时不做锐化
}


int ImgEnhance::IRotate(cv::Mat src,int idx)
{
	for (int j = m_Rotate_Step; j < 360; j += m_Rotate_Step)
	{
		if (src.empty())
			return -1;
		if (src.cols != src.rows)
			return -1;
		cv::Mat dst;
		Do_rotate(src, dst, j);
		std::string enhance_path;
		if (m_Other_Path_Flag)
		{
			enhance_path = (m_Other_Path + ClassifyFloder(idx) + name[idx] + "Rotate"
				+ to_string(j) + type[idx]);
			m_enhance_fpath.push_back(enhance_path);
		}
		else
		{
			enhance_path = (path[idx] + name[idx] + "Rotate" + to_string(j) + type[idx]);
			m_enhance_fpath.push_back(enhance_path);
		}
		if (!dst.empty())
			imwrite(enhance_path, dst);
	}
	return 0;
}

int ImgEnhance::IRotate90(cv::Mat src, int idx)
{
	if (src.empty())
		return -1;
	if (src.cols != src.rows)
		return -1;
	cv::Mat dst1, dst2;
	Do_rotate(src, dst1, 90);
	Do_rotate(src, dst2, 270);
	std::string enhance_path1, enhance_path2;
	if (m_Other_Path_Flag)
	{
		enhance_path1 = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "Rotate90" + type[idx]);
		enhance_path2 = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "Rotate270" + type[idx]);
		m_enhance_fpath.push_back(enhance_path1);
		m_enhance_fpath.push_back(enhance_path2);
	}
	else
	{
		enhance_path1 = (path[idx] + name[idx] + "Rotate90" + type[idx]);
		enhance_path2 = (path[idx] + name[idx] + "Rotate270" + type[idx]);
		m_enhance_fpath.push_back(enhance_path1);
		m_enhance_fpath.push_back(enhance_path2);
	}
	if (!src.empty())
	{
		imwrite(enhance_path1, dst1);
		imwrite(enhance_path2, dst2);
	}
	return 0;
}

int ImgEnhance::IFlipX(cv::Mat src, int idx)
{
	if (src.empty())
		return -1;
	Mat dst;
	flip(src, dst, 0);
	std::string enhance_path;
	if (m_Other_Path_Flag)
	{
		enhance_path = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "FlipX" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	else
	{
		enhance_path = (path[idx] + name[idx] + "FlipX" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	if (!dst.empty())
		imwrite(enhance_path, dst);
	return 0;
}

int ImgEnhance::IFlipY(cv::Mat src, int idx)
{
	if (src.empty())
		return -1;
	Mat dst;
	flip(src, dst, 1);
	std::string enhance_path;
	if (m_Other_Path_Flag)
	{
		enhance_path = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "FlipX" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	else
	{
		enhance_path = (path[idx] + name[idx] + "FlipX" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	if (!dst.empty())
		imwrite(enhance_path, dst);
	return 0;
}

int ImgEnhance::IFlipXY(cv::Mat src, int idx)
{
	if (src.empty())
		return -1;
	Mat dst;
	flip(src, dst, -1);
	std::string enhance_path;
	if (m_Other_Path_Flag)
	{
		enhance_path = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "FlipX" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	else
	{
		enhance_path = (path[idx] + name[idx] + "FlipX" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	if (!dst.empty())
		imwrite(enhance_path, dst);
	return 0;
}

int ImgEnhance::ICrop(cv::Mat src, int idx)
{
	m_Crop_Scale = MAX(m_Crop_Scale, 0.5);
	m_Crop_Scale = MIN(m_Crop_Scale, 0.9);
	if (src.empty())
		return -1;
	CvRect roi1, roi2, roi3, roi4;
	double sub = 1 - m_Crop_Scale;
	roi1 = cvRect(0, 0, src.cols*m_Crop_Scale, src.rows*m_Crop_Scale);
	roi2 = cvRect(src.cols*sub - 1, 0, src.cols*m_Crop_Scale, src.rows*m_Crop_Scale);
	roi3 = cvRect(0, src.rows*sub - 1, src.cols*m_Crop_Scale, src.rows*m_Crop_Scale);
	roi4 = cvRect(src.cols*sub - 1, src.rows*sub - 1,
		src.cols*m_Crop_Scale, src.rows*m_Crop_Scale);
	Mat dst1, dst2, dst3, dst4;
	dst1 = src(roi1);
	dst2 = src(roi2);
	dst3 = src(roi3);
	dst4 = src(roi4);
	cv::resize(dst1, dst1, src.size());
	cv::resize(dst2, dst2, src.size());
	cv::resize(dst3, dst3, src.size());
	cv::resize(dst4, dst4, src.size());
	std::string enhance_path1, enhance_path2, enhance_path3, enhance_path4;
	if (m_Other_Path_Flag)
	{
		enhance_path1 = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "Crop1" + type[idx]);
		enhance_path2 = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "Crop2" + type[idx]);
		enhance_path3 = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "Crop3" + type[idx]);
		enhance_path4 = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "Crop4" + type[idx]);
		m_enhance_fpath.push_back(enhance_path1);
		m_enhance_fpath.push_back(enhance_path2);
		m_enhance_fpath.push_back(enhance_path3);
		m_enhance_fpath.push_back(enhance_path4);
	}
	else
	{
		enhance_path1 = (path[idx] + name[idx] + "Crop1" + type[idx]);
		enhance_path2 = (path[idx] + name[idx] + "Crop2" + type[idx]);
		enhance_path3 = (path[idx] + name[idx] + "Crop3" + type[idx]);
		enhance_path4 = (path[idx] + name[idx] + "Crop4" + type[idx]);
		m_enhance_fpath.push_back(enhance_path1);
		m_enhance_fpath.push_back(enhance_path2);
		m_enhance_fpath.push_back(enhance_path3);
		m_enhance_fpath.push_back(enhance_path4);
	}
	if (!src.empty())
	{
		imwrite(enhance_path1, dst1);
		imwrite(enhance_path2, dst2);
		imwrite(enhance_path3, dst3);
		imwrite(enhance_path4, dst4);
	}
	return 0;
}

int ImgEnhance::IBlur(cv::Mat src, int idx)
{
	if (src.empty())
		return -1;
	Mat dst;
	blur(src, dst, cvSize(3, 3));
	std::string enhance_path;
	if (m_Other_Path_Flag)
	{
		enhance_path = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "Blur" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	else
	{
		enhance_path = (path[idx] + name[idx] + "Blur" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	if (!dst.empty())
		imwrite(enhance_path, dst);
	return 0;
}

int ImgEnhance::IContrast(cv::Mat src, int idx)
{
	for (int j = m_Contrast_Step; j < m_Contrast_Max; j += m_Contrast_Step)
	{
		if (src.empty())
			return -1;
		cv::Mat dst;
		Do_Vintensity(src, dst, 0 - j);
		std::string enhancepath;
		if (m_Other_Path_Flag)
		{
			enhancepath = (m_Other_Path + ClassifyFloder(idx) +
				name[idx] + "Contrast" + to_string(j) + type[idx]);
			m_enhance_fpath.push_back(enhancepath);
		}
		else
		{
			enhancepath = (path[idx] + name[idx] + "Contrast" + to_string(j) + type[idx]);
			m_enhance_fpath.push_back(enhancepath);
		}
		if (!dst.empty())
			imwrite(enhancepath, dst);
	}
	return 0;
}

int ImgEnhance::IBrightness(cv::Mat src, int idx)
{
	for (int j = m_Brightness_Step; j < m_Brightness_Max; j += m_Brightness_Step)
	{
		if (src.empty())
			return -1;
		cv::Mat dst;
		Do_Vintensity(src, dst, j);
		std::string enhancepath;
		if (m_Other_Path_Flag)
		{
			enhancepath = (m_Other_Path + ClassifyFloder(idx) +
				name[idx] + "Brightness" + to_string(j) + type[idx]);
			m_enhance_fpath.push_back(enhancepath);
		}
		else
		{
			enhancepath = (path[idx] + name[idx] + "Brightness" + to_string(j) + type[idx]);
			m_enhance_fpath.push_back(enhancepath);
		}
		if (!dst.empty())
			imwrite(enhancepath, dst);
	}
	return 0;
}

int ImgEnhance::INoise(cv::Mat src, int idx)
{
	if (src.empty())
		return -1;
	Mat GaussianMat = src.clone();
	RNG rng;
	rng.fill(GaussianMat, cv::RNG::NORMAL, rng.uniform((double)0, (double)20),
		rng.uniform((double)0, (double)20));
	Mat dst;
	add(src, GaussianMat, dst);
	std::string enhance_path;
	if (m_Other_Path_Flag)
	{
		enhance_path = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "Noise" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	else
	{
		enhance_path = (path[idx] + name[idx] + "Noise" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	if (!dst.empty())
		imwrite(enhance_path, dst);
	return 0;
}

int ImgEnhance::ISharpen(cv::Mat src, int idx)
{
	if (src.empty())
		return -1;
	Mat dst;
	Do_Sharpen(src, dst, m_Sharpen_Thre, m_Sharpen_Amount);
	std::string enhance_path;
	if (m_Other_Path_Flag)
	{
		enhance_path = (m_Other_Path + ClassifyFloder(idx) +
			name[idx] + "Sharpen" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	else
	{
		enhance_path = (path[idx] + name[idx] + "Sharpen" + type[idx]);
		m_enhance_fpath.push_back(enhance_path);
	}
	if (!dst.empty())
		imwrite(enhance_path, dst);
	return 0;
}

void ImgEnhance::ClassifyTrainVal()
{
	string train_file = m_Other_Path + "/train.txt";
	string val_file = m_Other_Path + "/val.txt";
	ofstream train, val;
	train.open(train_file);
	val.open(val_file);
	bool train_f;
	int judege;
	string filename;
	for (size_t i = 0; i < m_fullpath.size(); i++)
	{
		cv::Mat src;
		src = imread(m_fullpath[i], -1);
		string fullpath = m_Other_Path + ClassifyFloder(i) + name[i] + type[i];
		imwrite(fullpath, src);
		GiveTxtInfo(fullpath, train_f, judege, filename);
		if (train_f)
			train << filename << " " << judege << endl;
		else
			val << filename << " " << judege << endl;
	}
	for (size_t i = 0; i < m_enhance_fpath.size(); i++)
	{
		GiveTxtInfo(m_enhance_fpath[i], train_f, judege, filename);
		if (train_f)
			train << filename << " " << judege << endl;
		else
			val << filename << " " << judege << endl;
	}
	train.close();
	val.close();
}

//std::string ImgEnhance::ClassifyFloder(std::string strl)
//{
//	int x = rand() % 1000;
//	int y1 = strl.find("NG");
//	int y2 = strl.find("OK");
//	int y3 = strl.find("UN");
//	if (y1 > 0 && x < m_Val_Scale)
//		return "\\val\\NG\\";
//	else if (y1 > 0 && x >= m_Val_Scale)
//		return "\\train\\NG\\";
//	else if (y2 > 0 && x < m_Val_Scale)
//		return "\\val\\OK\\";
//	else if (y2 > 0 && x >= m_Val_Scale)
//		return "\\train\\OK\\";
//	else if (y3 > 0 && x < m_Val_Scale)
//		return "\\val\\UN\\";
//	else if (y3 > 0 && x >= m_Val_Scale)
//		return "\\train\\UN\\";
//}

std::string ImgEnhance::ClassifyFloder(int idx){
	int x = rand() % 1000;
	std::string result = "\\";
	if (x < m_Val_Scale)
	{
		result += "val\\";
	}
	else
	{
		result += "train\\";
	}
	result += labels[idx];
	result += "\\";
	return result;
}

void ImgEnhance::GiveTxtInfo(std::string fullpath, bool & train_flag, int & info, std::string &filename)
{
	string str1, str2, str3;
	if (PathDevision(fullpath, str1, str2, str3) != 0)
		return;
	std::string fpath = GetFatherDir(str1);
	std::string fname = GetFatherDirName(str1);
	std::string ffname = GetFatherDirName(fpath);

	if (ffname == "train"){
		train_flag = true;
		filename = "train/" + fname + "/" + str2 + str3;
	}
	else{
		train_flag = false;
		filename = "val/" + fname + "/" + str2 + str3;
	}
	info = getValue(m_label_map, fname);
}

void ImgEnhance::TrainStart(string out_dir)
{
	//开始cmd
	vector<string> cmd;
	cmd.push_back("rd /s/q ..\\temp\\lmdb\\train");
	cmd.push_back("rd /s/q ..\\temp\\lmdb\\val");
	string convert = DirAddSubdir(caffe_tools_dir, "convert_imageset.exe");
	string compute = DirAddSubdir(caffe_tools_dir, "compute_image_mean.exe");
	string caffe = DirAddSubdir(caffe_tools_dir, "caffe.exe");
	string caffemodel = DirAddSubdir(this_dir, "temp");
	caffemodel = DirAddSubdir(caffemodel, "caffemodel");
	cmd.push_back(convert + " --shuffle --resize_height=0 --resize_width=0 ..\\temp\\data\\ ..\\temp\\data\\train.txt ..\\temp\\lmdb\\train");
	cmd.push_back(convert + " --shuffle --resize_height=0 --resize_width=0 ..\\temp\\data\\ ..\\temp\\data\\val.txt ..\\temp\\lmdb\\val");
	cmd.push_back(compute + " ..\\temp\\lmdb\\train ..\\temp\\caffemodel\\mean.binaryproto");
	cmd.push_back(caffe + " train -solver ..\\temp\\caffemodel\\solver.prototxt");
	cmd.push_back("copy ..\\temp\\caffemodel\\mean.binaryproto " + out_dir);
	cmd.push_back("copy ..\\temp\\caffemodel\\caffenet_train_iter_10000.caffemodel " + out_dir + "\\caffenet.caffemodel");
	cmd.push_back("del " + caffemodel + "\\*.caffemodel");
	cmd.push_back("del " + caffemodel + "\\*.solverstate");
	for (size_t i = 0; i < cmd.size(); i++)
	{
		std::system(cmd[i].c_str());
	}
}

void ImgEnhance::CmdTrainStart(std::string out_dir)
{
	ofstream outfile;
	outfile.open(_TRAIN_CMD_);
	string convert1 = DirAddSubdir(caffe_tools_dir, "convert_imageset.exe");
	string compute1 = DirAddSubdir(caffe_tools_dir, "compute_image_mean.exe");
	string caffe1 = DirAddSubdir(caffe_tools_dir, "caffe.exe");

	string convert = "\"" + convert1 + "\"";
	string compute = "\"" + compute1 + "\"";
	string caffe = "\"" + caffe1 + "\"";

	string caffemodel = DirAddSubdir(out_dir, "caffemodel");
	string traindir = DirAddSubdir(out_dir, "lmdb\\train");
	string valdir = DirAddSubdir(out_dir, "lmdb\\val");
	string meanfile = DirAddSubdir(out_dir, "caffemodel\\mean.binaryproto");
	string datadir = DirAddSubdir(out_dir, "temp\\");
	string traintxt = DirAddSubdir(out_dir, "temp\\train.txt");
	string valtxt = DirAddSubdir(out_dir, "temp\\val.txt");
	string solver = DirAddSubdir(out_dir, "caffemodel\\solver.prototxt");
	string bestmdoel = DirAddSubdir(out_dir, "caffemodel\\caffenet_train_iter_10000.caffemodel");
	outfile << "set GLOG_log_dir=" + datadir << endl;
	outfile << "rd /s/q " + traindir << endl;
	outfile << "rd /s/q " + valdir << endl;
	outfile << convert + " --shuffle --resize_height=0 --resize_width=0 " + datadir + " " + traintxt + " " + traindir << endl;
	outfile << convert + " --shuffle --resize_height=0 --resize_width=0 " + datadir + " " + valtxt + " " + valdir << endl;
	outfile << compute + " " + traindir + " " + meanfile << endl;
	outfile << caffe + " train -solver " + solver << endl;
	outfile << "copy " + meanfile + " " + out_dir << endl;
	outfile << "copy " + bestmdoel + " " + out_dir + "\\caffenet.caffemodel" << endl;
	outfile << "del " + caffemodel + "\\*.caffemodel" << endl;
	outfile << "del " + caffemodel + "\\*.solverstate" << endl;
	outfile.close();
	std::system(_TRAIN_CMD_);
}
