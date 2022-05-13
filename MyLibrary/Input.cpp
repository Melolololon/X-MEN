#include "Input.h"
#include"LibMath.h"
#include"LibWinAPI.h"
#include"Library.h"

#include<string>


using namespace MelLib;

HINSTANCE Input::hInstance;//ハンドル
HRESULT Input::dIResult;//結果取得
LPDIRECTINPUT8 Input::dInput8;//dxInputを入れる変数
LPDIRECTINPUTDEVICE8 Input::keyDev;//デバイスを入れる変数


IDirectInput8* Input::dInputDevice;
IDirectInputDevice8* Input::devKeyBoard;
IDirectInputDevice8* Input::devMouse;



BYTE Input::keysState[256];
BYTE Input::preKeysState[256];
std::vector<BYTE> Input::currentPressKeys;

bool Input::callGetPressKeyChars = false;
bool Input::callPreGetPressKeyChars = false;

std::unordered_map<std::string, HWND>Input::hwnds;
std::vector<std::unordered_map<UCHAR, char>>Input::returnChars =
{
	{
		{DIK_1,'1'},
		{DIK_2,'2'},
		{DIK_3,'3'},
		{DIK_4,'4'},
		{DIK_5,'5'},
		{DIK_6,'6'},
		{DIK_7,'7'},
		{DIK_8,'8'},
		{DIK_9,'9'},
		{DIK_0,'0'},
		{DIK_MINUS,'-'},
		{DIK_CIRCUMFLEX,'^'},

		{DIK_Q,'q'},
		{DIK_W,'w'},
		{DIK_E,'e'},
		{DIK_R,'r'},
		{DIK_T,'t'},
		{DIK_Y,'y'},
		{DIK_U,'u'},
		{DIK_I,'i'},
		{DIK_O,'o'},
		{DIK_P,'p'},
		{DIK_AT,'@'},
		{DIK_LBRACKET,'['},

		{DIK_A,'a'},
		{DIK_S,'s'},
		{DIK_D,'d'},
		{DIK_F,'f'},
		{DIK_G,'g'},
		{DIK_H,'h'},
		{DIK_J,'j'},
		{DIK_K,'k'},
		{DIK_L,'l'},
		{DIK_SEMICOLON,';'},
		{DIK_COLON,':'},
		{DIK_RBRACKET,']'},

		{DIK_Z,'z'},
		{DIK_X,'x'},
		{DIK_C,'c'},
		{DIK_V,'v'},
		{DIK_B,'b'},
		{DIK_N,'n'},
		{DIK_M,'m'},
		{DIK_APOSTROPHE,','},
		{DIK_PERIOD,'.'},
		{DIK_SLASH,'/'},

		{DIK_SPACE,' '},

		{DIK_ADD,'+'},
		{DIK_SUBTRACT,'-'},
		{DIK_MULTIPLY,'*'},
		{DIK_DIVIDE,'/'},
		{DIK_DECIMAL,'.'},
	},

	{
		{DIK_1,'!'},
		{DIK_2,'"'},
		{DIK_3,'#'},
		{DIK_4,'$'},
		{DIK_5,'%'},
		{DIK_6,'&'},
		{DIK_7,'7'},
		{DIK_8,'('},
		{DIK_9,')'},
		{DIK_0,'0'},
		{DIK_MINUS,'='},
		{DIK_CIRCUMFLEX,'~'},
		{DIK_YEN,'|'},

		{DIK_Q,'Q'},
		{DIK_W,'W'},
		{DIK_E,'E'},
		{DIK_R,'R'},
		{DIK_T,'T'},
		{DIK_Y,'Y'},
		{DIK_U,'U'},
		{DIK_I,'I'},
		{DIK_O,'O'},
		{DIK_P,'P'},
		{DIK_AT,'`'},
		{DIK_LBRACKET,'{'},

		{DIK_A,'A'},
		{DIK_S,'S'},
		{DIK_D,'D'},
		{DIK_F,'F'},
		{DIK_G,'G'},
		{DIK_H,'H'},
		{DIK_J,'J'},
		{DIK_K,'K'},
		{DIK_L,'L'},
		{DIK_SEMICOLON,'+'},
		{DIK_COLON,'*'},
		{DIK_RBRACKET,'}'},

		{DIK_Z,'Z'},
		{DIK_X,'X'},
		{DIK_C,'C'},
		{DIK_V,'V'},
		{DIK_B,'B'},
		{DIK_N,'N'},
		{DIK_M,'M'},
		{DIK_APOSTROPHE,'<'},
		{DIK_PERIOD,'>'},
		{DIK_SLASH,'?'},
		{DIK_BACKSLASH,'_'},

		{DIK_SPACE,' '},

		{DIK_ADD,'+'},
		{DIK_SUBTRACT,'-'},
		{DIK_MULTIPLY,'*'},
		{DIK_DIVIDE,'/'},
		{DIK_DECIMAL,'.'},
	}


};
const std::vector<wchar_t>Input::hiragana =
{
	L'あ',L'い',L'う',L'え',L'お',
	L'か',L'き',L'く',L'け',L'こ',
	L'さ',L'し',L'す',L'せ',L'そ',
	L'た',L'ち',L'つ',L'て',L'と',
	L'な',L'に',L'ぬ',L'ね',L'の',
	L'は',L'ひ',L'ふ',L'へ',L'ほ',
	L'ま',L'み',L'む',L'め',L'も',
	L'や',L'い',L'ゆ',L'え',L'よ',
	L'ら',L'り',L'る',L'れ',L'ろ',
	L'わ',L' ' ,L' ' ,L' ' ,L'を',
	L'ん',L' ' ,L' ' ,L' ' ,L'  ',
	L'が',L'ぎ',L'ぐ',L'げ',L'ご',
	L'ざ',L'じ',L'ず',L'ぜ',L'ぞ',
	L'だ',L'ぢ',L'づ',L'で',L'ど',
	L'ば',L'び',L'ぶ',L'べ',L'ぼ',
	L'ぱ',L'ぴ',L'ぷ',L'ぺ',L'ぽ',
	L'ぁ',L'ぃ',L'ぅ',L'ぇ',L'ぉ',
	L'っ',L'ゃ',L'ゅ',L'ょ',L'ー',
	L'ゎ',L' ' ,L' ' ,L' ' ,L' '
};
const std::vector<wchar_t>Input::katakana =
{
	L'ア',L'イ',L'ウ',L'エ',L'オ',
	L'カ',L'キ',L'ク',L'ケ',L'コ',
	L'サ',L'シ',L'ス',L'セ',L'ソ',
	L'タ',L'チ',L'ツ',L'テ',L'ト',
	L'ナ',L'ニ',L'ヌ',L'ネ',L'ノ',
	L'ハ',L'ヒ',L'フ',L'ヘ',L'ホ',
	L'マ',L'ミ',L'ム',L'メ',L'モ',
	L'ヤ',L'イ',L'ユ',L'エ',L'ヨ',
	L'ラ',L'リ',L'ル',L'レ',L'ロ',
	L'ワ',L'イ',L'ウ',L'エ',L'ヲ',
	L'ン',L'イ',L'ウ',L'エ',L'オ',
	L'ガ',L'ギ',L'グ',L'ゲ',L'ゴ',
	L'ザ',L'ジ',L'ズ',L'ゼ',L'ゾ',
	L'ダ',L'ヂ',L'ヅ',L'デ',L'ド',
	L'バ',L'ビ',L'ブ',L'ベ',L'ボ',
	L'パ',L'ピ',L'プ',L'ペ',L'ポ',
	L'ヴ',L' ' ,L' ' ,L' ' ,L' ' ,
	L'ァ',L'ィ',L'ゥ',L'ェ',L'ォ',
	L'ッ',L'ャ',L'ュ',L'ョ',L'ー',
	L'ヮ',L'ヵ',L'ヶ',L' ' ,L' '
};

DIMOUSESTATE Input::mouseState;
DIMOUSESTATE Input::mousePrevious;


HWND Input::mHwnd;

int Input::winHeight;
int Input::winWidth;

DirectX::XMMATRIX Input::viewMat;
DirectX::XMMATRIX Input::projectionMat;
float Input::nearNum;
float Input::farNum;


XINPUT_STATE Input::padState[4];
XINPUT_STATE Input::padPrevious[4];
bool Input::padConnected[4];


LRESULT Input::ParamChildWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Input::Initialize(HWND hwnd, const int windowWidth, const int windowHeight)
{
	mHwnd = hwnd;
	winWidth = windowWidth;
	winHeight = windowHeight;

	hInstance = GetModuleHandle(nullptr);//ハンドル
	//dInput8 = nullptr;//dxInputを入れる変数



	//デバイス作成
	dInputDevice = nullptr;
	auto result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInputDevice, nullptr);

#pragma region キーボード

	devKeyBoard = nullptr;
	result = dInputDevice->CreateDevice(GUID_SysKeyboard, &devKeyBoard, NULL);
	result = devKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	result = devKeyBoard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	currentPressKeys.reserve(256);
#pragma endregion

#pragma region マウス
	devMouse = nullptr;
	dInputDevice->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	devMouse->SetDataFormat(&c_dfDIMouse);
	devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

#pragma endregion

}

void Input::Update()
{


#pragma region キーボード

	auto result = devKeyBoard->Acquire();
	for (int i = 0; i < 256; i++)
	{
		preKeysState[i] = keysState[i];
	}

	result = devKeyBoard->GetDeviceState(sizeof(keysState), keysState);

	currentPressKeys.clear();
	for (int i = 0; i < 256; i++)
	{
		if (keysState[i] != 0)
		{
			currentPressKeys.push_back(i);
		}
	}


	callPreGetPressKeyChars = callGetPressKeyChars;
	callGetPressKeyChars = false;
#pragma endregion

#pragma region マウス
	devMouse->Acquire();
	mousePrevious = mouseState;
	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
#pragma endregion

#pragma region パッド




	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		// コントローラーの前フレームを保存
		padPrevious[i] = padState[i];


		if (XInputGetState(i, &padState[i]) != ERROR_SUCCESS)padConnected[i] = false;
		else padConnected[i] = true;
	}

#pragma endregion
}

void Input::Finalize()
{

	devKeyBoard->Unacquire();
	devKeyBoard->Release();
	dInputDevice->Release();

}

#pragma region キーボード


bool Input::KeyState(const BYTE keyDef)
{
	if (keysState[keyDef]) return true;
	return false;

}

bool Input::KeyTrigger(const BYTE keyDef)
{
	if (KeyState(keyDef) && !preKeysState[keyDef]) return true;
	return false;
}

bool Input::KeyRelease(const BYTE keyDef)
{
	if (!KeyState(keyDef) && preKeysState[keyDef]) return true;
	return false;
}

std::string Input::GetPressKeyChars()
{
	std::string returnString;
	returnString.reserve(sizeof(char));
	for (const auto& key : currentPressKeys)
	{
		bool isThere = false;
		for (const auto& c : returnChars[1])
		{
			if (c.first == key)
			{
				isThere = true;
				break;
			}
		}
		if (!isThere)continue;

		returnString.push_back(returnChars[KeyState(DIK_LSHIFT) | KeyState(DIK_RSHIFT)][key]);
	}

	if (returnString != "")callGetPressKeyChars = true;

	return returnString;
}

std::string Input::GetTriggerKeyChars()
{
	std::string returnString = GetPressKeyChars();

	if (!callPreGetPressKeyChars && callGetPressKeyChars) return returnString;
	return std::string();
}

void Input::CreateStringInputWindow
(
	const std::wstring& initStr,
	const Vector2& position,
	const Vector2& size,
	HWND parentHWND,
	const std::string& name
)
{
	if (!parentHWND)parentHWND = mHwnd;

	hwnds.emplace
	(
		name,
		LibWinAPI::CreateNormalWindow
		(
			L"EDIT",
			initStr.data(),
			WS_CHILD | WS_VISIBLE,
			(DWORD)position.x,
			(DWORD)position.y,
			(DWORD)size.x,
			(DWORD)size.y,
			parentHWND,
			ParamChildWindowProc
		)
	);


}

void Input::DeleteStringInputWindow(const std::string& name)
{
	DestroyWindow(hwnds[name]);
	hwnds.erase(name);
}

std::wstring Input::GetInputString(const std::string& name)
{
	std::wstring returnStr;
	returnStr.resize(99);
	SendMessage(hwnds[name], WM_GETTEXT, (WPARAM)returnStr.size(), (LPARAM)returnStr.data());

	return returnStr;
}


float Input::ArrowKeyAngle()
{
	bool right = Input::KeyState(DIK_RIGHT);
	bool left = Input::KeyState(DIK_LEFT);
	bool up = Input::KeyState(DIK_UP);
	bool down = Input::KeyState(DIK_DOWN);

	if (right)return 0.0f;
	if (up)return 90.0f;
	if (left)return 180.0f;
	if (down)return 270.0f;

	if (right && up)return 45.0f;
	if (up && left)return 135.0f;
	if (left && down)return 225.0f;
	if (down && right)return 315.0f;

	return -1.0f;
}
#pragma endregion

#pragma region マウス

Vector2 Input::GetMousePosition()
{
	POINT p;

	//マウスのスクリーン座標を取得
	GetCursorPos(&p);

	//クライアント座標に変換
	ScreenToClient(mHwnd, &p);

	RECT rect;//現在のウィンドウサイズを入れる
	Vector2 pos = { (float)p.x,(float)p.y };
	float num;

	//これいらない?
	/*p.x = p.x <= 0 ? 0 : p.x;
	p.y = p.y <= 0 ? 0 : p.y;*/

	//現在のウィンドウサイズを取得
	GetClientRect(mHwnd, &rect);

	//ウィンドウサイズを変えても座標の上限が変わらないように変換
	num = (float)rect.right / (float)winWidth;
	pos.x = (float)pos.x / num;

	num = (float)rect.bottom / (float)winHeight;
	pos.y = (float)pos.y / num;

	return pos;

}

Vector2 Input::GetCenterToMouseVector()
{
	Vector2 point = { static_cast<float>(winWidth) / 2.0f, static_cast<float>(winHeight) / 2.0f };
	return Vector2Normalize(GetMousePosition() - point);
}

float Input::GetMouseAngle()
{
	return LibMath::Vector2ToAngle(GetCenterToMouseVector(), false);
}

void Input::GetMouse3DLine(Vector3& nearPoint, Vector3& farPoint)
{
	Vector2 mousePos = GetMousePosition();

	DirectX::XMMATRIX mousePosMatrix;
	DirectX::XMMATRIX viewportMatrix;
	DirectX::XMMATRIX worldMousePosMatrix;

	//逆行列
	DirectX::XMMATRIX invViewPortMatrix;
	DirectX::XMMATRIX invViewMatrix;
	DirectX::XMMATRIX invProjectionMatrix;


	//ビューポート行列を作成
	viewportMatrix = DirectX::XMMatrixIdentity();
	viewportMatrix.r[0].m128_f32[0] = (float)winWidth / 2.0f;
	viewportMatrix.r[1].m128_f32[1] = -(float)winHeight / 2.0f;
	viewportMatrix.r[3].m128_f32[0] = (float)winWidth / 2.0f;
	viewportMatrix.r[3].m128_f32[1] = (float)winHeight / 2.0f;

	//マウス座標の行列
	mousePosMatrix = DirectX::XMMatrixIdentity();

	invViewPortMatrix = DirectX::XMMatrixInverse(nullptr, viewportMatrix);
	invViewMatrix = DirectX::XMMatrixInverse(nullptr, viewMat);
	invProjectionMatrix = DirectX::XMMatrixInverse(nullptr, projectionMat);

#pragma region Z = 0(最近点)

	//z = 0の場合
	mousePosMatrix = DirectX::XMMatrixTranslation(mousePos.x, mousePos.y, 0.0f);
	//スクリーン座標をワールド座標に
	worldMousePosMatrix = DirectX::XMMatrixIdentity();

	worldMousePosMatrix *= mousePosMatrix;
	worldMousePosMatrix *= invViewPortMatrix;
	worldMousePosMatrix *= invProjectionMatrix;

	//W除算
	worldMousePosMatrix.r[3].m128_f32[0] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[1] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[2] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[3] /= worldMousePosMatrix.r[3].m128_f32[3];

	worldMousePosMatrix *= invViewMatrix;

	//最近点
	nearPoint =
	{
		worldMousePosMatrix.r[3].m128_f32[0] ,
		worldMousePosMatrix.r[3].m128_f32[1] ,
		worldMousePosMatrix.r[3].m128_f32[2]
	};
#pragma endregion

#pragma region z = 1(最遠点)


	//z = 1の場合
	mousePosMatrix = DirectX::XMMatrixTranslation(mousePos.x, mousePos.y, 1.0f);

	//スクリーン座標をワールド座標に
	worldMousePosMatrix = DirectX::XMMatrixIdentity();

	worldMousePosMatrix *= mousePosMatrix;
	worldMousePosMatrix *= invViewPortMatrix;
	worldMousePosMatrix *= invProjectionMatrix;

	//W除算
	worldMousePosMatrix.r[3].m128_f32[0] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[1] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[2] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[3] /= worldMousePosMatrix.r[3].m128_f32[3];

	worldMousePosMatrix *= invViewMatrix;


	//最遠点
	farPoint =
	{
		worldMousePosMatrix.r[3].m128_f32[0] ,
		worldMousePosMatrix.r[3].m128_f32[1] ,
		worldMousePosMatrix.r[3].m128_f32[2]
	};

#pragma endregion

}

void MelLib::Input::GetMouse3DLine2(Camera* pCamera, Vector3* pNear, Vector3* pFar)
{
	if (!pCamera)return;

	Vector2 mousePos = GetMousePosition();

	DirectX::XMMATRIX mousePosMatrix;
	DirectX::XMMATRIX viewportMatrix;
	DirectX::XMMATRIX worldMousePosMatrix;

	//逆行列
	DirectX::XMMATRIX invViewPortMatrix;
	DirectX::XMMATRIX invViewMatrix;
	DirectX::XMMATRIX invProjectionMatrix;


	//ビューポート行列を作成
	viewportMatrix = DirectX::XMMatrixIdentity();

	static const float WIDTH = (float)Library::GetWindowWidth();
	static const float HEIGHT = (float)Library::GetWindowHeight();
	viewportMatrix.r[0].m128_f32[0] = WIDTH / 2.0f;
	viewportMatrix.r[1].m128_f32[1] = -HEIGHT / 2.0f;
	viewportMatrix.r[3].m128_f32[0] = WIDTH / 2.0f;
	viewportMatrix.r[3].m128_f32[1] = HEIGHT / 2.0f;

	//マウス座標の行列
	mousePosMatrix = DirectX::XMMatrixIdentity();

	invViewPortMatrix = DirectX::XMMatrixInverse(nullptr, viewportMatrix);
	invViewMatrix = DirectX::XMMatrixInverse(nullptr, pCamera->GetViewMatrix());
	invProjectionMatrix = DirectX::XMMatrixInverse(nullptr, pCamera->GetProjectionMatrix());

#pragma region Z = 0(最近点)

	//z = 0の場合
	mousePosMatrix = DirectX::XMMatrixTranslation(mousePos.x, mousePos.y, 0.0f);
	//スクリーン座標をワールド座標に
	worldMousePosMatrix = DirectX::XMMatrixIdentity();

	worldMousePosMatrix *= mousePosMatrix;
	worldMousePosMatrix *= invViewPortMatrix;
	worldMousePosMatrix *= invProjectionMatrix;

	//W除算
	worldMousePosMatrix.r[3].m128_f32[0] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[1] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[2] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[3] /= worldMousePosMatrix.r[3].m128_f32[3];

	worldMousePosMatrix *= invViewMatrix;

	//最近点
	if (pNear)
	{
		*pNear =
		{
			worldMousePosMatrix.r[3].m128_f32[0] ,
			worldMousePosMatrix.r[3].m128_f32[1] ,
			worldMousePosMatrix.r[3].m128_f32[2]
		};
	}
#pragma endregion


#pragma region z = 1(最遠点)


	//z = 1の場合
	mousePosMatrix = DirectX::XMMatrixTranslation(mousePos.x, mousePos.y, 1.0f);

	//スクリーン座標をワールド座標に
	worldMousePosMatrix = DirectX::XMMatrixIdentity();

	worldMousePosMatrix *= mousePosMatrix;
	worldMousePosMatrix *= invViewPortMatrix;
	worldMousePosMatrix *= invProjectionMatrix;

	//W除算
	worldMousePosMatrix.r[3].m128_f32[0] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[1] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[2] /= worldMousePosMatrix.r[3].m128_f32[3];
	worldMousePosMatrix.r[3].m128_f32[3] /= worldMousePosMatrix.r[3].m128_f32[3];

	worldMousePosMatrix *= invViewMatrix;


	//最遠点
	if (pFar)
	{
		*pFar =
		{
			worldMousePosMatrix.r[3].m128_f32[0] ,
			worldMousePosMatrix.r[3].m128_f32[1] ,
			worldMousePosMatrix.r[3].m128_f32[2]
		};
	}

#pragma endregion
}

#pragma region ボタン

//ボタン配列
//0 左ボタン
//1 右ボタン
//2 中ボタン
//3 左端のボタン2つの下側(上は反応無し)

bool Input::MouseButtonState(const MouseButton mouseButton)
{
	if (mouseState.rgbButtons[(int)mouseButton]) return true;
	return false;
}
bool Input::MouseButtonTrigger(const MouseButton mouseButton)
{
	if (!mousePrevious.rgbButtons[(int)mouseButton])
	{
		if (MouseButtonState(mouseButton)) return true;
	}
	return false;
}
bool Input::MouseButtonRelease(const MouseButton mouseButton)
{
	if (mousePrevious.rgbButtons[(int)mouseButton])
	{
		if (!MouseButtonState(mouseButton)) return true;
	}
	return false;
}
#pragma endregion


#pragma endregion

#pragma region ゲームパッド


bool Input::GetPadConnectedFlag(const UCHAR padNum)
{
	return padConnected[padNum - 1];
}

bool Input::PadCheck(const UCHAR padNum)
{
	if (padNum > 3)
	{
#ifdef _DEBUG

		OutputDebugString(L"パッド番号が4を超えています。\n");
#endif // _DEBUG
		return false;
	}
	if (!GetPadConnectedFlag(padNum))
	{
#ifdef _DEBUG
		std::wstring padNumWStr = std::to_wstring(padNum);
		padNumWStr += L"Pコントローラーが接続されていません。\n";
		OutputDebugString(padNumWStr.c_str());
#endif // _DEBUG
		return false;
	}

	return true;
}

#pragma region ボタン


bool Input::PadButtonState(const PadButton button, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;

	int num = padState[padNum - 1].Gamepad.wButtons & (int)button;
	if (num == (int)button)
		return true;

	return false;
}

bool Input::PadButtonTrigger(const PadButton button, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;

	int num = padPrevious[padNum - 1].Gamepad.wButtons & (int)button;
	if (num != (int)button &&
		PadButtonState(button, padNum))
		return true;

	return false;
}

bool Input::PadButtonRelease(const PadButton button, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;

	int num = padPrevious[padNum - 1].Gamepad.wButtons & (int)button;
	if (num == (int)button &&
		!PadButtonState(button, padNum))
		return true;

	return false;
}

float Input::DirectionalButtonAngle(const UCHAR padNum)
{
	if (!PadCheck(padNum))return -1.0f;

	bool right = Input::PadButtonState(PadButton::RIGHT, padNum);
	bool left = Input::PadButtonState(PadButton::LEFT, padNum);
	bool up = Input::PadButtonState(PadButton::UP, padNum);
	bool down = Input::PadButtonState(PadButton::DOWN, padNum);

	if (right && up)return 45.0f;
	if (up && left)return 135.0f;
	if (left && down)return 225.0f;
	if (down && right)return 315.0f;

	if (right)return 0.0f;
	if (up)return 90.0f;
	if (left)return 180.0f;
	if (down)return 270.0f;

	return -1.0f;
}


#pragma endregion


#pragma region スティック

#pragma region 左

#pragma region 状態


bool Input::LeftStickLeft(const float lXPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;


	if (-lXPar / 100.0f * MAX_AXIS_VALUE >= padState[padNum - 1].Gamepad.sThumbLX) return true;

	return false;
}

bool Input::LeftStickRight(const float lXPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;
	if (lXPar / 100.0f * MAX_AXIS_VALUE <= padState[padNum - 1].Gamepad.sThumbLX)
		return true;

	return false;
}

bool Input::LeftStickUp(const float lYPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;

	if (lYPar / 100.0f * MAX_AXIS_VALUE <= padState[padNum - 1].Gamepad.sThumbLY)
		return true;

	return false;
}

bool Input::LeftStickDown(const float lYPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;

	if (-lYPar / 100.0f * MAX_AXIS_VALUE >= padState[padNum - 1].Gamepad.sThumbLY) return true;

	return false;
}

#pragma endregion

#pragma region トリガー



bool Input::LeftStickLeftTrigger(const float lXPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;
	if (-lXPar / 100.0f * MAX_AXIS_VALUE > padPrevious[padNum - 1].Gamepad.sThumbLX
		&& LeftStickLeft(lXPar, padNum))return true;

	return false;
}

bool Input::LeftStickRightTrigger(const float lXPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;
	if (lXPar / 100.0f * MAX_AXIS_VALUE < padPrevious[padNum - 1].Gamepad.sThumbLX
		&& LeftStickRight(lXPar, padNum))return true;

	return false;
}

bool Input::LeftStickUpTrigger(const float lYPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;
	if (lYPar / 100.0f * MAX_AXIS_VALUE > padPrevious[padNum - 1].Gamepad.sThumbLY
		&& LeftStickUp(lYPar, padNum))return true;

	return false;
}

bool Input::LeftStickDownTrigger(const float lYPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;
	if (-lYPar / 100.0f * MAX_AXIS_VALUE < padPrevious[padNum - 1].Gamepad.sThumbLY
		&& LeftStickDown(lYPar, padNum))return true;

	return false;
}


#pragma endregion

float Input::LeftStickAngle(const float par, const UCHAR padNum)
{
	if (!PadCheck(padNum))return -1.0f;

	float x = static_cast<float>(padState[padNum - 1].Gamepad.sThumbLX);
	float y = static_cast<float>(padState[padNum - 1].Gamepad.sThumbLY);

	//未入力は-1をリターン
	if (x == 0 && y == 0)return -1.0f;

	float absX = abs(x);
	float absY = abs(y);
	if (par / 100.0f * MAX_AXIS_VALUE > absX + absY)return -1.0f;

	return LibMath::Vector2ToAngle(Vector2Normalize({ x,y }), true);
}

Vector2 Input::LeftStickVector2(const float par, const bool dimention3D, const UCHAR padNum)
{
	float angle = LeftStickAngle(par,padNum);
	if (angle < 0)return 0;


	return LibMath::AngleToVector2(angle, dimention3D);
}


Vector3 Input::LeftStickVector3(const float par, Camera* pCamera, const bool rotX, const bool rotY, const UCHAR padNum)
{
	float angle = LeftStickAngle(par, padNum);
	if (angle < 0)return 0;
	Vector2 vec2 = LibMath::AngleToVector2(angle, true);
	Vector3 result(vec2.x, 0, vec2.y);
	if (!pCamera) return result;

	//カメラに合わせて回転
	//Yは0固定にするように
	Vector3 cameraAngle = pCamera->GetAngle();
	if (rotX)result = LibMath::RotateVector3(result, Vector3(-1, 0, 0), cameraAngle.x);
	if (rotY)result = LibMath::RotateVector3(result, Vector3(0, -1, 0), cameraAngle.y);
	return result;
}



#pragma endregion

#pragma region 右


bool Input::RightStickLeft(const float lXPar, const UCHAR padNum)
{

	if (!PadCheck(padNum))return false;

	if (-lXPar / 100.0f * MAX_AXIS_VALUE >= padState[padNum - 1].Gamepad.sThumbRX)
		return true;

	return false;
}

bool Input::RightStickRight(const float lXPar, const UCHAR padNum)
{

	if (!PadCheck(padNum))return false;
	if (lXPar / 100.0f * MAX_AXIS_VALUE <= padState[padNum - 1].Gamepad.sThumbRX)
		return true;

	return false;
}

bool Input::RightStickUp(const float lYPar, const UCHAR padNum)
{

	if (!PadCheck(padNum))return false;

	if (lYPar / 100.0f * MAX_AXIS_VALUE <= padState[padNum - 1].Gamepad.sThumbRY)
		return true;

	return false;
}

bool Input::RightStickDown(const float lYPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;

	if (-lYPar / 100.0f * MAX_AXIS_VALUE >= padState[padNum - 1].Gamepad.sThumbRY)
		return true;

	return false;
}


#pragma region トリガー

bool Input::RightStickLeftTrigger(const float lXPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;
	if (-lXPar / 100.0f * MAX_AXIS_VALUE <= padPrevious[padNum - 1].Gamepad.sThumbRX
		&& RightStickLeft(lXPar, padNum))return true;

	return false;
}

bool Input::RightStickRightTrigger(const float lXPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;
	if (lXPar / 100.0f * MAX_AXIS_VALUE >= padPrevious[padNum - 1].Gamepad.sThumbRX
		&& RightStickRight(lXPar, padNum))return true;

	return false;
}

bool Input::RightStickUpTrigger(const float lYPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;
	if (lYPar / 100.0f * MAX_AXIS_VALUE <= padPrevious[padNum - 1].Gamepad.sThumbRY
		&& RightStickUp(lYPar, padNum))return true;

	return false;
}

bool Input::RightStickDownTrigger(const float lYPar, const UCHAR padNum)
{
	if (!PadCheck(padNum))return false;
	if (-lYPar / 100.0f * MAX_AXIS_VALUE >= padPrevious[padNum - 1].Gamepad.sThumbRY
		&& RightStickDown(lYPar, padNum))return true;

	return false;
}


#pragma endregion

Vector2 Input::RightStickVector2(const  float par, const bool dimention3D, const UCHAR padNum)
{
	float angle = RightStickAngle(par,padNum);
	if (angle < 0)return 0;
	return LibMath::AngleToVector2(angle, dimention3D);
}

Vector3 Input::RightStickVector3(const  float par, Camera* pCamera, const bool rotX, const bool rotY, const UCHAR padNum)
{
	float angle = RightStickAngle(par, padNum);
	if (angle < 0)return 0;
	Vector2 vec2 = LibMath::AngleToVector2(angle, true);
	Vector3 result(vec2.x, 0, vec2.y);
	if (!pCamera) return result;

	//カメラに合わせて回転
	//Yは0固定にするように
	Vector3 cameraAngle = pCamera->GetAngle();
	if (rotX)result = LibMath::RotateVector3(result, Vector3(-1, 0, 0), cameraAngle.x);
	if (rotY)result = LibMath::RotateVector3(result, Vector3(0, -1, 0), cameraAngle.y);
	return result;
}
#pragma endregion




float Input::RightStickAngle(const  float par, const UCHAR padNum)
{
	if (!PadCheck(padNum))return -1.0f;

	float x = static_cast<float>(padState[padNum - 1].Gamepad.sThumbRX);
	float y = static_cast<float>(padState[padNum - 1].Gamepad.sThumbRY);
	if (x == 0 && y == 0)return -1.0f;

	float absX = abs(x);
	float absY = abs(y);
	if (par / 100.0f * MAX_AXIS_VALUE > absX + absY)return -1.0f;



	return LibMath::Vector2ToAngle(Vector2Normalize({ x,y }), true);
}

#pragma endregion

#pragma region 振動

void Input::PadVibration(const float leftPar, const float rightPar, const UCHAR padNum)
{
	float leftParsent = leftPar;
	float rightParsent = rightPar;
	if (leftPar > 100.0f)
	{
		leftParsent = 100.0f;
	}
	if (leftPar < 0.0f)
	{
		leftParsent = 0.0f;
	}
	if (rightPar > 100.0f)
	{
		rightParsent = 100.0f;
	}
	if (rightPar < 0.0f)
	{
		rightParsent = 0.0f;
	}

	if (!PadCheck(padNum))return;

	XINPUT_VIBRATION v;
	v.wLeftMotorSpeed = static_cast<WORD>(leftParsent / 100.0f) * MAX_VIBRATION_VALUE;
	v.wRightMotorSpeed = static_cast<WORD>(rightParsent / 100.0f) * MAX_VIBRATION_VALUE;
	XInputSetState(padNum - 1, &v);
}

#pragma endregion
#pragma endregion

