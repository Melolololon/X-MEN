#include "InputDeviceManager.h"

InputDeviceManager* InputDeviceManager::GetInstance()
{
	static InputDeviceManager instance;
	return &instance;
}

void InputDeviceManager::Initialize()
{
	currentInputDevice = InputDeviceType::KEYBOARD;
}

void InputDeviceManager::Update()
{
	bool isInputKeyboard = false;
	bool isInputController = false;

	// wasd space�̂����ꂩ��������Ă���Ȃ�L�[�{�[�h�t���O���I���ɂ���
	if (MelLib::Input::KeyState(DIK_W) ||
		MelLib::Input::KeyState(DIK_A) ||
		MelLib::Input::KeyState(DIK_S) ||
		MelLib::Input::KeyState(DIK_D) ||
		MelLib::Input::KeyState(DIK_SPACE))
	{
		isInputKeyboard = true;
		currentInputDevice = InputDeviceType::KEYBOARD;
	}

	// �R���g���[���[�̃X�e�B�b�N�����͂���Ă���Ȃ�R���g���[���[�t���O���I���ɂ���
	if (MelLib::Input::LeftStickAngle(InputDeviceInfo::STICK_START_ANGLE) > 0 || MelLib::Input::RightStickAngle(InputDeviceInfo::STICK_START_ANGLE) > 0)
	{
		isInputController = true;
		currentInputDevice = InputDeviceType::CONTROLLER;
	}


	if (isInputKeyboard && isInputController)
	{
		currentInputDevice = priorityDevice;
	}
}

InputDeviceType InputDeviceManager::GetCurrentInputDevice()
{
	return currentInputDevice;
}

void InputDeviceManager::SetPriorityDevice(InputDeviceType deviceType)
{
	priorityDevice = deviceType;
}

InputDeviceType InputDeviceManager::GetPriorityDevice()
{
	return priorityDevice;
}

InputDeviceManager::InputDeviceManager()
	: currentInputDevice(InputDeviceType::KEYBOARD)
	, priorityDevice(InputDeviceType::KEYBOARD)
{
}
