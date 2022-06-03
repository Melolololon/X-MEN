#include"BarrierEnemy.h"

#include"../Player.h"
#include<array>
#include"../MyLibrary/GameObjectManager.h"
#include "../FieldObjectWall.h"
#include "../GameManager.h"
#include <Random.h>

void BarrierEnemy::RefBallObject()
{
	if (refBallObject)return;
	// �{�[���̈ʒu��ݒ�
	for (const auto& v : MelLib::GameObjectManager::GetInstance()->GetRefGameObject()) {
		if (typeid(*v) == typeid(Ball)) {
			refBallObject = v;
			break;
		}
	}
}

void BarrierEnemy::SetRandomPosition()
{
	std::srand(std::rand());

	MelLib::Vector3 setPos;
	setPos.x = MelLib::Random::GetRandomNumberRangeSelect(-10, 10);
	setPos.z = MelLib::Random::GetRandomNumberRangeSelect(-10, 10);

	SetPosition(setPos);
}

BarrierEnemy::BarrierEnemy()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	const float SCALE = 2;
	const float MODEL_SIZE = 2 * SCALE;
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
	sphereDatas["main"][0].SetRadius(MODEL_SIZE * 0.5f);

	// �ϐ��̏�����
	hp = BarrierEnemyStatus::MAX_HP;
	playerDir = EnemyStatus::initPlayerDir;
	playerPos = EnemyStatus::initPlayerPos;
	// �}�W�b�N�i���o�[?�Ȃ̂Ō�ŕύX����
	frontDir = { 0,0,0 };
	// 0�ŏ�����
	ballDir.fill(0);

	ballCurrentNum = 0;
	ballBeforeNum = 0;
	firstCountflg = false;

	// �G�̈ړ��f�B���C�p
	delayPlayerPos.fill(0);
	moveCurrentNum = 0;
	moveBeforeNum = 0;
	delayStartFlg = false;

	SetRandomPosition();
}

void BarrierEnemy::Move()
{
	// �ړ��x�N�g��
	MelLib::Vector3 moveVector;
	// �ړ����x
	static const float MOVE_SPEED = 0.1f;

	if (delayStartFlg)
	{

		// �������v�Z���A��苗���ȓ��Ȃ�O�ɏo��悤�Ɉړ�
		const float DISTANCE_X = GetPosition().x - delayPlayerPos[moveBeforeNum].x;
		const float DISTANCE_Z = GetPosition().z - delayPlayerPos[moveBeforeNum].z;

		// moveBeforeNum�̒l��ύX���Ď��̃t���[���Ŏg���z��ԍ��̏���
		++moveBeforeNum;
		if (moveBeforeNum >= delayPlayerPos.size())
		{
			moveBeforeNum = 0;
		}

		float distance = sqrt(DISTANCE_X * DISTANCE_X + DISTANCE_Z * DISTANCE_Z);
		// ���݈�苗����lerp���g�킸�ۂ悤�ɂȂ��Ă���
		// ����g�p����悤�ɏC��

		const float DISTANCE_RESULT = distance - BarrierEnemyStatus::DISTANCE_TO_PLAYER;

		// ������ۂ��̌덷�͈�
		const float EPSILON = 0.5f;

		// �����ɉ����ċ߂Â����ǂ������C��
		if (DISTANCE_RESULT <= 0)
		{

			moveVector = -playerDir * MOVE_SPEED;
		}
		else if (DISTANCE_RESULT > EPSILON)
		{
			moveVector = playerDir * MOVE_SPEED;
		}

		else moveVector = { 0,0,0 };


		// ���Z
		// AddPosition�ASetPosition�͓����蔻����ꏏ�ɓ���
		AddPosition(moveVector * GameManager::GetInstance()->GetGameTime());

		pastVelocity = moveVector * GameManager::GetInstance()->GetGameTime();
	}
	else
	{
		// delay����O�̓����ǉ�����ꍇ�͂�����

	}


	// �����ϊ��p
	ChangePose();
}


void BarrierEnemy::ChangePose()
{
	if (firstCountflg)
	{

		const float PI = 3.1415926f;
		const float CALC_ANGLE = 180;

		// �����x�N�g�������Ɍ���������ύX
		MelLib::Vector3 result;

		// atan2�ŕ����x�N�g������v�Z
		result.y = atan2f(-ballDir[ballBeforeNum].z, ballDir[ballBeforeNum].x) * CALC_ANGLE / PI;

		SetAngle(result);

		// ���ʃx�N�g���̏�������
		frontDir = ballDir[ballBeforeNum];

		++ballBeforeNum;
		if (ballBeforeNum >= ballDir.size())
		{
			ballBeforeNum = 0;
		}

		// �o���A�p
		// �������ɂƂ肠�����Ń{�[���ւ̕����x�N�g��
		pBarrier.get()->SetBarrierPosition(GetPosition(), frontDir);
	}
	else
	{
		// ���ʃx�N�g���̏�������
		frontDir = ballDir[0];

		pBarrier.get()->SetBarrierPosition(GetPosition(), frontDir);
	}
	
}

void BarrierEnemy::Update()
{
	RefBallObject();

	if (!refBallObject)return;

	if (!moveCancel)
	{
		// �o���A���͂���Ă��邩�ǂ���
		if (pBarrier.get()->GetIsOpen()) 
		{ 
			Move(); 
		}
		else 
		{
			FollowToPlayer(BarrierEnemyStatus::FOLLOW_SPEED); 
		}
	}

	PushPosition();

	SetBallDir(refBallObject.get()->GetPosition());

	// hp���Ȃ��Ȃ����Ƃ��ɊǗ��N���X����폜
	if (hp <= 0)
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

	// �ǂƂ̏Փ˔���
	if (typeid(object) == typeid(FieldObjectWall))
	{
		// �q�b�g������Q���̃q�b�g�����@�������ɉ����o��
		MelLib::Vector3 otherNormal = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		MelLib::Vector3 pos = GetPosition() + -pastVelocity;
		SetPosition(pos);

		// �ǂ���x�N�g�����v�Z
		MelLib::Vector3 moveVector = pastVelocity - MelLib::Vector3Dot(pastVelocity, otherNormal) * otherNormal;
		moveVector *= pastVelocity.Length();

		AddPosition(moveVector);
	}
}

void BarrierEnemy::SetBallDir(const MelLib::Vector3& pos)
{
	// �{�[���̈ʒu�ւ̕����x�N�g�����Ƃ�ϐ��ɃZ�b�g
	MelLib::Vector3 result = pos - GetPosition();

	ballDir[ballCurrentNum] = result.Normalize();
	++ballCurrentNum;
	if (ballCurrentNum >= ballDir.size())
	{
		ballCurrentNum = 0;
		firstCountflg = true;
	}
}

void BarrierEnemy::SetBarrier(std::shared_ptr<EnemyBarrier> barrier)
{
	pBarrier = barrier;
}

void BarrierEnemy::SetPlayerPos(const MelLib::Vector3& pos)
{
	// �v���C���[�̈ʒu��z��Ɋi�[
	delayPlayerPos[moveCurrentNum] = pos;

	++moveCurrentNum;
	// �z�񐔂����傫���Ȃ�����0��
	if (moveCurrentNum >= delayPlayerPos.size())
	{
		moveCurrentNum = 0;
		delayStartFlg = true;
	}
}
