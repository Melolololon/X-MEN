#pragma once
class Game final
{
private:
	Game();
	~Game();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

public:
	Game(const Game& g) = delete;
	Game& operator=(const Game& g) = delete;

	static Game* GetInstance();
	void Run();
};

