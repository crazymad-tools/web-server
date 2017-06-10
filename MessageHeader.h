/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-06-08 19:36
 * Filename         : MessageHeader.h
 * Description      : 
 *********************************************************/

#ifndef _MESSAGEHEADER_H
#define _MESSAGEHEADER_H

#include <string>
#include <map>
#include "ContentType.h"

typedef std::string string;

class MessageHeader {
private:
	std::map<string, string> headers;	// 消息报头合集

public:
	int makeDate();						// 生成消息生成时间消息头
	int makeKeepAlive(int time, int max);	// 保持长连接的信息
	int makeConnection(bool statu);			// 生成连接状态的消息
	int makeContentType(int mode);				// 生成数据格式的消息
	int makeContentLength(int length);
	int makeServer(string ServerName);		// 生成服务器名字消息
	int getHeaders(char* data);
	int getSize();						// 获取消息报头的总字节长度
};

#endif

