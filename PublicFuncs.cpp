// PublicFuncs.cpp: implementation of the PublicFuncs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
using namespace std;

/*---------------ȫ�ֱ�����---------------*/

//1.��¼��Ϣ
///���͹�˾����
TThostFtdcBrokerIDType g_BrokerID;
///�����û�����
TThostFtdcUserIDType	g_UserID;
///����
TThostFtdcPasswordType	g_Password;

//2.��ַ��Ϣ
char g_frontaddr_data[BUFLEN];
char g_frontaddr_trade[BUFLEN];

//3.������Ϣ
int g_nOrdLocalID = 0;

//4. ������Ϣ
const char* trading_date;

//5. ���к�Լ��list 
vector<string>instru_vec;

// API
CThostFtdcMdApi *g_pmduserapi = NULL;
CThostFtdcTraderApi * g_puserapi = NULL;

/*---------------ȫ�ֺ�����---------------*/

#ifdef WIN32
int WINAPI SFT_Func(LPVOID pParam)
#else
void * SFT_Func(void *pParam)
#endif
{
	cout << "start from trader api"<<endl;
	//�Զ�������
	check_folder();
	QryInstruments_subscribing();

	////�˵����
	//int g_choose;
	//while (true){
	//	cout << "#------------Ŀ¼-----------#" << endl;
	//	cout << "0. һ������" << endl;
	//	cout << "#---       ��������      ---#" << endl;
	//	cout << "1. ������ǰ�������ڵ��ļ���" << endl;
	//	cout << "3. ���ȫ�г���Լ" << endl;
	//	cout << "4. ����ȫ�г���Լ" << endl;
	//	cout << "5. �˳� "<<endl;
	//	cout << "6. �ֶ����ĺ�Լ" << endl;
	//	cout << "7. ��ѯ��ǰ��������" << endl;
	//	cout << "9. Test" << endl;
	//	cout << "#---------------------------#" << endl;
	//	cin.clear();
	//	cin >> g_choose;
	//	cout << "��ѡ����ǣ�[" << g_choose << "]" << endl;
	//	switch (g_choose)
	//	{
	//	case 0:
	//		check_folder();
	//		QryInstruments_subscribing();
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
	//		Trader_Logout();
	//      Trader_Login();
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

//��¼
void Trader_Login(){
	CThostFtdcReqUserLoginField reqUserLogin;
	// get BrokerID
	strcpy(reqUserLogin.BrokerID, g_BrokerID);
	// get userid
	strcpy(reqUserLogin.UserID, g_UserID);
	// get password
	strcpy(reqUserLogin.Password, g_Password);
	cout << "���׷����������¼��..." << endl;
	// ������½����
	g_puserapi->ReqUserLogin(&reqUserLogin, 0);

}
void Data_Login(){
	CThostFtdcReqUserLoginField reqUserLogin;
	// get BrokerID
	strcpy(reqUserLogin.BrokerID, g_BrokerID);
	// get userid
	strcpy(reqUserLogin.UserID, g_UserID);
	// get password
	strcpy(reqUserLogin.Password, g_Password);
	cout << "��������������¼��..." << endl;
	// ������½����
	g_pmduserapi->ReqUserLogin(&reqUserLogin, 0);
}

//�Լ��ӵĺڿƼ���trader�˳�
void Trader_Logout(){
	//��Ҫ��ʼ����
	//���˳�
	CThostFtdcUserLogoutField reqUserLogout;
	// get BrokerID
	strcpy(reqUserLogout.BrokerID, g_BrokerID);
	// get userid
	strcpy(reqUserLogout.UserID, g_UserID);
	g_puserapi->ReqUserLogout(&reqUserLogout, 0);
}


void user_SubcribeMarketData_byhand(){
	int ins_num=0;
	cout<<"�������붩�ĵĺ�Լ������"<<endl;
	cin>>ins_num;

	vector<string>ins(ins_num);
	for (int i = 0; i < ins_num; i++){
		cout << "�������"<<i+1<<"����Լ�Ĵ��룺" << endl;
		cin >> ins[i];
	}
    //convert to stupid char** in c
	char ** ins_c = new char*[ins.size()];
	for (int i = 0; i < ins.size(); i++){
		ins_c[i] = new char[ins[i].size() + 1];
		strcpy(ins_c[i], ins[i].c_str());
	}

	// ��������
	g_pmduserapi->SubscribeMarketData(ins_c, ins.size());

	//���˶�����
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

	// ��������
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
		cout<<"�������USERAPIδ����"<<endl;
		//���µ�¼
		return;
	}

	//�ļ���
	char filePath[100] = { '\0' };
	sprintf(filePath, "%s\\contract_table_%s.csv",trading_date ,trading_date);

	//����ļ�������,create�����غ�Լ����,����������ȡ(����)
	fstream _file;
	_file.open(filePath, ios::in);
	if (!_file)
	{
		ofstream outFile;
		outFile.open(filePath, ios::out); // �¿��ļ�
		outFile << "����������" << ","
			<< "��Լ����" << ","
			<< "��������" << ","
			<< "��С�䶯��λ" << ","
			<< "��Լ������"
			<< endl;
		outFile.close();

		CThostFtdcQryInstrumentField QryInstrument;
		memset(&QryInstrument, 0, sizeof(CThostFtdcQryInstrumentField));
		g_puserapi->ReqQryInstrument(&QryInstrument, g_nOrdLocalID++);
		cout << "�ȴ�ȫ�г���Լ�����ѯ���..." << endl;
		//�ȴ��ص��������
		//�����ڻص�������
	}
	else{
		//�ļ�������򿪶�ȡ
		instru_vec.clear();
		string line,ticker;
		ifstream inFile(filePath);
		getline(inFile, line); //ȥ��header
		while (getline(inFile,line)){
			int i = 1;
			istringstream is(line);
			while (getline(is, ticker, ',')){
				if (i == 2){
					//cout << ticker << endl;
					instru_vec.push_back(ticker);
				}
				i++;
			}
		}
		cout << "���ض�ȡ��Լ��ɹ�" << endl;
		//����
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
		cout << "StartQry  USERAPIδ����" << endl;
		return "";
	}
#ifdef WIN32
	Sleep(200);
#else
	usleep(200);
#endif
	return g_puserapi->GetTradingDay();
}

bool check_folder(){
	if (_access(trading_date, 0) != 0){
		cout << "�ļ��в����ڣ���������..." << endl;
		_mkdir(trading_date);
		return false;
	}
	else{ 
		cout << "�ļ����Ѿ����ڣ�" << endl; 
		return true;
	}
}

bool start_from_trader()
{
	//int dwIDThread;
	unsigned long dwIDThread;
	THREAD_HANDLE hThread;	/**< �߳̾�� */
	bool ret = true;
#ifdef WIN32
	hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SFT_Func, NULL, 0, &dwIDThread);
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


int WINAPI SFDU_Func(LPVOID pParam)
{
	cout << "start from datauser api" << endl;
	////�Զ�������
	SubscribeMarketData_all();
	return 0;
}

bool start_from_datauser()
{
	//int dwIDThread;
	unsigned long dwIDThread;
	THREAD_HANDLE hThread;	/**< �߳̾�� */
	bool ret = true;
	hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SFDU_Func, NULL, 0, &dwIDThread);
	if (hThread == NULL)
	{
		ret = false;
	}
	SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
	ResumeThread(hThread);
	return ret;
}