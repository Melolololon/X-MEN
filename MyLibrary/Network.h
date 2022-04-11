//#pragma once
//#include<winsock2.h>
//#pragma comment(lib,"Ws2_32.lib")
//
//namespace MelLib 
//{
//	//チュートリアル
//	// https://docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock
//
//	// WinSock2を利用した通信クラス
//	class Network
//	{
//	public:
//		enum class NetWorkError
//		{
//			ERROR_NONE,
//			FAILED_SOCKET_OPEN,
//			FAILED_BIND,
//			FAILED_START_SOCKET_CONNECT_STANDBY,
//			FAILED_CONNECT,
//			FAILED_FIND_HOST,
//		};
//
//	private:
//		WSADATA wsaData = {};
//
//		SOCKET serverAcceptSocket = {};
//		SOCKET clientAcceptSocket = {};
//
//	private:
//		Network() {}
//		~Network() {}
//	public:
//		Network(Network& n) = delete;
//		Network operator=(Network& n) = delete;
//		static Network* GetInstance();
//
//		bool Initialize();
//		void Finalize();
//
//#pragma region 受信側(サーバー)
//		NetWorkError StartServer(const USHORT portNum);
//#pragma endregion
//
//#pragma region 送信側
//		NetWorkError StartClient(const std::string& address, const USHORT portNum);
//#pragma endregion
//
//
//	};
//
//}