#pragma once
#include "UltimateSkill.h"
#include<GameObject.h>

namespace PlayerInitializeInfo
{
	const float MAX_HP = 100;
}

class Player :
    public MelLib::GameObject
{
private:
	float hp;

	// �K�E�Z�Q�[�W
	float ultimateSkillValue;

	bool isBarrier;
	bool isThrowingBall;

	UltimateSkill ultimateSkill;

	MelLib::Vector3 dirVector;
	// Barrier* barrier;
	// Ball* ball;

private:
	// ���݂̓��̓f�o�C�X����󂯎�������ʂɊ�Â��ăx�N�g����Ԃ�
	// ���K�����
	MelLib::Vector3 GetInputVector();

	// �����̃x�N�g�������Ɉړ�
	void Move(const MelLib::Vector3& vec);

	// �v���C���[�������������v�Z
	void CalclateDirection();

	// �o���A�t���O���I���ɂ���
	void UseBarrier(bool key);

	// ������t���O���I���ɂ���
	void ThrowingBall(bool key);

	// �K�E�Z�t���O���I���ɂ���
	void UseUltimateSkill(bool key);

public:
	Player();

	~Player();
	
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

	// �K�E�Z�Q�[�W�p�̒l�ɂ�������value�����Z
	void AddUltimatekillValue(int value);

	// �����̒l���g����HP�����炷
	void Damage(int value);

#pragma region Getter
	// �o���A�t���O���擾
	bool GetIsBarrier() const;
	// �{�[���𓊂���t���O���擾
	bool GetIsThrowingBall() const;
	// �K�E�Z�t���O���擾
	bool GetIsUltimateSkill() const;
	// �v���C���[�������Ă�������̃x�N�g�����擾
	MelLib::Vector3 GetDirection() const;
#pragma endregion

#pragma region Setter
	// �o���A�t���O������������
	void SetIsBarrier(bool flag);
	// �{�[���𓊂���t���O������������
	void SetIsThrowingBall(bool flag);
#pragma endregion
};
