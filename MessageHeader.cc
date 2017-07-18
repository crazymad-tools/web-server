/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-06-08 19:48
 * Filename         : MessageHeader.cc
 * Description      : 
 *********************************************************/

#include "MessageHeader.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

using namespace std;

// 生成时间消息
int MessageHeader::makeDate() {
	string weeks[8] = {
		"", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
	};
	string months[13] = {
		"Jan", "Feb", "Mar", "Apr", "May", "May", "Jul", "Aug", "Sep", "Oct", "Nov", " Dec"
	};
	string header = "Date: ";
	string data = "";
	char tmp[100] = { 0 };
	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);			// 获取具体时间
	sprintf(tmp, "%s, %02d %s %d %02d:%02d:%02d GMT\r\n", 
			weeks[p->tm_wday].c_str(), p->tm_mday, months[p->tm_mon].c_str(), 
			p->tm_year+1900, p->tm_mon, p->tm_mday, 
			p->tm_hour, p->tm_min, p->tm_sec);
	data = tmp;
	headers.insert(pair<string, string>(header, data));
	return 1;
}
// 生成服务器保存长连接的时间信息
int MessageHeader::makeKeepAlive(int time, int max) {
	string header = "Keep-Alive: ";
	string data = "";
	char tmp[100] = { 0 };
	sprintf(tmp, "timeout=%d, max=%d\r\n", time, max);
	data = tmp;
	headers.insert(pair<string, string>(header, data));
	return 1;
}
// 生成链接状态的信息
int MessageHeader::makeConnection(bool statu) {
	string header = "Connection: ";
	string data = (statu ? "Keep-Alive" : "Close");
	data += "\r\n";
	headers.insert(pair<string, string>(header, data));
	return 1;
}
// 生成传输数据类型信息
int MessageHeader::makeContentType(int mode) {
	string header = "Content-Type: ";
	string data = "";
	switch (mode) {
	case TEXTHTML:
		data = "text/html; ";
		break;
	case TEXTXML:
		data = "text/xml; ";
		break;
	case IMAGEPNG:
		data = "image/png; ";
		break;
	case IMAGEGIF:
		data = "imgae/gif; ";
	};
	//data += "charset=UTF-8\r\n";
	headers.insert(pair<string, string>(header, data));
	return 1;
}
// 根据文件名字生成传输数据类型信息
int MessageHeader::makeContentType(string filename) {
	int dot = filename.rfind('.');
	string suffix = filename.c_str()+dot;
	int mode = 0;
	if (suffix == "png") {
		mode = 21;
	} else if (suffix == "gif") {
		mode = 22;
	} else if (suffix == "html") {
		mode = 11;
	}
	makeContentType(mode);
}

int MessageHeader::makeContentLength(int length) {
	string header = "Content-Length: ";
	string data = "";
	char tmp[100] = { 0 };
	sprintf(tmp, "%d\r\n", length);
	data = tmp;
	headers.insert(pair<string, string>(header, data));
	return 1;
}

int MessageHeader::makeServer(string ServerName) {
	string header = "Server: ";
	string data = ServerName;
	headers.insert(pair<string, string>(header, data));
	return 1;
}

int MessageHeader::getHeaders(char* data) {
	data[0] = '\0';
	for (map<string, string>::iterator it = headers.begin(); it != headers.end(); it++) {
		strncat(data, it->first.c_str(), it->first.size());
		strncat(data, it->second.c_str(), it->second.size());
	}
	strncat(data, "\r\n\r\n", 4);
	return 1;
}

int MessageHeader::getSize() {
	int res = 0;
	for (map<string, string>::iterator it = headers.begin(); it != headers.end(); it++) {
		//printf("%s%s\n", it->first.c_str(), it->second.c_str());
		res += it->first.size();
		res += it->second.size();
	}
	return res+4;
}
