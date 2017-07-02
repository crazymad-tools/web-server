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
#include <signal.h>
#include <sys/epoll.h>
#include <crazy/net/Socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <MessageHeader.h>
#include <MessageParse.h>
#include <FileReader.h>

using namespace std;

map<int, Socket*> fds;					// 现存的套接字与Socket的映射关系

void init_para(int argc, char* argv[]) {
	int pid = getpid();
	int fd = open("/root/.chttp.pid", O_RDWR);
	if (fd == -1) {
		fd = creat("/root/.chttp.pid", 0000);
		char spid[10] = "\0";
		sprintf(spid, "%d", pid);
		write(fd, spid, strlen(spid));
		close(fd);
	} else {
		if (argc == 3) {
			if (!strcmp(argv[1], "-s") && !strcmp(argv[2], "quit")) {				// 通过 "-s quit" 指令关闭现有chttp进程
				char buf[100] = "kill -9 ";
				read(fd, buf+strlen(buf), 10);
				close(fd);
				system(buf);
				remove("/root/.chttp.pid");
			}
		} else {
			printf("chttp is exsited!\n");
		}
		exit(0);
	}
}

void quit(int) {
	remove("/root/.chttp.pid");
	exit(0);
}

void init_signal() {					// 信号处理
	signal(SIGQUIT, quit);				// Ctrl+\  两者都是退出进程的信号
	signal(SIGINT, quit);				// Ctrl+C
}

void init_daemon() {
	int pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(1);
	} else if (pid != 0) {
		exit(0);
	} else {
		setsid();
		umask(0);
		close(0);
		close(1);
	}
}

void init(int argc, char* argv[]) {
	chdir("/var/www/html");				// 改变工作目录
	init_daemon();						// 守护进程设置
	init_para(argc, argv);				// 进程命令行参数读取
	init_signal();						// 进程信号捕捉设置
}

void epoll_add(int epfd, int fd) {
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_del(int epfd, int fd) {
	epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
}

void sendResponse(int fd) {
	Socket* client = fds[fd];
	char tmp[1000];
	int ret = client->Read(tmp);
	if (ret == 0) {
		client->Close();
		delete client;
		for (map<int, Socket*>::iterator it = fds.begin(); it != fds.end(); it++) {
			if (it->first == fd) {
				fds.erase(it);
				break;	
			}
		}
		return;
	}
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
	client->Write(buf2, strlen(statu)+header.getSize());
	client->Write(text, length);
}

int main(int argc, char* argv[]) {
	init(argc, argv);
	struct epoll_event events[101];
	int epfd = epoll_create(100);			// 最大并发量设置为100

	// 建立网络链接
	Socket server(8080);
	//Socket server(80);
	server.init();
	server.Bind();
	server.Listen(10);

	epoll_add(epfd, server.getFd());
	while (1) {
		memset(events, 0, sizeof events);
		int n = epoll_wait(epfd, events, 100, -1);
		if (n == -1) {
			perror("epoll_wait");
			exit(0);
		}
		for (int i = 0; i < n; i++) {
			if (events[i].data.fd == server.getFd()) {
				SocketData socketData;
				server.Accept(&socketData);
				Socket *client = new Socket(socketData);
				fds.insert(pair<int, Socket*>(client->getFd(), client));
				epoll_add(epfd, client->getFd());
			} else {
				sendResponse(events[i].data.fd);
			}
		}
	}

	return 0;
}

// 测试在网络连接、响应包头生成器
int main2(int argc, char* argv[]) {
	init_para(argc, argv);
	init_signal();
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
