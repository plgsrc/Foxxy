#include "Game.hpp"

namespace CTRPluginFramework
{
	std::vector<u32> items;
	std::vector<std::string> keyBoardItems;
	u8 index;
	bool useDropChain = false;
	Keyboard *optDCKB;
	Keyboard *DCKB;
	u32 *defPtr;
	u32 defItem;

	volatile u32(*pfunction0)();
	volatile u32(*pfunction1)(u32 param1);
	volatile u32(*pfunction2)(u32 param1, u32 param2);
	volatile u32(*pfunction3)(u32 param1, u32 param2, u32 param3);
	volatile u32(*pfunction4)(u32 param1, u32 param2, u32 param3, u32 param4);
	volatile u32(*pfunction5)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5);
	volatile u32(*pfunction6)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6);
	volatile u32(*pfunction7)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7);
	volatile u32(*pfunction8)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8);
	volatile u32(*pfunction9)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9);
	volatile u32(*pfunction10)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9, u32 param10);
	volatile u32(*pfunction11)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9, u32 param10, u32 param11);

	u32 Player::GetInstance(u8 PlayerIndex, bool noChecks)
	{
		Process::Write32((u32)&pfunction2, 0x5C3EA0);
		return pfunction2(PlayerIndex, noChecks);
	}
	u32 Player::TryGetInstance(u8 PlayerIndex)
	{
		return 0;
	}
	bool Player::Exists(u8 PlayerIndex)
	{
		if (GameHelper::GetOnlinePlayerCount() == 0 && PlayerIndex == GameHelper::GetOnlinePlayerIndex()) return true;
		if (*(u8 *)(0x31FE28ED + PlayerIndex) != 0xA5) return true;
		return false;
	}
	bool Player::GetWorldCoords(u32 *worldx, u32 *worldy, u8 PlayerIndex, bool noChecks)
	{
		Process::Write32((u32)&pfunction4, 0x5C13AC);
		return pfunction4((u32)worldx, (u32)worldy, PlayerIndex, noChecks);
	}
	u32 Player::GetAnimationInstance(u32 playerInstance, u8 someVal1, u8 someVal2, u32 encVal)
	{
		Process::Write32((u32)&pfunction4, 0x6576F8);
		return pfunction4(playerInstance, someVal1, someVal2, encVal);
	}
	bool Player::ExecuteAnimation(u32 pPlayerObj, u8 animID, u32 pAnimObj, bool u0)
	{
		Process::Write32((u32)&pfunction4, 0x64DB90);
		return pfunction4(pPlayerObj, animID, pAnimObj, u0);
	}
	bool Player::ExecuteAnimationWrapper(u8 pIndex, u8 animID, u32 animItem, u8 emotion, u16 snake, u16 sound, bool u0, u8 wX, u8 wY, bool directSend, u8 appearance[])
	{
		u8 actualIndex = *(u8 *)((*(u32 *)0x954648) + 0x13268);
		bool forced = (pIndex != actualIndex) && (actualIndex <= 3);
		bool needOverwrite = (GameHelper::GetOnlinePlayerIndex() != pIndex);
		u32 player = Player::GetInstance(pIndex, 1);
		u32 animInstance;
		float coords[3];
		
		if (player == 0) return 0;
		animInstance = Player::GetAnimationInstance(player, 0, 0, 0);
		if (animInstance == 0) return 0;
		Process::Write32((u32)&pfunction2, 0x5D4C88);
		pfunction2(animInstance + 0x2, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords)); 

		if (animID == 0xB7 || animID == 0x71 || animID == 0x72 || animID == 0x73 || animID == 0x24 || animID == 0x3E || animID == 0x3F || animID == 0x3D || animID == 0x38 || animID == 0x92 || animID == 0x93)
		{
			Sleep(Milliseconds(75));
			Process::Write16(animInstance + 0xE, animItem & 0xFFFF);
			Process::Write16(animInstance + 0x10, (animItem >> 0x10) & 0xFFFF);
			if (animID == 0x3E)
			{
				Process::Write16(animInstance + 0x12, snake);
				Process::Write32(animInstance + 0x14, 0x00020101);
			}
			if (animID == 0x38)
			{
				Process::Write8(animInstance + 0x12, 0x01);
			}
		}
		else if (animID == 0xAF || animID == 0x88)
		{
			Process::Write8(animInstance + 0xE, emotion & 0xFF);
			Process::Write8(animInstance + 0xF, 1); // sets animation to 0x6 after executing
			Process::Write8(animInstance + 0x15, 4); // continuous emote
		}
		else if (animID == 0xC4)
		{
			Process::Write16(animInstance + 0xE, snake & 0xFFF);
			Process::Write16(animInstance + 0x10, sound & 0xFFFF);
			Process::Write8(animInstance + 0x13, 1);
			Process::Write16(animInstance + 0x15, 0xFFFE);
			Process::Write8(animInstance + 0x17, 4);
			Process::Write8(animInstance + 0x18, *(u8 *)(player + 0x2F));
		}
		else if (animID == 0xC5)
		{
			Process::Write16(animInstance + 0xE, snake & 0xFFF);
			Process::Write16(animInstance + 0x10, snake & 0xFFFF);
			Process::Write8(animInstance + 0x12, *(u8 *)(player + 0x2F));
			Process::Write8(animInstance + 0x13, 1);
			Process::Write8(animInstance + 0x17, 5);
		}
		else if (animID == 0x45)
		{
			Process::Write32(0x65F7CC, 0xE1A00000);
			Process::Write16(animInstance + 0xE, sound & 0xFFFF);
			Process::Write8(animInstance + 0x11, 1);
			Process::Write32((u32)&pfunction2, 0x5D4C88);
			pfunction2(animInstance + 0x12, (u32)Player::GetCoordinates(pIndex));
		}
		else if (animID == 0x3C)
		{
			Process::Write32(animInstance + 0xE, animItem);
			Process::Write8(animInstance + 0x12, wX);
			Process::Write8(animInstance + 0x13, wY);
			Process::Write8(animInstance + 0x15, emotion);
		}
		else if (animID == 0x4A)
		{
			Process::Write8(animInstance + 0xE, wX);
			Process::Write8(animInstance + 0xF, wY);
			Process::Write16(animInstance + 0x10, emotion);
		}
		else if (animID == 0x36)
		{
			Process::Write32(animInstance + 0xE, animItem);
			Process::Write8(animInstance + 0x12, emotion);
			Process::Write32(animInstance + 0x1C, 0x01480000);
		}
		else if (animID == 0x33)
		{
			Process::Write32((u32)&pfunction2, 0x5D4C88);
			pfunction2(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
			pfunction2(animInstance + 0x10, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
		}
		else if (animID >= 0x7B && animID <= 0x7F)
		{
			Process::Write32((u32)&pfunction2, 0x5D4C88);
			pfunction2(animInstance + 0xF, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
			Process::Write8(animInstance + 0x13, *(u8 *)(player + 0x2F));
		}
		else if (animID == 0x1C || animID == 0x25 || animID == 0x1D || animID == 0xC1 || animID == 0xE2 || animID == 0xE5 || (animID >= 0xA8 && animID <= 0xAE) || (animID >= 0x80 && animID <= 0x87) || (animID >= 0x2B && animID <= 0x33))
		{
			Process::Write32((u32)&pfunction2, 0x5D4C88);
			pfunction2(animInstance + 0x2, (u32)Player::GetCoordinates(pIndex));
			Process::Write32((u32)&pfunction2, 0x5D4C88);
			pfunction2(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
			Process::Write8(animInstance + 0x12, *(u8 *)(player + 0x2F));
			Process::Write8(animInstance + 0x13, emotion);
		}
		else if (animID == 0xB5 || animID == 0xB3 || animID == 0xB4)
		{
			Process::Write32((u32)&pfunction2, 0x5D4C88);
			pfunction2(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
			Process::Write32(animInstance + 0x12, animItem);
		}
		else if (animID == 0xB9)
		{
			Process::Write8(animInstance + 0xC, 0x10);
			Process::Write8(animInstance + 0xD, 0);
			Process::Write8(animInstance + 0xE, appearance[0]);
			Process::Write8(animInstance + 0xF, appearance[1]);
			Process::Write8(animInstance + 0x10, appearance[2]);
			Process::Write16(animInstance + 0x11, animItem & 0xFFFF);
		}
		else
		{
			Process::Write8(animInstance + 0xE, wX);
			Process::Write8(animInstance + 0xF, wY);
			Process::Write16(animInstance + 0x10, animItem);
			//Process::Write8(animInstance + 0x13, 2);
		}

		if (!directSend)
		{
			if (forced && needOverwrite)
			{
				Process::Write32(0x5C3EB0, 0xE3A00000 + pIndex);
				Process::Write32(0x1ABADC, 0xE3A00000 + pIndex);
			}
			if (forced)
			{
				Process::Write32(0x2FEB64, 0xE1A00000);
				Process::Write32(0x677530, 0xE1A01006);
				Process::Write32(0x677534, 0xEBFD31D0);
				Process::Write32(0x677454, 0xE1A00000);
				Process::Write32(0x6774F0, 0xE1A00000);
				Process::Write32(0x677504, 0xE1A00000);
				Process::Write32(0x628B54, 0xE3A01000 + pIndex); // in sendPkt Function GetOnlinePlayerIndex inline
				Sleep(Milliseconds(5));
			}
			Player::ExecuteAnimation(player, animID, animInstance, u0);
			if (forced)
			{
				Sleep(Milliseconds(5));
				Process::Write32(0x628B54, 0xE5D11268);
				Process::Write32(0x677530, 0x01A01006);
				Process::Write32(0x677534, 0x0BFD31D0);
				Process::Write32(0x677504, 0x1A000017);
				Process::Write32(0x6774F0, 0x0A00001C);
				Process::Write32(0x677454, 0x0A000038);
				Process::Write32(0x5C3EB0, 0x2BF5080E);
				Process::Write32(0x2FEB64, 0x1A000002);
				Process::Write32(0x1ABADC, 0xE3A00004);
			}
		}
		else
		{
			if (forced) Player::SendAnimPacket(actualIndex, animInstance, animID, Player::GetRoom(pIndex), pIndex);
			else Player::ExecuteAnimation(player, animID, animInstance, u0);
		}
		Sleep(Milliseconds(25));
		return 1;
	}
	bool Player::Pickup(u32 PlayerObj, u8 worldx, u8 worldy, bool u0, bool u1, bool u2)
	{
		Process::Write32((u32)&pfunction6, 0x661524);
		return pfunction6(PlayerObj, worldx, worldy, u0, u1, u2);
	}
	float *Player::GetCoordinates(u8 PlayerIndex)
	{
		u32 i = GetInstance(PlayerIndex, 1);
		if (i != 0) i += 0x14;
		return (float *)i;
	}
	bool Player::GetCoordinatesRef(u8 PlayerIndex, float &X, float &Z)
	{
		u32 i = GetInstance(PlayerIndex, 1);
		if (i != 0) 
		{
			i += 0x14;
			X = *(volatile float *)i;
			Z = *(volatile float *)(i + 8);
			return true;
		}
		else return false;
	}
	float *Player::GetCollisionSize(u8 PlayerIndex)
	{
		u32 i = GetInstance(PlayerIndex, 1);
		if (i != 0) i += 0x4F4;
		return (float *)i;
	}
	u8 *Player::GetAnimation(u8 PlayerIndex)
	{
		u8 *ptr = (u8 *)(GetInstance(PlayerIndex, 1) + 0x1A9);
		if ((u32)ptr != 0x1A9) return ptr;
		else return 0;
	}
	u8 Player::GetLocation()
	{
		return *(u8 *)0x330774FC;
	}
	u32 *Player::GetHeldItem()
	{

	}
	float *Player::GetVelocity(u8 PlayerIndex)
	{
		return (float *)(GetInstance(PlayerIndex, 1) + 0x40);
	}
	u8 Player::GetRoom(u8 PlayerIndex)
	{
		u32 animObjPtr;
		Process::Write32((u32)&pfunction1, 0x5C3DDC);
		animObjPtr = pfunction1(PlayerIndex);
		return animObjPtr == 0 ? 0xFF : *(u8 *)animObjPtr;
	}
	bool Player::SendAnimPacket(u8 senderIndex, u32 animObj, u8 animID, u8 roomID, u8 targetPlayerIndex)
	{
		Process::Write8(animObj, roomID);
		Process::Write8(animObj + 1, animID);
		Process::Write32(0x628B54, 0xE3A01000 + targetPlayerIndex); // in sendPkt Function GetOnlinePlayerIndex inline
		Process::Write8(0x5C3CAC, targetPlayerIndex);
		Sleep(Milliseconds(5));
		Process::Write32((u32)&pfunction2, 0x5C3C7C);
		pfunction2(targetPlayerIndex, animObj);
		Sleep(Milliseconds(5));
		Process::Write32(0x628B54, 0xE5D11268);
		Process::Write8(0x5C3CAC, 4);
	}





	void GameHelper::WriteItem(u32 ID)
	{

	}
	u32 GameHelper::PlaceItem(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4)
	{
		Process::Write32((u32)&pfunction11, 0x59FC7C);
		return pfunction11(ID, (u32)ItemToReplace, (u32)ItemToPlace, (u32)ItemToShow, worldx, worldy, u0, u1, u2, u3, u4);
	}
	u8 GameHelper::GetOnlinePlayerIndex()
	{
		Process::Write32((u32)&pfunction0, 0x305EF0);
		return pfunction0();
	}
	u8 GameHelper::GetActualPlayerIndex()
	{
		u8 index = *(u8 *)((*(u32 *)0x954648) + 0x13268);
		if (index >= 4) return 0;
		else return index;
	}
	u8 GameHelper::GetOnlinePlayerCount()
	{
		Process::Write32((u32)&pfunction1, 0x75EFF8);
		return pfunction1(*(u32 *)0x954648);
	}
	bool GameHelper::Warp0()
	{
		Process::Write32((u32)&pfunction0, 0x61E708);
		return pfunction0();
	}
	bool GameHelper::ArriveAtIsland(u32 u0)
	{
		Process::Write32((u32)&pfunction1, 0x61E8BC);
		return pfunction1(u0);
	}
	bool GameHelper::ArriveAtTourdesk(u32 u0)
	{
		Process::Write32((u32)&pfunction1, 0x61F81C);
		return pfunction1(u0);
	}
	bool GameHelper::ArriveAtTown(u32 u0)
	{
		Process::Write32((u32)&pfunction1, 0x61F258);
		return pfunction1(u0);
	}
	bool GameHelper::LeaveIsland(u32 u0)
	{
		Process::Write32((u32)&pfunction1, 0x61E954);
		return pfunction1(u0);
	}
	u32 *GameHelper::GetItemAtWorldCoords(u32 MapPtr, u32 x, u32 y, bool u0)
	{
		Process::Write32((u32)&pfunction4, 0x2FEF9C);
		return (u32 *)pfunction4(MapPtr, x, y, u0);
	}
	u32 GameHelper::GetCurrentMap()
	{
		Process::Write32((u32)&pfunction0, 0x6A690C);
		return pfunction0();
	}
	void GameHelper::PlaceBuildingUpdateCollisions(u32 x, u32 y, u16 buildingID)
	{
		Process::Write32((u32)&pfunction3, 0x2425D8);
		pfunction3(x, y, buildingID);
	}
	u32 GameHelper::APressAction(u32 x, u32 y, u32 ID)
	{
		Process::Write32((u32)&pfunction3, 0x597154);
		return pfunction3(x, y, ID);
	}
	void GameHelper::DropIDSwitch(u32 itemToRemove, u32 itemToShow, u32 itemToPlace, u8 u0, u8 playerIndex, u8 wX, u8 wY, u8 dropID, u8 u1, u8 u2, u8 u3, u8 u4, u8 roomID)
	{
		dropInfo obj{ itemToRemove, itemToShow, itemToPlace, u0, playerIndex, wX, wY, dropID, u1, u2, u3, u4, roomID };
		Process::Write32((u32)&pfunction1, 0x5A22F4);
		pfunction1((u32)&obj);
	}
	void GameHelper::playSound(u32 playerIndex, u32 ID)
	{
		Process::Write32((u32)&pfunction1, 0x5C119C);
		Process::Write32((u32)&pfunction3, 0x2505E0);
		u32 ptr = pfunction1(playerIndex); // get playerObj + 0x5EC
		if (ptr != 0) pfunction3(ptr, ID, 1);
	}
	bool GameHelper::dropCheck(u32 *wX, u32 *wY, u32 *u0, bool disallowConcrete, bool u1)
	{
		Process::Write32((u32)&pfunction5, 0x5990AC);
		return (bool)pfunction5((u32)wX, (u32)wY, (u32)u0, disallowConcrete, u1);
	}
	void pseudoType0x21(dropPkt *pkt)
	{
		dropPkt *drop = new dropPkt();
		u32 x, y;
		s8 diffX, diffY;
		u32 *item;

		drop->replaceID = 0x71;
		drop->placeID = pkt->placeID;
		drop->showID = 0x71;
		drop->paramFlags = 0;
		drop->processFlags = 0;
		drop->u0 = 0;
		drop->u1 = 0;
		drop->u2 = 0;
		drop->roomID = pkt->roomID;
		drop->wX = pkt->wX;
		drop->wY = pkt->wY;
		drop->combinedDropIDPlayerIndex = GameHelper::GetOnlinePlayerIndex() & 3 | 0x84;
		drop->processFlags = drop->processFlags & 0xC3 | (GameHelper::GetOnlinePlayerCount() > 1 ? 0x1C : 0xC);
		drop->flags = 0;
		Player::GetWorldCoords(&x, &y, 4, 1);
		pkt->wX = x;
		pkt->wY = y;
		pkt->replaceID = 0x71;
		item = GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0);
		if (item == nullptr) return;
		pkt->placeID = *item;
		pkt->showID = drop->placeID == 0x7FFE ? 0x2001 : drop->placeID;
		pkt->paramFlags = 0x8;
		diffX = drop->wX - x;
		diffY = drop->wY - y;
		pkt->flags = (((diffX + 4) & 0xF) | (((diffY + 4) & 0xF) << 4));
		pkt->u0 = 0;
		pkt->u1 = 0;
		pkt->u2 = 0;
		Process::Write32((u32)&pfunction1, 0x59FFC0);
		pfunction1((u32)drop);
		return;
	}
	bool GameHelper::PlaceItemWrapper(u8 ID, u32 ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4, u8 waitAnim, u8 roomID)
	{
		static Hook h;
		float coords[3];
		u32 crashPreventItem = Player::GetLocation() != 0xFF ? 0x2001 : 0x80007FFE;
		u8 autoWaitAnim = waitAnim;
		u32 player = Player::GetInstance(4, 1);
		u32 *pItemAtCoords = GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), worldx, worldy, 0);
		u32 *actualItemToPlace = useDropChain ? ItemSequence::Next() : ItemToPlace;
		u32 *actualItemToShow = useDropChain ? actualItemToPlace : ItemToShow;
		u32 animInstance;
		u8 currentIndex = GameHelper::GetOnlinePlayerIndex();
		bool forced = (currentIndex != GameHelper::GetActualPlayerIndex()) && (GameHelper::GetActualPlayerIndex() <= 3);

		if (GameHelper::GetLockedSpotIndex(worldx, worldy, roomID) != 0xFFFFFFFF) return 0;
		if (pItemAtCoords == nullptr) return 0;
		u32 *actualItemToReplace = ItemToReplace == 0xFFFFFFFF ? pItemAtCoords : &ItemToReplace;
		if (*actualItemToReplace != *pItemAtCoords && !(*actualItemToReplace == 0x7FFE && *pItemAtCoords == 0x80007FFE)) return 0;
		if (Player::GetLocation() == 0xFF && (*actualItemToPlace == 0x207A || *actualItemToPlace == 0x2079)) return 0;
		if (Player::GetLocation() != 0xFF && (*actualItemToPlace <= 0xFF || (*actualItemToPlace & 0x80000000) != 0 || ID == 8 || ID == 9 || ID == 0xE || ID == 0xF)) return 0;
		if (ID == 9 || ID == 0xF) *actualItemToReplace = 0x26;
		if (ID == 8 || ID == 0xE) *actualItemToReplace = 0x3E;
		if (ID == 3 || ID == 4 || ID == 5) *actualItemToReplace = 0x2001;
		

		if (roomID != 0xA5)
		{
			*(vu32 *)0x5A0358 = 0xE3A00000 + roomID;
			for (u8 i = 0; i < 4; i++)
			{
				if (Player::Exists(i) && Player::GetRoom(i) == roomID)
				{
					Process::Write32(0x5A02C0, 0xE3A00000 + i);
				}
			}
			Sleep(Milliseconds(5));
		}
		if (ID == 0x21)
		{
			if (player == 0) return 0;
			if (!h.flags.isEnabled)
			{
				Process::Write32(0x5A04A0, 0xE1A00000);
				Sleep(Milliseconds(5));
				h.Initialize(0x5A04A0, (u32)&pseudoType0x21);
				h.Enable();
			}
			Sleep(Milliseconds(5));
		}
		else
		{
			h.Disable();
			//Sleep(Milliseconds(5));
			Process::Write32(0x5A04A0, 0xEB000019);
		}
		GameHelper::PlaceItem(ID == 0x21 ? 0xF : ID, *actualItemToReplace == 0x7FFE ? &crashPreventItem : actualItemToReplace, *actualItemToPlace == 0x7FFE && ID != 0x1C && !(ID >= 0x6 && ID <= 0x9) && ID != 0xE && ID != 0xF ? &crashPreventItem : actualItemToPlace, *actualItemToShow == 0x7FFE && ID != 0x1C && !(ID >= 0x6 && ID <= 0x9) && ID != 0xE && ID != 0xF ? &crashPreventItem : actualItemToShow, worldx, worldy, u0, u1, ID == 0x8 || ID == 0xE ? 1 : 0, u3, u4);
	
		if (roomID != 0xA5)
		{
			Sleep(Milliseconds(5));
			Process::Write32(0x5A0358, 0xEBF55C09);
			Process::Write32(0x5A02C0, 0xEBF5970A);
		}

		if (!(ID >= 0xa && ID <= 0xd) && ID != 0x22)
		{
			bool noWait = false;
			if (player == 0) return 0;

			animInstance = Player::GetAnimationInstance(player, 0, 0, 0);
			*(u32 *)(player + 0x844) = 0;
			*(u8 *)(player + 0x8CC) = ID;

			if (waitAnim == 0x5D || waitAnim == 0x6B || waitAnim == 0x4F || waitAnim == 0x4C || waitAnim == 0x50 || waitAnim == 0x52 || waitAnim == 0x5A || waitAnim == 0x5F || waitAnim == 0x60 || waitAnim == 0x61 || waitAnim == 0x7E || waitAnim == 0x87 || waitAnim == 0xAC)
				noWait = true;
			if (waitAnim == 0x3D || waitAnim == 0x40 || waitAnim == 0x41)
			{
				noWait = true;
				goto noWaitPick;
			}

			if (ID >= 0x1 && ID <= 0x6 && !noWait) 
			{
				if (forced)
				{
					switch (ID)
					{
					case 1:
					case 2:
						autoWaitAnim = 0x3D;
						break;
					case 3:
					case 4:
					case 5:
						autoWaitAnim = 0x41;
						break;
					case 6:
						autoWaitAnim = 0x40;
						break;
					}
				}
				else
				{
					autoWaitAnim = 0x3C;
				}
				noWaitPick:
				//Process::Write32(0x59A24C, 0xE3A00000 + ID); //outside
				//Process::Write32(0x599FD0, 0xE3A00000 + ID); //inside
				Process::Write32(animInstance + 0xE, *(u32 *)actualItemToReplace);
				Process::Write8(animInstance + 0x12, ID == 0x21 ? *(u8 *)(player + 0x47C) : worldx);
				Process::Write8(animInstance + 0x13, ID == 0x21 ? *(u8 *)(player + 0x480) : worldy);
				Process::Write8(animInstance + 0x15, ID);
				Process::Write32((u32)&pfunction2, 0x5D4C88);
				pfunction2(animInstance + 0x17, (u32)Player::GetCoordinates(currentIndex));
				//Sleep(Milliseconds(40));
				//return Player::Pickup(player, worldx, worldy, 0, 0, 0);
			}
			else
			{
				*(u8 *)(animInstance + 0xE) = ID == 0x21 ? *(u8 *)(player + 0x47C) : worldx;
				*(u8 *)(animInstance + 0xF) = ID == 0x21 ? *(u8 *)(player + 0x480) : worldy;
				*(u16 *)(animInstance + 0x10) = *(u16 *)actualItemToReplace == 0x7FFE ? 0x2001 : *(u16 *)actualItemToReplace;
				if (waitAnim == 0x60 || waitAnim == 0x61)
				{
					volatile float *pCoords = Player::GetCoordinates();
					if (pCoords != nullptr)
					{
						*pCoords = worldx * 0x20 + 0x10;
						*((float *)((vu32)pCoords + 8)) = worldy * 0x20 + 0x10;
					}
				}
				if (ID == 0x7 && !noWait && waitAnim != 0x5C && waitAnim != 0x6A) autoWaitAnim = forced ? 0x5D : 0x5C;
				else if (ID >= 0x8 && ID <= 0xF && !noWait && waitAnim != 0x6A) autoWaitAnim = forced ? 0x6B : 0x6A;
				else if (forced && !noWait) autoWaitAnim = 0x4C;

				if (((ID >= 0x13 && ID <= 0x1C) || autoWaitAnim == 0x4A) && !noWait && !forced)
				{
					autoWaitAnim = 0x4A;
					*(u32 *)0x682430 = 0xE3A00000; // do not execute drop func from animation sub
					//*(u32 *)0x6760FC = 0xE3A07000 + ID;
					Process::Write16(animInstance + 0x10, ID);
					Sleep(Milliseconds(5));
					//*(u32 *)0x682430 = 0xEBFC5347;
					//*(u32 *)0x676054 = 0xAA000006;
					//*(u32 *)0x6760FC = 0xE5D77002 + ID;
				}
				else if (forced) autoWaitAnim = 0x4F;
			}
			if (waitAnim == 0x87)
			{
				Process::Write32((u32)&pfunction2, 0x5D4C88);
				pfunction2(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy + 1, coords));
				Process::Write8(animInstance + 0x12,0xC0);
				Process::Write8(animInstance + 0x13, 0);
				Process::Write8(animInstance + 0x14, 2);
			}
			else if (waitAnim == 0xAC)
			{
				Process::Write32((u32)&pfunction2, 0x5D4C88);
				pfunction2(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy + 1, coords));
				Process::Write8(animInstance + 0x12, 0);
				pfunction2(animInstance + 0x13, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy + 1, coords));
				Process::Write8(animInstance + 0x17, 0);
				Process::Write8(animInstance + 0x18, 2);
			}
			else if (waitAnim == 0x7E)
			{
				Process::Write32((u32)&pfunction2, 0x5D4C88);
				pfunction2(animInstance + 0xF, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy - 1, coords));
				Process::Write8(animInstance + 0x13, 0);
				pfunction2(animInstance + 0x17, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy - 1, coords));
				Process::Write8(animInstance + 0x16, 1);
				Process::Write16(animInstance + 0x14, 2);
			}
			if (currentIndex == GameHelper::GetActualPlayerIndex()) Player::ExecuteAnimation(player, autoWaitAnim, animInstance, 0);
			else Player::SendAnimPacket(GameHelper::GetActualPlayerIndex(), animInstance, autoWaitAnim, roomID == 0xA5 ? Player::GetRoom(currentIndex) : roomID, currentIndex);
		
			if (!noWait)
			{
				Sleep(Milliseconds(40));
				*(u32 *)0x682430 = 0xEBFC5347;
			}
		}
		return true;
	}
	bool GameHelper::RemoveItems(bool trample, u8 wX, u8 wY, u8 width, u8 length, bool allowAbort, bool removeEverything)
	{
		bool res = false;
		//if (Player::GetLocation() == 0xFF)
		//{
		u8 x = wX, y = wY;
		u32 count = 0, player = Player::GetInstance(4, 1);
		if (player == 0) return res;
		if (removeEverything)
		{
			if (Player::GetLocation() == 0xFF)
			{
				x = 0x10;
				y = 0x10;
			}
			else
			{
				x = 0;
				y = 0;
			}
		}

		res = true;
		if (trample)
		{
			Process::Write32(0x597FAC, 0xE1A00000);
			Process::Write32(0x597F38, 0xE1A00000);
			Process::Write32(0x597F54, 0xE1A00000);
		}
		while (res && (x - wX < width || removeEverything))
		{
			while (res && (y - wY < length || removeEverything))
			{
				if ((u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) != 0)
				{
					if (*GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) != 0x7FFE)
					{
						count++;
						if (trample)
						{
							//*Player::GetCoordinates() = x * 0x20 + 0x10;
							//*(float *)((u32)Player::GetCoordinates() + 8) = y * 0x20 + 0x8;
							GameHelper::TrampleAt(x, y);
							//Sleep(Milliseconds(8));
						}
						else
						{
							GameHelper::PlaceItemWrapper(0x6, 0xFFFFFFFF, (u32 *)0x95CFFC, (u32 *)0x95CFFC, x, y, 0, 0, 0, 0, 0, 0x3C, 0xA5);
						}
						Controller::Update();
						if (Controller::IsKeyPressed(Key::B) && allowAbort)
						{
							OSD::Notify("Aborted.");
							goto end;
						}
					}
				}
				else
				{
					res = false;
				}
				y++;
			}
			res = true;
			if (removeEverything)
			{
				if (Player::GetLocation() == 0xFF) y = 0x10;
				else y = 0;
			}
			else y = wY;
			x++;
			if ((u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) == 0)
			{
				res = false;
			}
		}
	end:
		if (trample)
		{
			Process::Write32(0x597FAC, 0x0A000039);
			Process::Write32(0x597F38, 0x0A000056);
			Process::Write32(0x597F54, 0x0A00004F);
		}
		else Process::Write32(0x59A268, 0x31F2DBF0);
		OSD::Notify(std::to_string(count) << " items removed!");
		return true;
		//}
	}
	void GameHelper::Particles(u32 particleID, float *floats, u32 u0, u32 u1)
	{
		if (floats == nullptr) return;
		Process::Write32((u32)&pfunction4, 0x207B90);
		pfunction4(particleID, (u32)floats, u0, u1);
	}
	void GameHelper::clearLockedSpot(u8 wX, u8 wY, u8 roomID, u32 param_4)
	{
		Process::Write32((u32)&pfunction4, 0x5A1278);
		pfunction4(wX, wY, roomID, param_4);
	}
	u32 GameHelper::createLockedSpot(u8 DropID, u8 wX, u8 wY, u8 roomID, bool sendPkt)
	{
		u32 index;
		Process::Write32((u32)&pfunction5, 0x5A13C4);
		if (*(u32 *)0x5A13C8 == 0xE3A00000)
		{
			*(u32 *)0x5A13C8 = 0xE24DD01C;
			*(u32 *)0x5A13CC = 0xE1A07001;
			
			index = pfunction5(DropID, wX, wY, roomID, sendPkt);

			*(u32 *)0x5A13C8 = 0xE3A00000;
			*(u32 *)0x5A13CC = 0xE8BD83F0;

			return index;
		}
		else
		{
			return pfunction5(DropID, wX, wY, roomID, sendPkt);
		}
	}

	u32 GameHelper::GetLockedSpotIndex(u8 wX, u8 wY, u8 roomID)
	{
		Process::Write32((u32)&pfunction3, 0x5A11BC);
		return pfunction3(wX, wY, roomID);
	}
	bool GameHelper::TrampleAt(u8 wX, u8 wY)
	{
		u32 pItem = (u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), wX, wY, 0);
		if (pItem != 0)
		{
			u8 room = Player::GetRoom(GameHelper::GetActualPlayerIndex());

			if (GameHelper::GetOnlinePlayerCount() != 0)
			{
				
				TramplePkt data { *(u32 *)pItem, room, wX, wY, 0 };
				Process::Write32((u32)&pfunction4, 0x625488);
				pfunction4(0x47, 4, (u32)&data, 8);
				
			}

			Process::Write32((u32)&pfunction5, 0x168E20);
			pfunction5(wX, wY, 0, room, 0x95CFFC);
			Process::Write32((u32)&pfunction3, 0x59F144);
			pfunction3(wX, wY, 1);
		}
	}
	float *GameHelper::WorldCoordsToCoords(u8 wX, u8 wY, float res[3])
	{
		volatile float *coords = Player::GetCoordinates();
		if (coords != nullptr) res[1] = *(volatile float *)((u32)coords + 4);
		res[0] = (float)(wX * 0x20 + 0x10);
		res[2] = (float)(wY * 0x20 + 0x10);
		return &res[0];
	}


	bool NPC::GetCoordinatesRef(u32 npc, float &X, float &Z)
	{
		if (npc == 0) return false;
		if (*(u32 *)(npc + 0x14) == 0) return false;
		X = *(volatile float *)(npc + 0x14);
		Z = *(volatile float *)(npc + 0x1C);
		return true;
	}

	std::string IDList::GetItemName(u32 ID)
	{
		std::string t;
		if ((ID & 0xFFFF) > 0x2000 && (ID & 0xFFFF) < 0x372B)
		{
			u32 addr = 0x31758960;
			addr = addr + *(u32 *)(addr + 4 + 4 * (ID - 0x2000)) + 0xC;
			u8 i = 0;
			while (*(u8 *)(i * 2 + addr) != 0)
			{
				t.push_back(*(char *)(i * 2 + addr));
				i++;
			}
		}
		else t.assign("???");
		return t;
	}

	std::string IDList::GetBuildingName(u8 ID)
	{
		Process::Write32((u32)&pfunction1, 0x5CA20C);
		return (char *)pfunction1(ID);
	}

	std::string IDList::GetSnakeName(u16 ID)
	{
		if (ID < 0x276 && ID != 0)
			return Color::Green << (std::string)((char *)(*(u32 *)(ID * 0x10 + 0x866D08)));
		else return Color::Red << "Invalid ID!";
	}
	
	std::string IDList::GetRoomName(u8 ID)
	{
		Process::Write32((u32)&pfunction1, 0x5B4BE4);
		if (ID <= 0xA5) return Color::Green << (std::string)((char *)(pfunction1(ID)));
		else return Color::Red << "Invalid ID!";
	}

	u8 Item::GetBehaviour(u32 *pItem)
	{
		Process::Write32((u32)&pfunction1, 0x2FCCD0);
		pfunction1((u32)pItem);
	}
	bool Item::hasTreeBehaviour(u32 *pItem)
	{
		Process::Write32((u32)&pfunction1, 0x2FE930);
		pfunction1((u32)pItem);
	}


	u32 Camera::GetInstance()
	{
		return *(u32 *)0x951884;
	}
	void Camera::AddToX(float val)
	{
		*(float *)(Camera::GetInstance() + 4) += val;
	}
	void Camera::AddToY(float val)
	{
		*(float *)(Camera::GetInstance() + 8) += val;
	}
	void Camera::AddToZ(float val)
	{
		*(float *)(Camera::GetInstance() + 0xC) += val;
	}
	void Camera::AddToYRotation(u16 val)
	{
		*(u16 *)(Camera::GetInstance() + 0x1C) += val;
	}

	void ItemSequence::init(u32 defaultPtr)
	{
		optDCKB = new Keyboard("Choose option:");
		DCKB = new Keyboard("Enter new ID:\n(B to remove)");
		items.clear();
		items.push_back(defaultPtr);
		defPtr = (u32 *)defaultPtr;
		optDCKB->CanAbort(true);
		DCKB->CanAbort(true);
		DCKB->IsHexadecimal(true);
		DCKB->SetMaxLength(8);
		DCKB->DisplayTopScreen = true;
	}
	u32 *ItemSequence::Next()
	{
		if (items.size() - 1 > index) index++;
		else index = 0;
		if (index == 0) return defPtr;
		else return &items.at(index);
	}
	u32 ItemSequence::PeekNext()
	{
		if (index + 1 == items.size()) return *defPtr;
		else return items.at(index + 1);
	}
	bool ItemSequence::openKB()
	{
		int8_t val;
		u32 newItem;
		keyBoardItems.clear();
		keyBoardItems.push_back((items.size() == 1 || index + 1 == items.size() ? "->Item in first slot" : "Item in first slot"));
		for (u8 i = 1; i < items.size(); i++) keyBoardItems.push_back((index + 1 == i  ? "->" : "") << Hex(items.at(i)));
		keyBoardItems.push_back("Add...");
		optDCKB->Populate(keyBoardItems);
		val = optDCKB->Open();
		switch (val)
		{
		case -1:
			return false;

		case 0:
			Sleep(Milliseconds(100));
			newItem = *defPtr;
			if (DCKB->Open(newItem, newItem) == 0) *defPtr = newItem;
			break;

		default:
			Sleep(Milliseconds(100));
			if (val + 1 == keyBoardItems.size())
			{
				if (DCKB->Open(newItem) == 0) items.push_back(newItem);
			}
			else
			{
				if (DCKB->Open(items.at(val), items.at(val)) == -1)	items.erase(items.begin() + val);
			}
			break;
		}
		Sleep(Milliseconds(100));
		openKB();
		return true;
	}
	void ItemSequence::Switch(bool enable)
	{
		useDropChain = enable;
		//if (enable) defItem = *(u32 *)0x31F2DBF0;
		//if (!enable) *(u32 *)0x31F2DBF0 = defItem;
	}
	bool ItemSequence::Enabled()
	{
		return useDropChain;
	}
}