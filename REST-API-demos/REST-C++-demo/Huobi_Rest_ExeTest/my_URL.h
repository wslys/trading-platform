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
char dec2hexChar(short int n);
short int hexChar2dec(char c);
string escapeURL(const string &URL);
string deescapeURL(const string &URL);
string string_To_UTF8(const std::string & str);

