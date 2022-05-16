#pragma once
#include <Input.h>

enum class InputDeviceType
{
	KEYBOARD,
	CONTROLLER,
};

namespace InputDeviceInfo
{
	// 0 ~ 100 %�\�L
	const float STICK_START_ANGLE = 20;
}

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

	// �V���O���g���ɂ��邽�ߊe��R���X�g���N�^�E�f�X�g���N�^���v���C�x�[�g��
	 InputDeviceManager();
	 InputDeviceManager(const InputDeviceManager&) = delete;
	~InputDeviceManager() = default;
	 InputDeviceManager& operator=(const InputDeviceManager&) = delete;
};
