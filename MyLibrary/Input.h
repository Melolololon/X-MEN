#pragma once

#include<Windows.h>
#include<Xinput.h>
#pragma comment(lib,"Xinput9_1_0.lib")


#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include<DirectXMath.h>
#include<unordered_map>

#include"Vector.h"
#include"Camera.h"

namespace MelLib
{
	// ゲームパッドのボタンの列挙
	enum class PadButton
	{
		UP = 0x0001,//十字キー上
		DOWN = 0x0002,//十字キー下
		LEFT = 0x0004,//十字キー左
		RIGHT = 0x0008,//十字キー右
		START = 0x00010,//スタートボタン PS4以降のOPTION
		BACK = 0x00020,//バックボタン PS(1～3)、SwitchのStart PS4以降のSHARE
		A = 0x1000,//PSの× SwitchのB
		B = 0x2000,//PSの○ SwitchのA
		X = 0x4000,//PSの□ SwitchのY
		Y = 0x8000,//PSの△ SwitchのX
		L_STICK = 0x0040,//PSのL3
		R_STICK = 0x0080,//PSのR3
		RB = 0x0200,//PSのR1 SwitchのR
		LB = 0x0100,//PSのL1 SwitchのL
	};

	enum class MouseButton
	{
		LEFT,
		RIGHT,
		CENTER,//ホイール押し込み
	};

	// Input リファレンス
	// https://sites.google.com/view/melgames/%E8%87%AA%E4%BD%9C%E3%83%A9%E3%82%A4%E3%83%96%E3%83%A9%E3%83%AA/%E3%83%AA%E3%83%95%E3%82%A1%E3%83%AC%E3%83%B3%E3%82%B9/%E4%BD%BF%E7%94%A8%E4%BB%BB%E6%84%8F%E3%82%AF%E3%83%A9%E3%82%B9/%E5%85%A5%E5%8A%9B

	// メモエリア
	// GetPressKeyChars使わずに、WinAPIのエディットボックスで文字取得するようにする?
	// エディットボックスを透明にできるか試す
	// そもそも子ウィンドウ扱いだからゲーム画面に表示できない?できる?
	// できなかった
	// もしかして隠れてても入力はできる?ウィンドウタイプで設定すればいける

	// デバイスごとにクラス分けしてもいいかも。
	// Inputは各デバイスの処理の呼び出しをするクラスにする

	// 入力クラス
	class Input
	{

	private:
		Input() {}
		~Input() {}

#pragma region DirectInput


		static HINSTANCE hInstance;
		static HRESULT dIResult;//結果取得
		static LPDIRECTINPUT8 dInput8;//dxInputを入れる変数
		static LPDIRECTINPUTDEVICE8 keyDev;//デバイスを入れる変数

		static IDirectInput8* dInputDevice;
		static IDirectInputDevice8* devKeyBoard;
		static IDirectInputDevice8* devMouse;

		//現在のキー情報
		static BYTE keysState[256];

		//1フレーム前のキー情報
		static BYTE preKeysState[256];

		//現在押されているキー
		static std::vector<BYTE> currentPressKeys;

		//GetPressKeyChars()から文字列を取得したかどうか
		static bool callGetPressKeyChars;
		//前フレームでGetPressKeyChars()から文字列を取得したかどうか
		static bool callPreGetPressKeyChars;


		static std::unordered_map<std::string, HWND>hwnds;
		//キーを押した時に入力文字を返すための配列
		static std::vector<std::unordered_map<UCHAR, char>>returnChars;

		//パッドでの入力に使えるかもと思い、用意したもの
		static const std::vector<wchar_t>hiragana;
		static const std::vector<wchar_t>katakana;


		//マウス
		static DIMOUSESTATE mouseState;
		static DIMOUSESTATE mousePrevious;

		//取得したウィンドウハンドル
		static HWND mHwnd;

		static LRESULT ParamChildWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#pragma endregion

#pragma region XInput


		static XINPUT_STATE padState[4];
		static XINPUT_STATE padPrevious[4];


		static bool padConnected[4];

		//軸の最高値
		static const short MAX_AXIS_VALUE = 32767;
		//振動の最高値
		static const WORD MAX_VIBRATION_VALUE = 65535;
#pragma endregion

		//行列
		static DirectX::XMMATRIX viewMat;
		static DirectX::XMMATRIX projectionMat;

		//カメラ
		static float nearNum;
		static float farNum;


		//ウィンドウサイズ
		static int winWidth;
		static int winHeight;

		/// <summary>
		/// パッド番号が3を超えていないかと、接続されているかを確認します。
		/// </summary>
		/// <param name="padNum"></param>
		/// <returns></returns>
		static bool PadCheck(UCHAR padNum);


	public:
		static void Initialize(HWND hwnd, int windowWidth, int windowHeight);
		static void Update();
		static void Finalize();

		/// <summary>
		/// 指定したパッドが接続されているかを確認します。
		/// </summary>
		/// <param name="padNum"></param>
		/// <returns></returns>
		static bool GetPadConnectedFlag(UCHAR padNum);


#pragma region キーボード

		/// <summary>
		/// キーが押されていたらtrueを返します。
		/// </summary>
		/// <param name="keyDef"></param>
		/// <returns></returns>
		static bool KeyState(BYTE keyDef);

		/// <summary>
		/// キーが押された瞬間(押されたフレーム)だったらtrueを返します。
		/// </summary>
		/// <param name="keyDef"></param>
		/// <returns></returns>
		static bool KeyTrigger(BYTE keyDef);

		/// <summary>
		/// キーを押すのをやめた瞬間(やめたフレーム)だったらtrueを返します。
		/// </summary>
		/// <param name="keyDef"></param>
		/// <returns></returns>
		static bool KeyRelease(BYTE keyDef);


#pragma region 文字取得
		//GetPressKeyCharsとかは気軽に文字取得できるし、
		//タイピングゲームみたいに枠用意する必要ないときや、必要ないときに使えるから残しといていい

		/// <summary>
		/// 現在押されているキーの文字を返します。
		/// </summary>
		/// <returns></returns>
		static std::string GetPressKeyChars();

		/// <summary>
		/// 押された瞬間にキーの文字を返します。
		/// </summary>
		/// <returns></returns>
		static std::string GetTriggerKeyChars();


		static void CreateStringInputWindow
		(
			const std::wstring& initStr,
			const Vector2& position,
			const Vector2& size,
			HWND parentHWND,
			const std::string& name
		);

		static void DeleteStringInputWindow(const std::string& name);

		static std::wstring GetInputString(const std::string& name);

#pragma endregion
		/// <summary>
		/// 現在押されているキーを取得します。
		/// </summary>
		/// <returns></returns>
		static const std::vector<BYTE>& GetStateKeys() { return currentPressKeys; }

		/// <summary>
		/// アローキーが何度を示しているかを角度で返します。	
		/// </summary>
		/// <returns></returns>
		static float ArrowKeyAngle();
#pragma endregion

#pragma region マウス


#pragma region カーソル


		static void SetViewMatrix(const DirectX::XMMATRIX& viewMatrix) { viewMat = viewMatrix; }
		static void SetProjectionMatrix(const DirectX::XMMATRIX& projectionMatrix) { projectionMat = projectionMatrix; }
		static void SetNear(float nearNumber) { nearNum = nearNumber; }
		static void SetFar(float farNumber) { farNum = farNumber; }

		/// <summary>
		/// クライアント座標に変換し、最初に設定したウィンドウサイズに応じて補正したマウスの座標を取得します
		/// </summary>
		/// <returns></returns>
		static Vector2 GetMousePosition();

		/// <summary>
		/// ウィンドウの中心を基準としたカーソルのベクトルを取得します。
		/// </summary>
		/// <returns>ベクトル</returns>
		static Vector2 GetCenterToMouseVector();

		/// <summary>
		/// ウィンドウの中心を基準としたカーソルの角度を取得します。
		/// </summary>
		/// <returns>角度</returns>
		static float GetMouseAngle();


		/// <summary>
		/// マウスカーソルから出ている線分の最近点と最遠点を取得します
		/// </summary>
		/// <param name="nearPoint"></param>
		/// <param name="farPoint"></param>
		static void GetMouse3DLine(Vector3& nearPoint, Vector3& farPoint);

		/// <summary>
		/// カメラの情報を元に、マウスカーソルを座標変換したときの最近点と最遠点を求めます。
		/// </summary>
		/// <param name="pCamera"></param>
		/// <param name="pNear"></param>
		/// <param name="pFar"></param>
		static void GetMouse3DLine2
		(
			Camera* pCamera,
			Vector3* pNear,
			Vector3* pFar
		);
#pragma endregion


#pragma region ボタン


		/// <summary>
		/// マウスのボタンを取得します。
		/// </summary>
		/// <param name="mouseButton"></param>
		/// <returns></returns>
		static bool MouseButtonState(MouseButton mouseButton);

		/// <summary>
		/// マウスのボタンが押された瞬間(押されたフレーム)かどうかを取得します。
		/// </summary>
		/// <param name="mouseButton"></param>
		/// <returns></returns>
		static bool MouseButtonTrigger(MouseButton mouseButton);

		/// <summary>
		/// マウスのボタンが離された瞬間(押すのを止めたフレーム)かどうかを取得します。
		/// </summary>
		/// <param name="mouseButton"></param>
		/// <returns></returns>
		static bool MouseButtonRelease(MouseButton mouseButton);


#pragma endregion

#pragma endregion

#pragma region ゲームパッド


#pragma region ボタン
		static bool PadButtonState(PadButton button, UCHAR padNum = 1);
		static bool PadButtonTrigger(PadButton button, UCHAR padNum = 1);
		static bool PadButtonRelease(PadButton button, UCHAR padNum = 1);

		/// <summary>
		/// 十字ボタンが示す角度を取得します。
		/// </summary>
		/// <returns></returns>
		static float DirectionalButtonAngle(UCHAR padNum = 1);
#pragma endregion

#pragma region アナログスティック

#pragma region 左

#pragma region 状態

		static bool LeftStickLeft(float lXPar, UCHAR padNum = 1);
		static bool LeftStickRight(float lXPar, UCHAR padNum = 1);
		static bool LeftStickUp(float lYPar, UCHAR padNum = 1);
		static bool LeftStickDown(float lYPar, UCHAR padNum = 1);

#pragma endregion

#pragma region トリガー

		/// <summary>
		/// 前フレームのスティックの傾きの割合がlXPar以下、現在のフレームの傾きがlXPar以上だったらtrueを返します。
		/// </summary>
		/// <param name="lXPar">trueを返す基準</param>
		/// <param name="padNum">パッド番号</param>
		/// <returns></returns>
		static bool LeftStickLeftTrigger(float lXPar, UCHAR padNum = 1);

		static bool LeftStickRightTrigger(float lXPar, UCHAR padNum = 1);

		static bool LeftStickUpTrigger(float lXPar, UCHAR padNum = 1);

		static bool LeftStickDownTrigger(float lXPar, UCHAR padNum = 1);


#pragma endregion


		/// <summary>
		/// 左スティックの角度を返します。スティックが傾いていない場合、-1を返します。
		/// </summary>
		/// <returns></returns>
		static float LeftStickAngle(UCHAR padNum = 1);

		/// <summary>
		/// スティックが傾いている方向をVector2で返します。
		/// </summary>
		static Vector2 LeftStickVector2(bool dimention3D, UCHAR padNum = 1);

		/// <summary>
		/// スティックが傾いている方向をVector3で返します。
		/// </summary>
		static Vector3 LeftStickVector3(Camera* pCamera = nullptr, bool rotX = false, bool rotY = false, UCHAR padNum = 1);
#pragma endregion

#pragma region 右

#pragma region 状態

		static bool RightStickLeft(float lXPar, UCHAR padNum = 1);
		static bool RightStickRight(float lXPar, UCHAR padNum = 1);
		static bool RightStickUp(float lYPar, UCHAR padNum = 1);
		static bool RightStickDown(float lYPar, UCHAR padNum = 1);
#pragma endregion

#pragma region トリガー
		static bool RightStickLeftTrigger(float lXPar, UCHAR padNum = 1);

		static bool RightStickRightTrigger(float lXPar, UCHAR padNum = 1);

		static bool RightStickUpTrigger(float lYPar, UCHAR padNum = 1);

		static bool RightStickDownTrigger(float lYPar, UCHAR padNum = 1);
#pragma endregion



		/// <summary>
		/// 右スティックが何度かを角度で返します。スティックが傾いていない場合、-1を返します。
		/// </summary>
		/// <returns></returns>
		static float RightStickAngle(UCHAR padNum = 1);


		static Vector2 RightStickVector2(bool dimention3D, UCHAR padNum = 1);
		static Vector3 RightStickVector3(Camera* pCamera = nullptr, bool rotX = false, bool rotY = false, UCHAR padNum = 1);
#pragma endregion


#pragma endregion

#pragma region 振動
		/// <summary>
		/// ゲームパッドを振動させます
		/// </summary>
		/// <param name="leftPar">左側の振動量(0%～100%)</param>
		/// <param name="rightPar">右側の振動量(0%～100%)</param>
		/// <param name="padNum">パッド番号(1Pや2Pのこと)</param>
		static void PadVibration(float leftPar, float rightPar, UCHAR padNum = 1);

#pragma endregion
#pragma endregion

	};
}