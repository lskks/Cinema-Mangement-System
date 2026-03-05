/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: common.c			 
* Description : ͨ�ú�������	 
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��		 
*/

#include "common.h"

#include <stdio.h>
#include <fcntl.h>
#include <time.h>

//���ַ���str�͵�ת��Ϊ��д�ַ������������ַ���ͷָ��
char *Str2Upper(char *str) {
	if (NULL == str)
		return NULL;
	else {
		char *p = str;
		while ('\0' != *p) {
			if (*p >= 'a' && *p <= 'z')
				*p -= 32;
			p++;
		}
		return str;
	}
}

//���ַ���str�͵�ת��ΪСд�ַ������������ַ���ͷָ��
char *Str2Lower(char *str) {
	if (NULL == str)
		return NULL;
	else {
		char *p = str;
		while ('\0' != *p) {
			if (*p >= 'A' && *p <= 'Z')
				*p += 32;
			p++;
		}
		return str;
	}
}

//�Ƚ�����dt1, dt2�Ĵ�С����ȷ���0��dt1<dt2����-1������1
int DateCmp(user_date_t dt1, user_date_t dt2) {
	if (dt1.year < dt2.year)
		return -1;
	else if (dt1.year == dt2.year && dt1.month < dt2.month)
		return -1;
	else if (dt1.year == dt2.year && dt1.month == dt2.month && dt1.day < dt2.day)
		return -1;
	else if (dt1.year == dt2.year && dt1.month == dt2.month
			&& dt1.day == dt2.day)
		return 0;
	else
		return 1;
}

//��ȡϵͳ��ǰ����
user_date_t DateNow() {
	user_date_t curDate;
	time_t now;         //ʵ����time_t�ṹ
	struct tm *timeNow;         //ʵ����tm�ṹָ��
	time(&now);
	timeNow = localtime(&now);
	curDate.year=timeNow->tm_year+1900;
	curDate.month=timeNow->tm_mon+1;
	curDate.day=timeNow->tm_mday;

	return curDate;
}

//��ȡϵͳ��ǰʱ��
user_time_t TimeNow(){
	user_time_t curTime;
	time_t now;         //ʵ����time_t�ṹ
	struct tm *timeNow;         //ʵ����tm�ṹָ��
	time(&now);
	timeNow = localtime(&now);
	curTime.hour=timeNow->tm_hour;
	curTime.minute=timeNow->tm_min;
	curTime.second=timeNow->tm_sec;
	return curTime;
}

/*
 *
 *
 int ScanKeyboard() {
 int in;
 struct termios new_settings;
 struct termios stored_settings;
 tcgetattr(0, &stored_settings);
 new_settings = stored_settings;
 new_settings.c_lflag &= (~ICANON);
 new_settings.c_cc[VTIME] = 0;
 tcgetattr(0, &stored_settings);
 new_settings.c_cc[VMIN] = 1;
 tcsetattr(0, TCSANOW, &new_settings);

 in = getchar();

 tcsetattr(0, TCSANOW, &stored_settings);
 return in;
 }

 */


