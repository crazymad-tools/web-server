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
#include <FileReader.h>

using namespace std;

int main() {
	// 建立网络链接
	Socket server(8080);
	server.init();
	server.Bind();
	server.Listen(10);
	// 读取html文件
	FileReader reader("./index.html");
	reader.Read();
	char *text = (char*)reader.Get();
	int length = reader.getSize();
	//cout << text << endl << length << endl;
	// 服务器开始接受浏览器的访问
	while (1) {
		SocketData data;
		server.Accept(&data);
		Socket client(data);
		char tmp[1000];
		client.Read(tmp);
		memset(tmp, 0, sizeof tmp);

		MessageHeader header;
		header.makeDate();
		header.makeServer("CrazyMad/0.0.1 (CentOS)");
		header.makeContentLength(length);
		header.makeConnection(false);
		header.makeContentType(TEXTHTML);
		header.makeKeepAlive(5, 100);
		char buf[1000] = "";
		header.getHeaders(buf);
		char statu[100] = "HTTP/1.1 200 OK\r\n";
		char buf2[1000] = { 0 };
		strncat(buf2, statu, strlen(statu));
		strncat(buf2, buf, header.getSize());
		strncat(buf2, text, length);
		client.Write(buf2, strlen(statu)+length+header.getSize());
		client.Close();
		//cout << buf << endl;
	}

	return 0;	
}
