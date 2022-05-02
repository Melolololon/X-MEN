#include "Player.h"

#include<Input.h>
#include "InputDeviceManager.h"
#include"TestObject.h"

MelLib::Vector3 Player::GetInputVector()
{
	auto inputDeviceManager = InputDeviceManager::GetInstance();
	auto currentInputDevice = inputDeviceManager->GetCurrentInputDevice();
	MelLib::Vector3 moveVector;
	bool isInput = false;

	// ���݂̓��̓f�o�C�X�����ƂɈړ��x�N�g�����v�Z
	if (currentInputDevice == InputDeviceType::CONTROLLER)
	{
		moveVector = MelLib::Input::LeftStickVector3();
		isInput = MelLib::Input::LeftStickAngle() > 0 ? true : false;
	}

	if (currentInputDevice == InputDeviceType::KEYBOARD)
	{
		if (MelLib::Input::KeyState(DIK_A))
		{
			moveVector.x--;
			isInput = true;
		}
		if (MelLib::Input::KeyState(DIK_D))
		{
			moveVector.x++;
			isInput = true;
		}
		if (MelLib::Input::KeyState(DIK_W))
		{
			moveVector.z++;
			isInput = true;
		}
		if (MelLib::Input::KeyState(DIK_S))
		{
			moveVector.z--;
			isInput = true;
		}
	}
	
	// �ړ��L�[����͂��Ă���Ȃ�����x�N�g�����X�V
	if (isInput)dirVector = moveVector.Normalize();

	return moveVector.Normalize();
}

void Player::Move(const MelLib::Vector3& vec)
{
	static const float MOVE_SPEED = 0.7f;
	MelLib::Vector3 addVector = vec * MOVE_SPEED;

	GameObject::AddPosition(addVector);
	CalclateDirection();
}

void Player::CalclateDirection()
{
	const float PI = 3.141592f;
	const float MAX_ANGLE = 180.0f;

	// �����x�N�g�������Ƃɉ�]�l���v�Z
	float angleY = MAX_ANGLE / PI * std::atan2f(-dirVector.z, dirVector.x);
	GameObject::SetAngle(MelLib::Vector3(0, angleY, 0));
}

void Player::UseBarrier(bool key)
{
	if (!key)return;

	// �o���A���g�p���Ȃ瑦�I��
	if (isBarrier)return;
	//// �{�[����ێ����Ă���Ȃ�o���A�͓W�J�ł��Ȃ�
	//if(ball)return;

	isBarrier = true;
}

void Player::ThrowingBall(bool key)
{
	if (!key)return;

	//// �{�[����ێ����Ă��Ȃ��Ȃ�I��
	//if(!ball)return;

	isThrowingBall = true;
}

void Player::UseUltimateSkill(bool key)
{
	if (!key)return;

	// �X�L���g�p���Ȃ瑦�I��
	if (ultimateSkill.GetIsUsingSkill())return;

	// �X�L���g�p
	ultimateSkill.Use();
}

Player::Player()
	: hp(PlayerInitializeInfo::MAX_HP)
	, ultimateSkillValue(0)
	, isBarrier(false)
	, isThrowingBall(false)
	, ultimateSkill(UltimateSkill())
	, dirVector(MelLib::Vector3())
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ʒu��0,0,5��
	SetPosition(MelLib::Vector3(0, 0, 5));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);
}

Player::~Player()
{
	// �Ǘ��N���X����폜
	eraseManager = true;
}

void Player::Update()
{
	Move(GetInputVector());

	ultimateSkill.Update();

	if (MelLib::Input::KeyTrigger(DIK_SPACE))ultimateSkill.AddValue(10);

	// �e�Z�������s���֐��ɑΉ������L�[�̃g���K�[�𑗂��Ċ֐����Ŏ��s���邩���f������
	UseBarrier(MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A));
	ThrowingBall(MelLib::Input::KeyTrigger(DIK_SPACE) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::A));
	UseUltimateSkill(MelLib::Input::KeyTrigger(DIK_Z) || MelLib::Input::PadButtonTrigger(MelLib::PadButton::X));

	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255));
}

void Player::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	AllDraw();
	// �K�E�Z�Q�[�W�̕`��
	ultimateSkill.Draw();
}

void Player::Hit
(
	const GameObject& object,
	const MelLib::ShapeType3D shapeType,
	const std::string& shapeName,
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{
	// �����ɓ����������̏������L�q
	// typeid�Ȃǂŏ����𕪂����肷��

	// �e�X�g�I�u�W�F�N�g�ƏՓ˂�����F�ύX
	if (typeid(object) == typeid(TestObject))
	{
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));
	}
}

void Player::AddUltimatekillValue(int value)
{
	ultimateSkillValue += value;
}

void Player::Damage(int value)
{
	hp -= value;
}

bool Player::GetIsBarrier() const
{
	return isBarrier;
}

bool Player::GetIsThrowingBall() const
{
	return isThrowingBall;
}

bool Player::GetIsUltimateSkill() const
{
	return ultimateSkill.GetIsUsingSkill();
}

MelLib::Vector3 Player::GetDirection() const
{
	return dirVector;
}

void Player::SetIsBarrier(bool flag)
{
	isBarrier = flag;
}

void Player::SetIsThrowingBall(bool flag)
{
	isThrowingBall = flag;
}
