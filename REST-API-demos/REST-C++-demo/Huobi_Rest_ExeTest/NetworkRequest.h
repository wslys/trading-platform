#pragma once
#include "stdafx.h"
#include <stdio.h>  
#include <iostream>  
#include <fstream>  
//#include "json.h"  
#include "boost/asio.hpp"  
#include <boost/lexical_cast.hpp>   
#include <boost/regex.hpp>

#include"stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <atlstr.h>
#include <windows.h>//sleep��
#include < math.h>//������
#include <iostream>//cin cout��
#include <string>//cin cout��
#include <io.h>//��ȡ�ļ���С_filelengthi64

#include<windows.h>
#include<wininet.h>
#include<iostream>
#include <time.h>//ʱ��
#include <conio.h>//ʱ��

#include <metahost.h>
#include <fstream>

#include "hmac-sha256.h"
#include "base64.h" 
#include "my_URL.h"
#include "time2xtime.h"
#pragma comment(lib,"wininet.lib")
using namespace std;




/// POST����  
CString PostRequest(CString CShost, CString path, CString form);
/// GET����  
CString GetRequest(CString sendmsg);//ͨ����ַGET��ҳ

