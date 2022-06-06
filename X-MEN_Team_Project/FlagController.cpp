#include "FlagController.h"

FlagController::FlagController()
	: isFlag(false)
	, time(0)
	, maxTime(1)
{
}

FlagController::~FlagController()
{
}

void FlagController::Initialize()
{
	isFlag = false;
	time = 0;
}

void FlagController::Update(const float DELTA_TIME)
{
	if (!isFlag)return;
	if (time >= 1)
	{
		isFlag = false;
		time = 0;
	}


	time += DELTA_TIME / maxTime;
}

void FlagController::FlagOn(bool isTimeReset)
{
	isFlag = true;

	if (isTimeReset)time = 0;
}

bool FlagController::IsFlag() const
{
	return isFlag;
}

float FlagController::GetTime() const
{
	return time;
}

void FlagController::SetMaxTime(const float VALUE)
{
	maxTime = VALUE;
}
