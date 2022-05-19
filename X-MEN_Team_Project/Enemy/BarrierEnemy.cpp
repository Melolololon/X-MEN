#include"BarrierEnemy.h"

#include"../Player.h"
#include<array>

BarrierEnemy::BarrierEnemy()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	const float MODEL_SIZE = 2;
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ڂ��킩��₷���悤�� ������f���f�[�^���ł����Ƃ��ɏC��
	modelObjects["main"].SetMulColor(MelLib::Color(255, 0, 255, 255));
	modelObjects["main"].SetScale(MODEL_SIZE);
	// �����ʒu��0,0,5��
	SetPosition(MelLib::Vector3(0, 0, 5));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(MODEL_SIZE*0.5f);

	// �ϐ��̏�����
	hp = BarrierEnemyStatus::MAX_HP;
	playerDir = EnemyStatus::initPlayerDir;
	playerPos = EnemyStatus::initPlayerPos;
	// �}�W�b�N�i���o�[?�Ȃ̂Ō�ŕύX����
	changePoseFrameCount = 0;
	frontDir = { 0,0,0 };
	// 0�ŏ�����
	ballDir.fill(0);
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
}


void BarrierEnemy::ChangePose()
{
	if (changePoseFrameCount >= BarrierEnemyStatus::CHANGE_POSE_FRAME)
	{
		const float PI = 3.1415926f;
		const float CALC_ANGLE = 180;

		// �����x�N�g�������Ɍ���������ύX

		MelLib::Vector3 result;

		// atan2�ŕ����x�N�g������v�Z
		result.x = atan2f(ballDir[0].x, ballDir[0].y) * CALC_ANGLE / PI;
		result.y = atan2f(-ballDir[0].z, ballDir[0].x) * CALC_ANGLE / PI;
		result.z = atan2f(ballDir[0].y, -ballDir[0].z) * CALC_ANGLE / PI;

		SetAngle(result);

		// ���ʃx�N�g���̏�������
		frontDir = ballDir[0];

		// �o���A�p
		// �������ɂƂ肠�����Ń{�[���ւ̕����x�N�g��
		pBarrier.get()->SetBarrierPosition(GetPosition(), frontDir);

		// �J�E���g��0��
		changePoseFrameCount = 0;
	}
	else
	{
		changePoseFrameCount++;
	}
}

void BarrierEnemy::BallDirSort()
{
	// �O�ɔz����l�߂�
	for (int i = 0; i < ballDir.size() -1; i++)
	{
		ballDir[i] = ballDir[i + 1];
	}

}

void BarrierEnemy::Update()
{	
	BallDirSort();

	Move();
	PushPosition();

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
	Enemy::Hit(object, shapeType, shapeName, hitObjShapeType, hitShapeName);
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

	ballDir.back() = result.Normalize();
}

void BarrierEnemy::SetBarrier(std::shared_ptr<EnemyBarrier> barrier)
{
	pBarrier = barrier;
}
