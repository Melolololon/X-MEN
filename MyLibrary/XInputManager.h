//#pragma once
//#include<Windows.h>
//#include<Xinput.h>
//#pragma comment(lib,"Xinput9_1_0.lib")
//
//
//enum class XInputButton
//{
//	UP_BUTTON = 0x0001,//十字キー上
//	DOWN_BUTTON = 0x0002,//十字キー下
//	LEFT_BUTTON = 0x0004,//十字キー左
//	RIGHT_BUTTON = 0x0008,//十字キー右
//	START_BUTTON = 0x00010,
//	BACK_BUTTON = 0x00020,
//	A_BUTTON = 0x1000,//PSの× SwitchのB
//	B_BUTTON = 0x2000,//PSの○ SwitchのA
//	X_BUTTON = 0x4000,//PSの□ SwitchのY
//	Y_BUTTON = 0x8000,//PSの△ SwitchのX
//	R_STICK_BUTTON = 0x0040,//PSのR3
//	L_STICK_BUTTON = 0x0080,//PSのL3
//	RB_BUTTON = 0x0200,//PSのR1 SwitchのR
//	LB_BUTTON = 0x0100,//PSのL1 SwitchのL
//};
//
//class XInputManager
//{
//private:
//	XInputManager(){}
//	~XInputManager(){}
//
//	static XINPUT_STATE padState[4];
//	static XINPUT_STATE padPrevious[4];
//
//	static bool padConnected[4];
//
//	/// <summary>
//	/// パッド番号が3を超えていないかと、接続されているかを確認します。
//	/// </summary>
//	/// <param name="padNum"></param>
//	/// <returns></returns>
//	static bool PadCheck(const UCHAR& padNum);
//
//	//軸の最高値
//    static const short MAX_AXIS_VALUE = 32767;
//	//振動の最高値
//	static const WORD MAX_VIBRATION_VALUE = 65535;
//
//public:
//	static void Update();
//
//	static bool GetPadConnectedFlag(const UCHAR& padNum);
//
//#pragma region ボタン
//	static bool ButtonState(const XInputButton& button,const UCHAR& padNum);
//	static bool ButtonTrigger(const XInputButton& button,const UCHAR& padNum);
//	static bool ButtonRelease(const XInputButton& button,const UCHAR& padNum);
//
//	/// <summary>
//	/// 十字ボタンが示す角度を取得します。
//	/// </summary>
//	/// <returns></returns>
//	static float DirectionalButtonAngle(const UCHAR& padNum);
//#pragma endregion
//
//#pragma region アナログスティック
//	static bool LeftStickLeft(const float& lXPar , const UCHAR& padNum);
//	static bool LeftStickRight(const float&  lXPar, const UCHAR& padNum);
//	static bool LeftStickUp(const float&  lYPar, const UCHAR& padNum);
//	static bool LeftStickDown(const float&  lYPar, const UCHAR& padNum);
//	static bool RightStickLeft(const float&  lXPar, const UCHAR& padNum);
//	static bool RightStickRight(const float&  lXPar, const UCHAR& padNum);
//	static bool RightStickUp(const float&  lYPar, const UCHAR& padNum);
//	static bool RightStickDown(const float&  lYPar, const UCHAR& padNum);
//
//	/// <summary>
//	/// 左スティックが何度かを角度で返します。スティックが傾いていない場合、-1を返します。
//	/// </summary>
//	/// <returns></returns>
//	static float LeftStickAngle(const UCHAR& padNum);
//
//	/// <summary>
//	/// 右スティックが何度かを角度で返します。スティックが傾いていない場合、-1を返します。
//	/// </summary>
//	/// <returns></returns>
//	static float RightStickAngle(const UCHAR& padNum);
//#pragma endregion
//
//#pragma region 振動
//	/// <summary>
//	/// ゲームパッドを振動させます
//	/// </summary>
//	/// <param name="leftPar">左側の振動量(0%～100%)</param>
//	/// <param name="rightPar">右側の振動量(0%～100%)</param>
//	/// <param name="padNum">パッド番号(1Pや2Pのこと)</param>
//	static void PadVibration(const float& leftPar,const float& rightPar,const UCHAR& padNum);
//
//#pragma endregion
//
//};
//
