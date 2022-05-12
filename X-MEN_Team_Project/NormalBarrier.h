#pragma once
#include<GameObject.h>
namespace
{
	//�W�J�ł��鎞��
	int openTime = 60;
	//Player�ƃo���A�̊Ԃ̊Ԋu
	float radius = 3.0f;
}
class NormalBarrier :
	public MelLib::GameObject
{
public:
	NormalBarrier();

	// �X�V
	void Update()override;

	//�`��
	void Draw()override;

	/// <summary>
	/// �����������ɌĂ΂��֐�
	/// </summary>
	/// <param name="object">�Փ˂����I�u�W�F�N�g�̎Q��</param>
	/// <param name="shapeType">����ƏՓ˂��������̓����蔻��̌`��</param>
	/// <param name="shapeName">����ƏՓ˂��������̓����蔻��̖��O</param>
	/// <param name="hitObjShapeType">�Փ˂����I�u�W�F�N�g�̓����蔻��̌`��</param>
	/// <param name="hitShapeName">�Փ˂����I�u�W�F�N�g�̔���̖��O</param>
	void Hit
	(
		const GameObject& object,
		const MelLib::ShapeType3D shapeType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjShapeType,
		const std::string& hitShapeName
	)override;
	/// <summary>
	/// �w�肵�����W�𒆐S�Ƀo���A��W�J
	/// �ړ��ʂŊp�x�v�Z
	/// </summary>
	/// <param name="positon">�o���A�������������L�����N�^�[�̍��W</param>
	/// <param name="move">�o���A�������������L�����N�^�[�̈ړ��l(velocity��)</param>
	void SetBarrierPosition(MelLib::Vector3 positon, MelLib::Vector3 move);

	//Setter
	void SetIsOpen(bool flag) { isOpen = flag; }
	//Getter
	bool GetIsOpen()const { return isOpen; }

private:
	//�W�J����Ă��邩
	bool isOpen;
	//����
	int time;
};

