#if !defined(PUBLICFUNCS_H)
#define PUBLICFUNCS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <memory.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiStruct.h"

#ifdef WIN32
#include <windows.h>
typedef HANDLE THREAD_HANDLE;
#else
#include <pthread.h>
typedef pthread_t THREAD_HANDLE;
#endif
using namespace std;

const int BUFLEN = 512;

extern TThostFtdcBrokerIDType g_BrokerID;
extern TThostFtdcUserIDType	g_UserID;
extern TThostFtdcPasswordType	g_Password;
extern CThostFtdcMdApi * g_pmduserapi;
extern CThostFtdcTraderApi * g_puserapi;

extern char g_frontaddr_trade[BUFLEN];
extern char g_frontaddr_data[BUFLEN];
//extern HANDLE g_hEvent;
extern int g_nOrdLocalID;

extern string date_string;
extern vector<string>instru_vec;

//����
void user_SubcribeMarketData_byhand();
void SubscribeMarketData_all();
void QryInstruments_Trading();
bool StartMenu();

#endif