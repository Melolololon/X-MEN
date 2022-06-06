#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
	for (auto& ui : uies)
	{
		ui = nullptr;
	}

	for (auto& ui : buttonUies)
	{
		ui = nullptr;
	}
}

void UIManager::Initialize()
{
	for (auto& ui : uies)
	{
		if (!ui)continue;

		ui.get()->Initialize();
	}

	for (auto& ui : buttonUies)
	{
		if (!ui)continue;

		ui.get()->Initialize();
	}
}

void UIManager::Update()
{
	for (auto& ui : uies)
	{
		if (!ui)continue;

		ui.get()->Update();
	}

	for (auto& ui : buttonUies)
	{
		if (!ui)continue;

		ui.get()->Update();
	}
}

void UIManager::Draw()
{
	for (auto& ui : uies)
	{
		if (!ui)continue;

		ui.get()->Draw();
	}

	for (auto& ui : buttonUies)
	{
		if (!ui)continue;

		ui.get()->Draw();
	}
}

void UIManager::Finalize()
{
	for (auto& ui : uies)
	{
		ui = nullptr;
	}

	for (auto& ui : buttonUies)
	{
		ui = nullptr;
	}
}

void UIManager::AddUI(std::shared_ptr<UI> newUI)
{
	uies.push_back(newUI);
}

void UIManager::AddButtonUI(std::shared_ptr<IButtonUI> newUI)
{
	buttonUies.push_back(newUI);
}
