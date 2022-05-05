#pragma once

#include"GameObject.h"

namespace MelLib 
{
	// objects�̃I�u�W�F�N�g���ǂ��}�l�[�W���[�ɒǉ�����?

	class SceneEditer
	{
	private:
		SceneEditer(){}
		~SceneEditer(){}

		SceneEditer(SceneEditer& s) = delete;
		SceneEditer& operator=(SceneEditer& s) = delete;

		void Save();

		void SelectObjectUpdate();
	private:
		// �����ێ����Ƃ��΃��W�I�{�^���̒l��for�ŉ񂵂ăf�[�^�ɃA�N�Z�X�ł���(int�ŃA�N�Z�X�ł���)����map
		// �����𑼂̔z��ɕۑ�����̂����������B���[�v����K�v�Ȃ��Ȃ邩�瑁���B���ǃ������g��
		// for�Ŏ擾����ƎQ�Ƃɑ���ł��Ȃ����Avector�̃|�C���^�͌l�I�Ɏg�������Ȃ����Afor�ł����Q�Ƃł��Ȃ����番����
		// ���W�I�{�^���̏��Ԃ߂��Ⴍ����ɂȂ邩��map�ɕύX
		// �I�u�W�F�N�g
		std::map< std::string,std::vector<std::shared_ptr<MelLib::GameObject>>>pObjects;
		// ���W�I�{�^���̖߂�l�ŏ��objects�ɃA�N�Z�X���邽�߂̔z��
		std::unordered_map<int ,std::string>objectOrderDatas;
		 
		// �I������Ă���^�C�v
		int selectType = 0;

		GameObject* pSelectObject = nullptr;

		bool isEdit = false;
		bool releaseEdit = false;
	public:
		static SceneEditer* GetInstance();

		/// <summary>
		/// �I�u�W�F�N�g���G�f�B�^�ɓo�^���܂��B
		/// </summary>
		/// <param name="object">�I�u�W�F�N�g</param>
		/// <param name="objectType">�I�u�W�F�N�g�̎��</param>
		void RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject,const std::string& objectType);
		
		void Update();

		void Draw();

		/// <summary>
		/// �G�f�B�^�̃I���I�t��ݒ肵�܂��B�����[�X�r���h���ɃG�f�B�^���I���ɂł���悤�ɂ��ĂȂ��ꍇ�̓I���ɂł��܂���B
		/// </summary>
		/// <param name="flag"></param>
		void SetEditFlag(const bool flag);
		
		/// <summary>
		/// �����[�X�r���h���ɃG�f�B�^���I���ɂł���悤�ɂ��邩�ǂ�����ݒ肵�܂��B
		/// </summary>
		/// <param name="flag"></param>
		void SetReleaseEditFlag(const bool flag) { releaseEdit = flag; }
	};
}
