#include "stdafx.h"
#include "huobiAPI.h"
//#include "HttpConnect.h"
#include <boost/lexical_cast.hpp>   
#include <boost/regex.hpp>
#include <boost/asio.hpp>//post��
using namespace std;//cin cout��
using namespace boost;

#include "NetworkRequest.h"  

huobiAPI::huobiAPI()
{

}

huobiAPI::~huobiAPI()
{
	balanceMap.clear();
}
CString huobiAPI::Get_Times()
{
	SYSTEMTIME st = { 0 };
	GetSystemTime(&st);//UTCʱ��
	CString Timestamp;
	Timestamp.Format(_T("%d-%02d-%02dT%02d%%3A%02d%%3A%02d"),
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond);
	return Timestamp;
}
//����������ǩ��get�õ�ַ post����Ҫ��ɾ����"/v1"
//����1��������{ "AccessKeyId","=xxxxx-xxxx-xxxxx-xx","SignatureMethod","=HmacSHA256"..... }������2����,����3API��ַ
CString huobiAPI::Get_Sendmsg(CString* intmp, int mun, CString Sub,CString GetOfPost)
{
	//GetOfPost = "GET"/"POST";
	CString Url = "api.huobi.pro";
	CString *tmp = new CString[mun + 2];
	for (int i = 0; i < mun; i++)
	{
		tmp[i] = intmp[i];
	}
	for (int i = 0; i < mun; i += 2)
	{
		if (tmp[i] > tmp[i + 2])
		{
			tmp[mun] = tmp[i];
			tmp[mun + 1] = tmp[i + 1];
			tmp[i] = tmp[i + 2];
			tmp[i + 1] = tmp[i + 3];
			tmp[i + 2] = tmp[mun];
			tmp[i + 3] = tmp[mun + 1];
		}
	}
	CString Q = GetOfPost + "\n" + Url + "\n" + Sub + "\n";
	Q += tmp[0] + tmp[1];
	for (int i = 2; i < mun; i += 2)
	{
		Q += "&";
		Q += tmp[i];
		Q += tmp[i + 1];
	}
	unsigned char* ucVar = (unsigned char*)(char*)(LPCTSTR)Q;
	unsigned char* ucVar1 = (unsigned char*)(char*)(LPCTSTR)Secret_Key;
	hmac_sha256 hmac;
	hmac_sha256_initialize(&hmac, ucVar1, 32);
	hmac_sha256_update(&hmac, ucVar, Q.GetLength());
	hmac_sha256_finalize(&hmac, NULL, 0);

	CString str;
	string str1 = "";
	for (int i = 0; i < 32; ++i) {
		str.Format("%02lx", (unsigned long)hmac.digest[i]);
		str1 += str;
	}
	CString  sendmsg = "https://";
	sendmsg = sendmsg + Url + Sub + "?";//��ַ+����+?
	CString Q1 = intmp[0] + intmp[1];
	for (int i = 2; i < mun; i += 2)
	{
		Q1 += "&";
		Q1 += intmp[i];
		Q1 += intmp[i + 1];
	}
	sendmsg += Q1;//����
	sendmsg += "&Signature=";
	//string encoded = base64_encode(hmac.digest, 32);//base64����
	sendmsg += escapeURL(base64_encode(hmac.digest, 32)/*base64����*/)/*URL����*/.c_str();
	return sendmsg;
}
CString huobiAPI::find_str(CString inCStr, const char* exptxt)
{
	string str(inCStr.GetBuffer(inCStr.GetLength()));
	regex expression(exptxt);
	smatch what;
	string::const_iterator start = str.begin();
	string::const_iterator end = str.end();
	if (boost::regex_search(start, end, what, expression))
	{
		//cout << what[0];
		str = what[0];
	}
	else {
		return "-1";
	}
	return str.c_str();
}

void huobiAPI::writeLog(CString msg, CString fxname)
{
	ofstream outfile;
	outfile.open("err_log.txt", ios::app);
	if (!outfile) //����ļ��Ƿ�������//�������ڼ���ļ��Ƿ����
	{
		MessageBox(NULL, msg, "err_log.txt��ʧ�ܣ�", MB_OK);
		abort(); //��ʧ�ܣ���������
	}
	else
	{
		outfile << Get_Times() << "\t" << "huobiAPI::" << fxname << "\t" << msg << endl;
		outfile.close();
	}
}

/************************************************************************/
/* ��ȡ�ұҽ����˻�ID����balanceMap										*/
/************************************************************************/
CString huobiAPI::Get_id()
{
	CString Q[8] = { "AccessKeyId", "=" + Access_Key, 
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	CString sendmsg = Get_Sendmsg(Q, 8, "/v1/account/accounts", "GET");
	CString msg = GetRequest(sendmsg);
	if (msg != "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!"&&-1==msg.Find("error"))//���ͳɹ�
	{
		CString sstr = find_str(msg, "[0-9]+");
		balanceMap.insert(make_pair("account_id", sstr));
		return sstr;
	}
	writeLog(msg, "Get_id()");
	return msg;
}
/************************************************************************/
/*��ѯ�˻�������balanceMap											*/
/*balanceMap<����_trade><����> //����ʹ��CString �޾�����ʧ			    */
/*����ֵ���ɹ�TRUE ʧ��FALSE��д��err_log.txt                           */
/************************************************************************/
CString huobiAPI::Get_balance_matchresults_order(CString sub,CString order_id)
{
	CString phat;
	if (sub=="���")
	{
		phat = "/v1/account/accounts/" + balanceMap.find("account_id")->second + "/balance";
	}
	else if(sub == "�����ɽ�����"){
		phat = "/v1/order/orders/"+ order_id +"/matchresults";
	}
	else if (sub == "��������") {
		phat = "/v1/order/orders/" + order_id;
	}
	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	CString sendmsg = Get_Sendmsg(Q, 8, phat, "GET");
	CString msg = GetRequest(sendmsg);
	if (msg != "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!"&&-1 == msg.Find("error"))//���ͳɹ�
	{
		if (sub == "balance")
		{
		msg.Delete(0, msg.Find("[", 0) + 1);			//�� { �������ţ���һ��ɾ��
		//msg.Delete(msg.Find("]", 0)+1, msg.GetLength());	//�� ] �������ţ���ɾ������
		CString  balance, temp;
		int i = 0;
		int munstar = 0;
		int munend = 0;
		//DWORD dwTime = clock();//�����ʱ
		msg.Replace("\"", "");
		msg.Replace("{currency:", "");
		msg.Replace(",type:", "_");
		msg.Replace("balance:", "");
		msg.Replace("}}", "");
		while(1)
		{
			i = msg.Find("}", 0);
			if (i>=0)
			{
				munstar=msg.Find(",", 0);
				temp = msg.Left(munstar);
				msg.Delete(0, munstar+1);
				munend = msg.Find("}", 0);
				balanceMap.insert(make_pair(temp, msg.Left(munend)));
				msg.Delete(0, munend +2);
			}
			else
			{
				break;
			}
		}
		}
		//DWORD dwTime1 = clock() - dwTime;//�����ʱ
		return msg;
	}
	writeLog(msg,"Get_balance(sub:"+ sub+",order_id:"+ order_id+")");
	return msg;
}
/************************************************************************/
/*��ѯ����ϵͳ֧�ֽ��׵ı��ִ���currencysMap	                        */
/*currencysMap<����><����_trade> //_trade����Ϊ���balanceMap��������   */
/*����ֵ���ɹ�TRUE ʧ��FALSE��д��err_log.txt                           */
/************************************************************************/
BOOL huobiAPI::Get_currencys()
{
	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	CString sendmsg = Get_Sendmsg(Q, 8, "/v1/common/currencys", "GET");
	CString msg = GetRequest(sendmsg);
	CString rmsg = msg;
	CString rrmsg = msg;
	int i = 0;
		int tmpi=0;
	if (msg != "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!"&&-1 == msg.Find("error"))//���ͳɹ�
	{
		msg.Delete(0, msg.Find("[", 0) + 1);			//�� { �������ţ���һ��ɾ��
		//DWORD dwTime = clock();//�����ʱ
		while (1)
		{
			i = msg.Find(",", 0);
			if (i > 0)
			{
				msg.Delete(0, i + 1);
			}
			else
			{
				break;
			}
		}
		CString temp;
		rmsg.Replace("{\"status\":\"ok\",\"data\":[\"", "");
		rmsg.Replace("\"]}", "");
		rmsg.Replace("\",\"", ",");
		while (1)
		{
			i = rmsg.Find(",", 0);
			if (i > 0)
			{
				currencysMap.insert(make_pair(rmsg.Left(i),rmsg.Left(i)+"_trade"));
				rmsg.Delete(0, i + 1);
			}
			else
			{
				break;
			}
			//tmpi++;
		}
		//DWORD dwTime1 = clock() - dwTime;//�����ʱ
		return TRUE;
	}
	writeLog(rmsg, "Get_currencys()");
	return FALSE;
}
/**********************************************************************************/
/*��ȡ���н��׶Լ��������symbolsMap������SYMBOLS�ṹ��					          */
/*symbolsMap<���׶�><SYMBOLS�ṹ��>												  */
/*(symbolsMap.find(symbol)->second).find("price_precision")->second				  */
/*SYMBOLS�ṹ			  												          */
/*CString base_currency;//��������										          */
/*CString quote_currency;//�Ƽ۱���										          */
/*CString price_precision;	//�۸񾫶ȣ�0Ϊ��λ��						          */
/*CString amount_precision;	//�������ȣ�0Ϊ��λ��						          */
/*CString symbol_partition;//��������main������innovation��������bifurcation�ֲ���*/
/*����ֵ���ɹ�TRUE ʧ��FALSE��д��err_log.txt								      */
/**********************************************************************************/
BOOL huobiAPI::Get_symbols()
{
	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	CString sendmsg = Get_Sendmsg(Q, 8, "/v1/common/symbols", "GET");
	CString msg = GetRequest(sendmsg);
	int i = 0;
	if (msg != "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!"&&-1 == msg.Find("error"))//���ͳɹ�
	{
		msg.Delete(0, msg.Find("[", 0) + 2);			//�� { �������ţ���һ��ɾ��
		msg.Delete(msg.ReverseFind(']'), msg.GetLength());	//�� ] �������ţ���һ��ɾ��
		CString  balance, temp;
		int i = 0;
		//DWORD dwTime = clock();//�����ʱ
		msg.Replace("\"", "");
		msg.Replace("base-currency:", "");
		msg.Replace("quote-currency:", "");
		msg.Replace("price-precision:", "");
		msg.Replace("amount-precision:", "");
		msg.Replace("symbol-partition:", "");
		msg.Replace(",{", "");
		msg.Replace("}", ",");

			string str(msg.GetBuffer(msg.GetLength()));
			boost::regex expression("[0-9]+");
			boost::smatch what;
			std::string::const_iterator start = str.begin();
			std::string::const_iterator end = str.end();
			string tmpstr;
		while (1)
		{
			i = msg.Find(",", 0);
			SYMBOLS tmp;
			int mun;
			if (i > 0)
			{
				mun = msg.Find(",", 0);
				tmp.base_currency = msg.Left(mun);
				msg.Delete(0, mun + 1);

				mun = msg.Find(",", 0);
				tmp.quote_currency = msg.Left(mun);
				msg.Delete(0, mun + 1);

				mun = msg.Find(",", 0);
				tmp.price_precision = msg.Left(mun);
				msg.Delete(0, mun + 1);
				mun = msg.Find(",", 0);
				tmp.amount_precision = msg.Left(mun);
				msg.Delete(0, mun + 1);
				mun = msg.Find(",", 0);
				tmp.symbol_partition = msg.Left(mun);
				msg.Delete(0, mun + 1);
				symbolsMap.insert(make_pair(tmp.base_currency+ tmp.quote_currency, tmp));
			}
			else
			{
				break;
			}
		}
		return TRUE;
	}
	writeLog(msg, "Get_symbols()");

	return FALSE;
}
/*************************************************************************************/
/*���������ȴ���buyvpMap ������ȴ���sellvpMap                                     */
/*����1�����׶�                                                                      */
/*����2��step0, step1, step2, step3, step4, step5���ϲ����0-5����step0ʱ�����ϲ����*/
/*����3��Ҫ��ȡ���������� ���ּ�����buy_depth��                                      */
/*����4��Ҫ��ȡ���������� ���ּ�����sell_depth��                                     */
/*����ֵ���ɹ�TRUE ʧ��FALSE��д��err_log.txt										 */
/*************************************************************************************/
BOOL huobiAPI::Get_market_depth(CString symbol,CString type,int buy_depth,int sell_depth)
{
	CString Q[12] = { "AccessKeyId", "=" + Access_Key,
		"symbol","="+symbol,"type","="+ type,//������
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times()  };
	CString sendmsg = Get_Sendmsg(Q, 12, "/market/depth", "GET");
	CString msg = GetRequest(sendmsg);
	if (msg != "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!"&&-1 == msg.Find("error"))//���ͳɹ�
	{
		msg.Delete(0, msg.Find("[", 0) + 2);			//�� { �������ţ���һ��ɾ��
		msg.Replace("[", "");
		msg.Replace("]", "");
		CString bids, asks;
		int tmpmun=msg.Find("\"asks\"", 0);
		bids = msg.Left(tmpmun);
		msg.Delete(0, tmpmun + 7);
		msg.Delete(msg.Find("\"ts\":", 0), msg.GetLength());
		asks = msg;
		MARKET_DEPTH tmp;
		int mun = 0;
		for (int i=0;i<buy_depth*2;i+=2)
		{
			tmpmun = bids.Find(",", 0);
			tmp.price= atof(bids.Left(tmpmun));
			bids.Delete(0, tmpmun + 1);
			tmpmun = bids.Find(",", 0);
			tmp.vol = atof(bids.Left(tmpmun));
			bids.Delete(0, tmpmun + 1);
			buyvpMap.insert(make_pair(mun, tmp));
			mun++;
		}
		mun = 0;
		for (int i = 0; i < sell_depth * 2; i += 2)
		{
			tmpmun = asks.Find(",", 0);
			tmp.price = atof(asks.Left(tmpmun));
			asks.Delete(0, tmpmun + 1);
			tmpmun = asks.Find(",", 0);
			tmp.vol = atof(asks.Left(tmpmun));
			asks.Delete(0, tmpmun + 1);
			sellvpMap.insert(make_pair(mun, tmp));
			mun++;
		}
		return TRUE;
	}
	CString strtemp;
	strtemp.Format("%d", buy_depth);
	CString strtemp1;
	strtemp1.Format("%d", sell_depth);
	writeLog(msg, "Get_market_depth(symbol:"+ symbol+",type:"+  type + ",buy_depth:" + strtemp + ",sell_depth:" + strtemp1+")");
	return FALSE;
}
/************************************************************************/
/*����г�����ɽ���Ϣ����historyMap                                    */
/*����1�����׶�                                                         */
/*����2��Ҫ��õ���Ϣ����                                               */
/*����ֵ���ɹ�TRUE ʧ��FALSE��д��err_log.txt                           */
/************************************************************************/
BOOL huobiAPI::Get_market_history(CString symbol, int mun)
{
	CString strtemp;
	strtemp.Format("%d", mun);
	CString Q[12] = { "AccessKeyId", "=" + Access_Key,
		"symbol","=" + symbol,"size","=" + strtemp,//������
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	CString sendmsg = Get_Sendmsg(Q, 12, "/market/history/trade", "GET");
	CString msg = GetRequest(sendmsg);
	if (msg != "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!"&&-1 == msg.Find("error"))//���ͳɹ�
	{
		UDT_MAP_INT_MARKET_HISTORY historyMaptmp;
		MARKET_HISTORY market_history_tmp;
		string str(msg.GetBuffer(msg.GetLength()));
		boost::regex expression("[0-9]+[/.]+[0-9]+");
		boost::smatch what;
		std::string::const_iterator start = str.begin();
		std::string::const_iterator end = str.end();
		string strtmp;
		int i = 0;
		while (boost::regex_search(start, end, what, expression))
		{
			if (i==mun)
			{
				break;
			}
			//���ɽ���
			strtmp=what[0];
			market_history_tmp.amount = strtmp.c_str();
			start = what[0].second;
			//���ɽ���
			boost::regex_search(start, end, what, expression);
			strtmp = what[0];
			market_history_tmp.price = strtmp.c_str();
			start = what[0].second;
			//���ɽ�����
			expression = "\"direction\":\"[a-z]+\"";
			boost::regex_search(start, end, what, expression);
			strtmp = what[0];
			market_history_tmp.direction = strtmp.c_str();
			start = what[0].second;
			//���ɽ�ʱ��
			expression = "[0-9]+";
			boost::regex_search(start, end, what, expression);
			strtmp = what[0];
			market_history_tmp.ts = strtmp.c_str();
			start = what[0].second;

			expression = "[0-9]+[/.]+[0-9]+";
			historyMaptmp.insert(make_pair(i, market_history_tmp));
			i++;
		}
		historyMap = historyMaptmp;
		return TRUE;
	}

	CString strtemp1;
	strtemp1.Format("%d", mun);
	writeLog(msg, "Get_market_history(symbol:" + symbol + ",mun:" + strtemp1 + ")");
	return FALSE;
}
/************************************************************************/
/*�¶���													            */
/*����1�����׶�                                                         */
/*����2������                                                           */
/*����3������                                                           */
/*����4��apiΪ�ұҽ����˻� margin-apiΪ����˻�	                        */
/*����5��buy-market���м���,                                            */
/*       sell-market���м���,                                           */
/*       buy-limit���޼���,                                             */
/*       sell-limit���޼���                                             */
/*����ֵ���ɹ����ض����� ʧ�ܷ��ط�����������ԭʼ��Ϣ��д��err_log.txt	*/
/************************************************************************/
CString huobiAPI::Post_place(CString symbol,CString price, CString mun,CString source,CString type)
{
	//�������������������������������ȵ���������������������������������
	if (0==(symbolsMap.size()))//û��ȡ������
	{
		Get_symbols();
	}
	int priceD = price.Find(".", 0);
	int munD = mun.Find(".", 0);
	int wantlen;
	if (priceD>=0)//��С������
	{
		wantlen= _ttoi((symbolsMap.find(symbol)->second).price_precision)+1;/*��ȡ�۸񾫶�*/
		if (price.GetLength()>priceD + wantlen)
		{
			price=price.Left(priceD + wantlen);
		}
	}
	if (munD >= 0)//��С������
	{
		wantlen = _ttoi((symbolsMap.find(symbol)->second).amount_precision)+1;/*��ȡ�۸񾫶�*/
		if (mun.GetLength() > munD + wantlen)
		{
			mun=mun.Left(munD + wantlen);
		}
	}
	//�������������������������ݾ���ɾ�����೤�ȡ�������������������
	CString sendmsg;
	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	CString send_data;
	CString account_id;
	if (source=="margin-api")
	{
		account_id = balanceMap.find("margin_id")->second;
	}
	else {
		account_id = balanceMap.find("account_id")->second;
	}
	if (type=="buy-limit"||type=="sell-limit")//����
	{
		send_data = "{\"account-id\":" + balanceMap.find("account_id")->second + ",\
						\"amount\":" + mun + ",\
						\"symbol\":\"" + symbol + "\",\
						\"type\":\"" + type + "\",\
						\"source\":\"" + source + "\",\
						\"price\":" + price + "}";
	}else if (type == "buy-market" || type == "sell-market")//�м�
	{
		send_data = "{\"account-id\":" + balanceMap.find("account_id")->second + ",\
						\"amount\":" + mun + ",\
						\"symbol\":\"" + symbol + "\",\
						\"type\":\"" + type + "\",\
						\"source\":\"" + source + "\"}";
	}
	sendmsg = Get_Sendmsg(Q, 8, "/v1/order/orders/place", "POST");
	sendmsg.Delete(0, sendmsg.Find("/v1", 0) );
	CString msg = PostRequest("api.huobi.pro", sendmsg, send_data);
	if (-1 == msg.Find("error"))//���ͳɹ�
	{
		msg.Delete(0, msg.Find("data", 0));
		CString sstr = find_str(msg, "[0-9]+");
		return sstr;
	}
	writeLog(msg, "Post_place(symbol:" +symbol+",price:" + price+",mun:" + mun+",source:"+ source+",type:"+ type+")");
	return msg;
}
/************************************************************************/
/*post ����																*/
/*����1������															*/
/*����ֵ���ɹ�TRUE ʧ��FALSE��д��err_log.txt                           */
/************************************************************************/
BOOL huobiAPI::Post_submitcancel(CString order_id)
{
	CString sendmsg;
	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	sendmsg = Get_Sendmsg(Q, 8, "/v1/order/orders/"+ order_id +"/submitcancel", "POST");
	sendmsg.Delete(0, sendmsg.Find("/v1", 0));
	CString msg = PostRequest("api.huobi.pro", sendmsg, "");
	if (-1 == msg.Find("error"))//���ͳɹ�
	{
		/*msg.Delete(0, msg.Find("data", 0));
		CString sstr = find_str(msg, "[0-9]+");*/
		return TRUE;
	}
	writeLog(msg, "Post_submitcancel(order_id:" + order_id+ ")");
	return FALSE;
}
/************************************************************************/
/*��������ϲ�����Get_balance_matchresults_order(sub,order_id)			*/
/*����1������															*/
/*����ֵ���ɹ�/ʧ�� ����ԭʼ���� ʧ��д��err_log.txt                    */
/************************************************************************/
//CString huobiAPI::Get_ordermsg(CString order_id)
//{
//	CString sendmsg;
//	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
//		"SignatureMethod", "=HmacSHA256",
//		"SignatureVersion", "=2",
//		"Timestamp","=" + Get_Times() };
//	sendmsg = Get_Sendmsg(Q, 8, "/v1/order/orders/" + order_id , "GET");
//	CString msg = GetRequest(sendmsg);
//	if (msg != "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!"&&-1 == msg.Find("error"))//���ͳɹ�
//	{
//		/*msg.Delete(0, msg.Find("data", 0));
//		CString sstr = find_str(msg, "[0-9]+");*/
//		return msg;
//	}
//	writeLog(msg, "Get_ordermsg(order_id:" + order_id + ")");
//	return "-1";
//}
/************************************************************************/
/*post ��������                                                         */
/*����1����������                                                       */
/*����2����������                                                       */
/*����ֵ���ɹ�TRUE ʧ��FALSE��д��err_log.txt                           */
/************************************************************************/
BOOL huobiAPI::Post_batchcancel(CString *order_id, int mun)
{
	CString sendmsg;
	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	sendmsg = Get_Sendmsg(Q, 8, "/v1/order/orders/batchcancel", "POST");
	sendmsg.Delete(0, sendmsg.Find("/v1", 0));
	/*string postmsg = sendmsg.GetBuffer(sendmsg.GetLength());
	char *buf = new char[strlen(postmsg.c_str()) + 1];
	strcpy(buf, postmsg.c_str());*/
	CString senddata = "{\"order-ids\":[\"";
	for (int i=0;i<mun;i++)
	{
		senddata += order_id[i];
		if (i!=mun-1)
		{
			senddata += "\",\"";
		}
		else {
			senddata += "\"]}";
		}
	}
	string reponse_data = PostRequest("api.huobi.pro", sendmsg, senddata);
	CString msg = reponse_data.c_str();
	if (-1 == msg.Find("error"))//���ͳɹ�
	{
		/*msg.Delete(0, msg.Find("data", 0));
		CString sstr = find_str(msg, "[0-9]+");*/
		return TRUE;
	}
	CString str; 
	str.Format("%d", mun);
	writeLog(reponse_data.c_str(), "Post_batchcancel(order_id:x,mun:" + str + ")");
	return FALSE;
}
/************************************************************************/
/*��ȡ����˻�ID  ���ʲ����	API�ֲ�У�����˻�����                 */
/*����1	���׶�		 													*/
/*����ֵ���ɹ�TRUE ʧ��FALSE��д��err_log.txt                           */
/*����鿴������														*/
/*(marginMap.find(symbol)->second).find("margin_id")->second			*/
/************************************************************************/
BOOL huobiAPI::Get_margin_balance(CString symbol)
{
		CString Q[10] = { "AccessKeyId", "=" + Access_Key,
			"SignatureMethod", "=HmacSHA256",
			"SignatureVersion", "=2",
			"Timestamp","=" + Get_Times(),
			"symbol","="+ symbol };
		CString sendmsg = Get_Sendmsg(Q, 10, "/v1/margin/accounts/balance", "GET");
		CString msg = GetRequest(sendmsg);
		if (msg != "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!"&&-1 == msg.Find("error"))//���ͳɹ�
		{
			UDT_MAP_CSTRING_CSTRING tmp;
			//tmp.margin_id = find_str(msg, "[0-9]+");
			tmp.insert(make_pair("margin_id", find_str(msg, "[0-9]+")));
			CString tmpstr= find_str(msg, "fl-price\":\"[0-9]+[/.]*[0-9]*");
			//tmp.fl_price = find_str(tmpstr, "[0-9]+[/.]*[0-9]*");
			tmp.insert(make_pair("fl_price", find_str(tmpstr, "[0-9]+[/.]*[0-9]*")));
			msg.Delete(0, msg.Find("risk-rate", 0));
			string str(msg.GetBuffer(msg.GetLength()));
			boost::smatch what;
			std::string::const_iterator start = str.begin();
			std::string::const_iterator end = str.end();
			string strtmp,inttmp;
			//������
			boost::regex expression("[0-9]+[/.]+[0-9]+");
			boost::regex_search(start, end, what, expression);
			strtmp = what[0];
			//tmp.risk_rate = strtmp.c_str();
			tmp.insert(make_pair("risk_rate", strtmp.c_str()));
			msg.Delete(0, msg.Find("[", 0) + 1);
			msg.Replace("\"", "");
			msg.Replace("{currency:", "");
			msg.Replace(",type:", "-");
			msg.Replace(",balance:", "");
			str = msg.GetBuffer(msg.GetLength());
			start = str.begin();
			end = str.end();
			for (int i=0;i<12;i++)
			{
				expression = "[a-z]+\-[a-z]+([\-]*[a-z]*)*";
				boost::regex_search(start, end, what, expression);
				strtmp = what[0];
				//tmp.xxx_trade = strtmp.c_str();
				expression = "[0-9]+[/.]+[0-9]+";
				boost::regex_search(start, end, what, expression);
				inttmp = what[0];
				start = what[0].second;
				tmp.insert(make_pair(strtmp.c_str(), inttmp.c_str()));

			}
			marginMap.insert(make_pair(symbol, tmp));
			// xxx_trade;//���ñ�
			// xxx_frozen;//����˻��Ҷ���
			// xxx_loan;//�ѽ����
			// xxx_interest;//�Ҵ�����Ϣ
			// xxx_transfer_out_available;//��ת��
			// xxx_loan_available;//�ɽ��
			// usdt_trade;//����usdt
			// usdt_frozen;//����˻�usdt����
			// usdt_loan;//�ѽ��usdt
			// usdt_interest;//usdt������Ϣ
			// usdt_transfer_out_available;//��תusdt
			// usdt_loan_available;//�ɽ�usdt
			//CString tid = (marginMap.find(symbol)->second).find("margin_id")->second;
			return TRUE;
		}
		writeLog(msg, "Get_margin_balance(symbol"+ symbol +")");
		return FALSE;
	
}
/************************************************************************/
/*�ֻ������˺ŵ�ת��ת����������                                    */
/*phat����ת��ת������˻���������					                */
/*ת�룺phat="/v1/dw/transfer-in/margin"								*/
/*ת����phat="/v1/dw/transfer-out/margin"								*/
/*���룺phat="/v1/margin/orders"										*/
/*symbol	true	string	���׶�	                                    */
/*currency	true	string	����	                                    */
/*amount	true	string	���	                                    */
/*����ֵ���ɹ����ػ�תID �򶩵��� ʧ�ܷ���ԭʼ��Ϣд��err_log.txt       */
/************************************************************************/
CString huobiAPI::Post_transfer_inout_apply(CString phat, CString symbol, CString currency, CString amount)//�������
{
	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times()};
	CString sendmsg = Get_Sendmsg(Q, 8, phat, "POST");
	sendmsg.Delete(0, sendmsg.Find("/v1", 0));
	CString senddata = "{\"symbol\":\""+ symbol +"\",\"currency\":\""+ currency +"\",\"amount\":\""+amount+"\"}";
	string reponse_data = PostRequest("api.huobi.pro", sendmsg, senddata);
	CString msg = reponse_data.c_str();
	if (-1 == msg.Find("error"))//���ͳɹ�
	{
		msg.Delete(0, msg.Find("data", 0));
		CString sstr = find_str(msg, "[0-9]+");
		return sstr;
	}
	writeLog(msg, "Post_transfer_inout(symbol:" + symbol + ",currency:" + currency + ",amount:" + amount + ")");
	return msg;
}
/************************************************************************/
/*�黹���																*/
/*����1������															*/
/*����2��������															*/
/*����ֵ���ɹ�TRUE ʧ��FALSE��д��err_log.txt                           */
/************************************************************************/
BOOL huobiAPI::Post_margin_repay(CString order_id,CString amount)
{
	CString sendmsg;
	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	sendmsg = Get_Sendmsg(Q, 8, "/v1/margin/orders/" + order_id + "/repay", "POST");
	sendmsg.Delete(0, sendmsg.Find("/v1", 0));
	CString msg = PostRequest("api.huobi.pro", sendmsg, "{\"amount\":\""+ amount +"\"}");
	if (-1 == msg.Find("error"))//���ͳɹ�
	{
		/*msg.Delete(0, msg.Find("data", 0));
		CString sstr = find_str(msg, "[0-9]+");*/
		return TRUE;
	}
	writeLog(msg, "Post_submitcancel(order_id:" + order_id + ")");
	return FALSE;
}
//GET /v1/order/orders ��ѯ��ǰί�С���ʷί��
/*�������:

��������	�Ƿ����	����	����								ȡֵ��Χ
symbol		true		string	���׶�								btcusdt, bccbtc, rcneth ...
types		false		string	��ѯ�Ķ���������ϣ�ʹ��','�ָ�		buy-market���м���, sell-market���м���, buy-limit���޼���, sell-limit���޼���
start-date	false		string	��ѯ��ʼ����, ���ڸ�ʽyyyy-mm-dd
end-date	false		string	��ѯ��������, ���ڸ�ʽyyyy-mm-dd
states		true		string	��ѯ�Ķ���״̬��ϣ�ʹ��','�ָ�		pre-submitted ׼���ύ, submitted ���ύ, partial-filled ���ֳɽ�, partial-canceled ���ֳɽ�����, filled ��ȫ�ɽ�, canceled �ѳ���
from		false		string	��ѯ��ʼ ID
direct		false		string	��ѯ����							prev ��ǰ��next ���
size		false		string	��ѯ��¼��С		*/
//GET /v1/margin/loan-orders �������
/*�������

��������	�Ƿ����	����	����								ȡֵ��Χ
symbol		true		string	���׶�
currency	true		string	����
start-date	false		string	��ѯ��ʼ����, ���ڸ�ʽyyyy-mm-dd
end-date	false		string	��ѯ��������, ���ڸ�ʽyyyy-mm-dd
states		true		string	״̬
from		false		string	��ѯ��ʼ ID
direct		false		string	��ѯ����							prev ��ǰ��next ���
size		false		string	��ѯ��¼��С		*/
//GET /v1/order/matchresults ��ѯ��ǰ�ɽ�����ʷ�ɽ�
/*�������:

��������	�Ƿ����	����	����								ȡֵ��Χ
symbol		true		string	���׶�								btcusdt, bccbtc, rcneth ...
types		false		string	��ѯ�Ķ���������ϣ�ʹ��','�ָ�		buy-market���м���, sell-market���м���, buy-limit���޼���, sell-limit���޼���
start-date	false		string	��ѯ��ʼ����, ���ڸ�ʽyyyy-mm-dd
end-date	false		string	��ѯ��������, ���ڸ�ʽyyyy-mm-dd
from		false		string	��ѯ��ʼ ID
direct		false		string	��ѯ����							prev ��ǰ��next ���
size		false		string	��ѯ��¼��С		*/
//GET /market/history/kline ��ȡK������
/*�������:

��������	�Ƿ����	����	����		ȡֵ��Χ
symbol		true		string	���׶�		btcusdt, bccbtc, rcneth ...
period		true		string	K������		1min, 5min, 15min, 30min, 60min, 1day, 1mon, 1week, 1year
size		false		integer	��ȡ����	150	[1,2000]*/
//GET /market/detail/merged ��ȡ�ۺ�����(Ticker)
/*�������:

��������	�Ƿ����	����	����		ȡֵ��Χ
symbol		true		string	���׶�		btcusdt, bccbtc, rcneth ...*/
//GET /market/detail ��ȡ Market Detail 24Сʱ�ɽ�������
/*�������:

��������	�Ƿ����	����	����		ȡֵ��Χ
symbol		true		string	���׶�		btcusdt, bccbtc, rcneth ...*/
//GET /v1/common/timestamp ��ѯϵͳ��ǰʱ�� ����2��������ָ�� ����3λ0


/************************************************************************/
/*�ұҽ��׺ͽ�����׵���ʷ������ѯ K�߲�ѯ �ۺ�����	24Сʱ�ɽ���	    */
/*����1phat��ͬ ��ѯ�ͬ ֵ���Ϸ�ע�Ϳ飬ʹ��������main����			*/
/*����2����Ԫ��ֵ��ͬ ����ο������ע��                                */
/*����ֵ���ɹ�/ʧ�� ����ԭʼ���� ʧ��д��err_log.txt                    */
/************************************************************************/
CString huobiAPI::Get_·���޲���ͨ��(CString phat, CString *instr, int mun)
{
	int j = 8;
	CString Q[64] = {"0"};
	CString Q1[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	for (int i=0;i<8;i++)
	{
		Q[i] = Q1[i];
	}
	for (int i = 0; i < mun; i++)
	{
		Q[j] = instr[i];
		j++;
	}
	CString sendmsg = Get_Sendmsg(Q, mun+8, phat, "GET");
	CString msg = GetRequest(sendmsg);
	if (msg != "Can't InternetOpen!"&&msg != "Can't InternetOpenUrl!"&&msg != "Can't InternetReadFile!"&&-1 == msg.Find("error"))//���ͳɹ�
	{
		/*msg.Delete(0, msg.Find("data", 0));
		CString sstr = find_str(msg, "[0-9]+");*/
		return msg;
	}
	CString str;
	str.Format("%d", mun);
	writeLog(msg, "Get_·���޲���ͨ��(phat:"+ phat+", *instr:x,mun" + str + ")");
	return msg;

}
//POST /v1/dw/withdraw/api/create �������������
/*�������:

��������	�Ƿ����	����	����						ȡֵ��Χ
address		true		string	���ֵ�ַ
amount		true		string	�������
currency	true		string	�ʲ�����					btc, ltc, bcc, eth, etc ...(���Pro֧�ֵı���)
fee			false		string	ת��������
addr-tag	false		string	����ҹ����ַtag��XRP����	��ʽ, "123"��������ַ���*/
//POST /v1/dw/withdraw-virtual/{withdraw-id}/cancel ����ȡ�����������
/*�������:

��������	�Ƿ����	����	����	
withdraw-id	true		long	����ID������path��	*/
/************************************************************************/
/* ���ݸ�ʽΪ��{"x":"x","x":"x","x":"x"} �Ķ�������                     */
/************************************************************************/
CString huobiAPI::Post_��д·��ͨ��(CString phat, CString *instr, int mun)
{
	CString sendmsg;
	CString Q[8] = { "AccessKeyId", "=" + Access_Key,
		"SignatureMethod", "=HmacSHA256",
		"SignatureVersion", "=2",
		"Timestamp","=" + Get_Times() };
	sendmsg = Get_Sendmsg(Q, 8, "/v1/order/orders/batchcancel", "POST");
	sendmsg.Delete(0, sendmsg.Find("/v1", 0));
	CString senddata = "{\"";
	for (int i = 0; i<mun; i+=2)
	{
		senddata += instr[i];
		senddata += "\":\"";
		senddata += instr[i+1];
		if (i != mun - 2)
		{
			senddata += "\",\"";
		}
		else {
			senddata += "\"}";
		}
	}
	CString msg = PostRequest("api.huobi.pro", sendmsg, "");
	if (-1 == msg.Find("error"))//���ͳɹ�
	{
		/*msg.Delete(0, msg.Find("data", 0));
		CString sstr = find_str(msg, "[0-9]+");*/
		return msg;
	}
	CString str;
	str.Format("%d", mun);
	writeLog(msg, "Post_��д·��ͨ��(phat:" + phat + ", *instr:x,mun" + str + ")");
	return msg;
}