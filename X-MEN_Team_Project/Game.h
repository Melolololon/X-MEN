#pragma once

#include<ModelObject.h>

class Game final
{
private:
	Game();
	~Game();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	// ƒeƒXƒg
	MelLib::ModelObject testModel;

public:
	Game(const Game& g) = delete;
	Game& operator=(const Game& g) = delete;

	static Game* GetInstance();
	void Run();
};

