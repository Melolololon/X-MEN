#include"FollowEnemy.h"

#include"../Player.h"


FollowEnemy::FollowEnemy()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ڂ��킩��₷���悤�� ������f���f�[�^���ł����Ƃ��ɏC��
	modelObjects["main"].SetMulColor(MelLib::Color(0, 0, 255, 255));


	// �����ʒu��0,0,5��
	SetPosition(MelLib::Vector3(0, 0, 5));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);

	// �ϐ��̏�����
	hp = FollowEnemyStatus::MAX_HP;
	playerDir = EnemyStatus::initPlayerDir;
	playerPos = EnemyStatus::initPlayerPos;
}

void FollowEnemy::Move()
{
	// �ړ��x�N�g��
	MelLib::Vector3 moveVector;
	// �ړ����x
	static const float MOVE_SPEED = 0.5f;

	moveVector = playerDir * MOVE_SPEED;

	// ���Z
	// AddPosition�ASetPosition�͓����蔻����ꏏ�ɓ���
	AddPosition(moveVector);

}

void FollowEnemy::Update()
{
	Move();

	static const float ZERO = 0.0f;
	// hp���Ȃ��Ȃ����Ƃ��ɊǗ��N���X����폜
	if (hp <= ZERO)
	{
		eraseManager = true;
	}

	modelObjects["main"].SetMulColor(MelLib::Color(0, 0, 255, 255));

}

void FollowEnemy::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	AllDraw();
}

void FollowEnemy::Hit(const GameObject& object, const MelLib::ShapeType3D shapeType, const std::string& shapeName, const MelLib::ShapeType3D hitObjShapeType, const std::string& hitShapeName)
{
	// �v���C���[�ƏՓ˂�����F�ύX�@����{�[���Ɠ��������Ƃ��ɂ��K�p
	if (typeid(object) == typeid(Player))
	{
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));
	}

}