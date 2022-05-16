#pragma once
#include <Input.h>

enum class InputDeviceType
{
	KEYBOARD,
	CONTROLLER,
};

namespace InputDeviceInfo
{
	// 0 ~ 100 %表記
	const float STICK_START_ANGLE = 20;
}

// 現在の操作デバイスを管理するシングルトンクラス
class InputDeviceManager
{
private:
	// 現在使用中のデバイス
	InputDeviceType currentInputDevice;

	// ふたつのデバイスを操作している際にどちらを優先させるか判定する際につかう
	InputDeviceType priorityDevice;
public:
	static InputDeviceManager* GetInstance();
	void Initialize();
	void Update();

	InputDeviceType GetCurrentInputDevice();

	// ふたつのデバイスを操作している際にどちらを優先させるか設定
	void SetPriorityDevice(InputDeviceType deviceType);
private:
	InputDeviceType GetPriorityDevice();

	// シングルトンにするため各種コンストラクタ・デストラクタをプライベート化
	 InputDeviceManager();
	 InputDeviceManager(const InputDeviceManager&) = delete;
	~InputDeviceManager() = default;
	 InputDeviceManager& operator=(const InputDeviceManager&) = delete;
};
