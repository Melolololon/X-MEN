#pragma once
#include"ModelObject.h"

//頂点求めるのもったいないから、
//ここでこそアウトプットストリーム使って頂点をもらうべき?
//頂点さえあれば問題ないし

//判定はどうする?メッシュコライダー?


namespace MelLib 
{

	//布シミュレータを行うクラス
	class ClothSimulator
	{
	public:
		//テッセレータでの分割数を元に、当たり判定をセットする関数
		static void SetClothCollisionData(ModelObject& modelObj);


	};
}

