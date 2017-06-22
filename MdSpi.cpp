#include "stdafx.h"
using namespace std;

CSimpleHandler::CSimpleHandler(CThostFtdcMdApi *pUserApi) : m_pUserApi(pUserApi) {}

CSimpleHandler::~CSimpleHandler() {}

// 当客户端与交易托管系统建立起通信连接，客户端需要进行登录
void CSimpleHandler::OnFrontConnected()
{
	//连上了就登陆
	Data_Login();
#ifdef WIN32
	Sleep(1000);
#else
	usleep(1000);
#endif
}

// 当客户端与交易托管系统通信连接断开时，该方法被调用
void CSimpleHandler::OnFrontDisconnected(int nReason)
{
	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	cout<<"行情连接断开，正在重连..."<<endl;
}

// 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
void CSimpleHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo->ErrorID != 0) {
		// 端登失败，客户端需进行错误处理
		printf("-----------------------------\n");
		printf("行情登录失败...错误原因：%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	// 端登成功
	cout<<"行情服务器登陆成功！！！"<<endl;
	////////---------------------------------------------------------////////
	//登录成功后开始订阅行情
	//Sleep(1000);
	//Trader_Logout();
	if (instru_vec.size() != 0){
		start_from_datauser();
	}
}

//订阅行情应答
void CSimpleHandler::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout<<pSpecificInstrument->InstrumentID<<"订阅成功!"<<endl;
		//不同的合约单独存储 
		//文件名加上日期
		char filePath[100] = { '\0' };
		sprintf(filePath, "%s\\%s_%s.csv", trading_date, pSpecificInstrument->InstrumentID, trading_date);

		//如果文件不存在,create
		fstream _file;
		_file.open(filePath, ios::in);
		if (!_file)
		{
			cout << filePath << "不存在，将被创建..." << endl;
			ofstream outFile;
			outFile.open(filePath, ios::out); // 新开文件
			outFile << "合约代码" << ","
				<< "交易日期" << ","
				<< "更新时间" << ","
				<< "最新价" << ","
				<< "买价一" << ","
				<< "买量一" << ","
				<< "卖价一" << ","
				<< "卖量一" << ","
				<< "昨结算价" << ","
				<< "成交量" << ","
				<< "持仓量" << ","
				<< "涨停板价" << ","
				<< "跌停板价"
				<< endl;
			outFile.close();
		}
	}
	else
		cerr << "返回错误! ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
}
//退订行情应答
void CSimpleHandler::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << pSpecificInstrument->InstrumentID << "退订成功!" << endl;
}

// 行情应答
void CSimpleHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField*pDepthMarketData)
{
	//收到深度行情
	printf("[%s]  [%s]  最新价=[%.2lf]  [% .2f%%]\n", pDepthMarketData->UpdateTime, pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, 100 * (pDepthMarketData->LastPrice / pDepthMarketData->PreSettlementPrice - 1));
	// 逐tick地存入文件或数据库  

	//文件名加上日期
	char filePath[100] = { '\0' };
	sprintf(filePath, "%s\\%s_%s.csv", trading_date, pDepthMarketData->InstrumentID, trading_date);
	ofstream outFile;
	outFile.open(filePath, ios::app); // 文件追加写入   
	outFile << pDepthMarketData->InstrumentID << ","
		<< pDepthMarketData->TradingDay << ","
		<< pDepthMarketData->UpdateTime << "." << pDepthMarketData->UpdateMillisec << ","
		<< pDepthMarketData->LastPrice << ","
		<< pDepthMarketData->BidPrice1 << ","
		<< pDepthMarketData->BidVolume1 << ","
		<< pDepthMarketData->AskPrice1 << ","
		<< pDepthMarketData->AskVolume1 << ","
		<< pDepthMarketData->PreSettlementPrice << ","
		<< pDepthMarketData->Volume << ","
		<< pDepthMarketData->OpenInterest << ","
		<< pDepthMarketData->UpperLimitPrice << ","
		<< pDepthMarketData->LowerLimitPrice
		<< endl;
	outFile.close();

};

// 针对用户请求的出错通知
void CSimpleHandler::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("OnRspError:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,pRspInfo->ErrorMsg);
	// 客户端需进行错误处理
}
