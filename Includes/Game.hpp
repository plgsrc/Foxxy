#ifndef GAME_HPP
#define GAME_HPP

#include "CTRPluginFramework.hpp"
#include "strings.hpp"

namespace CTRPluginFramework
{
	class Player
	{
	public:
		static u32		GetInstance(u8 PlayerIndex, bool noChecks);
		static u32		TryGetInstance(u8 PlayerIndex);
		static bool		Exists(u8 PlayerIndex);
		static bool		GetWorldCoords(u32 *worldx, u32 *worldy, u8 PlayerIndex, bool noChecks);
		static u32		GetAnimationInstance(u32 playerInstance, u8 u0, u8 u1, u32 u2);
		static bool		ExecuteAnimation(u32 pPlayerObj, u8 animID, u32 pAnimObj, bool u0);
		static bool		ExecuteAnimationWrapper(u8 playerIndex, u8 animID, u32 animItem, u8 emotion, u16 snake, u16 sound, bool u0, u8 wX, u8 wY, bool directSend = false, u8 appearance[] = nullptr);
		static bool		Pickup(u32 PlayerObj, u8 worldx, u8 worldy, bool u0, bool u1, bool u2);
		static float	*GetCoordinates(u8 PlayerIndex = 4);
		static bool		GetCoordinatesRef(u8 PlayerIndex, float &X, float &Z);
		static float	*GetCollisionSize(u8 PlayerIndex = 4);
		static u8		*GetAnimation(u8 PlayerIndex = 4);
		static u8		GetLocation();
		static u32		*GetHeldItem();
		static float	*GetVelocity(u8 PlayerIndex = 4);
		static u8		GetRoom(u8 PlayerIndex);
		static bool		SendAnimPacket(u8 senderIndex, u32 animObj, u8 animID, u8 roomID, u8 targetPlayerIndex);
	};

	class GameHelper
	{
	public:
		static void		WriteItem(u32 ID);
		static u32		PlaceItem(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool isInside, bool u1, bool u2, bool u3, bool u4);
		static u8		GetOnlinePlayerIndex();
		static u8		GetActualPlayerIndex();
		static u8		GetOnlinePlayerCount();
		static bool		Warp0();
		static bool		ArriveAtIsland(u32 u0 = 0x31FE4ECC);
		static bool		ArriveAtTourdesk(u32 u0 = 0x31FE4ECC);
		static bool		ArriveAtTown(u32 u0 = 0x31FE4ECC);
		static bool		LeaveIsland(u32 u0 = 0x31FE4ECC);
		static u32		*GetItemAtWorldCoords(u32 MapPtr, u32 x, u32 y, bool u0);
		static u32		GetCurrentMap();
		static void		PlaceBuildingUpdateCollisions(u32 x, u32 y, u16 buildingID);
		static u32		APressAction(u32 x, u32 y, u32 ID);
		static void		DropIDSwitch(u32 itemToRemove, u32 itemToShow, u32 itemToPlace, u8 u0, u8 playerIndex, u8 wX, u8 wY, u8 dropID, u8 u1, u8 u2, u8 u3, u8 u4, u8 roomID);
		static void		playSound(u32 playerIndex, u32 ID);
		static bool		dropCheck(u32 *wX, u32 *wY, u32 *u0, bool disallowConcrete, bool u1);
		static bool		PlaceItemWrapper(u8 ID, u32 ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4, u8 waitAnim, u8 roomID);
		static bool		RemoveItems(bool trample, u8 wX, u8 wY, u8 width, u8 length, bool allowAbort, bool removeEverything);
		static void		Particles(u32 particleID, float *floats, u32 u0 = 0x976C0E, u32 u1 = 0xAE6870);
		static void		clearLockedSpot(u8 wX, u8 wY, u8 roomID, u32 param_4 = 4);
		static u32		createLockedSpot(u8 DropID, u8 wX, u8 wY, u8 roomID, bool sendPkt = 1);
		static u32		GetLockedSpotIndex(u8 wX, u8 wY, u8 roomID = 0xA5);
		static bool		TrampleAt(u8 wX, u8 wY);
		static float	*WorldCoordsToCoords(u8 wX, u8 wY, float res[3]);
	};
	
	class NPC
	{
	public:
		static bool		GetCoordinatesRef(u32 npc, float &X, float &Z);
	};

	class IDList
	{
	public:
		static std::string GetItemName(u32 ID);
		static std::string GetBuildingName(u8 ID);
		static std::string GetSnakeName(u16 ID);
		static std::string GetRoomName(u8 ID);
	};

	class Item
	{
	public:
		static u8		GetBehaviour(u32 *pItem);
		static bool		hasTreeBehaviour(u32 *pItem);
	};

	class Camera
	{
	public:
		static u32		GetInstance();
		static void		AddToX(float val);
		static void		AddToY(float val);
		static void		AddToZ(float val);
		static void		AddToYRotation(u16 val);
	};

	class ItemSequence
	{
	public:
		static void		init(u32 defaultPtr);
		static u32		*Next();
		static u32		PeekNext();
		static bool		openKB();
		static void		Switch(bool enable);
		static bool		Enabled();
	};

	struct dropInfo
	{
		u32 itemToRemove;
		u32 itemToShow;
		u32 itemToPlace;
		u8 unknown0;
		u8 playerindex;
		u8 wX;
		u8 wY;
		u8 dropID;
		u8 unknown1;
		u8 unknown2;
		u8 unknown3;
		u8 unknown4;
		u8 roomID;
	};

	struct dropPkt
	{
		u32 replaceID;
		u32 placeID;
		u32 showID;
		u8 combinedDropIDPlayerIndex;
		u8 processFlags;
		u8 paramFlags;
		u8 flags;
		u8 u0;
		u8 u1;
		u8 roomID;
		u8 wX;
		u8 wY;
		u8 u2;
	};

	struct TramplePkt
	{
		u32 item;
		u8 roomID;
		u8 wX;
		u8 wY;
		u8 u0;
	};
}
#endif