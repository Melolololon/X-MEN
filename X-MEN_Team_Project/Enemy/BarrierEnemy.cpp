#include"BarrierEnemy.h"

#include"../Player.h"


BarrierEnemy::BarrierEnemy()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ڂ��킩��₷���悤�� ������f���f�[�^���ł����Ƃ��ɏC��
	modelObjects["main"].SetMulColor(MelLib::Color(255, 0, 255, 255));

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
	// �}�W�b�N�i���o�[�ł͂���̂Ō�ŕύX����
	ChangePoseFrameCount = 0;
	ballDir = { 0,0,0 };

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


	// �����ϊ��p
	ChangePose();

	// �o���A�p
	// �������ɂƂ肠�����Ń{�[���ւ̕����x�N�g��
	pBarrier.get()->SetBarrierPosition(GetPosition(), ballDir);
}


void BarrierEnemy::ChangePose()
{

	MelLib::Vector3 temp = GetAngle();

	const float PI = 3.1415926f;
	const float CALC_ANGLE = 180;

	// �����x�N�g�������Ɍ���������ύX
	
	
	// �O�̊p�x���擾
	MelLib::Vector3 angle = GetAngle();

	MelLib::Vector3 result;

	// atan2�ŕ����x�N�g������v�Z
	result.x = atan2f(ballDir.x, ballDir.y) * CALC_ANGLE / PI;
	result.y = atan2f(-ballDir.z, ballDir.x) * CALC_ANGLE / PI;
	result.z = atan2f(ballDir.y, -ballDir.z) * CALC_ANGLE / PI;

	SetAngle(result);

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

	modelObjects["main"].SetMulColor(MelLib::Color(255, 0, 255, 255));

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

void BarrierEnemy::SetBallDir(const MelLib::Vector3& pos)
{
	// �{�[���̈ʒu�ւ̕����x�N�g�����Ƃ�ϐ��ɃZ�b�g
	MelLib::Vector3 result = pos - GetPosition();

	ballDir = result.Normalize();
}

void BarrierEnemy::SetBarrier(std::shared_ptr<EnemyBarrier> barrier)
{
	pBarrier = barrier;
}
