#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <atlstr.h>
#include <windows.h>//sleep��
#include < math.h>//������
#include <iostream>//cin cout��
#include <string>//cin cout��
#include <io.h>//��ȡ�ļ���С_filelengthi64
using namespace std;//cin cout��

#include"stdafx.h"
#include<windows.h>
#include<wininet.h>
#include<iostream>
#include <time.h>//ʱ��
#include <conio.h>//ʱ��


/*
** SYSTEMTIMEתtime_t
*/
time_t systime_to_timet(const SYSTEMTIME& st);
/*
**time_tתSYSTEMTIME
*/
SYSTEMTIME Time_tToSystemTime(time_t t);
/*
**time_tתSYSTEMTIME
*/
SYSTEMTIME TimetToSystemTime(time_t t);
/*
**SYSTEMTIMEתtime_t
*/
time_t SystemTimeToTimet(SYSTEMTIME st);