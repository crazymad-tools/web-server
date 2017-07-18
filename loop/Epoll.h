/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-07-11 10:52
 * Filename         : Epoll.h
 * Description      : 
 *********************************************************/

#ifndef _EPOLL_H
#define _EPOLL_H

#include <Event.h>
#include <map>

class Epoll {
private:
	int epfd;				// Epoll本体套接字
	std::map<int, Event*> eventList;		// 事件列表
	
public:
	Epoll();
	void add(Event* ev);			// 添加监听事件
	void del(int fd);				// 删除监听事件
	void get(void );				// 从监听器中获取监听到的事件

};

#endif

