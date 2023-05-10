#pragma once
#include "IAudio.h"

namespace ody
{
    class ServiceLocator final
    {
    public:
        static void Provide(IAudio* audio);

        static IAudio& GetSoundSystem();
    private:
        static IAudio* m_pAudioSystemInstance;
        static NullSoundSystem m_pNullSoundSystemInstance;
    };
}