/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-06-08 19:35
 * Filename         : main.cc
 * Description      : 
 *********************************************************/

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <crazy/net/Socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <MessageHeader.h>
#include <MessageParse.h>
#include <FileReader.h>

using namespace std;

// 测试在网络连接、响应包头生成器
int main() {
	//cout << chroot("/var/www/html") << endl;;
	chdir("/var/www/html");
	// 建立网络链接
	Socket server(8080);
	//Socket server(80);
	server.init();
	server.Bind();
	server.Listen(10);
	// 开启事件循环
	while (1) {
		// 创建连接并读取数据
		SocketData data;
		server.Accept(&data);
		Socket client(data);
		char tmp[1000];
		client.Read(tmp);
		// 创建request报头解析器
		MessageParse parse;
		parse.parse(tmp, sizeof tmp);
		memset(tmp, 0, sizeof tmp);
		// 读取文件
		//cout << parse.getRequestData().dest << endl;
		FileReader reader(parse.getRequestData().dest.c_str());
		reader.Read();
		char *text = (char*)reader.Get();
		int length = reader.getSize();

		MessageHeader header;
		header.makeDate();
		header.makeServer("CrazyMad/0.0.1 (CentOS)");
		header.makeContentLength(length);
		header.makeConnection(false);
		header.makeContentType(parse.getRequestData().dest);
		header.makeKeepAlive(5, 100);
		char buf[1000] = "";
		header.getHeaders(buf);
		char statu[100] = "HTTP/1.1 200 OK\r\n";
		char buf2[1000] = { 0 };
		strncat(buf2, statu, strlen(statu));
		strncat(buf2, buf, header.getSize());
		//strncat(buf2, text, length);
		client.Write(buf2, strlen(statu)+header.getSize());
		client.Write(text, length);
		client.Close();
		//cout << buf << endl;
	}

	return 0;	
}
