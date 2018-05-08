#pragma once

//#include <afx.h>
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
using namespace std;//cin cout��
typedef map<CString, CString> UDT_MAP_CSTRING_CSTRING;
typedef map<int, CString> UDT_MAP_INT_CSTRING;
typedef struct SYMBOLS//���н��׶Լ����� ȫ����ΪCS ��Ҫ����ĳ��ʱ��ת�� �ɽ��ʹ���
{
	CString base_currency;//��������
	CString quote_currency;//�Ƽ۱���
	CString price_precision;	//�۸񾫶ȣ�0Ϊ��λ��
	CString amount_precision;	//�������ȣ�0Ϊ��λ��
	CString symbol_partition;//��������main������innovation��������bifurcation�ֲ���
};
typedef map<CString, SYMBOLS> UDT_MAP_CSTRING_SYMBOLS;
typedef struct MARKET_DEPTH//�г��ҵ��۸� ����
{
	 double price;
	 double vol;
};
typedef map<int, MARKET_DEPTH> UDT_MAP_INT_MARKET_DEPTH;
typedef struct MARKET_HISTORY//market/history/trade ��ʷ�ɽ���¼
{
	CString amount;//�ɽ���
	CString price;//�ɽ��۸�
	CString direction;//��������
	CString ts;//ʱ�� ��λms
};
typedef map<int, MARKET_HISTORY> UDT_MAP_INT_MARKET_HISTORY;


typedef map<CString, UDT_MAP_CSTRING_CSTRING> UDT_MAP_CSTRING_MAPCSCS;
class huobiAPI
{
public:
	huobiAPI();
	~huobiAPI();
	CString Secret_Key;//API ������Կ(Secret Key) 
	CString Access_Key;//API ��Կ(Access Key)
	UDT_MAP_CSTRING_CSTRING balanceMap;//�ұ��˻���ID���ұ� account_id	
	UDT_MAP_CSTRING_CSTRING currencysMap;//�г�֧�ֱ���
	UDT_MAP_CSTRING_SYMBOLS symbolsMap;//���н��׶Լ�����
	UDT_MAP_INT_MARKET_HISTORY historyMap;//�г�����ɽ������ۣ�����ʱ��
	UDT_MAP_CSTRING_MAPCSCS marginMap;//����˻� ���׶� margin�ṹ��
	/*******************************************************************************/
	/* �����ҵ�Map ֻ��������Ч ����ֻ��һ�����֣���IP����²����Ի��ȫ���������� */
	/*******************************************************************************/
	UDT_MAP_INT_MARKET_DEPTH buyvpMap;//�������̹ҵ�
	UDT_MAP_INT_MARKET_DEPTH sellvpMap;//�������̹ҵ�
public:
	/************************************************************************/
	/* 					GET								                    */
	/************************************************************************/
	CString Get_id();//��ȡid
	CString Get_balance_matchresults_order(CString sub, CString order_id);//����˻���� �򶩵����� �򶩵��ɽ���ϸ
	BOOL Get_currencys();//��ѯϵͳ����֧�ֵı���
	BOOL Get_symbols();//���н��׶Լ�����
	BOOL Get_market_depth(CString symbol, CString type, int buy_depth, int sell_depth);//�г��ҵ��� Ҫ�鿴���������buyvpMap ���������sellvpMap
	BOOL Get_market_history(CString symbol, int mun);//�г�����ɽ������ۣ�����ʱ��
	//CString Get_ordermsg(CString order_id);//order-id��ѯ��������  ,   order-id+"/matchresults" ��ѯĳ�������ĳɽ���ϸ  ����ԭʼ����
	BOOL Get_margin_balance(CString symbol);//����˻����� �������׶�xxxusdt

	CString Get_·���޲���ͨ��(CString phat, CString *instr, int mun);
	/************************************************************************/
	/* 					POST							                    */
	/************************************************************************/
	CString Post_place(CString symbol, CString price, CString mun, CString source, CString type);//�ҵ��� ���� �۸� ���� ����� ���ͣ�//���ض�����
	CString Post_transfer_inout_apply(CString phat, CString symbol, CString currency, CString start);//�ֻ������˺ŵ�ת��ת��
	BOOL Post_submitcancel(CString order_id);//����
	BOOL Post_batchcancel(CString *order_id, int mun);//��������������1���������� ����2����Ԫ�ظ���
	BOOL Post_margin_repay(CString order_id, CString amount);//�黹���
	CString Post_��д·��ͨ��(CString phat, CString *instr, int mun);
private:
	CString Get_Sendmsg(CString* intmp, int mun, CString Sub, CString GetOfPost);//���� ������Կ ����ƴ�Ӻ�ĵ�ַ
	CString Get_Times();//��ȡ��Ҹ�ʽ�ı���UTCʱ��
	CString find_str(CString inCStr, const char* exptxt);//������ʽ���������ص�һ��
	void writeLog(CString msg, CString fxname);
	
};