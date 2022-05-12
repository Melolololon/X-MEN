#pragma once
#include<GameObject.h>
namespace parameter
{
	//�ϋv�l�̏����l
	const float HP = 150.0f;
	//Player�ƃo���A�̊Ԃ̊Ԋu
	const float RADIUS = 3.0f;
}
class EnemyBarrier :
	public MelLib::GameObject
{
public:
	EnemyBarrier();

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
	void SetBarrierPosition(MelLib::Vector3 position, MelLib::Vector3 move);
	//�o���A�W�J
	void OpenBarrier();
	//Setter
	void SetIsOpen(bool flag) { isOpen = flag; }
	void SetHp(int hp) { currentHp = hp; }
	//Getter
	bool GetIsOpen()const { return isOpen; }
	int GetHp()const { return currentHp; }
private:
	//�W�J����Ă��邩
	bool isOpen;
	//���݂̑ϋv�l
	float currentHp;

};

