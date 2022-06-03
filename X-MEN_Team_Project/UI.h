#pragma once

class UI
{
private:
public:
	UI() {}
	virtual ~UI() {}
	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Draw() {}
};
