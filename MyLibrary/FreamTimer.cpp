#include "FrameTimer.h"

using namespace MelLib;
std::vector<FrameTimer*>FrameTimer::pTimers;

FrameTimer::FrameTimer()
{
	pTimers.push_back(this);
}



FrameTimer::~FrameTimer()
{
	int count = 0;
	for(auto& p : pTimers)
	{
		if(p == this)
		{
			break;
		}
		count++;
	}
	pTimers.erase(pTimers.begin() + count);
}

void FrameTimer::AllUpdate()
{
	for (auto& p : pTimers)
	{
		p->Update();
	}
}

void FrameTimer::Update()
{

	if (!isStop) 
	{
		if (isDecrement && time > minTime) time--;
		else if (!isDecrement && time < maxTime)time++;
	}

	timeMaxMoment = false;
	timeMinMoment = false;
	if (preTime != time && time == maxTime)timeMaxMoment = true;
	if (preTime != time && time == minTime)timeMinMoment = true;
	preTime = time;

	if (isStop)return;

	if (resetFlag) 
	{
		if (time == maxTime)
		{
			time = resetMaxTime;
		}
		else if (time == minTime)
		{
			time = resetMinTime;
		}
	}
}


