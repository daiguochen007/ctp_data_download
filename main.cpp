// ctptest-md-sim.cpp : 定义控制台应用程序的入口点。
// Guochen Dai
// 2017

#include "stdafx.h"
#include "PublicFuncs.h"
#include "MdSpi.h"
#include "TraderSpi.h"
using namespace std;

void usage(char* pName)
{
	cout << "程序路径: [" << pName << "]" << endl;
	cout << "行情前置: [tcp://180.168.146.187:10010]" << endl;
	cout << "交易前置: [tcp://180.168.146.187:10000]" << endl;
	time_t local_t = time(NULL);
	cout << "本机时间: "<<ctime(&local_t)<< endl;
	return;
}

void getid(TThostFtdcBrokerIDType &g_BrokerID, TThostFtdcUserIDType	&g_UserID, TThostFtdcPasswordType &g_Password){
	ifstream userfile("C://NYU poly//ctp_futures//ctp_trade_sim//ctptest-sim//user.txt");
	if (!userfile){
		cout << "User file not exit";
		exit(1);
	}
	char buffer[128];
	int i = 0;
	while (!userfile.eof())
	{
		userfile.getline(buffer, 128);
		if (buffer[0] != '#' && strlen(buffer) != 0){
			//cout << buffer << endl;
			i++;
		}
		if (i == 1){
			strcpy(g_BrokerID, buffer);
		}
		if (i == 2){
			strcpy(g_UserID, buffer);
		}
		if (i == 3){
			strcpy(g_Password, buffer);
		}
	}
	userfile.close();
	return;
}

////得到交易日期
//void get_date_string(string &date_string){
//	stringstream date_oss;
//
//	time_t local_t = time(NULL);
//	struct tm *local_t_ptr;
//	local_t_ptr = localtime(&local_t);
//	//判断
//	date_oss << local_t_ptr->tm_year + 1900;
//	date_oss.width(2);
//	date_oss.fill('0');
//	date_oss << local_t_ptr->tm_mon + 1;
//	date_oss.width(2);
//	date_oss.fill('0');
//	date_oss << local_t_ptr->tm_mday;
//
//	date_string = date_oss.str();
//}

int main(int argc, char* argv[])
{
	usage(argv[0]);

	

	memset(g_frontaddr_data, 0, BUFLEN);
	memset(g_frontaddr_trade, 0, BUFLEN);
	strcpy(g_frontaddr_data, "tcp://180.168.146.187:10010");
	strcpy(g_frontaddr_trade, "tcp://180.168.146.187:10000");

	getid(g_BrokerID, g_UserID, g_Password);
	//get_date_string(date_string);

	// ----------------------------------------------交易
	// 产生一个CThostFtdcTraderApi实例
	CThostFtdcTraderApi *p_Trader_UserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	g_puserapi =  p_Trader_UserApi;
	// 产生一个事件处理的实例
	CTraderSpi myspi(p_Trader_UserApi);
	// 设置交易托管系统服务的地址，可以注册多个地址备用
	p_Trader_UserApi->RegisterFront(g_frontaddr_trade);
	// 注册一事件处理的实例
	p_Trader_UserApi->RegisterSpi(&myspi);
	// 订阅私有流
	p_Trader_UserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	// 订阅公共流
	p_Trader_UserApi->SubscribePublicTopic(THOST_TERT_RESUME);
	// TERT_RESTART:从本交易日开始重传
	// TERT_RESUME:从上次收到的续传
	// TERT_QUICK:只传送登录后私有流的内容
	cout << " 交易Api版本: [" << p_Trader_UserApi->GetApiVersion() << "]" << endl;
	// 使客户端开始与后台服务建立连接
	p_Trader_UserApi->Init();

	// ----------------------------------------------行情
	// 产生一个CThostFtdcMdApi实例
	CThostFtdcMdApi *p_Md_UserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	g_pmduserapi = p_Md_UserApi;
	// 产生一个事件处理的实例
	CSimpleHandler mymdspi(p_Md_UserApi);
	// 注册一事件处理的实例
	p_Md_UserApi->RegisterSpi(&mymdspi);
	// 设置交易托管系统服务的地址，可以注册多个地址备用
	p_Md_UserApi->RegisterFront(g_frontaddr_data);
	cout << " 行情Api版本: [" << p_Md_UserApi->GetApiVersion() << "]" << endl;
	// 使客户端开始与后台服务建立连接
	p_Md_UserApi->Init();
    
	//-----------------------------------------------释放
	p_Trader_UserApi->Join();
	// 释放API实例
	p_Trader_UserApi->Release();

	p_Md_UserApi->Join();
	// 释放API实例
	p_Md_UserApi->Release();
	return 0;
}

