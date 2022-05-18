#pragma once
#include <Sprite2D.h>

// �v���C���[��HP�Q�[�W�Ȃǂ̃Q�[�WUI���X�v���C�g�`�悷��N���X
class GaugeUI
{
private:
	// �Q�[�W�̗�
	float value;

	// �Q�[�W�̍ő�l
	float maxValue;

	// �E�B���h�E�ɑ΂���傫��
	MelLib::Vector2 sizePercent;

	// �Q�[�W�̗ʂ�`�������p
	MelLib::Sprite2D frontGauge;

	// �Q�[�W�̍ő�l��`�悷��p
	MelLib::Sprite2D backGauge;

	MelLib::Vector2 frontDrawSize;
	MelLib::Vector2 backDrawSize;

	MelLib::Vector2 drawPosition;

	MelLib::Color frontColor;
	MelLib::Color backColor;
private:
	// window�̃T�C�Y����`��T�C�Y���v�Z
	void CalcSize(const MelLib::Vector2& windowSize);
public:
	GaugeUI();
	~GaugeUI();
	void Update(float setValue);
	void Initialize();
	void Draw();

#pragma region Setter
	// �Q�[�W�̍ő�l���Z�b�g
	void SetMaxValue(float setValue);

	// �Q�[�W�̃T�C�Y���E�B���h�E�ɑ΂��Ăǂꂭ�炢�ɂ��邩
	// ���� : 0 ~ 100
	void SetSizePercent(const MelLib::Vector2& setSizePercent);

	void SetPosition(const MelLib::Vector2& pos);
	void SetFrontColor(const MelLib::Color& setColor);
	void SetBackColor(const MelLib::Color& setColor);
#pragma endregion
};
