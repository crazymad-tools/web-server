/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-06-15 05:43
 * Filename         : MessageParse.cc
 * Description      : 
 *********************************************************/

#include <MessageParse.h>
#include <string.h>
#include <iostream>

// 当message开头不是CRLF时返回真值
#define notCRLF(message) (*(message) != '\r' || *(message+1) != '\n')		

using namespace std;

namespace detail {
int findNoDel(char* message, char del) {
	char* back = message;
	for (; notCRLF(message) && *message == del; message++) { };
	return static_cast<int>(message-back);
}
int split(char* message, char del/*delimiter*/) {
	char* back = message;
	for (; notCRLF(message) && *message == del; message++) { }
	for (; notCRLF(message) && *message != del; message++) { }
	return static_cast<int>(message-back);
}
int split(char* message/*, char del delimiter*/) {			// 寻找单元
	char* back = message;						// 原址备份
	return static_cast<int>(message-back);
}
// 获取单词
int getPiece(char* message, char* piece, char prefix, char suffix) {
	char* back = message;
	message += findNoDel(message, prefix);
	int ret = split(message, suffix);
	strncpy(piece, message, ret);
	piece[ret] = '\0';
	message += ret;
	return static_cast<int>(message-back);
}
int findNoSpace(char* message) {
	char* back = message;
	return static_cast<int>(message-back);
}
}

int MessageParse::getDest(char* url) {
	requestData.dest = url+1;
	if (requestData.dest == "") {
		requestData.dest = "index.html";
	}
	return 1;
}

char* MessageParse::getRequest(char* message) {
	char buf[100] = { 0 };
	message += detail::getPiece(message, buf, ' ', ' ');		// 获取上传方法
	cout << buf << ' ';
	requestData.type = buf;

	message += detail::getPiece(message, buf, ' ', ' ');
	getDest(buf);

	message += detail::getPiece(message, buf, ' ', ' ');
	cout << buf << endl;
	
	cout << "type: " << requestData.type << endl;
	cout << "dest: " << requestData.dest << endl;

	message += 2;			// 最后因为换行，指针后移
	return message;
}

int MessageParse::parse(char* message, int len) {
	char* back = message;
	getRequest(message);
	/*while (true) {
		while (*message != '\r' || *(message+1) != '\n') {
			//message += detail::findNoSpace(message);
			int ret = detail::getPiece(message);
			char piece[100] = { 0 };
			strncpy(piece, message, ret);
			cout << piece << ':';
			message += detail::split(message, ':');
			ret = detail::getPiece(message);
			memset(piece, 0, sizeof piece);
			strncpy(piece, message, ret);
			message++;
		}
		message += 2;
		if (*message == '\r' && *(message+1) == '\n') {
			break;
		}
	}*/
	message += 2;
	cout << endl << endl;
	return static_cast<int>(message-back);
}

void MessageParse::clear() {
	memset(&requestData, 0, sizeof requestData);
}



			/*char piece[100] = { 0 };	
			int len = detail::split(message);
			strncpy(piece, message, len);
			cout << piece << endl;
			message += (len);
			// 去除空白字符
			while ( (*(message) != '\r' && *(message+1) != '\n') &&
					(*(message) == ':' || isspace(*(message))) ) {
				message++;
			}*/		
