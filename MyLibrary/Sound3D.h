#pragma once
#include "Sound.h"
#include"Vector.h"

namespace MelLib 
{
	class Sound3D :
		public Sound
	{
	private:

	public:
		
		~Sound3D(){}


#pragma region ƒZƒbƒg

		void SetPosition(Vector3 pos);

#pragma endregion

	};
}

