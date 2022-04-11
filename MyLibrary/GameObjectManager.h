#pragma once
#include"GameObject.h"
#include"GameObject2D.h"
#include"CollisionType.h"
#include"MouseCursor.h"

#include<vector>
#include<memory>

//�J�[�\������p�̃\�[�g����āA�S�̃\�[�g�Ȃ���
//�J�[�\������p�z��Ɉڂ��ă\�[�g

//�����2��m�F����̏�������(objA��objB�̔��苁�߂�Ƃ��AobjA�����AobjB����Hit���Ăяo���̂����������Ȃ�)����ǂ��ɂ�����B

namespace MelLib
{
	class GameObjectManager
	{
	public:
		enum ObjectSortType
		{
			//XYZ�̍��v�l�Ń\�[�g
			OBJECT_SORT_NONE,
			OBJECT_SORT_XYZ_SUM,
			OBJECT_SORT_X,
			OBJECT_SORT_Y,
			OBJECT_SORT_Z,
			//�J��������߂���
			OBJECT_SORT_NEAR_DISTANCE,
			//�J�������牓����
			OBJECT_SORT_FAR_DISTANCE,
			//�\�[�g�ԍ��ł̃\�[�g
			OBJECT_SORT_SORT_NUMBER,
		};


	private:

		std::vector<std::shared_ptr<GameObject>>objects;
		//�ǉ����ꂽ���̂��ꎞ�I�ɓ���Ă����z��
		std::vector<std::shared_ptr<GameObject>>addObjects;

		std::vector<std::shared_ptr<GameObject2D>>object2Ds;
		std::vector<std::shared_ptr<GameObject2D>>addObject2Ds;
		//CollisionFlag checkCollision;


		//�ǉ������t���[�����ƂɃ\�[�g���邩
		ObjectSortType addObjectSort;
		bool addObjectSortOrderType;

		//�J�[�\������
		bool checkMouseCollision = false;
		Vector3 cameraPosition;
		std::unique_ptr<MouseCursor>cursor;

		//�f�[�^
		Vector3 nearPos;
		Vector3 farPos;

	private:

		GameObjectManager();
		~GameObjectManager();
		/// <summary>
		/// eraseManager��true���ǂ������m�F���܂��Btrue�̏ꍇ�͍폜����܂�
		/// </summary>
		void EraseObjectCheck();


	public:
		GameObjectManager(const GameObjectManager& obj) = delete;
		GameObjectManager& operator=(const GameObjectManager& obj) = delete;

		static GameObjectManager* GetInstance();

		void Initialize();

		void Update();

		void Draw();

		void Finalize();

		///// <summary>
		///// �ǂ̔�����s������ݒ肵�܂�
		///// </summary>
		///// <param name="type"></param>
		//void SetCollisionFlag3D(const CollisionFlag& type);

		/// <summary>
		/// �}�E�X�J�[�\���Ƃ̔�����Ƃ邩�ݒ肵�܂�
		/// </summary>
		/// <param name="flag"></param>
		void SetMouseCollisionFlag(const bool flag);

#pragma region �I�u�W�F�N�g�̔z��֌W

		/// <summary>
		/// �I�u�W�F�N�g�̔z��̃��������m�ۂ��܂��B
		/// </summary>
		/// <param name="reserveNum"></param>
		void ReserveObjectArray(const int reserveNum);

		/// <summary>
		/// �I�u�W�F�N�g��ǉ����܂��B
		/// </summary>
		/// <param name="object"></param>
		void AddObject(const std::shared_ptr<GameObject>& object);


		/// <summary>
		/// �I�u�W�F�N�g��ǉ����܂��B
		/// </summary>
		/// <param name="object"></param>
		void AddObject(const std::shared_ptr<GameObject2D>& object);

		/// <summary>
		/// �ǉ������t���[�����ƂɃ\�[�g���邩��ݒ肵�܂��B�ǉ����Ȃ��ꍇ�AorderType�͖�������܂��B
		/// </summary>
		/// <param name="sort">�\�[�g�̎d��</param>
		/// <param name="orderType">true ����  false �~��</param>
		void SetAddObjectSortState(const ObjectSortType& sort, const bool& orderType);


		/// <summary>
		/// �I�u�W�F�N�g�̔z����\�[�g���܂��B
		/// </summary>
		/// <param name="sort">�\�[�g�̎d��</param>
		/// <param name="orderType">true ����  false �~��</param>
		void ObjectSort(const ObjectSortType& sort, const bool& orderType);

		/// <summary>
		/// �I�u�W�F�N�g�}�l�[�W���[�Ɋi�[���ꂽ�I�u�W�F�N�g�̔z���const�Q�ƂŎ󂯎��܂�
		/// </summary>
		/// <returns></returns>
		const std::vector<std::shared_ptr<GameObject>>& GetRefGameObject() { return objects; };

		/// <summary>
		/// �I�u�W�F�N�g�}�l�[�W���[�Ɋi�[���ꂽ�I�u�W�F�N�g2D�̔z���const�Q�ƂŎ󂯎��܂�
		/// </summary>
		/// <returns></returns>
		const std::vector<std::shared_ptr<GameObject2D>>& GetRefGameObject2D() { return object2Ds; };

#pragma endregion



		/// <summary>
		/// �S�ẴI�u�W�F�N�g��z�񂩂�폜���܂�
		/// </summary>
		void AllEraseObject();

		void AllEraseObject2D();
		void AllEraseObject3D();
	};

}