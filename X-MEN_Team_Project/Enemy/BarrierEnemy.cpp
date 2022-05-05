#include"BarrierEnemy.h"

#include"../Player.h"


BarrierEnemy::BarrierEnemy()
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
	hp = BarrierEnemyStatus::MAX_HP;
	playerDir = EnemyStatus::initPlayerDir;
	playerPos = EnemyStatus::initPlayerPos;
}

void BarrierEnemy::Move()
{
	// �ړ��x�N�g��
	MelLib::Vector3 moveVector;
	// �ړ����x
	static const float MOVE_SPEED = 0.7f;

	// �������v�Z���A��苗���ȓ��Ȃ�O�ɏo��悤�Ɉړ�
	const float DISTANCE_X = GetPosition().x - playerPos.x;
	const float DISTANCE_Z = GetPosition().z - playerPos.z;

	float distance = sqrt(DISTANCE_X * DISTANCE_X + DISTANCE_Z * DISTANCE_Z);
	// ���݈�苗����lerp���g�킸�ۂ悤�ɂȂ��Ă���
	// ����g�p����悤�ɏC��
	if (distance <= BarrierEnemyStatus::DISTANCE_TO_PLAYER)moveVector = -playerDir * MOVE_SPEED;
	else moveVector = { 0,0,0 };


	// ���Z
	// AddPosition�ASetPosition�͓����蔻����ꏏ�ɓ���
	AddPosition(moveVector);
}

void BarrierEnemy::Update()
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

void BarrierEnemy::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	AllDraw();
}

void BarrierEnemy::Hit(const GameObject& object, const MelLib::ShapeType3D shapeType, const std::string& shapeName, const MelLib::ShapeType3D hitObjShapeType, const std::string& hitShapeName)
{
	// �v���C���[�ƏՓ˂�����F�ύX�@����{�[���Ɠ��������Ƃ��ɂ��K�p
	if (typeid(object) == typeid(Player))
	{
		modelObjects["main"].SetMulColor(MelLib::Color(100, 100, 100, 255));

	}

}
