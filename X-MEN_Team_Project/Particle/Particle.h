#pragma once
#include<ModelObject.h>

class Particle
{
private:
	std::shared_ptr<MelLib::ModelObject> model;
public:
	Particle();

	// �X�V
	void Update();

	//�`��
	void Draw();

	//Setter
	void SetPosition(const MelLib::Vector3 p) { model.get()->SetPosition(p); }
	void SetScale(const MelLib::Vector3 s) { model.get()->SetScale(s); }
	void SetColor(const MelLib::Color c) { model.get()->SetMulColor(c); }
	void SetAngle(const MelLib::Vector3 a) { model.get()->SetAngle(a); }
	void SetLiveFlag(const bool b) { liveFlag = b; }
	//Getter
	MelLib::Vector3 GetPosition() { return model.get()->GetPosition(); }
	MelLib::Vector3 GetScale() { return model.get()->GetScale(); }
	MelLib::Vector3 GetAngle() { return model.get()->GetAngle(); }
	bool GetLiveFlag() { return liveFlag; }
	int GetTIme() { return time; }
private:
	//�����Ă��邩
	bool liveFlag;
	//�W�J����Ă���̌o�ߎ���
	int time;

};