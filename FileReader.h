/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-06-10 15:33
 * Filename         : FileReader.h
 * Description      : 
 *********************************************************/

#ifndef _FILEREADER_H
#define _FILEREADER_H

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

class FileReader {
private:
	std::string fileName;		// 文件路径
	int fileSize;			// 读取出来的文件大小
	int fd;					// 打开文件用的文件描述符
	uint8_t* data;			// 文件数据
	struct stat stat_;		// 保存文件相关信息

public:
	FileReader(std::string fileName);		// 通过构造函数打开文件
	FileReader();
	~FileReader();/* {
		if (data) {
			delete data;
		}
	}*/
	int Open(std::string fileName);			// 通过成员方法打开文件
	int Read();						// 读取文件内容
	uint8_t* Get();						// 获取文件内容
	int getSize();						// 获取文件大小

};

#endif

