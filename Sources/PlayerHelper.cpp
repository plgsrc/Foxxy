#include "PlayerHelper.hpp"

using namespace CTRPluginFramework;

/*
    PlayerHelper Class
    This is a Singleton class, meaning there can only ever be one instance ('''allocation''') of this class.

    PlayerHelper::GetInstance() is used to get the instance of the class, but also create one if there isn't one yet.
*/

PlayerHelper* PlayerHelper::m_Instance = nullptr;

PlayerHelper::PlayerHelper()
{
    this->Update(); //Set the variables of the class when the instance is created
}

//This should never be called as the instance will always be used, but if it is, set the global instance pointer to nullptr
PlayerHelper::~PlayerHelper()
{
    m_Instance = nullptr;
}

//Used to get the instance of the class, but also create one if there isn't one yet.
PlayerHelper* PlayerHelper::GetInstance(void)
{
    if (m_Instance == nullptr)
        m_Instance = new PlayerHelper;

    return m_Instance;
}

/*
    void PlayerHelper::Update(void)
    => This function updates the variables of the class.
    => It should be added as a PluginMenu callback so it's called in the background and values stay up to date.
*/

void PlayerHelper::Update(void)
{
    Process::Read8(0x31FF5994, m_PlayerSlot);
    if (m_PlayerSlot == 4)
        m_PlayerSlot = 0;
   
    Process::Read32(0xAAE994 + m_PlayerSlot * 4, m_PlayerOffset);

    m_ValidOffset = (m_PlayerOffset == 0x330773BC) || (m_PlayerOffset == 0x330774E8);
}

u32 PlayerHelper::OffsetReturn()
{
    return m_PlayerOffset;
}

/*
    bool PlayerHelper::ValidOffset(void)
    => This function returns a bool for if the player offset is 0x330773BC or 0x330774E8.
    => ret value, m_ValidOffset, is set automatically in the PlayerHelper::Update() function.
*/

bool PlayerHelper::ValidOffset(void) 
{
    return m_ValidOffset;
}