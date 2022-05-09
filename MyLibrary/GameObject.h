#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionDetectionData.h"
#include"Physics.h"

#include"ModelObject.h"

#ifdef _DEBUG
#include"PipelineState.h"
#include"Material.h"
#endif // _DEBUG



namespace MelLib
{
	class GameObject
	{
	private:

		// �J���җp
#ifdef _DEBUG
		//����m�F�p���f���̃p�C�v���C��
		static PipelineState collisionCheckModelPipelineState;

		//����m�F�p���f���̃}�e���A��
		static ADSAMaterial material;

		////����m�F�p���f��

		//std::vector<ModelObject>sphereModelObjects;
		//std::vector<ModelObject>boxModelObjects;
		//std::vector<ModelObject>boardModelObjects;
		//std::array<std::vector<ModelObject>, 2>segmentModelObjects;

		////�J�v�Z�������L�΂��ƌ`����邩��A���Ɖ~���g�ݍ��킹�ĕ\�������ق�����������
		////����̃f�[�^�̕ϐ��͍��̂܂�(�p�x�����Ɖ~����g�ݍ��킹��̂���ςɂȂ�)
		//std::array<std::vector<ModelObject>, 2>capsuleModelObjects;

		// �V�������
		std::unordered_map<std::string, std::vector<ModelObject>>sphereModelObjects;
		std::unordered_map<std::string, std::vector<ModelObject>>boxModelObjects;
		std::unordered_map<std::string, std::vector<ModelObject>>boardModelObjects;
		std::unordered_map<std::string, std::array<std::vector<ModelObject>, 2>>segmentModelObjects;
		std::unordered_map<std::string, std::array<std::vector<ModelObject>, 2>>capsuleModelObjects;



#endif // _DEBUG

		Vector3 position = 0;
		Vector3 angle = 0;
		Vector3 scale = 1;

#pragma region �����֌W

		//���̂�������
		Vector3 force = 0;
		//�d��
		float mass = 1.0f;
		//���x
		Vector3 velocity = 0;
		//�d�͉����x
		static float gravutationalAcc;
		//�����x
		Vector3 acceleration = 0;

		//��������
		int fallTime = 0;
		//��������
		float fallStartSpeed = 0.0f;
		//���݂̗������x
		float currentFallVelovity = 0.0f;
		//�������Ă��邩�ǂ���
		bool isFall = false;
#pragma endregion

		//������������̃f�[�^
		SphereData hitSphereData;
		BoxData hitBoxData;
		Segment3DData hitSegment3DData;
		RayData hitLayData;
		PlaneData hitPlaneData;
		BoardData hitBoardData;
		CapsuleData hitCapsuleData;
		TriangleData hitTriangleData;

		// �Փˊm�F���Ɍv�Z���ꂽ���l
		SphereCalcResult sphereCalcResult;
		BoxCalcResult boxCalcResult;
		Segment3DCalcResult segmentCalcResult;
		Segment3DCalcResult capsuleCalcResult;
		RayCalcResult rayCalcResult;
		BoardCalcResult boardCalcResult;
		TriangleCalcResult triangleCalcResult;
		

	protected:

#pragma region ����f�[�^
		CollisionDetectionFlag collisionFlag;

		/*std::vector<SphereData> sphereData;
		std::vector<BoxData> boxData;
		std::vector<Segment3DData> segment3DData;
		std::vector<RayData> layData;
		std::vector<PlaneData>planeData;
		std::vector<BoardData>boardData;
		std::vector<CapsuleData>capsuleData;
		std::vector<TriangleData>triangleData;*/

		// ����V�������
		// std::unordered_map<�����蔻��̖��O,�����蔻��̔z��>
		std::unordered_map<std::string, std::vector<SphereData>>sphereDatas;
		std::unordered_map<std::string, std::vector<BoxData>>boxDatas;
		std::unordered_map<std::string, std::vector<Segment3DData>>segment3DDatas;
		std::unordered_map<std::string, std::vector<RayData>>layDatas;
		std::unordered_map<std::string, std::vector<PlaneData>>planeDatas;
		std::unordered_map<std::string, std::vector<BoardData>>boardDatas;
		std::unordered_map<std::string, std::vector<CapsuleData>>capsuleDatas;
		std::unordered_map<std::string, std::vector<TriangleData>>triangleDatas;
		std::unordered_map<std::string, std::vector<OBBData>>obbDatas;

#pragma endregion

		//�p�������N���X���i�[���A���莞�Ɏg���p
		std::vector<std::string>tags;

		std::unordered_map<std::string, ModelObject>modelObjects;

		//�\�[�g�p���l�B�\�[�g���������Œ������邽�߂̕ϐ�
		// 2022/04/15 ���ݖ�����
		short sortNumber = 0;

		//�����t���O(���ꂪtrue�ɂȂ�ƁA�I�u�W�F�N�g�}�l�[�W���[���珜�O�����)
		bool eraseManager = false;

		// ����\�����邩�ǂ���
		bool drawCollisionModel = true;


	private:
		/// <summary>
		/// �����蔻�萔�ɉ����ă��f���̐����A�폜���s���֐�
		/// </summary>
		void CollisionCheckModelCreateOrDelete
		(
			const std::unordered_map<std::string, size_t>& datas,
			std::unordered_map<std::string, std::vector<ModelObject>>& modelObjects,
			const ShapeType3D type
		);



		/// <summary>
		/// ���f���̈ړ�
		/// </summary>
		/// <param name="vec">�ړ���</param>
		void SetModelPosition(const Vector3& vec);

		/// <summary>
		/// ����̈ړ�
		/// </summary>
		/// <param name="vec"></param>
		void SetDataPosition(const Vector3& vec);

		void SetModelAngle(const Vector3& angle);

		void SetDataAngle(const Vector3& angle);

		void SetModelScale(const Vector3& scale);

		void SetDataScale(const Vector3& scale);


	protected:
		/// <summary>
		/// �S�Ă�ModelObject��`�悵�܂��B
		/// </summary>
		void AllDraw();
		
		// �Փ˂���������擾����֐�
		SphereData GetHitSphereData() const { return hitSphereData; }
		BoxData GetHitBoxData() const { return hitBoxData; }
		Segment3DData GetHitSegmentData() const { return hitSegment3DData; }
		PlaneData GetHitPlaneData() const { return hitPlaneData; }
		BoardData GetHitBoardData()const { return hitBoardData; }
		CapsuleData GetHitCapsuleData() const { return hitCapsuleData; }
		TriangleData GetHitTriangleData() const { return hitTriangleData; }

#pragma region �������Z
		//�����Ƃ���Hit�֐��Ŏ����ŌĂԂ悤�ɂ���?
		//���OPhysicsMove�ɕς���?
		//CalcMovePhysics�ŏd�͉����x�v�Z���Ȃ��悤�ɂ���?

		//�����������Z�܂Ƃ߂�?
		//CalcThrowUpPhysics�Ƃ����������͂܂Ƃ߂��Ȃ�

		//�������Z�s������}�l�[�W���[�ō��W�ړ�������?
		//�ړ��ʌv�Z����΂����ƈړ��ł���

		/// <summary>
		/// ���W�Ɋւ��镨�����Z���s���܂��B
		/// </summary>
		void CalcMovePhysics();



		/// <summary>
		/// �����������J�n���܂��B
		/// </summary>
		/// <param name="startSpeed">�����x</param>
		void FallStart(const float startSpeed)
		{
			this->fallStartSpeed = startSpeed;
			isFall = true;
		}

		/// <summary>
		/// �����������I�����܂��B
		/// </summary>
		void FallEnd()
		{
			fallTime = 0;
			fallStartSpeed = 0.0f;
			isFall = false;

			//�����ē����グ�ɂ������x���Ƃ肠��������
			//�{���͈������ɔ����̏���������ق�������
			velocity.y -= currentFallVelovity;


			currentFallVelovity = 0.0f;
		}



#pragma endregion

	public:

		//�R���X�g���N�^
		GameObject();
		//�f�X�g���N�^
		virtual ~GameObject();

		//�X�V
		virtual void Update();
		//�`��
		virtual void Draw();

		/*/// <summary>
		/// �����������̏���
		/// </summary>
		/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
		/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
		/// <param name="arrayNum">�����̉��ڂ̔���ɓ���������</param>
		/// <param name="hitObjColType">����̂ǂ̔���ɓ���������</param>
		/// <param name="hitObjArrayNum">����̉��ڂ̔���ɓ���������</param>*/

		/// <summary>
		/// �Փ˂������ɌĂяo����锻��
		/// </summary>
		/// <param name="object">�I�u�W�F�N�g</param>
		/// <param name="shapeType">�����̂ǂ̌`��̔���Ɠ���������</param>
		/// <param name="shapeName">���薼</param>
		/// <param name="hitObjShapeType">����̌`��</param>
		/// <param name="hitShapeName">����̔��薼</param>
		virtual void Hit
		(
			/*const GameObject* const object,
			const ShapeType3D& collisionType,
			const int arrayNum,
			const ShapeType3D& hitObjColType,
			const int hitObjArrayNum*/
			const GameObject& object,
			const ShapeType3D shapeType,
			const std::string& shapeName,
			const ShapeType3D hitObjShapeType,
			const std::string& hitShapeName
		);



		//void CalcHitPhysics(GameObject* hitObject,const Vector3& hutPreVelocity,const CollisionType& collisionType);

		//virtual const void* GetPtr()const;



		//�m���ɏ����l�����܂��Ă���ϐ�(eraseManager�Ȃ�)������������ϐ�(�������Y��h�~�p)
		//����ŏ����������Ƀw�b�_�ŏ���������?
		//�Ēǉ������Ƃ��ɏ��������������炱�̂܂܂ł���
		void FalseEraseManager();



#pragma region ���Z
		/// <summary>
		/// ���W�A���f���̍��W�A����̍��W�Ɉ��������Z���܂��B
		/// </summary>
		/// <param name="vec"></param>
		virtual void AddPosition(const Vector3& vec);
#pragma endregion


#pragma region �Z�b�g

		/// <summary>
		/// GameObject���ʂ̏d�͉����x���Z�b�g���܂��B
		/// </summary>
		/// <param name="acc"></param>
		static void SetGravutationalAcceleration(const float acc) { gravutationalAcc = acc; };

		/// <summary>
		/// ���W���Z�b�g���܂��B���f���ƏՓˊm�F�Ɏg���f�[�^�́A�Z�b�g�O�̍��W�Ƃ̍������ړ����܂��B
		/// </summary>
		/// <param name="pos"></param>
		virtual void SetPosition(const Vector3& pos);

		/// <summary>
		/// �p�x���Z�b�g���܂��B
		/// </summary>
		/// <param name="angle"></param>
		virtual void SetAngle(const Vector3& angle);

		virtual void SetScale(const Vector3& scale);

		/// <summary>
		/// �͂��Z�b�g���܂��B
		/// </summary>
		/// <param name="force"></param>
		void SetForce(const Vector3& force) { this->force = force; }

		/// <summary>
		/// �d�����Z�b�g���܂��B
		/// </summary>
		/// <param name="mass"></param>
		void SetMass(const float mass) { this->mass = mass; }

		void SetAddColor(const Color& color);
		void SetSubColor(const Color& color);
		void SetMulColor(const Color& color);

		void TrueEraseManager() { eraseManager = true; }

#pragma endregion

#pragma region �Q�b�g
		/// <summary>
		/// GameObject���ʂ̏d�͉����x���擾���܂��B
		/// </summary>
		/// <returns></returns>
		static float GetGravutationalAcceleration() { return gravutationalAcc; };

		/// <summary>
		/// ���W���擾���܂��B
		/// </summary>
		/// <returns></returns>
		Vector3 GetPosition()const { return position; }
		
		Vector3 GetAngle()const { return angle; }
		Vector3 GetScale()const { return scale; }

		/// <summary>
		/// ���x���擾���܂��B
		/// </summary>
		/// <returns></returns>
		Vector3 GetVelocity()const { return velocity; }

		/// <summary>
		/// �����x���擾���܂��B
		/// </summary>
		/// <returns></returns>
		Vector3 GetAcceleration()const { return acceleration; }

		/// <summary>
		/// �������Ă���͂��擾���܂��B
		/// </summary>
		/// <returns></returns>
		Vector3 GetForce()const { return force; }

		/// <summary>
		/// �d�����擾���܂��B
		/// </summary>
		/// <returns></returns>
		float GetMass()const { return mass; }

		/// <summary>
		/// �������܂��͓����グ�����ǂ������擾���܂��B
		/// </summary>
		/// <returns></returns>
		bool GetIsFall()const { return isFall; }

		short GetSortNumber() const { return sortNumber; }


		std::vector<std::string>GetTags()const { return tags; }

		/// <summary>
		/// ObjectManager����폜����t���O��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		bool GetEraseManager()const { return eraseManager; }

#pragma region ���f���֌W
		const std::unordered_map<std::string, ModelObject>& GetRefModelObjects()const { return modelObjects; }
#pragma endregion

#pragma region ����֌W

#pragma endregion

		/// <summary>
		/// �V�[���G�f�B�^����I�u�W�F�N�g�Ǘ��N���X�ɒǉ�����Ƃ��ɃI�u�W�F�N�g�Ǘ��N���X�ɒǉ�����|�C���^��Ԃ��֐��ł��B
		/// </summary>
		/// <returns>�I�u�W�F�N�g�Ǘ��N���X�ɒǉ�����shared_ptr</returns>
		virtual std::shared_ptr<GameObject> GetNewPtr();


#pragma endregion


#pragma region ����p�֐�


		// ����p�֐�
		CollisionDetectionFlag GetCollisionFlag() const { return collisionFlag; }
		//std::vector<SphereData> GetSphereData() const { return sphereData; }
		//std::vector<BoxData> GetBoxData() const { return boxData; }
		//std::vector<Segment3DData> GetSegmentData() const { return segment3DData; }
		//std::vector<PlaneData> GetPlaneData() const { return planeData; }
		//std::vector<BoardData> GetBoardData()const { return boardData; }
		//std::vector<CapsuleData>GetCapsuleData() const { return capsuleData; }
		//std::vector<TriangleData>GetTriangleData() const { return triangleData; }


		std::unordered_map<std::string, std::vector<SphereData>> GetSphereDatas() const { return sphereDatas; }
		std::unordered_map<std::string, std::vector<BoxData>> GetBoxDatas() const { return boxDatas; }
		std::unordered_map<std::string, std::vector<OBBData>> GetOBBDatas() const { return obbDatas; }
		std::unordered_map<std::string, std::vector<Segment3DData>> GetSegmentDatas() const { return segment3DDatas; }
		std::unordered_map<std::string, std::vector<PlaneData>> GetPlaneDatas() const { return planeDatas; }
		std::unordered_map<std::string, std::vector<BoardData>> GetBoardDatas()const { return boardDatas; }
		std::unordered_map<std::string, std::vector<CapsuleData>>GetCapsuleDatas() const { return capsuleDatas; }
		std::unordered_map<std::string, std::vector<TriangleData>>GetTriangleDatas() const { return triangleDatas; }



		// �����Q�Ǝ擾����Ȃ���Set�ɂ���?
		/*void SetSphereCalcResult(const SphereCalcResult& result, const UINT index) { sphereData[index].SetCalcResult(result); }
		void SetBoxCalcResult(const BoxCalcResult& result, const UINT index) { boxData[index].SetCalcResult(result); }
		void SetSegmentCalcResult(const Segment3DCalcResult& result, const UINT index) { segment3DData[index].SetCalcResult(result); }
		void SetBoardCalcResult(const BoardCalcResult& result, const UINT index) { boardData[index].SetCalcResult(result); }
		void SetCapsuleCalcResult(const Segment3DCalcResult& result, const UINT index)
		{
			capsuleData[index].GetRefSegment3DData().SetCalcResult(result);
		}
		void SetTriangleCalcResult(const TriangleCalcResult& result, const UINT index)
		{
			triangleData[index].SetCalcResult(result);
		}*/
		void SetSphereCalcResult(const SphereCalcResult& result) { sphereCalcResult = result; }
		void SetBoxCalcResult(const BoxCalcResult& result) { boxCalcResult = result;}
		void SetSegmentCalcResult(const Segment3DCalcResult& result) { segmentCalcResult = result;}
		void SetBoardCalcResult(const BoardCalcResult& result) { boardCalcResult = result;}
		void SetCapsuleCalcResult(const Segment3DCalcResult& result)
		{
			capsuleCalcResult = result;
		}
		void SetTriangleCalcResult(const TriangleCalcResult& result)
		{
			triangleCalcResult = result;
		}

		SphereCalcResult GetSphereCalcResult()const { return sphereCalcResult; }
		BoxCalcResult GetBoxCalcResult()const { return boxCalcResult; }
		Segment3DCalcResult GetSegmentCalcResult()const { return segmentCalcResult; }
		BoardCalcResult GetBoardCalcResult()const { return boardCalcResult; }
		Segment3DCalcResult GetCapsuleCalcResult()const { return capsuleCalcResult; }
		TriangleCalcResult GetTriangleCalcResult()const { return triangleCalcResult; }


		//Vector3& GetLineSegmentHitPosition(const int num);
		//Vector3& GetBoardHitPosition(const int num);
		//BoxHitDirection& GetSphereBoxHitDistance(const int num) { return sphereData[num].boxHitDistance; }
		//BoxHitDirection& GetBoxBoxHitDistance(const int num) { return boxData[num].boxHitDistance; }


		// �J���җp
		// �q�b�g��������̓����蔻��̏���n���֐��B

		void SetHitSphereData(const SphereData& sphere) { hitSphereData = sphere; }
		void SetHitBoxData(const BoxData& box) { hitBoxData = box; }
		void SetHitBoardData(const BoardData& board) { hitBoardData = board; }
		void SetHitPlaneData(const PlaneData& plane) { hitPlaneData = plane; }
		void SetHitSegment3DData(const Segment3DData& segment) { hitSegment3DData = segment; }
		void SetHitCapsuleData(const CapsuleData& capsule) { hitCapsuleData = capsule; }
		void SetHitTriangleData(const TriangleData& tri) { hitTriangleData = tri; }

		// �J���җp
#ifdef _DEBUG
		static void CreateCollisionCheckModelPipelineState();

		//�Փˊm�F�p���f�������A�폜
		void CreateCollisionCheckModel();
		//�Փˊm�F�p���f���̃f�[�^�Z�b�g
		void SetCollisionCheckModelData();
		//�Փˊm�F�p���f���̕`��
		void DrawCollisionCheckModel();

#endif // _DEBUG



#pragma endregion

	};
}