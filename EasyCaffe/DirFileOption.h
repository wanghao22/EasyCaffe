#pragma once
#include <io.h>
#include <fstream>
#include <iostream>
#include <string>

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace DirFile {
	//文件夹或者文件是否存在
	bool IsExisteDir(std::string path);

	//将父文件夹和子文件夹或者子文件合并为一个完整的路径
	std::string DirAddSubdir(std::string path, std::string subdir);

	//解决中文乱码的char*转wchar_t*
	CString zhToCString(std::string str);

	//wstring转string
	std::string wstring2string(const std::wstring &ws);

	//string转wstring
	std::wstring string2wstring(const std::string &s);

	//创建文件夹 完整路径
	int CreateDir(std::string dir);

	int CreateDir(CString dir);

	//创建文件夹 组合路径
	int CreateDir(std::string path, std::string subdir);

	//创建文件 完整路径
	int _CreateFile(std::string fullname);

	//创建文件 组合路径
	int _CreateFile(std::string path, std::string filename);

	//文件/文件夹 重命名 完整路径
	int RenameDirFile(std::string dir, std::string newdir);

	//文件/文件夹 重命名 组合路径
	int RenameDirFile(std::string fadir, std::string subdir, std::string newsubdir);

	//文件 删除 完整路径
	int _DeleteFile(std::string fullname);

	//文件 删除 组合路径
	int _DeleteFile(std::string dir, std::string filename);

	//文件夹 删除 完整路径
	int DeleteDir(std::string dir);

	//文件夹 删除 组合路径
	int DeleteDir(std::string dir, std::string subdir);

	//返回上一级的路径
	std::string GetFatherDir(std::string str);

	//返回上一级的路径的文件夹名
	std::string GetFatherDirName(std::string str);
}