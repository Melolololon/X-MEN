#include "MouseCursor.h"
#include"Input.h"


using namespace MelLib;
MouseCursor::MouseCursor()
{
	

}

MouseCursor::~MouseCursor(){}

//void MouseCursor::Initialize()
//{
//	nearPos = 0;
//	farPos = 0;
//	position2D = 0;
//}

void MouseCursor::Update()
{
	position2D = Input::GetMousePosition();
	Input::GetMouse3DLine(nearPos, farPos);

}
