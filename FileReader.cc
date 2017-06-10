/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-06-10 15:33
 * Filename         : FileReader.cc
 * Description      : 
 *********************************************************/

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <FileReader.h>

using namespace std;

FileReader::FileReader(string fileName) 
	: data(NULL), fd(-1)
{
	Open(fileName);
}

FileReader::FileReader() : data(NULL), fd(-1) { }

FileReader::~FileReader();/* {
	if (data) {
		delete data;
	}
}*/

int FileReader::Open(string fileName) {
	this->fileName = fileName;
	fd = open(fileName.c_str(), O_RDONLY);
	if (fstat(fd, &stat_) == -1) {
		perror("fstat error");
		return 0;
	}
	fileSize = stat_.st_size;
}

int FileReader::Read() {
	if (fd == -1) {
		return 0;
	}	
	data = new uint8_t[fileSize];
	read(fd, data, fileSize);
}

uint8_t* FileReader::Get() {
	return data;
}

int FileReader::getSize() {
	return fileSize;
}
