//#include "Easing.h"
//
//
//using namespace MelLib;
//
//Vector3 Easing::CalcEasing(const Vector3& startPos, const Vector3& endPos,const float t)
//{
//	return startPos * (1.0f - t) + endPos * t;
//}
//
//Vector3 Easing::EaseIn
//(
//	const Vector3& startPos,
//	const Vector3& endPos,
//	const float time
//)
//{
//	float t = (time * time);
//	return CalcEasing(startPos, endPos, time);
//}
//
//Vector3 Easing::EaseOut
//(
//	const Vector3& startPos,
//	const Vector3& endPos,
//	const float time
//)
//{
//	float t = time * (2 - time);
//	return CalcEasing(startPos, endPos, time);
//}
//
//Vector3 Easing::EaseInOut
//(
//	const Vector3& startPos,
//	const Vector3& endPos,
//	const float time
//)
//{
//	float t = t * t * (3 - 2 * t);
//	return CalcEasing(startPos, endPos, time);
//}
//
//
