#include<Windows.h>
#include"Game.h"
//#pragma commentで読み込めば、スタティックライブラリでも.lib読み込める?
//libfbxsdk-md.pdb' が見つかりませんでした。LibraryTest.exe は、デバッグにオブジェクト ファイルを必要とする /DEBUG:FASTLINK を使ってビルドされました。
//は、「ビルド」の、「ソリューションの完全なプログラムデータベースファイルを構築する」を実行すると直る

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Game::GetInstance()->Run();
	return 0;
}