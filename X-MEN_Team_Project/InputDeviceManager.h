#pragma once
#include <Input.h>

enum class InputDeviceType
{
	KEYBOARD,
	CONTROLLER,
};

// ���݂̑���f�o�C�X���Ǘ�����V���O���g���N���X
class InputDeviceManager
{
private:
	// ���ݎg�p���̃f�o�C�X
	InputDeviceType currentInputDevice;

	// �ӂ��̃f�o�C�X�𑀍삵�Ă���ۂɂǂ����D�悳���邩���肷��ۂɂ���
	InputDeviceType priorityDevice;
public:
	static InputDeviceManager* GetInstance();
	void Initialize();
	void Update();

	InputDeviceType GetCurrentInputDevice();

	// �ӂ��̃f�o�C�X�𑀍삵�Ă���ۂɂǂ����D�悳���邩�ݒ�
	void SetPriorityDevice(InputDeviceType deviceType);
private:
	InputDeviceType GetPriorityDevice();

	// �V���O���g���ɂ��邽�ߊe��
	 InputDeviceManager();
	 InputDeviceManager(const InputDeviceManager&) = delete;
	~InputDeviceManager() = default;
	 InputDeviceManager& operator=(const InputDeviceManager&) = delete;
};
