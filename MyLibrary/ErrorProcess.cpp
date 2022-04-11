#include "ErrorProcess.h"
#include<cassert>

MelLib::ErrorProcess* MelLib::ErrorProcess::GetInstance()
{
    static ErrorProcess e;
    return &e;
}



void MelLib::ErrorProcess::StartErroeProcess(const std::wstring& text, const bool assertFlag)
{
	UINT button = MB_OK;
	UINT icon = MB_ICONSTOP;
	MessageBox
	(
		errowMessageHWND,
		text.c_str(),
		L"Error!",
		button | icon
	);
	if (assertFlag)assert(0);

}
