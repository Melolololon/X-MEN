#pragma once
#include "UI.h"
#include "IButtonUI.h"
#include "IGaugeUI.h"
#include <vector>

class UIManager
{
private:
	// �Q�[�W�Ȃǂ̂����`�悳��Ă��邾����UI�̊��N���X�z��
	std::vector<std::shared_ptr<UI>> uies;
	// �|�[�Y�{�^���Ȃǂ̃N���b�N�����̃{�^�����������ۂɓ��삷��n��UI�̊��N���X�z��
	std::vector<std::shared_ptr<UI>> buttonUies;
public:
	UIManager();
	~UIManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	// �e�N�X�`����UI�̃^�C�v����V����UI��ǉ�
	void AddUI(std::shared_ptr<UI> newUI);

	template <typename T>
	void AddGaugeUI(std::shared_ptr<IGaugeUI<T>> newUI);

	void AddButtonUI(std::shared_ptr<IButtonUI> newUI);
};

template<typename T>
inline void UIManager::AddGaugeUI(std::shared_ptr<IGaugeUI<T>> newUI)
{
	uies.push_back(newUI);
}
