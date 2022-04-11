#pragma once
#include "GameObject2D.h"
#include "Particle2D.h"

// パーティクルクラスを作り、パーティクル作成時はそれを継承すること
// エミッター生成時に種類とパーティクル数を指定するようにする?
// 2Dと3Dで分ける?

namespace MelLib 
{
    class Emitter2D :public MelLib::GameObject2D
    {
    private:
        std::vector<Particle2D>particles;

    public:
        Emitter2D(const Particle2D* const pParticle, unsigned int particleNum);
        ~Emitter2D(){}

        void Update()override;
        void Draw()override;
        
    };

    //class Particle :
    //    public MelLib::GameObject
    //{

    //};

}