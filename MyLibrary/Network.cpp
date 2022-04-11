//#include "Network.h"
//#include"ErrorProcess.h"
//
//#include<Windows.h>
//
//MelLib::Network* MelLib::Network::GetInstance()
//{
//	static Network n;
//	return &n;
//}
//
//bool MelLib::Network::Initialize()
//{
//	int result = 0;
//
//	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if(result != 0)
//	{
//		ErrorProcess::GetInstance()->StartErroeProcess(L"Networkクラスの初期化に失敗しました。", true);
//#ifdef _DEBUG
//		OutputDebugString(L"Networkクラスの初期化に失敗しました。\n");
//#endif // _DEBUG
//
//		return false;
//	}
//}
//
//void MelLib::Network::Finalize()
//{
//	WSACleanup();
//}
//
//MelLib::Network::NetWorkError MelLib::Network::StartServer(const USHORT portNum)
//{
//	//待機用ソケット
//	SOCKET serverPreAcceptSocket = {};
//
//	// IPアドレスやポート番号を保持する構造体
//	SOCKADDR_IN serverDDR = {};
//	SOCKADDR_IN clientDDR = {};
//
//	// リスンソケットオープン
//	serverPreAcceptSocket = socket(AF_INET, SOCK_STREAM, 0);
//	if(serverPreAcceptSocket == INVALID_SOCKET)
//	{
//#ifdef _DEBUG
//		OutputDebugString(L"ソケットのオープンに失敗しました。\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_SOCKET_OPEN;
//	}
//
//	// ソケットに名前を付ける
//	memset(&serverDDR, 0, sizeof(serverDDR));
//	serverDDR.sin_family = AF_INET;
//
//	//htons ホストバイトオーダー(HBO)からネットワークバイトオーダー(NBO)に変換
//	serverDDR.sin_port = htons(portNum);
//	serverDDR.sin_addr.s_addr = INADDR_ANY;
//
//	//bind ローカルアドレス(サーバー側のSOCKADDR_IN)とソケットの関連付け
//	if(bind(serverPreAcceptSocket,(struct sockaddr*)&serverDDR,sizeof(serverDDR)) == SOCKET_ERROR)
//	{
//		closesocket(serverPreAcceptSocket);
//#ifdef _DEBUG
//		OutputDebugString(L"バインドに失敗しました。\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_BIND;
//	}
//
//	//listen ソケットの接続待機開始
//	if(listen(serverPreAcceptSocket,0) == SOCKET_ERROR)
//	{
//		closesocket(serverPreAcceptSocket);
//#ifdef _DEBUG
//		OutputDebugString(L"ソケットの接続待ち開始に失敗しました。\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_START_SOCKET_CONNECT_STANDBY;
//	}
//
//	//accept 接続許可をする
//	int clientDDRSize = sizeof(clientDDR);
//	serverAcceptSocket = accept(serverPreAcceptSocket, (SOCKADDR*)&clientDDR, &clientDDRSize);
//	if(serverAcceptSocket == INVALID_SOCKET)
//	{
//		closesocket(serverPreAcceptSocket);
//#ifdef _DEBUG
//		OutputDebugString(L"接続に失敗しました。\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_CONNECT;
//	}
//
//	//待機用のソケットを閉じる
//	closesocket(serverPreAcceptSocket);
//
//	return NetWorkError::ERROR_NONE;
//}
//
//MelLib::Network::NetWorkError MelLib::Network::StartClient(const std::string& address, const USHORT portNum)
//{
//	SOCKADDR_IN clientDDR = {};
//
//	//ソケットオープン
//	clientAcceptSocket = socket(AF_INET, SOCK_STREAM, 0);
//	if(clientAcceptSocket == INVALID_SOCKET)
//	{
//#ifdef _DEBUG
//		OutputDebugString(L"ソケットのオープンに失敗しました。\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_SOCKET_OPEN;
//	}
//
//	//HOSTENT ホストの情報をまとめたもの
//	HOSTENT* hostent = nullptr;
//
//	//gethostbyname サーバー名でサーバーを取得する関数?
//	hostent = gethostbyname(address.c_str());
//	if(!hostent)
//	{
//		UINT addr = inet_addr(address.c_str());
//		
//		//gethostbyaddr IPアドレスでサーバーを取得する関数?
//		hostent = gethostbyaddr(address.c_str(),4,AF_INET);
//		if(!hostent)
//		{
//#ifdef _DEBUG
//			OutputDebugString(L"ホストが見つかりませんでした。\n");
//#endif // _DEBUG
//			return NetWorkError::FAILED_FIND_HOST;
//		}
//	}
//
//	//ソケットをサーバーに接続
//	//clientDDRの先頭に0をsizeof(SOCKADDR_IN)の値分追加
//	memset(&clientDDR, 0, sizeof(SOCKADDR_IN));
//	clientDDR.sin_family = hostent->h_addrtype;
//	clientDDR.sin_port = htons(portNum);
//	clientDDR.sin_addr.s_addr = *((ULONG*)hostent->h_addr);
//	if(connect(clientAcceptSocket,(SOCKADDR*)&clientDDR,sizeof(clientDDR)) == SOCKET_ERROR)
//	{
//#ifdef _DEBUG
//		OutputDebugString(L"サーバーに接続できませんでした。\n");
//#endif // _DEBUG
//		return NetWorkError::FAILED_CONNECT;
//	}
//
//	return NetWorkError::ERROR_NONE;
//}
