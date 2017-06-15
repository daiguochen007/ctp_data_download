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
string date_string;

//5. ���к�Լ��list 
vector<string>instru_vec;

// API
CThostFtdcMdApi *g_pmduserapi = NULL;
CThostFtdcTraderApi * g_puserapi = NULL;

/*---------------ȫ�ֺ�����---------------*/

#ifdef WIN32
int WINAPI MenuFunc(LPVOID pParam)
#else
void * MenuFunc(void *pParam)
#endif
{
	int g_choose;
	while (true){
		cout << "#------------Ŀ¼-----------#" << endl;
		cout << "1. ����ȫ�г���Լ" << endl;
		cout << "2. ���ȫ�г���Լ" << endl;
		cout << "3. ����ȫ�г���Լ" << endl;
		cout << "4. �˳� "<<endl;
		cout << "#---       ��������      ---#" << endl;
		cout << "5. �ֶ����ĺ�Լ" << endl;
		cout << "#---------------------------#" << endl;
		cin.clear();
		cin >> g_choose;
		cout << "��ѡ����ǣ�[" << g_choose << "]" << endl;
		switch (g_choose)
		{
		case 1:
			QryInstruments_Trading();
			break;
		case 2:
			for (int i = 0; i < instru_vec.size(); i++){
				cout << instru_vec[i]<<" ";
			}
			break;
		case 3:
			SubscribeMarketData_all();
			break;
		case 4:
			exit(0);
		case 5:
			user_SubcribeMarketData_byhand();
			break;
		default:
			cout << "Input Error" << endl;
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			break;
		}
		Sleep(200);
		g_choose = -1;
		system("pause");
	}
	return 0;
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
}


void QryInstruments_Trading(){
	if (g_puserapi == NULL)
	{
		cout<<"StartQryInvestorAccount  USERAPIδ����"<<endl;
		return;
	}
	char filePath[100] = { '\0' };
	sprintf(filePath, "data\\contract_table_%s.csv", date_string.c_str());

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
	cout<<"�ȴ�ȫ�г���Լ�����ѯ���..."<<endl;
#ifdef WIN32
	Sleep(200);
#else
	usleep(200);
#endif
	return;
}


bool StartMenu()
{
	//int dwIDThread;
	unsigned long dwIDThread;
	THREAD_HANDLE hThread;	/**< �߳̾�� */
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