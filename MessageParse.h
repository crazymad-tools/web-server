/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-06-15 05:43
 * Filename         : MessageParse.h
 * Description      : 
 *********************************************************/

#ifndef _MESSAGEPARSE_H
#define _MESSAGEPARSE_H

#include <string>

struct RequestData {
	std::string dest;				// 目标资源
	std::string data;				// 附加上传数据
	std::string type;					// 附加数据上传方法
	std::string HTMLtype;			// HTML协议版本
	bool keepAlive;				// 是否保持长连接
};

class MessageParse {
private:
	RequestData requestData;			// 请求报头数据储存单元
	char* getRequest(char* message);		// 获取请求行
	int getDest(char* url);

public:
	int parse(char* message, int len);		// 解析请求报头
	int saveData(char* message, int len);	// 储存附加数据
	void clear();
	const RequestData getRequestData() {
		return static_cast<const RequestData>(requestData);
	}
};

#endif

