#include "stdafx.h"
#include "TraderSpi.h"
using namespace std;

CTraderSpi::CTraderSpi(CThostFtdcTraderApi *pUserApi) :m_pUserApi(pUserApi){};

CTraderSpi::~CTraderSpi(){};

void CTraderSpi::OnFrontConnected()
{
	CThostFtdcReqUserLoginField reqUserLogin;
	// get BrokerID
	strcpy(reqUserLogin.BrokerID, g_BrokerID);
	// get userid
	strcpy(reqUserLogin.UserID, g_UserID);
	// get password
	strcpy(reqUserLogin.Password, g_Password);
	// ������½����
	m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
	cout<<"���׷����������¼��..."<<endl;
#ifdef WIN32
	Sleep(1000);
#else
	usleep(1000);
#endif
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
	// ��������������API���Զ��������ӣ��ͻ��˿ɲ�������
	printf("OnFrontDisconnected.(����ǰ�����ӶϿ�)\n");
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//printf("OnRspUserLogin:\n");
	//printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	//printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	if (pRspInfo->ErrorID != 0) {
		// �˵�ʧ�ܣ��ͻ�������д�����
		printf("-----------------------------\n");
		printf("��¼ʧ��...����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}

	cout<<"���׷�������¼�ɹ�������"<<endl;
	//printf("  ǰ�ñ��:[%d]\n", pRspUserLogin->FrontID);//
	//printf("  �Ự���:[%d]\n", pRspUserLogin->SessionID);//
	//printf("  ��󱨵����:[%s]\n", pRspUserLogin->MaxOrderRef);
	g_nOrdLocalID = atoi(pRspUserLogin->MaxOrderRef); 
	
	//���ӳɹ����Զ�
	trading_date = get_tradingdate_string();

	StartMenu();
}




void CTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("�޸�����ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pUserPasswordUpdate == NULL)
	{
		printf("û���޸���������\n");
		return;
	}
	printf("-----------------------------\n");
	printf("�޸�����ɹ�\n");
	printf("-----------------------------\n");
	return;
}

//��������Ӧ ������
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField*pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("����ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pInputOrder == NULL)
	{
		printf("û�б�������\n");
		return;
	}
	printf("-----------------------------\n");
	printf("�����ɹ�\n");
	printf("-----------------------------\n");
	return;
};

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("����ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pOrderAction == NULL)
	{
		printf("û�г�������\n");
		return;
	}
	printf("-----------------------------\n");
	printf("�����ɹ�\n");
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if (pOrder->OrderStatus == 'a'){
		//printf("-----------------------------\n");
		//printf("�������󣡣���(ԭ��δ���̻��߱���������)\n");
		//printf("-----------------------------\n");
	}
	else if (pOrder->OrderStatus == '5'){
		printf("-----------------------------\n");
		printf("�ñ����Ѿ�����\n");
		printf("-----------------------------\n");
	}
	else{
		printf("-----------------------------\n");
		printf("�յ������ر�\n");
		Show(pOrder);
		printf("-----------------------------\n");
	}
	return;
}

void CTraderSpi::Show(CThostFtdcOrderField *pOrder)
{
	printf("-----------------------------\n");
	printf("����������=[%s]\n", pOrder->ExchangeID);
	printf("������=[%s]\n", pOrder->TradingDay);
	//printf("��Ա���=[%s]\n", pOrder->ParticipantID);
	printf("Ͷ���߱��=[%s]\n", pOrder->InvestorID);
	//printf("�ͻ���=[%s]\n", pOrder->ClientID);
	printf("ϵͳ�������=[%s]\n", pOrder->OrderSysID);
	printf("���ر������=[%s]\n", pOrder->OrderLocalID);
	printf("ǰ�ñ��=[%d]\n", pOrder->FrontID);
	printf("�Ự���=[%d]\n", pOrder->SessionID);
	printf("�û����ر�����=[%s]\n", pOrder->OrderRef);
	printf("��Լ����=[%s]\n", pOrder->InstrumentID);
	printf("�����۸�����=[%c]\n", pOrder->OrderPriceType);
	printf("��������=[%c]\n", pOrder->Direction);
	printf("��ƽ��־=[%c]\n", pOrder->CombOffsetFlag[0]);
	printf("Ͷ���ױ���־=[%c]\n", pOrder->CombHedgeFlag[0]);
	printf("�۸�=[%lf]\n", pOrder->LimitPrice);
	printf("����=[%d]\n", pOrder->VolumeTotalOriginal);
	//printf("������Դ=[%c]\n", pOrder->OrderSource);
	printf("����״̬=[%c]\n", pOrder->OrderStatus);
	printf("����ʱ��=[%s]\n", pOrder->InsertTime);
	printf("����ʱ��=[%s]\n", pOrder->CancelTime);
	//printf("��Ч������=[%c]\n", pOrder->TimeCondition);
	//printf("GTD����=[%s]\n", pOrder->GTDDate);
	//printf("��С�ɽ���=[%d]\n", pOrder->MinVolume);
	//printf("ֹ���=[%lf]\n", pOrder->StopPrice);
	//printf("ǿƽԭ��=[%c]\n", pOrder->ForceCloseReason);
	//printf("�Զ������־=[%d]\n", pOrder->IsAutoSuspend);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("��������ر� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pInputOrder == NULL)
	{
		printf("û������\n");
		return;
	}
}

void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	printf("-----------------------------\n");
	printf("�յ��ɽ��ر�\n");
	Show(pTrade);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::Show(CThostFtdcTradeField *pTrade)
{
	printf("-----------------------------\n");
	printf("����������=[%s]\n", pTrade->ExchangeID);
	printf("������=[%s]\n", pTrade->TradingDay);
	printf("��Ա���=[%s]\n", pTrade->ParticipantID);
	printf("Ͷ���߱��=[%s]\n", pTrade->InvestorID);
	printf("�ͻ���=[%s]\n", pTrade->ClientID);
	printf("�ɽ����=[%s]\n", pTrade->TradeID);
	printf("�û����ز�����=[%s]\n", pTrade->OrderRef);
	printf("���ر������=[%s]\n", pTrade->OrderLocalID);
	printf("��Լ����=[%s]\n", pTrade->InstrumentID);
	printf("��������=[%c]\n", pTrade->Direction);
	printf("��ƽ��־=[%c]\n", pTrade->OffsetFlag);
	printf("Ͷ���ױ���־=[%c]\n", pTrade->HedgeFlag);
	printf("�ɽ��۸�=[%lf]\n", pTrade->Price);
	printf("�ɽ�����=[%d]\n", pTrade->Volume);
	printf("�����Ա���=[%s]\n", pTrade->ClearingPartID);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pRspInvestorAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("��ѯͶ�����˻�ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pRspInvestorAccount == NULL)
	{
		printf("û�в�ѯ��Ͷ�����˻�\n");
		return;
	}
	printf("-----------------------------\n");
	printf("���͹�˾���=[%s]\n", pRspInvestorAccount->BrokerID);
	printf("�ʽ��ʺ�=[%s]\n", pRspInvestorAccount->AccountID);
	printf("�ϴν���׼����=[%lf]\n", pRspInvestorAccount->PreBalance);
	printf("�����=[%lf]\n", pRspInvestorAccount->Deposit);
	printf("������=[%lf]\n", pRspInvestorAccount->Withdraw);
	printf("����ı�֤��=[%lf]\n", pRspInvestorAccount->FrozenMargin);
	printf("����������=[%lf]\n", pRspInvestorAccount->FrozenCommission);
	printf("������=[%lf]\n", pRspInvestorAccount->Commission);
	printf("ƽ��ӯ��=[%lf]\n", pRspInvestorAccount->CloseProfit);
	printf("�ֲ�ӯ��=[%lf]\n", pRspInvestorAccount->PositionProfit);
	printf("�����ʽ�=[%lf]\n", pRspInvestorAccount->Available);
	printf("��ȡ�ʽ�=[%lf]\n", pRspInvestorAccount->WithdrawQuota);
	printf("��������֤��=[%lf]\n", pRspInvestorAccount->ExchangeMargin);
	printf("��֤���ܶ�=[%lf]\n", pRspInvestorAccount->CurrMargin);
	printf("��Ȩ��=[%lf]\n", pRspInvestorAccount->CurrMargin + pRspInvestorAccount->FrozenMargin + pRspInvestorAccount->Available + pRspInvestorAccount->FrozenCommission
		+ pRspInvestorAccount->CloseProfit + pRspInvestorAccount->PositionProfit + pRspInvestorAccount->Deposit - pRspInvestorAccount->Withdraw);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("��ѯ������ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pExchange == NULL)
	{
		printf("û�в�ѯ����������Ϣ\n");
		return;
	}
	printf("-----------------------------\n");
	printf("[%s]\n", pExchange->ExchangeID);
	printf("[%s]\n", pExchange->ExchangeName);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("��ѯͶ���ֲ߳�ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		return;
	}

	if (pInvestorPosition == NULL)
	{
		printf("û�в�ѯ��Ͷ���ֲ߳�\n");
		return;
	}
	printf("-----------------------------\n");
	printf("���͹�˾���=[%s]\n", pInvestorPosition->BrokerID);
	printf("Ͷ���߱��=[%s]\n", pInvestorPosition->InvestorID);
	printf("������=[%s]\n", pInvestorPosition->TradingDay);
	printf("��Լ����=[%s]\n", pInvestorPosition->InstrumentID);
	printf("��Լ��շ���=[%c]\n", pInvestorPosition->PosiDirection);
	printf("��ֲ���=[%d]\n", pInvestorPosition->Position);
	printf("�ֲ�����=[%c]\n", pInvestorPosition->PositionDate);
	printf("�ֲֳɱ�=[%lf]\n", pInvestorPosition->PositionCost);
	printf("�ֲ�ӯ��=[%lf]\n", pInvestorPosition->PositionProfit);
	printf("������=[%lf]\n", pInvestorPosition->Commission);
	return;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("��ѯ���ױ���ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		return;
	}
	if (pInstrument == NULL)
	{
		printf("û�в�ѯ����Լ����\n");
		return;
	}
	//Show(pInstrument);

	//�ļ���
	char filePath[100] = { '\0' };
	sprintf(filePath, "%s\\contract_table_%s.csv", trading_date, trading_date);

	ofstream outFile;
	outFile.open(filePath, ios::app); // �ļ�׷��д��   
	outFile << pInstrument->ExchangeID << ","
		<< pInstrument->InstrumentID << ","
		<< pInstrument->VolumeMultiple << ","
		<< pInstrument->PriceTick << ","
		<< pInstrument->ExpireDate
		<< endl;
	outFile.close();

	cout << pInstrument->InstrumentID << endl;
	instru_vec.push_back(pInstrument->InstrumentID);

	if (bIsLast){
		SubscribeMarketData_all();
	}
	return;
}

void CTraderSpi::Show(CThostFtdcInstrumentField *pRspInstrument)
{
	printf("-----------------------------\n");
	printf("����������=[%s]\n", pRspInstrument->ExchangeID);
	printf("��Ʒ����=[%s]\n", pRspInstrument->ProductID);
	printf("��Լ����=[%s]\n", pRspInstrument->InstrumentID);
	printf("��Լ����=[%s]\n", pRspInstrument->InstrumentName);
	printf("�������=[%d]\n", pRspInstrument->DeliveryYear);
	printf("������=[%d]\n", pRspInstrument->DeliveryMonth);
	printf("�޼۵�����µ���=[%d]\n", pRspInstrument->MaxLimitOrderVolume);
	printf("�޼۵���С�µ���=[%d]\n", pRspInstrument->MinLimitOrderVolume);
	printf("�м۵�����µ���=[%d]\n", pRspInstrument->MaxMarketOrderVolume);
	printf("�м۵���С�µ���=[%d]\n", pRspInstrument->MinMarketOrderVolume);

	printf("��������=[%d]\n", pRspInstrument->VolumeMultiple);
	printf("��С�䶯��λ=[%lf]\n", pRspInstrument->PriceTick);

	printf("������=[%s]\n", pRspInstrument->CreateDate);
	printf("������=[%s]\n", pRspInstrument->OpenDate);
	printf("������=[%s]\n", pRspInstrument->ExpireDate);
	printf("��ʼ������=[%s]\n", pRspInstrument->StartDelivDate);
	printf("��󽻸���=[%s]\n", pRspInstrument->EndDelivDate);
	printf("��ǰ�Ƿ���=[%d]\n", pRspInstrument->IsTrading);
	printf("-----------------------------\n");
}

void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("��ѯ����ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pOrder == NULL)
	{
		printf("û�в�ѯ����������\n");
		return;
	}
	///ȫ���ɽ� '0'
	///���ֳɽ����ڶ����� '1'
	///���ֳɽ����ڶ����� '2'
	///δ�ɽ����ڶ����� '3'
	///δ�ɽ����ڶ����� '4'
	///���� '5'
	///δ֪ 'a'
	///��δ���� 'b'
	///�Ѵ��� 'c'
	//ֻ��ʾδ�ɽ����ڶ�����
	if (pOrder->OrderStatus == '3' || pOrder->OrderStatus == '1')
	{
		printf("-------δ�ɽ�����-------\n");
		Show(pOrder);
	}
	return;
}

void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("��ѯ�ɽ�ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pTrade == NULL)
	{
		printf("û�в�ѯ���ɽ�����");
		return;
	}
	Show(pTrade);
	return;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("ȷ�Ͻ��㵥ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	printf("-----------------------------\n");
	printf("--------���㵥��ȷ�ϣ�-------\n");
	printf("���ͱ��=[%s]\n", pSettlementInfoConfirm->BrokerID);
	printf("Ͷ���߱��=[%s]\n", pSettlementInfoConfirm->InvestorID);
	printf("ȷ������=[%s]\n", pSettlementInfoConfirm->ConfirmDate);
	printf("ȷ��ʱ��=[%s]\n", pSettlementInfoConfirm->ConfirmTime);
	printf("-----------------------------\n");
	return;
}

void CTraderSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != NULL&&pRspInfo->ErrorID != 0)
	{
		printf("-----------------------------\n");
		printf("��ѯ����ʧ�� ����ԭ��%s\n", pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pDepthMarketData == NULL)
	{
		printf("û�в�ѯ����������");
		return;
	}
	printf("-----------------------------\n");
	printf("������=[%s]\n", pDepthMarketData->TradingDay);
	printf("����������=[%s]\n", pDepthMarketData->ExchangeID);
	printf("��Լ����=[%s]\n\n", pDepthMarketData->InstrumentID);

	//printf("�ǵ�����=[%.2f%%]\n", 100 * (pDepthMarketData->LastPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("���¼�=[%.2lf]  [% .2f%%]\n", pDepthMarketData->LastPrice, 100 * (pDepthMarketData->LastPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("��һ�۸�=[%.2lf]   ��һ��=[%.0lf]\n", pDepthMarketData->AskPrice1, pDepthMarketData->AskVolume1);
	printf("��һ�۸�=[%.2lf]   ��һ��=[%.0lf]\n\n", pDepthMarketData->BidPrice1, pDepthMarketData->BidVolume1);

	printf("��ͣ���=[%.2lf]  [% .2f%%]\n", pDepthMarketData->UpperLimitPrice, 100 * (pDepthMarketData->UpperLimitPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("��ͣ���=[%.2lf]  [% .2f%%]\n", pDepthMarketData->LowerLimitPrice, 100 * (pDepthMarketData->LowerLimitPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("���̼�=[%.2lf]  [% .2f%%]\n", pDepthMarketData->OpenPrice, 100 * (pDepthMarketData->OpenPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("����߼�=[%.2lf]  [% .2f%%]\n", pDepthMarketData->HighestPrice, 100 * (pDepthMarketData->HighestPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("����ͼ�=[%.2lf]  [% .2f%%]\n", pDepthMarketData->LowestPrice, 100 * (pDepthMarketData->LowestPrice / pDepthMarketData->PreSettlementPrice - 1));
	printf("������=[%.2lf]\n", pDepthMarketData->PreSettlementPrice);
	printf("�����̼�=[%.2lf]\n", pDepthMarketData->PreClosePrice);
	printf("�ɽ���=[%d]\n", pDepthMarketData->Volume);
	printf("�ֲ���=[%d]\n", pDepthMarketData->OpenInterest);
	printf("-----------------------------\n");
	return;
}


void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printf("OnRspError:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	// �ͻ�������д�����
}
