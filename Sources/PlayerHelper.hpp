#include "CTRPluginFramework.hpp"
#ifndef PLAYERHELPER_HPP
#define PLAYERHELPER_HPP

class PlayerHelper
{
    public:
        static PlayerHelper*  GetInstance(void);
        void Update(void);
        u32 OffsetReturn();
        bool ValidOffset(void);

    private:
        PlayerHelper(void);
        ~PlayerHelper(void);

        static PlayerHelper* m_Instance;

        u8 m_PlayerSlot;
        u32 m_PlayerOffset;
        bool m_ValidOffset;
};

#endif