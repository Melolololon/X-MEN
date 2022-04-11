//#include "DirectInput.h"
//#include"LibMath.h"
//
//HINSTANCE DirectInput::hInstance;//ハンドル
//HRESULT DirectInput::dIResult;//結果取得
//LPDIRECTINPUT8 DirectInput::dInput8;//dxInputを入れる変数
//LPDIRECTINPUTDEVICE8 DirectInput::keyDev;//デバイスを入れる変数
//
//
//IDirectInput8* DirectInput::dInputDevice;
//IDirectInputDevice8* DirectInput::devKeyBoard;
//IDirectInputDevice8* DirectInput::devMouse;
//IDirectInputDevice8* DirectInput::devPad;
//
////IDirectInputDevice8* DirectInput::padSetUp;
////DIDEVICEOBJECTDATA* DirectInput::lpdidod;
//
//DIDEVCAPS DirectInput::padCaps;
//
//std::vector<DIDEVICEINSTANCE> DirectInput::getEnum;
//
//BYTE DirectInput::key[256];
//BYTE DirectInput::preKey[256];
//DIMOUSESTATE DirectInput::mouseState;
//DIMOUSESTATE DirectInput::mousePrevious;
//DIJOYSTATE DirectInput::padState;
//DIJOYSTATE DirectInput::padPrevious;
//
//
//HWND DirectInput::mHwnd;
//bool DirectInput::setPad;
//
//int DirectInput::winHeight;
//int DirectInput::winWidth;
//
//DirectX::XMMATRIX DirectInput::viewMat;
//DirectX::XMMATRIX DirectInput::projectionMat;
//float DirectInput::nearNum;
//float DirectInput::farNum;
//
//void DirectInput::Initialize(HWND hwnd, int windowWidth, int windowHeight)
//{
//	mHwnd = hwnd;
//	winWidth = windowWidth;
//	winHeight = windowHeight;
//
//	hInstance = GetModuleHandle(nullptr);//ハンドル
//	//dInput8 = nullptr;//dxInputを入れる変数
//
//	////DrectInputオブジェクトの生成
//	//dIResult = DirectInput8Create(
//	//	hInstance,
//	//	DIRECTINPUT_VERSION,
//	//	IID_IDirectInput8A,
//	//	(LPVOID*)&dInput8,
//	//	nullptr
//	//);
//
//	////デバイスオブジェクトの生成
//
//	////キーボードデバイスの作成
//	//dInput8->CreateDevice(
//	//	GUID_SysKeyboard, //キーボードの入力を受け取るという設定?
//	//	&keyDev, //入力情報を受け取るためのもの?
//	//	nullptr);
//
//	////デバイスのセットアップ
//	//keyDev->SetCooperativeLevel(hwnd, DISCL_BACKGROUND);
//	//keyDev->SetDataFormat(&c_dfDIKeyboard);
//
//
//	////デバイスの取得
//	//dIResult = keyDev->Acquire();
//
//	//デバイス作成
//	dInputDevice = nullptr;
//	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInputDevice, nullptr);
//
//#pragma region キーボード
//
//	devKeyBoard = nullptr;
//	dInputDevice->CreateDevice(GUID_SysKeyboard, &devKeyBoard, NULL);
//	devKeyBoard->SetDataFormat(&c_dfDIKeyboard);
//	devKeyBoard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//
//#pragma endregion
//
//#pragma region マウス
//	devMouse = nullptr;
//	dInputDevice->CreateDevice(GUID_SysMouse, &devMouse, NULL);
//	devMouse->SetDataFormat(&c_dfDIMouse);
//	devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//
//#pragma endregion
//
//	/*if (getEnum.size() == 0)
//	{
//		dInputDevice->EnumDevices(DI8DEVCLASS_GAMECTRL, &diEnumDevicesCallBack, NULL, DIEDFL_ATTACHEDONLY);
//
//		if (getEnum.size() != 0)
//		{
//
//			WCHAR* w = getEnum[0].tszInstanceName;
//			WCHAR wArr[260];
//			for (int i = 0; i < 260; i++)
//				wArr[i] = w[i];
//			char c[260];
//
//			size_t size;
//			wcstombs_s(&size, c, wArr, sizeof(c));
//			std::string enumName;
//
//			for (int i = 0; i < 40; i++)
//			{
//				enumName += c[i];
//			}
//			if (enumName.find("DirectInput") == std::string::npos)
//			{
//				getEnum.clear();
//				getEnum.shrink_to_fit();
//			}
//		}
//	}*/
//}
//
////これをEnumDeviceに渡すとこれに接続したものを渡してくれる
////CALLBACK書いてないからエラー出て渡せなかった
////CALLBACKって書かなかったからCAKKBACK関数ってEnumDevices関数が認識しなかった?
//
//BOOL CALLBACK DirectInput::diEnumDevicesCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
//{
//	if (lpddi != nullptr)
//	{
//		//vectorをポインタにして入れるとCreateできない?
//		getEnum.push_back(*lpddi);
//		return DIENUM_CONTINUE;
//	}
//
//	return DIENUM_STOP;
//}
//
//BOOL CALLBACK DirectInput::diEnumDevicesObjectCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
//{
//	return DIENUM_STOP;
//}
//
//void DirectInput::Update()
//{
//
//#pragma region パッド
//	//BOOL(*calc)(DIDEVICEINSTANCE*,LPVOID);
//	//LPDIENUMDEVICESCALLBACK(enumInst,);//2つ目にはEnumDeviceのpvRefを渡す
//	//LPVOID callV = nullptr;
//
//	//ゲームパッドの設定にしたのにペンタブ認識した
//	//padPrevious = padState;
//
//	//
//	//
//	//
//	//if (getEnum.size() != 0 && !setPad)
//	//{
//	//	//パッド接続するとGUID_JoystickでS_OK戻ってくる(やっていいかはわからん)
//	//	//enumのやつ渡す場合、インスタンスのやつを渡せばいいっぽい?
//	//	auto ifresult = dInputDevice->CreateDevice(getEnum[0].guidInstance, &devPad, NULL);
//	//	padCaps.dwSize = sizeof(DIDEVCAPS);
//	//	ifresult = devPad->GetCapabilities(&padCaps);
//	//	ifresult = devPad->SetDataFormat(&c_dfDIJoystick);
//	//	ifresult = devPad->SetCooperativeLevel(mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//	//	//result = devPad->EnumObjects();
//	//	//joyPadState = new DIJOYSTATE();
//	//
//	//	setPad = true;
//
//	//	
//	//}
//	//
//
//	//if (setPad) 
//	//{
//	//	//アクセスエラー
//	//	 auto ifresult = devPad->Acquire();
//
//	//	//ここエラー
//	//	ifresult = devPad->Poll();
//	//	ifresult = devPad->GetDeviceState(sizeof(padState), &padState);
//	//}
//	//
//#pragma endregion
//
//
//#pragma region キーボード
//
//	auto result = devKeyBoard->Acquire();
//	for (int i = 0; i < 256; i++)
//	{
//		preKey[i] = key[i];
//	}
//
//	//キー情報取得
//	//dIResult = keyDev->GetDeviceState(256, key);
//	result = devKeyBoard->GetDeviceState(sizeof(key), key);
//
//#pragma endregion
//
//#pragma region マウス
//	devMouse->Acquire();
//	mousePrevious = mouseState;
//	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
//#pragma endregion
//
//
//#ifdef _DEBUG
//	if (DirectInput::KeyTrigger(DIK_SPACE))
//	{
//		int a = 0;
//	}
//
//#endif // _DEBUG
//
//}
//
//#pragma region キーボード
//
//
//bool DirectInput::KeyState(const BYTE& keyDef)
//{
//	if (key[keyDef])
//	{
//		//押してるとき
//		return true;
//	}
//	else
//	{
//		//押してないとき
//		return false;
//	}
//}
//
//bool DirectInput::KeyTrigger(const BYTE& keyDef)
//{
//	//1フレーム前がtrueかfalseか
//	bool check;
//	if (preKey[keyDef])
//	{
//		check = true;
//	}
//	else
//	{
//		check = false;
//	}
//
//	if (KeyState(keyDef) && !check)
//	{
//		//押された瞬間(前フレーム押されてない&現フレーム押された)か
//		return true;
//	}
//
//	return false;
//}
//
//bool DirectInput::KeyRelease(const BYTE& keyDef)
//{
//	bool check;
//	if (preKey[keyDef])
//	{
//		check = true;
//	}
//	else
//	{
//		check = false;
//	}
//
//	if (!KeyState(keyDef) && check)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//char DirectInput::KeyChar()
//{
//	char returnChar = 0;
//
//
//	return returnChar;
//}
//
//float DirectInput::ArrowKeyAngle()
//{
//	bool right = DirectInput::KeyState(DIK_RIGHT);
//	bool left = DirectInput::KeyState(DIK_LEFT);
//	bool up = DirectInput::KeyState(DIK_UP);
//	bool down = DirectInput::KeyState(DIK_DOWN);
//
//	if (right && up)return 45.0f;
//	if (up && left)return 135.0f;
//	if (left && down)return 225.0f;
//	if (down && right)return 315.0f;
//
//	if (right)return 0.0f;
//	if (up)return 90.0f;
//	if (left)return 180.0f;
//	if (down)return 270.0f;
//
//	return -1.0f;
//}
//#pragma endregion
//
//#pragma region パッド
//
//bool DirectInput::ButtonState(int button)
//{
//	if (!devPad)return false;
//	if (padState.rgbButtons[button])
//	{
//		//押してるとき
//		return true;
//	}
//	else
//	{
//		//押してないとき
//		return false;
//	}
//}
//
//bool DirectInput::ButtonTrigger(int button)
//{
//	if (!devPad)return false;
//	//1フレーム前がtrueかfalseか
//	bool check;
//	if (padPrevious.rgbButtons[button])
//	{
//		check = true;
//	}
//	else
//	{
//		check = false;
//	}
//
//	if (ButtonState(button) && !check)
//	{
//		//押された瞬間(前フレーム押されてない&現フレーム押された)か
//		return true;
//	}
//
//	return false;
//}
//
//bool DirectInput::ButtonRelease(int button)
//{
//	if (!devPad)return false;
//	bool check;
//	if (padPrevious.rgbButtons[button])
//	{
//		check = true;
//	}
//	else
//	{
//		check = false;
//	}
//
//	if (!ButtonState(button) && check)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//
//bool DirectInput::DirectionalButtonState(int button)
//{
//	//押してるとき
//
//	if (!devPad)return false;
//
//	switch (button)
//	{
//	case UpButton:
//		if (padState.rgdwPOV[0] <= 4500 ||
//			padState.rgdwPOV[0] >= 31500 &&
//			padState.rgdwPOV[0] <= 36000)
//		{
//			return true;
//		}
//		break;
//
//	case RightButton:
//		if (padState.rgdwPOV[0] <= 13500 &&
//			padState.rgdwPOV[0] >= 4500) return true;
//		break;
//
//	case DownButton:
//		if (padState.rgdwPOV[0] <= 22500 &&
//			padState.rgdwPOV[0] >= 13500) return true;
//		break;
//
//	case LeftButton:
//		if (padState.rgdwPOV[0] <= 31500 &&
//			padState.rgdwPOV[0] >= 22500)
//		{
//			return true;
//		}
//		break;
//	}
//	return false;
//
//
//}
//
//bool DirectInput::DirectionalButtonTrigger(int button)
//{
//
//	//押してるとき
//
//	if (!devPad)return false;
//
//	switch (button)
//	{
//	case UpButton:
//		if (padState.rgdwPOV[0] <= 4500 ||
//			padState.rgdwPOV[0] >= 31500 &&
//			padState.rgdwPOV[0] <= 36000)
//		{
//			if (padPrevious.rgdwPOV[0] <= 4500 ||
//				padPrevious.rgdwPOV[0] >= 31500 &&
//				padPrevious.rgdwPOV[0] <= 36000) {
//			}
//			else
//				return true;
//		}
//		break;
//
//	case RightButton:
//		if (padState.rgdwPOV[0] <= 13500 &&
//			padState.rgdwPOV[0] >= 4500)
//		{
//			if (padPrevious.rgdwPOV[0] <= 13500 &&
//				padPrevious.rgdwPOV[0] >= 4500) {
//			}
//			else
//				return true;
//		}
//		break;
//
//	case DownButton:
//		if (padState.rgdwPOV[0] <= 22500 &&
//			padState.rgdwPOV[0] >= 13500)
//		{
//			if (padPrevious.rgdwPOV[0] <= 22500 &&
//				padPrevious.rgdwPOV[0] >= 13500) {
//			}
//			else
//				return true;
//		}
//		break;
//
//	case LeftButton:
//		if (padState.rgdwPOV[0] <= 31500 &&
//			padState.rgdwPOV[0] >= 22500)
//		{
//			if (padPrevious.rgdwPOV[0] <= 31500 &&
//				padPrevious.rgdwPOV[0] >= 22500) {
//			}
//			else
//				return true;
//		}
//		break;
//	}
//	return false;
//}
//
//bool DirectInput::LeftStickLeft(int lXNum)
//{
//	if (!devPad)return false;
//
//	if (32767 - lXNum >= padState.lX)
//	{
//		return true;
//	}
//	return false;
//
//}
//bool DirectInput::LeftStickRight(int lXNum)
//{
//	if (!devPad)return false;
//	if (32767 + lXNum <= padState.lX)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::LeftStickUp(int lYNum)
//{
//	if (!devPad)return false;
//	if (32767 - lYNum >= padState.lY)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::LeftStickDown(int lYNum)
//{
//	if (!devPad)return false;
//	if (32767 + lYNum <= padState.lY)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::RightStickLeft(int lXNum)
//{
//	if (!devPad)return false;
//	if (32767 - lXNum >= padState.lZ)
//	{
//		return true;
//	}
//	return false;
//
//}
//bool DirectInput::RightStickRight(int lXNum)
//{
//	if (!devPad)return false;
//	if (32767 + lXNum <= padState.lZ)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::RightStickUp(int lYNum)
//{
//	if (!devPad)return false;
//	if (32767 - lYNum >= padState.lRz)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::RightStickDown(int lYNum)
//{
//	if (!devPad)return false;
//	if (32767 + lYNum <= padState.lRz)
//	{
//		return true;
//	}
//	return false;
//}
//
//
//#pragma endregion
//
//
//
//void DirectInput::Release()
//{
//	//DirectInputを閉じる
//	/*keyDev->Unacquire();
//	keyDev->Release();
//	dInput8->Release();*/
//
//	devKeyBoard->Unacquire();
//	devKeyBoard->Release();
//	/*if (devPad)
//	{
//		devPad->Unacquire();
//		devPad->Release();
//	}*/
//	dInputDevice->Release();
//
//}
//
//#pragma region マウス
//
//void DirectInput::SetMatrixAndNearFar(DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, float nearNumber, float farNumber)
//{
//	viewMat = viewMatrix;
//	projectionMat = projectionMatrix;
//	nearNum = nearNumber;
//	farNum = farNumber;
//}
//
//Vector2 DirectInput::GetMousePosition()
//{
//	POINT p;
//
//	//マウスのスクリーン座標を取得
//	GetCursorPos(&p);
//
//	//クライアント座標に変換
//	ScreenToClient(mHwnd, &p);
//
//	RECT rect;//現在のウィンドウサイズを入れる
//	Vector2 pos = { (float)p.x,(float)p.y };
//	float num;
//
//	//これいらない?
//	/*p.x = p.x <= 0 ? 0 : p.x;
//	p.y = p.y <= 0 ? 0 : p.y;*/
//
//	//現在のウィンドウサイズを取得
//	GetClientRect(mHwnd, &rect);
//
//	//ウィンドウサイズを変えても座標の上限が変わらないように変換
//	num = (float)rect.right / (float)winWidth;
//	pos.x = (float)pos.x / num;
//
//	num = (float)rect.bottom / (float)winHeight;
//	pos.y = (float)pos.y / num;
//
//	return pos;
//
//}
//
//Vector2 DirectInput::GetCenterToMouseVector()
//{
//	Vector2 point = { static_cast<float>(winWidth) / 2.0f, static_cast<float>(winHeight) / 2.0f };
//	return Vector2Normalize(GetMousePosition() - point);
//}
//
//float DirectInput::GetMouseAngle()
//{
//	return LibMath::Vecto2ToAngle(GetCenterToMouseVector(), false);
//}
//
//void DirectInput::GetMouse3DLine(Vector3& nearPoint, Vector3& farPoint)
//{
//	Vector2 mousePos = GetMousePosition();
//
//	DirectX::XMMATRIX mousePosMatrix;
//	DirectX::XMMATRIX viewportMatrix;
//	DirectX::XMMATRIX worldMousePosMatrix;
//
//	//逆行列
//	DirectX::XMMATRIX invViewPortMatrix;
//	DirectX::XMMATRIX invViewMatrix;
//	DirectX::XMMATRIX invProjectionMatrix;
//
//
//	//ビューポート行列を作成
//	viewportMatrix = DirectX::XMMatrixIdentity();
//	viewportMatrix.r[0].m128_f32[0] = (float)winWidth / 2.0f;
//	viewportMatrix.r[1].m128_f32[1] = -(float)winHeight / 2.0f;
//	viewportMatrix.r[3].m128_f32[0] = (float)winWidth / 2.0f;
//	viewportMatrix.r[3].m128_f32[1] = (float)winHeight / 2.0f;
//
//	//マウス座標の行列
//	mousePosMatrix = DirectX::XMMatrixIdentity();
//
//	invViewPortMatrix = DirectX::XMMatrixInverse(nullptr, viewportMatrix);
//	invViewMatrix = DirectX::XMMatrixInverse(nullptr, viewMat);
//	invProjectionMatrix = DirectX::XMMatrixInverse(nullptr, projectionMat);
//
//#pragma region Z = 0(最近点)
//
//	//z = 0の場合
//	mousePosMatrix = DirectX::XMMatrixTranslation(mousePos.x, mousePos.y, 0.0f);
//	//スクリーン座標をワールド座標に
//	worldMousePosMatrix = DirectX::XMMatrixIdentity();
//
//	worldMousePosMatrix *= mousePosMatrix;
//	worldMousePosMatrix *= invViewPortMatrix;
//	worldMousePosMatrix *= invProjectionMatrix;
//
//	//W除算
//	worldMousePosMatrix.r[3].m128_f32[0] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[1] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[2] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[3] /= worldMousePosMatrix.r[3].m128_f32[3];
//
//	worldMousePosMatrix *= invViewMatrix;
//
//	//最近点
//	nearPoint =
//	{
//		worldMousePosMatrix.r[3].m128_f32[0] ,
//		worldMousePosMatrix.r[3].m128_f32[1] ,
//		worldMousePosMatrix.r[3].m128_f32[2]
//	};
//#pragma endregion
//
//
//#pragma region z = 1(最遠点)
//
//
//	//z = 1の場合
//	mousePosMatrix = DirectX::XMMatrixTranslation(mousePos.x, mousePos.y, 1.0f);
//
//	//スクリーン座標をワールド座標に
//	worldMousePosMatrix = DirectX::XMMatrixIdentity();
//
//	worldMousePosMatrix *= mousePosMatrix;
//	worldMousePosMatrix *= invViewPortMatrix;
//	worldMousePosMatrix *= invProjectionMatrix;
//
//	//W除算
//	worldMousePosMatrix.r[3].m128_f32[0] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[1] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[2] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[3] /= worldMousePosMatrix.r[3].m128_f32[3];
//
//	worldMousePosMatrix *= invViewMatrix;
//
//
//	//最遠点
//	farPoint =
//	{
//		worldMousePosMatrix.r[3].m128_f32[0] ,
//		worldMousePosMatrix.r[3].m128_f32[1] ,
//		worldMousePosMatrix.r[3].m128_f32[2]
//	};
//
//#pragma endregion
//}
//
//#pragma region ボタン
//
////ボタン配列
////0 左ボタン
////1 右ボタン
////2 中ボタン
////3 左端のボタン2つの下側(上は反応無し)
//
//bool DirectInput::MouseButtonState(MouseButton mouseButton)
//{
//	if (mouseState.rgbButtons[mouseButton]) return true;
//	return false;
//}
//bool DirectInput::MouseButtonTrigger(MouseButton mouseButton)
//{
//	if (!mousePrevious.rgbButtons[mouseButton])
//	{
//		if (MouseButtonState(mouseButton)) return true;
//	}
//	return false;
//}
//bool DirectInput::MouseButtonRelease(MouseButton mouseButton)
//{
//	if (mousePrevious.rgbButtons[mouseButton])
//	{
//		if (!MouseButtonState(mouseButton)) return true;
//	}
//	return false;
//}
//#pragma endregion
//
//
//#pragma endregion
