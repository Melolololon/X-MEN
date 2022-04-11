//#pragma once
//
//#define DIRECTINPUT_VERSION 0x0800
//#include<dinput.h>
//#pragma comment(lib,"dinput8.lib")
//#pragma comment(lib,"dxguid.lib")
//#include<Windows.h>
//#include<vector>
//#include"Vector.h"
//
//#include<DirectXMath.h>
//
//
//#pragma region パッドのボタン
////これグローバル定数にする?
//const int XButton = 0;
//const int YButton = 1;
//const int AButton = 2;
//const int BButton = 3;
//const int LBButton = 4;
//const int RBButton = 5;
//const int LTButton = 6;
//const int RTButton = 7;
//const int StartButton = 11;
//#pragma endregion
//#pragma region 十字キー
////enumに変える
//const int UpButton = 0;
//const int RightButton = 1;
//const int DownButton = 2;
//const int LeftButton = 3;
//#pragma endregion
//
//#pragma region マウスボタン
//
//enum MouseButton
//{
//	MOUSE_BUTTON_LEFT,
//	MOUSE_BUTTON_RIGHT,
//	MOUSE_BUTTON_CENTER,
//	//MOUSE_BUTTON_SIDE_DOWN,
//
//};
//#pragma endregion
//
//
//class DirectInput
//{
//private:
//	DirectInput(){}
//	~DirectInput(){}
//
//	//列挙用コールバック
//	static BOOL CALLBACK diEnumDevicesCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
//	static BOOL CALLBACK diEnumDevicesObjectCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
//
//	static HINSTANCE hInstance;
//	static HRESULT dIResult;//結果取得
//	static LPDIRECTINPUT8 dInput8;//dxInputを入れる変数
//	static LPDIRECTINPUTDEVICE8 keyDev;//デバイスを入れる変数
//
//	static IDirectInput8* dInputDevice;
//	static IDirectInputDevice8* devKeyBoard;
//	static IDirectInputDevice8* devMouse;
//	static IDirectInputDevice8* devPad;
//
//	//static IDirectInputDevice8* padSetUp;
//	//static DIDEVICEOBJECTDATA* lpdidod;
//	static std::vector<DIDEVICEINSTANCE>getEnum;
//	static DIDEVCAPS padCaps;
//
//
//	//パッド
//	static DIJOYSTATE padState;//パッド情報
//	static DIJOYSTATE padPrevious;//パッド情報(1ループ前)
//
//
//	//現在のキー情報
//	static BYTE key[256];
//	//1フレーム前のキー情報
//	static BYTE preKey[256];
//
//	//マウス
//	static DIMOUSESTATE mouseState;
//	static DIMOUSESTATE mousePrevious;
//
//
//	//取得したウィンドウハンドル
//	static HWND mHwnd;
//
//	//パッドを認識したかどうか
//	static bool setPad;
//
//	//ウィンドウサイズ
//	static int winWidth;
//	static int winHeight;
//
//	//行列
//	static DirectX::XMMATRIX viewMat;
//	static DirectX::XMMATRIX projectionMat;
//
//	//カメラ
//	static float nearNum;
//	static float farNum;
//
//public:
//
//
//	//生成
//	static void Initialize(HWND hwnd,int windowWidth,int windowHeight);
//	//情報取得
//	static void Update();
//
//
//	//DInputの開放
//	static void Release();
//
//#pragma region キーボード
//
//	//押しているか
//	static bool KeyState(const BYTE& keyDef);
//	//押した瞬間か
//	static bool KeyTrigger(const BYTE& keyDef);
//	//離した瞬間か
//	static bool KeyRelease(const BYTE& keyDef);
//
//	/// <summary>
//	/// キーの文字を返します
//	/// </summary>
//	/// <returns></returns>
//	static char KeyChar();
//
//	/// <summary>
//	/// アローキーが何度を示しているかを角度で返します。	
//	/// </summary>
//	/// <returns></returns>
//	static float ArrowKeyAngle();
//#pragma endregion
//
//#pragma region パッド
//#pragma region 通常ボタン
//
//	//押しているか
//	static bool ButtonState(int button);
//	//押した瞬間か
//	static bool ButtonTrigger(int button);
//	//離した瞬間か
//	static bool ButtonRelease(int button);
//#pragma endregion
//
//#pragma region 十字キー
//
//	static bool DirectionalButtonState(int button);
//	static bool DirectionalButtonTrigger(int button);
//#pragma endregion
//
//#pragma endregion
//
//#pragma region スティック
//	/// <summary>
//	/// 
//	/// </summary>
//	/// <param name="lNum">軸(32767が最高値)</param>
//	/// <returns></returns>
//	static bool LeftStickLeft(int lXNum);
//	static bool LeftStickRight(int lXNum);
//	static bool LeftStickUp(int lYNum);
//	static bool LeftStickDown(int lYNum);
//	static bool RightStickLeft(int lXNum);
//	static bool RightStickRight(int lXNum);
//	static bool RightStickUp(int lYNum);
//	static bool RightStickDown(int lYNum);
//
//	
//#pragma endregion
//
//#pragma region マウス
//
//#pragma region カーソル関係
//
//	static void SetMatrixAndNearFar(DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,float nearNumber ,float farNumber);
//
//	static void SetViewMatrix(const DirectX::XMMATRIX& viewMatrix) { viewMat = viewMatrix; }
//	static void SetProjectionMatrix(const DirectX::XMMATRIX& projectionMatrix) { projectionMat = projectionMatrix; }
//	static void SetNear(const float nearNumber) { nearNum = nearNumber; }
//	static void SetFar(const float farNumber) { farNum = farNumber; }
//
//	/// <summary>
//	/// クライアント座標に変換し、最初に設定したウィンドウサイズに応じて補正したマウスの座標を取得します
//	/// </summary>
//	/// <returns></returns>
//	static Vector2 GetMousePosition();
//
//	/// <summary>
//	/// ウィンドウの中心を基準としたカーソルのベクトルを取得します。
//	/// </summary>
//	/// <returns>ベクトル</returns>
//	static Vector2 GetCenterToMouseVector();
//
//	/// <summary>
//	/// ウィンドウの中心を基準としたカーソルの角度を取得します。
//	/// </summary>
//	/// <returns>角度</returns>
//	static float GetMouseAngle();
//#pragma endregion
//
//
//	/// <summary>
//	/// マウスカーソルから出ている線分の最近点と最遠点を取得します
//	/// </summary>
//	/// <param name="nearPoint"></param>
//	/// <param name="farPoint"></param>
//	static void GetMouse3DLine(Vector3& nearPoint,Vector3& farPoint);
//
//	static bool MouseButtonState(MouseButton mouseButton);
//
//	static bool MouseButtonTrigger(MouseButton mouseButton);
//	
//	static bool MouseButtonRelease(MouseButton mouseButton);
//
//#pragma endregion
//
//
//
//};
//
//
//
