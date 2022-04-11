#include "Random.h"
#include<random>
#include<ctime>
using namespace MelLib;
void Random::Initialize()
{
	SetSeedTime();
}

void Random::SetSeed(const UINT seed)
{
	srand(seed);
}

void Random::SetSeedTime()
{
	srand(static_cast<unsigned int>(time(NULL)));
}

int Random::GetRandomNumber(int number)
{
	return rand() % number;
}


int Random::GetRandomNumberRangeSelect(const int start, const int end)
{
	if (start >= end)return 0;

	return GetRandomNumber(abs(end - start) + 1) + start;
}

int Random::GetRandomNumberSetNumber(const std::vector<int>& nums)
{
	return nums[GetRandomNumber(nums.size())];
}


float Random::GetRandomFloatNumber(const float num, const int digits)
{
	if (digits < 0 || digits > 4)return 0.0f;

	int mag = 1;
	for (int i = 0; i < digits; i++)
	{
		mag *= 10;
	}

	int iNum = num * mag;
	return static_cast<float>(GetRandomNumber(iNum)) / static_cast<float>(mag);

}

float Random::GetRandomFloatNumberRangeSelect(const float start, const float end, const int digits)
{
	float mag = 1.0f;
	for (int i = 0; i < digits; i++)
	{
		mag *= 10.0f;
	}

	float addNum = 0;
	if(digits == 0)addNum = 1.0f;
	else addNum = 1.0f / mag;

	return GetRandomFloatNumber(abs(end - start) + addNum, digits) + start;

}


float Random::GetRandomNumberSetFloatNumber(const std::vector<float>& nums)
{
	return nums[GetRandomNumber(nums.size())];
}
