#include "Counter.h"

using namespace MelLib;
std::vector<Counter*>Counter::pCounter;

void Counter::Update()
{
	if(countReset)
	{
		if (count >= maxCount)count = resetMaxNum;
		else if (count <= minCount)count = resetMinNum;
	}

	preCount = count;
}

void Counter::AllUpdate()
{
	for(auto& p : pCounter)
	{
		p->Update();
	}
}

Counter::Counter()
{
	pCounter.push_back(this);
}

Counter::Counter(const int num) :count(num), preCount(count - 1)
{
	pCounter.push_back(this);
}

Counter::~Counter()
{
	int count = 0;
	for(const auto& p : pCounter)
	{
		if (p == this)break;
		count++;
	}
	pCounter.erase(pCounter.begin() + count);
}
