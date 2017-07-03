/*********************************************************
 * Author           : crazy_mad
 * Last modified    : 2017-07-03 12:05
 * Filename         : chttp.h
 * Description      : 
 *********************************************************/

#ifndef _CHTTP_H
#define _CHTTP_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>

// chttp专用日志结构体
struct chp_log {
	char logName[20];			// 日志文件名字
	int fd;
};

void getTime(char* buffer) {
	//struct timeval tv;
	time_t tm_t;
	time(&tm_t);
	//gettimeofday(&tv, NULL);
	struct tm* tm_m = localtime(&tm_t);
	mktime(tm_m);
	sprintf(buffer, "%d-%d-%d %02d:%02d:%02d ", 
			1900+tm_m->tm_year, 1+tm_m->tm_mon, tm_m->tm_mday,
			tm_m->tm_hour, tm_m->tm_min, tm_m->tm_sec);
}

void log_out(struct chp_log log, const char* format, ...) {
	char buffer[256] = "\0";
	getTime(buffer);
	va_list args;
	va_start(args, format);
	vsprintf(buffer+strlen(buffer), format, args);
	buffer[strlen(buffer)] = '\n';
	int ret = write(log.fd, buffer, strlen(buffer));
}

void init_log(struct chp_log* log, char* logName) {
	strcpy(log->logName, logName);
	log->fd = open(logName, O_RDWR | O_APPEND | O_CREAT, 0666);
	if (log->fd < 0) {
		log->fd = creat(logName, 0000);
	}
}

#endif

