// Huobi_Rest_ExeTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//#include <afx.h>
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

#include <metahost.h>
#include <fstream>

#include "hmac-sha256.h"
#include "base64.h" 
#include "my_URL.h"
#include "time2xtime.h"
#include "huobiAPI.h"
#pragma comment(lib,"wininet.lib")
//#include "HttpConnect.h"
//typedef map<CString, CString> UDT_MAP_CSTRING_CSTRING;
//#using "..\Debug\ConsoleApplication.dll"
//using namespace Huobi_Market_WebSocketAPI;
//using namespace std;
//CString Secret_Key;//API ������Կ(Secret Key) 
//CString Access_Key;//API ��Կ(Access Key)

CString GetIpByDomainName(char *szHost, char* szIp)//ȡIP������
{
	WSADATA        wsaData;

	HOSTENT   *pHostEnt;
	int             nAdapter = 0;
	struct       sockaddr_in   sAddr;
	if (WSAStartup(0x0101, &wsaData))
	{
		printf(" gethostbyname error for host:\n");
		return "err";
	}

	pHostEnt = gethostbyname(szHost);
	if (pHostEnt)
	{
		if (pHostEnt->h_addr_list[nAdapter])
		{
			memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);
			sprintf(szIp, "%s", inet_ntoa(sAddr.sin_addr));
		}
	}
	else
	{
		//      DWORD  dwError = GetLastError();
		//      CString  csError;
		//      csError.Format("%d", dwError);
	}
	WSACleanup();
	CString str1(szIp);
	return str1;
}


BOOL ExecDosCmd(char *get_websocket)
{
	SECURITY_ATTRIBUTES   sa;
	HANDLE   hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return   FALSE;
	}

	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//�ؼ����裬CreateProcess�����������������MSDN   
	if (!CreateProcess("D:\\vs2013cpp\\Huobi_Rest_ExeTest\\Huobi_Rest_ExeTest\\ConsoleApplication.exe", get_websocket
		, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		return   FALSE;
	}
	CloseHandle(hWrite);

	char   buffer[4096] = { 0 };
	DWORD   bytesRead;
	ofstream outfile("log.txt");

	while (true)
	{
		if (ReadFile(hRead, buffer, 1, &bytesRead, NULL) == NULL)
			break;
		//buffer�о���ִ�еĽ�������Ա��浽�ı���Ҳ����ֱ�����   
		//printf(buffer);   
		outfile << buffer /*<< endl*/;
		if (buffer=="}")
		{
			outfile << endl;
		}
		//Sleep(200);
	}

	outfile.close();

	return   TRUE;
}
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	SYSTEMTIME st = { 0 };
	//GetSystemTime(&st);//UTCʱ��
	st.wYear = 2017;
	st.wMonth = 12;
	st.wDay = 13;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;

	time_t seconds;
	//time(&seconds);

	seconds= systime_to_timet(st);

	char *get_websocket = " market.btcusdt.kline.1day id1 1513094400 1514649600";
	ExecDosCmd(get_websocket);
	return TRUE;
}
int main()
{
//	SYSTEMTIME st = { 0 };
//	//GetSystemTime(&st);//UTCʱ��
//	st.wYear = 2017;
//	st.wMonth = 12;
//	st.wDay = 13;
//	st.wHour = 0;
//	st.wMinute = 0;
//	st.wSecond = 0;
//	//st.wMilliseconds = 0;
//	SYSTEMTIME st1 = { 0 };
//	//GetSystemTime(&st);//UTCʱ��
//	st1.wYear = 2017;
//	st1.wMonth = 12;
//	st1.wDay = 31;
//	st1.wHour = 0;
//	st1.wMinute = 0;
//	st1.wSecond = 0;
//	//st1.wMilliseconds = 0;
//	time_t seconds;
//	time_t seconds1;
//	//time(&seconds);
//
//	seconds = systime_to_timet(st);//�뼶  *1000+ms ���Ǻ��뼶��
//	seconds1 = systime_to_timet(st1);
//	//websocketȡ��Ϣ
//
//	LPVOID lpParam;
//	DWORD dwThreadId[3];
//	HANDLE hThread[3];
//	hThread[0]=CreateThread(NULL,0, ThreadProc,lpParam,0, &dwThreadId[0]);
//	BOOL a = CloseHandle(hThread[0]);//�����ں˶��� ����ֹͣ�߳� ʧ�ܷ���0
//	//ExitThread(dwThreadId[0]);//��ȫ��
//	//BOOL b=TerminateThread(hThread,0);//�ⲿֹͣ�������߳�(���Ƽ� ����ȫ) ʧ�ܷ���0
//	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
//	/*   0 = ��ɫ      8 = ��ɫ
//	   1 = ��ɫ      9 = ����ɫ
//	   2 = ��ɫ      A = ����ɫ
//	   3 = ǳ��ɫ    B = ��ǳ��ɫ
//	   4 = ��ɫ      C = ����ɫ
//	   5 = ��ɫ      D = ����ɫ
//	   6 = ��ɫ      E = ����ɫ
//	   7 = ��ɫ      F = ����ɫ*/
//	SetConsoleTextAttribute(handle,	0x0A);
//	while (1)
//	{
//		cout<<"yes" << endl;
//		Sleep(1000);
//	}
	//char szHost[512] = "api.huobi.pro";
	//char* szIp=new char[125];
	//GetIpByDomainName(szHost, szIp);
	
	huobiAPI god;
	//god.Get_order_orders("hsrbtc", "5", "0.01", "api", "sell-limit");
	god.Secret_Key = "xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxx";//API ������Կ(Secret Key) 
	god.Access_Key = "xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxx";//API Access Key

	CString instr[6] = { "symbol", "=btcusdt","currency","=usdt", "states", "=canceled" };//��ѯһ��states���� ��ѯ������","�ָ��ǩ������
	god.Get_·���޲���ͨ��("/v1/margin/loan-orders",instr, 6);
	god.Get_id();//����ܶ�����õ� �����Ȼ�ȡ
	CString tmp = god.Post_place("hsrbtc", "5", "0.01", "api", "sell-limit");
	//�����ظ� ����god.Get_ordermsg(tmp);//��ѯ��������
	god.Post_transfer_inout_apply("/v1/dw/transfer-in/margin", "hsrusdt", "hsr", "1");//ת��ת��������
	god.Get_margin_balance("hsrusdt");//��ѯ����˻��ʲ����
	god.Post_submitcancel(tmp);//����
	int j=god.balanceMap.size();//���б���//ID�� account_id =CSֵ
	god.Get_balance_matchresults_order("���","");//���б������balanceMap  ��ѯ������ϸ��������Ҫ����2��д������
	god.Get_currencys();//�ɽ��ױ������currencysMap
	god.currencysMap;//�ɽ��ױ���
	god.Get_symbols();//���н��׶Լ��������symbolsMap������SYMBOLS�ṹ��
	god.symbolsMap;//���׶Լ�����

	int i = 0;
	CString order_id[50] = { "0" };
	for (;i<40;i++)
	{
		order_id[i]=god.Post_place("hsrbtc", "5", "0.01", "api", "sell-limit");//���һ��3�� ������ �ɿ������߳�дMap���̶߳�ȡMap
	}
	god.Post_batchcancel(order_id, 40);//��������
	god.Get_ordermsg("594548831");//��ѯ��������
	god.Post_submitcancel("594548831");//����
	
	god.Get_market_history("btcusdt", 200);
	god.Get_market_depth("btcusdt", "step0",50,50);//�г��ҵ��� ��1-150 ��1-150



	////��������õ����ʹ�
	//SYSTEMTIME st = { 0 };
	//GetSystemTime(&st);//UTCʱ��
	//CString Timestamp[2];
	//Timestamp[0] = "Timestamp";
	//Timestamp[1].Format(_T("=%d-%02d-%02dT%02d%%3A%02d%%3A%02d"),
	//	st.wYear,
	//	st.wMonth,
	//	st.wDay,
	//	st.wHour,
	//	st.wMinute,
	//	st.wSecond);
	//CString Q[8] = { "AccessKeyId", "="+ god.Access_Key, "SignatureMethod", "=HmacSHA256","SignatureVersion", "=2",
	//	Timestamp[0],Timestamp[1] };
	//CString sendmsg = Get_Sendmsg(Q, 8,"/v1/account/accounts");
	//DWORD dwTime = clock();//�����ʱ
	//CString msg = Get_Httpmsg(sendmsg);

	//DWORD dwTime1 = clock() - dwTime;//�����ʱ
	//if (msg!= "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!")//���ͳɹ�
	//{
	//	cout <<   msg<< endl;
	//}
	//����
    return 0;
}

