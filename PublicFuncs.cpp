// PublicFuncs.cpp: implementation of the PublicFuncs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
using namespace std;

/*---------------全局变量区---------------*/

//1.登录信息
///经纪公司代码
TThostFtdcBrokerIDType g_BrokerID;
///交易用户代码
TThostFtdcUserIDType	g_UserID;
///密码
TThostFtdcPasswordType	g_Password;

//2.地址信息
char g_frontaddr_data[BUFLEN];
char g_frontaddr_trade[BUFLEN];

//3.报单信息
int g_nOrdLocalID = 0;

//4. 日期信息
const char* trading_date;

//5. 所有合约的list 
vector<string>instru_vec;

// API
CThostFtdcMdApi *g_pmduserapi = NULL;
CThostFtdcTraderApi * g_puserapi = NULL;

/*---------------全局函数区---------------*/

#ifdef WIN32
int WINAPI MenuFunc(LPVOID pParam)
#else
void * MenuFunc(void *pParam)
#endif
{
	//自动拿数据
	check_folder();
	QryInstruments_subscribing();

	////菜单面板
	//int g_choose;
	//while (true){
	//	cout << "#------------目录-----------#" << endl;
	//	cout << "0. 一键下载" << endl;
	//	cout << "#---       特殊需求      ---#" << endl;
	//	cout << "1. 创建当前交易日期的文件夹" << endl;
	//	cout << "3. 输出全市场合约" << endl;
	//	cout << "4. 订阅全市场合约" << endl;
	//	cout << "5. 退出 "<<endl;
	//	cout << "6. 手动订阅合约" << endl;
	//	cout << "7. 查询当前交易日期" << endl;
	//	cout << "9. Test" << endl;
	//	cout << "#---------------------------#" << endl;
	//	cin.clear();
	//	cin >> g_choose;
	//	cout << "您选择的是：[" << g_choose << "]" << endl;
	//	switch (g_choose)
	//	{
	//	case 0:
	//		check_folder();
	//		QryInstruments_subscribing()
	//		break;
	//	case 2:
	//		break;
	//	case 3:
	//		for (int i = 0; i < instru_vec.size(); i++){
	//			cout << instru_vec[i]<<" ";
	//		}
	//		break;
	//	case 4:
	//		SubscribeMarketData_all();
	//		break;
	//	case 5:
	//		exit(0);
	//	case 6:
	//		user_SubcribeMarketData_byhand();
	//		break;
	//	case 7:
	//		cout<<trading_date<<endl;
	//		break;
	//	case 1:
	//		check_folder();
	//		break;
	//	case 9:
	//		break;
	//	default:
	//		cout << "Input Error" << endl;
	//		int c;
	//		while ((c = getchar()) != '\n' && c != EOF);
	//		break;
	//	}
	//	Sleep(200);
	//	g_choose = -1;
	//	system("pause");
	//}
	return 0;
}

void user_SubcribeMarketData_byhand(){
	int ins_num=0;
	cout<<"请输入想订阅的合约个数："<<endl;
	cin>>ins_num;

	vector<string>ins(ins_num);
	for (int i = 0; i < ins_num; i++){
		cout << "请输入第"<<i+1<<"个合约的代码：" << endl;
		cin >> ins[i];
	}
    //convert to stupid char** in c
	char ** ins_c = new char*[ins.size()];
	for (int i = 0; i < ins.size(); i++){
		ins_c[i] = new char[ins[i].size() + 1];
		strcpy(ins_c[i], ins[i].c_str());
	}

	// 订阅行情
	g_pmduserapi->SubscribeMarketData(ins_c, ins.size());

	//或退订行情
	//Sleep(5000);
	//g_pmduserapi->UnSubscribeMarketData(ins_c, ins.size());

    //delete stupid char** 
	for (int i = 0; i < ins.size(); i++){
		delete[] ins_c[i];
	}
	delete[] ins_c;
}

void SubscribeMarketData_all(){
	//convert to stupid char** in c
	char ** ins_vec_c = new char*[instru_vec.size()];
	for (int i = 0; i < instru_vec.size(); i++){
		ins_vec_c[i] = new char[instru_vec[i].size() + 1];
		strcpy(ins_vec_c[i], instru_vec[i].c_str());
	}

	// 订阅行情
	g_pmduserapi->SubscribeMarketData(ins_vec_c, instru_vec.size());

	//delete stupid char** 
	for (int i = 0; i < instru_vec.size(); i++){
		delete[] ins_vec_c[i];
	}
	delete[] ins_vec_c;
	return;
}

void QryInstruments_subscribing(){
	if (g_puserapi == NULL || g_pmduserapi == NULL)
	{
		cout<<"行情或交易USERAPI未创建"<<endl;
		return;
	}

	//文件名
	char filePath[100] = { '\0' };
	sprintf(filePath, "%s\\contract_table_%s.csv",trading_date ,trading_date);

	//如果文件不存在,create并下载合约名称,如果存在则读取(更快)
	fstream _file;
	_file.open(filePath, ios::in);
	if (!_file)
	{
		ofstream outFile;
		outFile.open(filePath, ios::out); // 新开文件
		outFile << "交易所代码" << ","
			<< "合约代码" << ","
			<< "数量乘数" << ","
			<< "最小变动价位" << ","
			<< "合约到期日"
			<< endl;
		outFile.close();

		CThostFtdcQryInstrumentField QryInstrument;
		memset(&QryInstrument, 0, sizeof(CThostFtdcQryInstrumentField));
		g_puserapi->ReqQryInstrument(&QryInstrument, g_nOrdLocalID++);
		cout << "等待全市场合约代码查询结果..." << endl;
		//等待回调函数完成
		//订阅在回调函数内
	}
	else{
		//文件存在则打开读取
		instru_vec.clear();
		string line,ticker;
		ifstream inFile(filePath);
		getline(inFile, line); //去掉header
		while (getline(inFile,line)){
			int i = 1;
			istringstream is(line);
			while (getline(is, ticker, ',')){
				if (i == 2){
					cout << ticker << endl;
					instru_vec.push_back(ticker);
				}
				i++;
			}
		}
		//订阅
		SubscribeMarketData_all();
	}
#ifdef WIN32
	Sleep(200);
#else
	usleep(200);
#endif
	return;
}

const char* get_tradingdate_string(){
	if (g_puserapi == NULL)
	{
		cout << "StartQry  USERAPI未创建" << endl;
		return "";
	}
#ifdef WIN32
	Sleep(200);
#else
	usleep(200);
#endif
	return g_puserapi->GetTradingDay();
}

void check_folder(){
	if (_access(trading_date, 0) != 0){
		cout << "文件夹不存在，将被创建..." << endl;
		_mkdir(trading_date);
	}
	else{ cout << "文件夹已经存在！" << endl; }
}

bool StartMenu()
{
	//int dwIDThread;
	unsigned long dwIDThread;
	THREAD_HANDLE hThread;	/**< 线程句柄 */
	bool ret = true;
#ifdef WIN32
	hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MenuFunc, NULL, 0, &dwIDThread);
	if (hThread == NULL)
	{
		ret = false;
	}
	SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
	ResumeThread(hThread);


#else 
	ret = (::pthread_create(&hThread, NULL, &OrderFunc, NULL) == 0);
#endif
	return ret;
}
 