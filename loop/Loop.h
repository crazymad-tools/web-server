/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-07-11 11:07
 * Filename         : Loop.h
 * Description      : 
 *********************************************************/

#ifndef _LOOP_H
#define _LOOP_H

#include <Event.h>
#include <Epoll.h>

class Loop {
private:
	Epoll epoll;

public:
	void start();

};

#endif

