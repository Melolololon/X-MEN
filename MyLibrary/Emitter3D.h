#pragma once
#include"GameObject.h"
#include"Particle3D.h"


// 生成頻度とか寿命も設定できるようにする
// 寿命はパーティクルクラスでいいかも
// こっちでも寿命指定できるようにしてもいいかも


namespace MelLib
{
    class Emitter3D :public GameObject
	{
    private:
        //std::vector<std::shared_ptr<Particle3D>>particles;
        std::vector<Particle3D>particles;

        // Update、Drawを行うかどうかのフラグ
        std::vector<bool>particleUpdateDrawFlag;
        
        // 放出間隔
        FrameTimer releaseTimer;
        // 放出停止フラグ
        bool isStop = false;

        
        Vector3 position;
    private:

        void CheckChangeUpdateDrawFlag();

        /// <summary>
        /// パーティクル更新処理
        /// </summary>
        void ParticleUpdate();

    public:
       // Emitter3D(const std::vector<std::shared_ptr<Particle3D>>& pParticle, const Vector3& pos, unsigned int releaseTime);
        Emitter3D(const Particle3D& pParticle, unsigned int particleNum, unsigned int releaseTime, const Vector3& pos = 0);
        ~Emitter3D() {}

        void Update()override;
        void Draw()override;

        void SetIsStop(const bool flag) 
        {
            isStop = flag; 
            releaseTimer.SetStopFlag(flag);
        }
        
        void SetPosition(const Vector3& pos) { position = pos; }

        Vector3 GetPosition()const { return position; }
	};
}

