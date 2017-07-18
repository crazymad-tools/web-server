/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-07-11 10:52
 * Filename         : Event.h
 * Description      : 
 *********************************************************/

#ifndef _EVENT_H
#define _EVENT_H

#include <functional>				// 函数指针类

class Event {
protected:
	int fd;								// 套接字
	std::function<void ()> cb;			// 回调函数（事件处理函数）
	
public:
	Event(int fd, std::function<void ()> cb);	
	int getFd();						// 获取套接字
};

class HttpEvent : public Event {
private:
	bool keepAlive;						// 是否保持长连接
	
public:

};

#endif

