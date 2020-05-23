#include "cheats.hpp"
#include "MenuEntryhelpers.hpp"
#include "PlayerHelper.hpp"
#include "text_color.h"

//0x891350 fixed room IDs start
namespace CTRPluginFramework
{
	Keyboard *kb = new Keyboard("Enter ID:");
	Keyboard *optKb = new Keyboard("Choose option:");

	volatile u32(*pfunction00)();
	volatile u32(*pfunction01)(u32 param1);
	volatile u32(*pfunction02)(u32 param1, u32 param2);
	volatile u32(*pfunction03)(u32 param1, u32 param2, u32 param3);
	volatile u32(*pfunction04)(u32 param1, u32 param2, u32 param3, u32 param4);
	volatile u32(*pfunction05)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5);
	volatile u32(*pfunction06)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6);
	volatile u32(*pfunction07)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7);
	volatile u32(*pfunction08)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8);
	volatile u32(*pfunction09)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9);
	volatile u32(*pfunction010)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9, u32 param10);
	volatile u32(*pfunction011)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9, u32 param10, u32 param11);

	struct Building {
		u16 id;
		u8 x;
		u8 y;
	};

	struct Island {
		Building b[2];
		u8 acres[16];
	};
	
	u32 emotion = 1, soundID = 0x01000660;
	u16 snake = 1;
	u8 appearance[3] = {0, 0, 0};
	u32 particleID = 0x0214;
	u8 playerindex = 0;
	u32 npc;
	u8 npcEmote;
	u32 npcSnake;
	u8 anim1 = 0x06, roomID = 0, waitAnim = 0x56, animToForce = 6, npcAnim = 0;
	u8 DropType = 0xA, DropRoom = 0xA5;
	u32 animItem = 0x2001, npcAnimItem = 0x2001;
	u32 itemIDToReplace = 0x7FFE;
	std::vector<u16> clipboard;

	Island isl{ 0x69, 0x1F, 0x15, 0x6A, 0x1D, 0x15, 0xAE, 0xAF, 0xAF, 0xB0, 0xAD, 0xCB, 0xB9, 0xAC, 0xAD, 0xC7, 0xC1, 0xAC, 0xAB, 0xA9, 0xA9, 0xAA };
	File * file = new File();
	const std::string path = "E:/plugin/island.bin";
	const std::string clipBoardsPath = "E:/plugin/Clipboards/";
	const std::string snakeKB = "Enter SNAKE animation ID / snake ID\n(Use with animation 0x3E, 0xC4 or 0xC5)";

	float coordSpeed = 7.0f, walkSpeed = 1.4f;

	bool turbo = false;
	bool selecting = false;
	bool bypassing = false;
	bool animExecuting = false;

	//tile selector
	u32 selectedX, selectedY, selectedItem = 0x10; 

	std::vector<std::string> cmnOpt{
		"Enable",
		"Disable"
	};
	std::vector<std::string> APressOpt
	{
		"Tree shaking",
		"Shovel",
		"Axe",
		"Restore",
	};
	std::vector<std::string> YPressOpt
	{
		"Pick up",
		"Fill hole",
		"Restore",
	};
	
	std::vector<std::string> dropOpt {
		"Pick Up",
		"Pick Up (Full)",
		"Pull Clover",
		"Pull Clover (Full)",
		"Pull Clover (Release)",
		"Pull",
		"Fill Hole",
		"Shake Fruit Tree",
		"Shake Tree",
		"Drop",
		"Bury",
		"Plant",
		"Display Pattern",
		"Chop Fruit Tree",
		"Chop Tree",
		"Hit Item",
		"Hit Money Rock",
		"Smash Rock",
		"Dig",
		"Dig up item",
		"Dig Item (Full)",
		"Dig Item (Tour)",
		"Dig Time Capsule",
		"Dig Villager Item",
		"Dig (Wrong item)",
		"Dig (Tour)",
		"Dig up stump",
		"Fill Hole (Shovel)",
		"Sit(Does nothing)",
		"Stand Up(Does nothing)",
		"Fall In Hole",
		"Fall In Pitfall",
		"Item Tree Drop",
		"Unknown",
		"Unknown",
		"Unknown",
		"Select an Animation",
	};

	std::vector<std::string> warpOpt{
		"Enter ID",
		"Heading to island",
		"Arrive at Hut",
		"Return to town",
		"Leave from island",
	};
	std::vector<std::string> animOpt{
		"Animation ID",
		"Set item",
		"SNAKE ID",
		"Emote ID",
		"Sound ID",
		"Appearance",
		"Online Animation ID",
	};
	std::vector<std::string> shapeOpt{
		"Full square",
		"Circle",
		"Horizontal line",
		"Vertical line",
		"Square",
		"NE to SW diagonal",
		"NW to SE diagonal",
		"Reset",
	};

	std::vector<std::string> clipBoardOpt
	{
		"Copy...",
		"Cut...",
		"Paste",
		"Settings...",
		"Save clipboard...",
		"Load clipboard...",
		"Delete clipboard...",
	};

	std::vector<std::string> funcOpt{
		"Call function",
		"Building Modifier",
		"Change Gametype",
		"Remove All Items",
		"Search and replace",
		"Reload Room",
		"Lock spot",
		"Unlock spot",
		"Lock Map",
		"Unlock Map",
		"Randomize Tool",
		"Randomize Outfit",
	};

	std::vector<std::string> npcOpt{
		"Choose NPC",
		"NPC animation",
		"Set item ID",
		"NPC snake",
		"NPC emote",
	};

	std::vector<std::string> npcNames{
		"Grams",
		"Leila",
		"Leilani",
		"Kapp'n",
		"Phineas",
		"Redd",
		"Isabelle",
		"Villager 0",
		"Villager 1",
		"Villager 2",
		"Villager 3",
		"Villager 4",
		"Villager 5",
		"Villager 6",
		"Villager 7",
		"Villager 8",
		"Villager 9",
	};

	std::vector<std::string> dropAnimOpt{
		"Pick up wait",
		"Pick up",
		"Pull",
		"Pick up show off",
		"Shovel wait",
		"Shovel hit",
		"Dig hole",
		"Dig out item",
		"Fill hole with shovel",
		"Bury item",
		"Axe wait",
		"Axe hit",
		"Fill hole wait",
		"Fill hole",
		"Fall in hole",
		"In hole",
		"Get out of hole",
		"Tree shake wait",
		"Tree shake",
		"Release Bug",
		"Stand up",
		"Get up",
		"Stand up",
	};
	u8 dropAnims[]{ 0x3C, 0x3D, 0x40, 0x41, 0x4A, 0x4C, 0x4F, 0x50, 0x52, 0x53, 0x56, 0x5A, 0x5C, 0x5D, 0x5F, 0x60, 0x61, 0x6A, 0x6B, 0x73, 0x7E, 0x87, 0xAC };
	std::vector<u8> anims;

	/*std::vector<std::string> selectorOpt{
		"Set item ID...",
		"Go to world coords...",
		"Set selection range...",
		"Lock spot",
		"Unlock spot",
		"Copy item on ground",
	};*/
	/*std::vector<std::string> playerOpt{
		"Selected player",
		"All players",
	};*/

	// basic addresses
	const u32 inv = 0x31F2DBF0;
	const u32 online_inv0 = 0xAAB0E0;
	const u32 online_inv1 = 0xA8C360;
	const u32 online_inv2 = 0xA967E0;
	const u32 online_inv3 = 0xAA0C60;
	const u32 room = 0x958342;
	const u32 islandAcres = 0x301DC292;
	const u32 islandBuildings = 0x301DD2B2;
	const u8 *Mapbool = (u8 *)0x950C30;
	const u32 playerPtrs = 0xAAE994;
	const u32 playerCount = 0x31FE272C + 0x1326f;
	const u32 playerIndex = 0x31FE272C + 0x13268;
	const u8 *ActualRoom = (u8 *)0x94F4C5;
	//const u8 *isplayeronline = 0x31FF59C9;


	// misc
	void fastSpeed(MenuEntry *entry) 
	{ 
		const u32 addr = 0x54DDB4;
		u8 val;
		static bool enabled = false;

		if (enabled)
		{
			if (Process::Read8(room, val) && (val == 0x60 || val == 0x61 || val == 0x69 || val == 0x6A || val == 0x8B || val == 0x8C || val == 0x8D))
				Process::Write32(addr, 0xE59400A0);
			else
				Process::Write32(addr, 0xE3E004FF);
		}
		if (Controller::IsKeysPressed(Key::L + Key::A))
		{
			if (enabled)
			{
				enabled = false;
				OSD::Notify("Frame Limitor: ReEnabled!");
				Process::Write32(addr, 0xE59400A0);
			}
			else
			{
				enabled = true;
				OSD::Notify("Frame Limitor: Disabled!");
			}
		}
	}

	void shopMod(MenuEntry *entry)
	{
		const u32 shop = 0x33038D90;
		u32 val;
		if (Process::Read32(shop, val) && val != 0)
		{
			kb->GetMessage() = "Enter ID:";
			kb->IsHexadecimal(true);
			kb->SetMaxLength(8);
			for (int i = 0; i < 4; i++)
			{
				if (kb->Open(val, val) == 0)
				{
					Process::Write32(shop + i * 4, val);
				}
				Process::Read32(shop + i * 4 + 4, val);
			}
			val = 0;
		}
	}

	void fixMenu(MenuEntry *entry)
	{
		if (Controller::IsKeysPressed(Key::R + Key::Y))
		{
			Process::Write32(0x9526BC, 0);
			Process::Write16(0x950D68, 2);
			Process::Write32(0x318A4F10, 0);
			Process::Write32(0x318A4F14, 0);
			Process::Write32(0x318A4F18, 0);
			Process::Write32(0x318A4F20, 0);
			OSD::Notify("Menu fixed");
		}
	}
	
	std::vector<std::string> PatternPickNames{
        "Send",
        "Recieve",
        "Disable"
    };

    void	pattern_picker(MenuEntry *entry)
    {
        std::vector<std::string> PatternPicker;
        static std::string YesValue;
        static int PatternPickChoice = 0;
        static u8 SourceValue = 0;
        static u8 DestValue = 0;

        if (entry->Hotkeys[0].IsDown())
        {
            for (int p = 0; p < 3; p++) 
            {
                std::string PatternNameString = "";
                
                if (p == 0)
                    PatternNameString = ("0x" + SourceValue);

                else if (p == 1)
                    PatternNameString = ("0x%01X", DestValue);

                if(p < 2)
                    YesValue = (" [" + PatternNameString + "]");
                else
                    YesValue = ("");

                PatternPicker.push_back(PatternPickNames[p] + YesValue);
            }

            Keyboard    keyboard("Pick an Option", PatternPicker);
            PatternPickChoice = keyboard.Open();

            if (PatternPickChoice < 0)
                return;

            Sleep(Seconds(0.1f));
            if (PatternPickChoice == 0)
            {
                Keyboard    keyboard("Source Pattern Slot");
                keyboard.IsHexadecimal(true);
                if (keyboard.Open(SourceValue, SourceValue) != -1)
                {
                    Process::Patch(0x1B5004, 0xE1CD50B0);
                    Process::Patch(0x1B5008, 0xE1CD00B2);
                    Process::Patch(0x1B500C, 0xE1A00000);
                    Process::Patch(0x1B5000, 0xE3A00000 + SourceValue);
                }
            }
            else if (PatternPickChoice == 1)
            {
                Keyboard    keyboard("Destination Pattern Slot");
                keyboard.IsHexadecimal(true);
                if (keyboard.Open(DestValue, DestValue) != -1)
                {
                    Process::Patch(0x1B5004, 0xE1CD50B0);
                    Process::Patch(0x1B5008, 0xE1CD00B2);
                    Process::Patch(0x1B500C, 0xE1A00000);
                    Process::Patch(0x1B4FFC, 0xE3A05000 + DestValue);
                }
            }
            else
            {
                SourceValue = 0;
                DestValue = 0;
                Process::Patch(0x1B4FFC, 0xE1550001);
                Process::Patch(0x1B5000, 0x11500001);
                Process::Patch(0x1B5004, 0x11CD50B0);
                Process::Patch(0x1B5008, 0x11CD00B2);
                Process::Patch(0x1B500C, 0x0A000004);
            }      
        }
    }

	std::vector<std::string> ChatPickNames{
        "P1",
        "P2",
        "P3",
        "P4",  
        "Restore"
    };

	void chat_picker(MenuEntry *entry)
    {
        std::vector<std::string> ChatPicker;
        static bool PlayerNameUpdate = false;
        static std::string YesPlayer;
        static std::string NoPlayer = " [N/A]";
        static int ChatPickChoice = 0;
        static int ColoredChoice = 0;
        u8 PIndex;
        u32 NameOffset;

        if (entry->Hotkeys[0].IsDown())
        {
            for (int p = 0; p < 5; p++) 
            {
                std::string PlayerNameString = "";
                Process::Read8(0xAAE990 + p, PIndex);
                if (p == 0)
                {
                    if (PIndex < 4)
                        NameOffset = 0x31F2C5C8 + (0xA480 * PIndex);
                    else if (PIndex == 7)
                        NameOffset = 0xAA9AB8;
                }
                else if (p < 4)
                {
                    if (PIndex < 8)
                       NameOffset = 0xA8AD38 + (0xA480 * p - 0xA480);
                }    
                
                if(p < 4)
                {
                    if(PIndex < 8)
                    {
                        PlayerNameUpdate = true;
                        Process::ReadString(NameOffset, PlayerNameString, 16, StringFormat::Utf16);
                        YesPlayer = (" [" + PlayerNameString + "]");
                    }
                    else
                        PlayerNameUpdate = false;
                }
                else
                {
                    PlayerNameUpdate = true;;
                    YesPlayer = ("");
                }

                std::string PlayerMenu = (PlayerNameUpdate == true) ? YesPlayer : NoPlayer;
                if(ColoredChoice == p)
                    ChatPicker.push_back(Color(0xFFB8D2FF) << ChatPickNames[p] + PlayerMenu);
                else
                    ChatPicker.push_back(ChatPickNames[p] + PlayerMenu);
            }

            Keyboard    keyboard("Pick a player you want to chat from!", ChatPicker);
            ChatPickChoice = keyboard.Open();

            if (ChatPickChoice >= 0)
                ColoredChoice = ChatPickChoice;

            if (ChatPickChoice < 0)
                return;

            if (ChatPickChoice == 4)
                Process::Write32(0x8471BC, 0x8F8E8D8C);
            
            else
            {
                u8 IndexSlot;
                Process::Read8(0x31FF5994, IndexSlot);
                if(IndexSlot < 4)
                    Process::Write8(0x8471BC + IndexSlot, 0x8C+ChatPickChoice);
            }
        }
    }
	
	void allseeding(MenuEntry *entry)
    {
		static int spriteID;
        static u32 allID = 0;
        static bool beendone = false;
        u16 remind;

        if (entry->WasJustActivated());
        {
            Process::Patch(0x839A00, 0xE59F1004);
            Process::Patch(0x839A04, 0xE12FFF1E);
            Process::Write32(0x839A0C, 0x9E0020);
        }
        if (entry->Hotkeys[0].IsDown())
        {
            Keyboard    keyboard("Choose an Item ID \nUse 0 to disable.");
            keyboard.IsHexadecimal(true);
            if (keyboard.Open(allID, allID) != -1)
            {
                if (allID != 0)
                {
                    Process::Patch(0x59FCD4, 0xEB0A6749);
                    Process::Write32(0x9E0020, allID);
                    if (spriteID == 0)
                        Process::Patch(0x59FCE0, 0xEB0A6746);

                    else if (spriteID == 1)
                        Process::Patch(0x59FCE0, 0xE59D1028);
                }
                else if (allID == 0)
                {
                    Process::Patch(0x59FCD4, 0xE59D1024);
                    Process::Patch(0x59FCE0, 0xE59D1028);
                }
            }
        }
        if (allID != 0)
        {
            Process::Read16(0x9513D4, remind);
            if (remind != 0xFFFF && !beendone)
            {
                OSD::Notify(Utils::Format("Reminder: All Seeder - 0x%08X", allID));
                beendone = true;
            }
            else if (remind == 0xFFFF && beendone)
                beendone = false;
        }
    }

	void extender(MenuEntry *entry)
	{
		u8 val;

		if (entry->Hotkeys[0].IsDown())
		{
			Process::Write8(0xAD7253, 0x1);
		}
		if (entry->Hotkeys[0].IsDown())
		{
			Process::Write8(0xAD75C0, 0x01);
		}
	}
	
	void Forcesendchat(MenuEntry *entry)
	{
		if (entry->Hotkeys[0].IsDown())
		{
			Process::Write8(0x95F0E9, 0x1);
		}
	}

    void kaomoji(MenuEntry *entry)
    {
        u32		keyboard1;
        u32		keyboard2;
        u32		typedtext;

        Process::Read32(0x95F11C, keyboard1);
        if (keyboard1 != 0)
        {
            Process::Read32(0x95F110, keyboard2);
            {
                Process::Read32(keyboard2, typedtext);
                if (typedtext == 0x25CF0021)
                    Process::WriteString(keyboard2, "(●´ω｀●)ゞ", StringFormat::Utf16);
                
                else if (typedtext == 0x002A0021)
                    Process::WriteString(keyboard2, "(*「･ω･)ﾝ？", StringFormat::Utf16);

                else if (typedtext == 0x30C40021)
                    Process::WriteString(keyboard2, "¯\\_(ツ)_/¯", StringFormat::Utf16);

                else if (typedtext == 0xFF350021)
                    Process::WriteString(keyboard2, "（Ｕ・ω・）", StringFormat::Utf16);

                else if (typedtext == 0x253B0021)
                    Process::WriteString(keyboard2, "┬─┬ノ( º _ ºノ)", StringFormat::Utf16);

                else if (typedtext == 0x00210021)
                    Process::WriteString(keyboard2, "┻━┻ ミヽ(`Д´)ﾉミ ┻━┻", StringFormat::Utf16);

                else if (typedtext == 0x04490021)
                    Process::WriteString(keyboard2, "(щ(ºДºщ)", StringFormat::Utf16);

                else if (typedtext == 0xF2F20021)
                    Process::WriteString(keyboard2, "*HUG!* :) I love everyone!!", StringFormat::Utf16);
				
				else if (typedtext == 0x30E20021)
					Process::WriteString(keyboard2, "モジ(((ＵωＵ｀ *)(* ´ＵωＵ)))モジ", StringFormat::Utf16);
				
				else if (typedtext == 0x00280021)
					Process::WriteString(keyboard2, "(*´∀`*)えへへ", StringFormat::Utf16);
				
				else if (typedtext == 0x30FD0021)
					Process::WriteString(keyboard2, "ヽ(▽ ｀)ノﾜｰｲ♪ヽ(´▽｀)ノﾜｰｲ♪ヽ( ´▽)ノ", StringFormat::Utf16);
				
				else if (typedtext == 0x002B0021)
					Process::WriteString(keyboard2, "+｡:.ﾟヽ(´∀`｡)ﾉﾟ.:｡+ﾟﾟ+｡:.ﾟヽ(*´∀`)ﾉﾟ.:｡+ﾟ", StringFormat::Utf16);
				
				else if (typedtext == 0x006F0021)
					Process::WriteString(keyboard2, "Oh you~☆(*´∀｀)σ)))))*3ﾟ),･´.､", StringFormat::Utf16);
				
				else if (typedtext == 0x22000021)
					Process::WriteString(keyboard2, "(*≧∀≦*)", StringFormat::Utf16);
				
				else if (typedtext == 0x005E0021)
					Process::WriteString(keyboard2, "(^-^*)ノ", StringFormat::Utf16);
				
				else if (typedtext == 0x03C90021)
					Process::WriteString(keyboard2, "(〃￣ω￣〃ゞ", StringFormat::Utf16);
				
				else if (typedtext == 0x26050021)
					Process::WriteString(keyboard2, "｡･:*:･ﾟ★,｡･:*:･ﾟ☆ﾟ･:*:･｡,★ﾟ･:*:･｡", StringFormat::Utf16);
				
				else if (typedtext == 0x00240021)
					Process::WriteString(keyboard2, "( /¯ °3° )/¯Hey sexaaayy~", StringFormat::Utf16);
				
				else if (typedtext == 0x007E0021)
					Process::WriteString(keyboard2, "pssSST( ﾟ∀(・－・) I watched you sleeping last night..~", StringFormat::Utf16);
				
				else if (typedtext == 0x003B0021)
					Process::WriteString(keyboard2, "(*.;´;Д;`:,);ﾞ;｀;:ﾞ;｀;:Aah~Stop iiiit~♪", StringFormat::Utf16);
				
				else if (typedtext == 0x04140021)
					Process::WriteString(keyboard2, "Huff(:.;ﾟ;Д;ﾟ;.:)Huff", StringFormat::Utf16);
				
				else if (typedtext == 0x20220021)
					Process::WriteString(keyboard2, "(•ω•)", StringFormat::Utf16);
            }
        }
    }

	void TranslateJAP(MenuEntry *entry)
	    {
        u32		JPkeyboard1;
        u32		JPkeyboard2;
        u32		JPtypedtext;

        Process::Read32(0x95F11C, JPkeyboard1);
        if (JPkeyboard1 != 0)
        {
            Process::Read32(0x95F110, JPkeyboard2);
            {
                Process::Read32(JPkeyboard2, JPtypedtext);
                if (JPtypedtext == 0x041D0021 || JPtypedtext == 0x00680021)
                    Process::WriteString(JPkeyboard2, "こんにちは", StringFormat::Utf16);
				
				if (JPtypedtext == 0x04120021 || JPtypedtext == 0x00620021)
                    Process::WriteString(JPkeyboard2, "さようなら", StringFormat::Utf16);
				
				if (JPtypedtext == 0x039D0021 || JPtypedtext == 0x006E0021)
                    Process::WriteString(JPkeyboard2, "いいえ", StringFormat::Utf16);
				
				if (JPtypedtext == 0x03A50021 || JPtypedtext == 0x00790021)
                    Process::WriteString(JPkeyboard2, "はい", StringFormat::Utf16);
				
				if (JPtypedtext == 0x04100021 || JPtypedtext == 0x04300021)
                    Process::WriteString(JPkeyboard2, "ごめんなさい", StringFormat::Utf16);
				
				if (JPtypedtext == 0x00530021 || JPtypedtext == 0x00730021)
                    Process::WriteString(JPkeyboard2, "やめてください", StringFormat::Utf16);
				
				if (JPtypedtext == 0x00560021 || JPtypedtext == 0x00760021)
                    Process::WriteString(JPkeyboard2, "すばらしいです", StringFormat::Utf16);
				
				if (JPtypedtext == 0x04210021 || JPtypedtext == 0x04410021)
                    Process::WriteString(JPkeyboard2, "ここに来てください", StringFormat::Utf16);
				
				if (JPtypedtext == 0x04200021 || JPtypedtext == 0x04400021)
                    Process::WriteString(JPkeyboard2, "えいごでおねがい", StringFormat::Utf16);
				
				if (JPtypedtext == 0x00490021 || JPtypedtext == 0x00690021)
                    Process::WriteString(JPkeyboard2, "えいごしかはなせない", StringFormat::Utf16);
                
				if (JPtypedtext == 0x00310021)
                    Process::WriteString(JPkeyboard2, "にほんごのこと、さっぱりわからない", StringFormat::Utf16);
				
				if (JPtypedtext == 0x00320021)
                    Process::WriteString(JPkeyboard2, "にほんごならつうじれないです", StringFormat::Utf16);
				
				if (JPtypedtext == 0x00740021 || JPtypedtext == 0x04220021)
                    Process::WriteString(JPkeyboard2, "ありがとう", StringFormat::Utf16);
            }
        }
    }
	
	void TranslateKOR(MenuEntry *entry)
	    {
        u32		KOkeyboard1;
        u32		KOkeyboard2;
        u32		KOtypedtext;

        Process::Read32(0x95F11C, KOkeyboard1);
        if (KOkeyboard1 != 0)
        {
            Process::Read32(0x95F110, KOkeyboard2);
            {
                Process::Read32(KOkeyboard2, KOtypedtext);
                if (KOtypedtext == 0x041D0021 || KOtypedtext == 0x00680021)
                    Process::WriteString(KOkeyboard2, "여보세요", StringFormat::Utf16);
				
				if (KOtypedtext == 0x04120021 || KOtypedtext == 0x00620021)
                    Process::WriteString(KOkeyboard2, "나중에 보자", StringFormat::Utf16);
				
				if (KOtypedtext == 0x039D0021 || KOtypedtext == 0x006E0021)
                    Process::WriteString(KOkeyboard2, "아니", StringFormat::Utf16);
				
				if (KOtypedtext == 0x03A50021 || KOtypedtext == 0x00790021)
                    Process::WriteString(KOkeyboard2, "예", StringFormat::Utf16);
				
				if (KOtypedtext == 0x04100021 || KOtypedtext == 0x04300021)
                    Process::WriteString(KOkeyboard2, "당신을 귀찮게해서 미안 해요", StringFormat::Utf16);
				
				if (KOtypedtext == 0x00530021 || KOtypedtext == 0x00730021)
                    Process::WriteString(KOkeyboard2, "제발 그만해", StringFormat::Utf16);
				
				if (KOtypedtext == 0x00560021 || KOtypedtext == 0x00760021)
                    Process::WriteString(KOkeyboard2, "매우 인상적", StringFormat::Utf16);
				
				if (KOtypedtext == 0x04210021 || KOtypedtext == 0x04410021)
                    Process::WriteString(KOkeyboard2, "당신은 나에게 올 수 있습니까?", StringFormat::Utf16);
				
				if (KOtypedtext == 0x00740021 || KOtypedtext == 0x04220021)
                    Process::WriteString(KOkeyboard2, "감사!", StringFormat::Utf16);
            }
        }
    }
	
	void TranslateGER(MenuEntry *entry)
	    {
        u32		GRkeyboard1;
        u32		GRkeyboard2;
        u32		GRtypedtext;

        Process::Read32(0x95F11C, GRkeyboard1);
        if (GRkeyboard1 != 0)
        {
            Process::Read32(0x95F110, GRkeyboard2);
            {
                Process::Read32(GRkeyboard2, GRtypedtext);
                if (GRtypedtext == 0x041D0021 || GRtypedtext == 0x00680021)
                    Process::WriteString(GRkeyboard2, "Hallo", StringFormat::Utf16);
				
				if (GRtypedtext == 0x04120021 || GRtypedtext == 0x00620021)
                    Process::WriteString(GRkeyboard2, "Auf Wiedersehen", StringFormat::Utf16);
				
				if (GRtypedtext == 0x039D0021 || GRtypedtext == 0x006E0021)
                    Process::WriteString(GRkeyboard2, "Nein", StringFormat::Utf16);
				
				if (GRtypedtext == 0x03A50021 || GRtypedtext == 0x00790021)
                    Process::WriteString(GRkeyboard2, "Ja", StringFormat::Utf16);
				
				if (GRtypedtext == 0x04100021 || GRtypedtext == 0x04300021)
                    Process::WriteString(GRkeyboard2, "Es tut mir Leid", StringFormat::Utf16);
				
				if (GRtypedtext == 0x00530021 || GRtypedtext == 0x00730021)
                    Process::WriteString(GRkeyboard2, "Bitte hör auf", StringFormat::Utf16);
				
				if (GRtypedtext == 0x00560021 || GRtypedtext == 0x00760021)
                    Process::WriteString(GRkeyboard2, "Sehr beeindruckend", StringFormat::Utf16);
				
				if (GRtypedtext == 0x04210021 || GRtypedtext == 0x04410021)
                    Process::WriteString(GRkeyboard2, "Kannst du bitte herkommen?", StringFormat::Utf16);
				
				if (GRtypedtext == 0x00740021 || GRtypedtext == 0x04220021)
                    Process::WriteString(GRkeyboard2, "Danke vielmals!", StringFormat::Utf16);
            }
        }
    }

	void TranslateFRA(MenuEntry *entry)
	    {
        u32		FRkeyboard1;
        u32		FRkeyboard2;
        u32		FRtypedtext;

        Process::Read32(0x95F11C, FRkeyboard1);
        if (FRkeyboard1 != 0)
        {
            Process::Read32(0x95F110, FRkeyboard2);
            {
                Process::Read32(FRkeyboard2, FRtypedtext);
                if (FRtypedtext == 0x041D0021 || FRtypedtext == 0x00680021)
                    Process::WriteString(FRkeyboard2, "Salut", StringFormat::Utf16);
				
				if (FRtypedtext == 0x04120021 || FRtypedtext == 0x00620021)
                    Process::WriteString(FRkeyboard2, "Au revoir", StringFormat::Utf16);
				
				if (FRtypedtext == 0x039D0021 || FRtypedtext == 0x006E0021)
                    Process::WriteString(FRkeyboard2, "Non", StringFormat::Utf16);
				
				if (FRtypedtext == 0x03A50021 || FRtypedtext == 0x00790021)
                    Process::WriteString(FRkeyboard2, "Oui", StringFormat::Utf16);
				
				if (FRtypedtext == 0x04100021 || FRtypedtext == 0x04300021)
                    Process::WriteString(FRkeyboard2, "Je suis désolé", StringFormat::Utf16);
				
				if (FRtypedtext == 0x00530021 || FRtypedtext == 0x00730021)
                    Process::WriteString(FRkeyboard2, "S'il vous plaît arrêter", StringFormat::Utf16);
				
				if (FRtypedtext == 0x00560021 || FRtypedtext == 0x00760021)
                    Process::WriteString(FRkeyboard2, "Cool", StringFormat::Utf16);
				
				if (FRtypedtext == 0x04210021 || FRtypedtext == 0x04410021)
                    Process::WriteString(FRkeyboard2, "Viens ici s'il te plait", StringFormat::Utf16);
                
				if (FRtypedtext == 0x00740021 || FRtypedtext == 0x04220021)
                    Process::WriteString(FRkeyboard2, "Je vous remercie!", StringFormat::Utf16);
            }
        }
    }

	void customSymbols(MenuEntry *entry)
	{
        u32		keyboard1;
       
        Process::Read32(0x95F11C, keyboard1);
        if (keyboard1 != 0)
        {
            keyboard1 += 0xC;
            Process::Write8(keyboard1, 0x41);
            keyboard1 += 0x11F;
            Process::Write8(keyboard1, 0x44);
        }
    }

	void playername(MenuEntry *entry)
    {
        std::string namevalue;
        u32 playslot;

        Keyboard    keyboard("Write your new name.");
        keyboard.SetMaxLength(9);
        if (keyboard.Open(namevalue) != -1)
        {
            Process::Read32(0xAB014C, playslot);
            Process::WriteString(playslot + 0x55A8, namevalue, StringFormat::Utf16);
        }
    }
	
	void	townname(MenuEntry *entry)
    {
        std::string townvalue;
        u32 playslot;

        Keyboard    keyboard("Write your new Town Name.");
        keyboard.SetMaxLength(8);
        if (keyboard.Open(townvalue) != -1)
        {
            Process::Read32(0xAB014C, playslot);
            Process::WriteString(playslot + 0x55BE, townvalue, StringFormat::Utf16);
            Process::WriteString(0x31F8913A, townvalue, StringFormat::Utf16);
        }
    }

	void tppexport(MenuEntry *entry)
    {
        std::string SavePName;

        Keyboard keyboard("Name the file");
        keyboard.SetMaxLength(30);
        if (keyboard.Open(SavePName) != -1) {
            Sleep(Seconds(0.1f));
            Directory SaveDir;
            if (Directory::IsExists("/FOXXY/jpg") == 0)
                Directory::Create("/FOXXY/jpg");
            
            File SaveFile;
            File::Open(SaveFile,"/FOXXY/jpg/"+SavePName+".jpg", File::RWC);
            SaveFile.Dump(0x31F2C758, 0x1400);
            SaveFile.Close();
            MessageBox("Photo Exported to FOXXY/jpg")();
        }
    }
	
	void tppimport(MenuEntry *entry)
    {
        std::vector<std::string> FileList;
        Directory SavePDir;
      
        Directory::Open(SavePDir,"/FOXXY/jpg", true);
        SavePDir.ListFiles(FileList, ".jpg");
        Keyboard keyboard("Choose a picture", FileList);
        int FileChoice = keyboard.Open();

        if(FileChoice < 0)
            return;

        Sleep(Seconds(0.1f));
        File SaveFile;    
        SavePDir.OpenFile(SaveFile, FileList[FileChoice], File::READ);
        SaveFile.Inject(0x31F2C758, 0x1400);
        MessageBox("Image injected!")();
    }

	void dumpdata(MenuEntry *entry)
    {
        std::string SaveName;

        Keyboard keyboard("Name the file");
        keyboard.SetMaxLength(30);
        if (keyboard.Open(SaveName) != -1) {
            Sleep(Seconds(0.1f));
            Directory SaveDir;
            if (Directory::IsExists("/FOXXY/Saves") == 0)
                Directory::Create("/FOXXY/Saves");
            
            File SaveFile;
            File::Open(SaveFile,"/FOXXY/Saves/"+SaveName+".sav", File::RWC);
            SaveFile.Dump(0x31F26F80, 0x89B00);
            SaveFile.Close();
            MessageBox("Dump complete!")();
        }
    }
	
	void restoredata(MenuEntry *entry)
    {
        std::vector<std::string> FileList;
        Directory SaveDir;
      
        Directory::Open(SaveDir,"/FOXXY/Saves", true);
        SaveDir.ListFiles(FileList, ".sav");
        Keyboard keyboard("Choose a save file!", FileList);
        int FileChoice = keyboard.Open();

        if(FileChoice < 0)
            return;

        Sleep(Seconds(0.1f));
        File SaveFile;    
        SaveDir.OpenFile(SaveFile, FileList[FileChoice], File::READ);
        SaveFile.Inject(0x31F26F80, 0x89B00);
        MessageBox("Save restored!")();
    }

	void tpccode(MenuEntry *entry)
    {
        static bool Message = false;
        u8 Part1;
        u8 Part2;
        u16 Part3;
        u16 Part4;

        {
            Keyboard Choices("Page 1 of 4");
            int UserChoice = Choices.Open(Part1);
            Keyboard Choices1("Page 2 of 4");
            int UserChoice1 = Choices1.Open(Part2);
            Keyboard Choices2("Page 3 of 4");
            int UserChoice2 = Choices2.Open(Part3);
            Keyboard Choices3("Page 4 of 4");
            int UserChoice3 = Choices3.Open(Part4);
            OSD::Notify("Updated Dream Code!");
            Message = true;
            {
                Process::Write16(0x31F2C719, Part1);
                Process::Write16(0x31F2C714, Part2);
                Process::Write16(0x31F2C712, Part3);
                Process::Write16(0x31F2C710, Part4);
            }
        }
    } 

	std::vector<std::string> sex{
		"Cock and balls",
		"Tits and vagina",
	};

    void	gender(MenuEntry *entry)
    {
        u32 playslot = 0;

        Keyboard    keyboard("What do you want to have?", sex);
        int         dickorpusschoice = keyboard.Open();

        if (dickorpusschoice < 0)
            return;

            Process::Write8(0x31F2C5DA, dickorpusschoice);
    }
	
	void censusMenu(MenuEntry *entry) { toggle2(0x31F2C74F, 0x40, 0x0); }
	
	std::vector<std::string> Player_Hair{
		"Bald",
		"Fancy Curl",
		"Fancy Swipe",
		"Spiked Ike",
		"High Class Resturant",
		"Guys Night Out",
		"Goody Two Shoes",
		"Basic Bitch",
		"Retard",
		"Thinks he's better than you",
		"Surfing on Mars",
		"Gay little bitch",
		"Douche bag",
		"Punk",
		"Generic nerd character",
		"Buttled Hair",
		"Bedhead",
		"Generic lady in her 40's",
		"Cinderella",
		"Wendy",
		"Bitch that plays w/ chalk",
		"Entitled Cunt",
		"Think they're special",
		"Some fat bitch",
		"Older lady in her 50's",
		"'Innocent' bitch",
		"Slutt",
		"Feminist",
		"Another slutt",
		"Dyke",
		"Super Dyke",
		"Hill Billy",
		"BOTW Zelda",
		"Female Bedhead",
	};
	
	void hairstylepick(MenuEntry *entry)
    {
        u32 playslot = 0;

        Keyboard    keyboard("Choose Hair Style", Player_Hair);
        int         Hairstylechoice = keyboard.Open();

        if (Hairstylechoice < 0)
            return;

        Process::Read32(0xAB014C, playslot);
        Process::Write8(playslot + 4, Hairstylechoice);
    }
	
	std::vector<std::string> Hair_Colors{
		"Dark Brown",
		"Light Brown",
		"Orange",
		"Light Blue",
		"Blonde",
		"Light Green",
		"Pink",
		"White",
		"Black",
		"Auburn",
		"Red",
		"Dark Blue",
		"Gold",
		"Dark Green",
		"Light Purple",
		"Ash Brown",
	};
	
	void    haircolorpick(MenuEntry *entry)
    {
        u32 playslot = 0;
        Keyboard    keyboard("Choose Hair Color", Hair_Colors);
        int         HairColorchoice = keyboard.Open();

        if (HairColorchoice < 0)
            return;

        Process::Read32(0xAB014C, playslot);
        Process::Write8(playslot + 5, HairColorchoice);
    }

	std::vector<std::string> g_Facestyles{
		"Mario Kart/ Shiny",
		"Stretchy Eyes/ Asian",
		"Lazy Eyes/ Blush",
		"Island Beggar/ Lazy",
		"Sk8r/ Mario kart",
		"Smash Bros/ Smash Bros",
		"Blush/ Sk8r",
		"Dot Eyes/ Island Beggar",
		"Asian/ Generic",
		"Neckbeard/ Neckbeard",
		"Lenny/ Lenny",
		"Lol, no one uses this",
    };

    void facestylepick(MenuEntry *entry)
    {
        u32 playslot = 0;

        Keyboard    keyboard("Please pick a face style.", g_Facestyles);
        int         facestylechoice = keyboard.Open();

        if (facestylechoice < 0)
            return;

        Process::Read32(0xAB014C, playslot);
        Process::Write8(playslot + 6, facestylechoice);
    }

	std::vector<std::string> g_EyeColor{
		"Black",
		"Orange",
		"Sea Green",
		"Grey Blue",
		"Soft Berry Blue",
		"Sky Blue 1",
		"Sky Blue 2",
		"Sky Blue 3",
    };

    void	eyecolorpick(MenuEntry *entry)
    {
        u32 playslot = 0;

        Keyboard    keyboard("Choose Eye Color", g_EyeColor);
        int         EyeColorchoice = keyboard.Open();

        if (EyeColorchoice < 0)
            return;

            Process::Read32(0xAB014C, playslot);
            Process::Write8(playslot + 7, EyeColorchoice);
    }
	
	std::vector<std::string> g_RegionList{
		"Country",
		"State",
    };
	
	void tpcregn(MenuEntry *entry)
    {
        u8 regnval;
        u32 playslot;

        Keyboard    keyboard("Choose Country or State", g_RegionList);
        int         RegionChoice = keyboard.Open();

        if(RegionChoice < 0)
            return;

        if(RegionChoice == 0)
        {
            Keyboard    keyboard("Type TPC Country value (Decimal)");
            keyboard.IsHexadecimal(false);
            if (keyboard.Open(regnval) != -1)
            {
                Process::Read32(0xAB014C, playslot);
                Process::Write8(playslot + 0x55D2, regnval);
            }
        }
        else
        {
            Keyboard    keyboard("Type TPC State value (Decimal)");
            keyboard.IsHexadecimal(false);
            if (keyboard.Open(regnval) != -1)
            {
                Process::Read32(0xAB014C, playslot);
                Process::Write8(playslot + 0x55D3, regnval);
            }
        }
    }

    void	menuspeed(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
            Process::Write32(0x56A2C0, 0x7F7FFFFF);

        if(!entry->IsActivated())
            Process::Write32(0x56A2C0, 0x3F800000);
    }

    void	cameramod(MenuEntry *entry)//why did that even matter, lol
    {
        u32 camoffset;
        u16 camvalue;
        float	cameraFloat;
        float	cameraspeed = *GetArg<float>(entry, 6.f);
        u16	cam1 = *GetArg<u16>(entry, 0x96);

        if (cameraspeed == 0.f)
            cameraspeed = 6.f;

        if (cam1 == 0)
            cam1 = 0x96;

        if (entry->Hotkeys[0].IsDown()) //reset camera
        {
            Process::Patch(0x764504, 0x2A000020);
            Process::Patch(0x1A3230, 0xE18020B4);
            Process::Patch(0x1A323C, 0xE18020B4);
        }
        Process::Read32(0x951884, camoffset);
        if (camoffset != 0)
        {
            if (Controller::IsKeyPressed(Key::R))
            {
                Process::Patch(0x1A3230, 0xE1A00000);
                Process::Patch(0x1A323C, 0xE1A00000);
            }
            if (Controller::IsKeysDown(Key::CPadUp + R))
            {
                camoffset += 0x12C;
                Process::Read16(camoffset, camvalue);
                Process::Write16(camoffset, camvalue - cam1);
            }
            if (Controller::IsKeysDown(Key::CPadDown + R))
            {
                camoffset += 0x12C;
                Process::Read16(camoffset, camvalue);
                Process::Write16(camoffset, camvalue + cam1);
                if (camvalue == 0)
                    Process::Write16(camoffset, 0xFFFF + cam1);
            }
            if (Controller::IsKeysDown(Key::CPadRight + R))
            {
                camoffset += 0x12E;
                Process::Read16(camoffset, camvalue);
                Process::Write16(camoffset, camvalue + cam1);
            }
            if (Controller::IsKeysDown(Key::CPadLeft + R))
            {
                camoffset += 0x12E;
                Process::Read16(camoffset, camvalue);
                Process::Write16(camoffset, camvalue - cam1);
                if (camvalue == 0)
                    Process::Write16(camoffset, 0xFFFF - cam1);
            }
        }
        if (entry->Hotkeys[1].IsDown())
        {
            Process::Patch(0x764504, 0xEA000020);
            Process::ReadFloat(0x9866F4, cameraFloat);
            Process::WriteFloat(0x9866F4, cameraFloat - cameraspeed);
        }
        if (entry->Hotkeys[2].IsDown())
        {
            Process::Patch(0x764504, 0xEA000020);
            Process::ReadFloat(0x9866F4, cameraFloat);
            Process::WriteFloat(0x9866F4, cameraFloat + cameraspeed);
        }
        if (entry->Hotkeys[3].IsDown())
        {
            Process::Patch(0x764504, 0xEA000020);
            Process::ReadFloat(0x9866FC, cameraFloat);
            Process::WriteFloat(0x9866FC, cameraFloat + cameraspeed);
        }
        if (entry->Hotkeys[4].IsDown())
        {
            Process::Patch(0x764504, 0xEA000020);
            Process::ReadFloat(0x9866FC, cameraFloat);
            Process::WriteFloat(0x9866FC, cameraFloat - cameraspeed);
        }
        if (entry->Hotkeys[5].IsDown())
        {
            Process::Patch(0x764504, 0xEA000020);
            Process::ReadFloat(0x9866F8, cameraFloat);
            Process::WriteFloat(0x9866F8, cameraFloat + cameraspeed);
        }
        if (entry->Hotkeys[6].IsDown())
        {
            Process::Patch(0x764504, 0xEA000020);
            Process::ReadFloat(0x9866F8, cameraFloat);
            Process::WriteFloat(0x9866F8, cameraFloat - cameraspeed);
        }
    }

	bool tpcoverlay(const Screen &screen)
	{
		if (screen.IsTop)
			{
				screen.Draw("" << Color::Purple << "Taking TPC Picture:", 12, 0);
				screen.Draw("" << Color::DeepSkyBlue << "Please press A when you're", 0, 10);
				screen.Draw("" << Color::DeepSkyBlue << "ready, and wait until the", 0, 20);
				screen.Draw("" << Color::DeepSkyBlue << "shutter sound plays!", 0, 30);
				screen.Draw("" << Color::Red << "ZR Must remain held down", 0, 40);
				screen.Draw("" << Color::Red << "until the photo is taken.", 0, 50);
				screen.DrawRect(154, 50, 90, 6, Color::White); //top bar
				screen.DrawRect(154, 50, 6, 130, Color::White); //left bar
				screen.DrawRect(154, 177, 90, 6, Color::White); //bottom bar
				screen.DrawRect(240, 50, 6, 133, Color::White); //right bar
			}
	}

	void freezeframe(MenuEntry *entry)
    {
		Process::Write32((u32)&pfunction04, 0x304A60);
		
		if (entry->Hotkeys[0].IsDown())
		{
			pfunction04(0x5C, 1, 1, 0);
		}
		if (entry->Hotkeys[1].IsDown())
		{
			OSD::Run(tpcoverlay);
			Process::Write8(0x32916F04, 0xFF);
		}
		if (!entry->Hotkeys[1].IsDown())
		{
			OSD::Stop(tpcoverlay);
		}
    }
	
	void dumpisland(MenuEntry *entry)
    {
        std::string SaveiName;

        Keyboard keyboard("Name the file");
        keyboard.SetMaxLength(30);
        if (keyboard.Open(SaveiName) != -1) {
            Sleep(Seconds(0.1f));
            Directory SaveDir;
            if (Directory::IsExists("/FOXXY/Islands") == 0)
                Directory::Create("/FOXXY/Islands");
            
            File SaveFile;
            File::Open(SaveFile,"/FOXXY/Islands/"+SaveiName+".isl", File::RWC);
            SaveFile.Dump(0x301DC2B2, 0x100A);
            SaveFile.Close();
            MessageBox("Dump complete!")();
        }
    }
	
	void restoreisland(MenuEntry *entry)
    {
        std::vector<std::string> FileList;
        Directory SaveDir;
      
        Directory::Open(SaveDir,"/FOXXY/Islands", true);
        SaveDir.ListFiles(FileList, ".isl");
        Keyboard keyboard("Choose a island file!", FileList);
        int FileChoice = keyboard.Open();

        if(FileChoice < 0)
            return;

        Sleep(Seconds(0.1f));
        File SaveFile;    
        SaveDir.OpenFile(SaveFile, FileList[FileChoice], File::READ);
        SaveFile.Inject(0x301DC2B2, 0x100A);
        MessageBox("Restore complete!")();
    }

	/*void timeTravel(MenuEntry *entry)
	{
		bool timetravelr(const Screen &screen);
		
		if (entry->IsActivated())
		{
			OSD::Run(timetravelr);
			Process::Write32(0x6D3CB4, 0xE3A0002E);
			Process::Write32(0x6578B0, 0xE3A0101A);
		}
		if (!entry->IsActivated())
		{
			OSD::Stop(timetravelr);
			Process::Write32(0x6D3CB4, 0xE3A0004A);
			Process::Write32(0x6578B0, 0xE3A01017);
		}
	}*/
	
	/*void functtest(MenuEntry *entry)
	{
		Process::Write32((u32)&pfunction01, 0x3040EB);
		
		if (entry->Hotkeys[0].IsDown())
		{
			pfunction01(1);
		}
	}*/

	bool MapDraw(const Screen &screen)
	{
		if (*Mapbool == 1)
		{
			if (!screen.IsTop)
			{
				screen.DrawRect(85, 68, 6, 6, Color::Blue);
				screen.DrawRect(120, 68, 6, 6, Color::Blue);
				screen.DrawRect(155, 68, 6, 6, Color::Blue);
				screen.DrawRect(190, 68, 6, 6, Color::Blue);
				screen.DrawRect(225, 68, 6, 6, Color::Blue);
				screen.DrawRect(85, 103, 6, 6, Color::Blue);
				screen.DrawRect(85, 138, 6, 6, Color::Blue);
				screen.DrawRect(85, 173, 6, 6, Color::Blue);
				screen.DrawRect(120, 103, 6, 6, Color::Blue);
				screen.DrawRect(120, 138, 6, 6, Color::Blue);
				screen.DrawRect(120, 173, 6, 6, Color::Blue);
				screen.DrawRect(155, 103, 6, 6, Color::Blue);
				screen.DrawRect(155, 138, 6, 6, Color::Blue);
				screen.DrawRect(155, 173, 6, 6, Color::Blue);
				screen.DrawRect(190, 103, 6, 6, Color::Blue);
				screen.DrawRect(190, 138, 6, 6, Color::Blue);
				screen.DrawRect(190, 173, 6, 6, Color::Blue);
				screen.DrawRect(225, 103, 6, 6, Color::Blue);
				screen.DrawRect(225, 138, 6, 6, Color::Blue);
				screen.DrawRect(225, 173, 6, 6, Color::Blue);
				screen.DrawSysfont("01", 79, 52, Color::Blue);
				screen.DrawSysfont("02", 114, 52, Color::Blue);
				screen.DrawSysfont("03", 149, 52, Color::Blue);
				screen.DrawSysfont("04", 184, 52, Color::Blue);
				screen.DrawSysfont("05", 219, 52, Color::Blue);
				screen.DrawSysfont("06", 79, 87, Color::Blue);
				screen.DrawSysfont("07", 114, 87, Color::Blue);
				screen.DrawSysfont("08", 149, 87, Color::Blue);
				screen.DrawSysfont("09", 184, 87, Color::Blue);
				screen.DrawSysfont("10", 215, 87, Color::Blue);
				screen.DrawSysfont("11", 73, 122, Color::Blue);
				screen.DrawSysfont("12", 108, 122, Color::Blue);
				screen.DrawSysfont("13", 143, 122, Color::Blue);
				screen.DrawSysfont("14", 178, 122, Color::Blue);
				screen.DrawSysfont("15", 215, 122, Color::Blue);
				screen.DrawSysfont("16", 73, 157, Color::Blue);
				screen.DrawSysfont("17", 108, 157, Color::Blue);
				screen.DrawSysfont("18", 143, 157, Color::Blue);
				screen.DrawSysfont("19", 178, 157, Color::Blue);
				screen.DrawSysfont("20", 215, 157, Color::Blue);
			}
		}
	}

	void MapEditor(MenuEntry *entry)
	{
		Process::Write32((u32)&pfunction04, 0x304A60);
		UIntVector Position = Touch::GetPosition();
		u8 AcreID;

		if (*Mapbool == 1)
		{
			if (Touch::IsDown())
			{
				switch (*(u8 *)room)
				{
				case 0:
					if (Position.x >= 83 && Position.x < 89 && Position.y >= 67 && Position.y < 73)
					{
						Keyboard Acre1("Type an ID for Acre 1.");
						if (Acre1.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A414, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 1 Modified!");
							OSD::Notify("Acre 1 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 118 && Position.x < 124 && Position.y >= 67 && Position.y < 73)
					{
						Keyboard Acre2("Type an ID for Acre 2.");
						if (Acre2.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A416, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 2 Modified!");
							OSD::Notify("Acre 2 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 153 && Position.x < 159 && Position.y >= 67 && Position.y < 73)
					{
						Keyboard Acre3("Type an ID for Acre 3.");
						if (Acre3.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A418, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 3 Modified!");
							OSD::Notify("Acre 3 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 188 && Position.x < 194 && Position.y >= 67 && Position.y < 73)
					{
						Keyboard Acre4("Type an ID for Acre 4.");
						if (Acre4.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A41A, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 4 Modified!");
							OSD::Notify("Acre 4 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 223 && Position.x < 229 && Position.y >= 67 && Position.y < 73)
					{
						Keyboard Acre5("Type an ID for Acre 5.");
						if (Acre5.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A41C, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 5 Modified!");
							OSD::Notify("Acre 5 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 83 && Position.x < 89 && Position.y >= 102 && Position.y < 108)
					{
						Keyboard Acre6("Type an ID for Acre 6.");
						if (Acre6.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A422, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 6 Modified!");
							OSD::Notify("Acre 6 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 118 && Position.x < 124 && Position.y >= 102 && Position.y < 108)
					{
						Keyboard Acre7("Type an ID for Acre 7.");
						if (Acre7.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A424, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 7 Modified!");
							OSD::Notify("Acre 7 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 153 && Position.x < 159 && Position.y >= 102 && Position.y < 108)
					{
						Keyboard Acre8("Type an ID for Acre 8.");
						if (Acre8.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A426, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 8 Modified!");
							OSD::Notify("Acre 8 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 188 && Position.x < 194 && Position.y >= 102 && Position.y < 108)
					{
						Keyboard Acre9("Type an ID for Acre 9.");
						if (Acre9.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A428, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 9 Modified!");
							OSD::Notify("Acre 9 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 223 && Position.x < 229 && Position.y >= 102 && Position.y < 108)
					{
						Keyboard Acre10("Type an ID for Acre 10.");
						if (Acre10.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A42A, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 10 Modified!");
							OSD::Notify("Acre 10 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 83 && Position.x < 89 && Position.y >= 137 && Position.y < 143)
					{
						Keyboard Acre11("Type an ID for Acre 11.");
						if (Acre11.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A430, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 11 Modified!");
							OSD::Notify("Acre 11 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 118 && Position.x < 124 && Position.y >= 137 && Position.y < 143)
					{
						Keyboard Acre12("Type an ID for Acre 12.");
						if (Acre12.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A432, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 12 Modified!");
							OSD::Notify("Acre 12 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 153 && Position.x < 159 && Position.y >= 137 && Position.y < 143)
					{
						Keyboard Acre13("Type an ID for Acre 13.");
						if (Acre13.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A434, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 13 Modified!");
							OSD::Notify("Acre 13 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 188 && Position.x < 194 && Position.y >= 137 && Position.y < 143)
					{
						Keyboard Acre14("Type an ID for Acre 14.");
						if (Acre14.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A436, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 14 Modified!");
							OSD::Notify("Acre 14 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 223 && Position.x < 229 && Position.y >= 137 && Position.y < 143)
					{
						Keyboard Acre15("Type an ID for Acre 15.");
						if (Acre15.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A438, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 15 Modified!");
							OSD::Notify("Acre 15 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 83 && Position.x < 89 && Position.y >= 172 && Position.y < 178)
					{
						Keyboard Acre16("Type an ID for Acre 16.");
						if (Acre16.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A43E, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 16 Modified!");
							OSD::Notify("Acre 16 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 118 && Position.x < 124 && Position.y >= 172 && Position.y < 178)
					{
						Keyboard Acre17("Type an ID for Acre 17.");
						if (Acre17.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A440, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 17 Modified!");
							OSD::Notify("Acre 17 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 153 && Position.x < 159 && Position.y >= 172 && Position.y < 178)
					{
						Keyboard Acre18("Type an ID for Acre 18.");
						if (Acre18.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A442, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 18 Modified!");
							OSD::Notify("Acre 18 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 188 && Position.x < 194 && Position.y >= 172 && Position.y < 178)
					{
						Keyboard Acre19("Type an ID for Acre 19.");
						if (Acre19.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A444, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 19 Modified!");
							OSD::Notify("Acre 19 : " << Hex(AcreID));
						}
					}
					if (Position.x >= 223 && Position.x < 229 && Position.y >= 172 && Position.y < 178)
					{
						Keyboard Acre20("Type an ID for Acre 20.");
						if (Acre20.Open(AcreID, AcreID) != -1)
						{
							Process::Write8(0x31F7A446, AcreID);
							pfunction04(0, 1, 1, 0);
							OSD::Notify("Acre 20 Modified!");
							OSD::Notify("Acre 20 : " << Hex(AcreID));
						}
					}
				//case 68:
				}
			}
			if (entry->IsActivated())
			{
				if (*ActualRoom == 0)
					OSD::Run(MapDraw);
				if (*ActualRoom != 0)
					OSD::Stop(MapDraw);
			}
			else if (!entry->IsActivated())
			{
				OSD::Stop(MapDraw);
			}
		}
	}
	
	void spamfriendrequests(MenuEntry *entry) //do we really need this?
	{
		u32 frstatus;
		u32 frstatus2;
		u8 donothing;
		
		if (*ActualRoom == 103)
		{
			if (Process::Read32(0x31FE5CB8, frstatus) && frstatus != 0x00000004)
            {
                Process::Write32(0x31FE5CB8, 0x00000004);
            }
            if (Process::Read32(0x31FE5CBC, frstatus2) && frstatus2 != 0x00000000)
            {
                Process::Write32(0x31FE5CBC, 0x00000000);
            }
		}
		if (*ActualRoom == 104)
		{
			if (Process::Read32(0x31FE5CB8, frstatus) && frstatus != 0x00000004)
            {
                Process::Write32(0x31FE5CB8, 0x00000004);
            }
            if (Process::Read32(0x31FE5CBC, frstatus2) && frstatus2 != 0x00000000)
            {
                Process::Write32(0x31FE5CBC, 0x00000000);
            }
		}
		else if (*ActualRoom != 103) //stupid lazy way to make sure this only works on the island. hoping to prevent any issues.
		{
			Process::Write8(0x9DEDEE, 0x00);
		}
		else if (*ActualRoom != 104)
		{
			Process::Write8(0x9DEDEE, 0x00);
		}
	}
	
	void onlineplayermod(MenuEntry *entry)
        {
        u32 vival;
		static bool Message = false;

        if (entry->Hotkeys[0].IsDown() && !Message)
        {
			if (Process::Read32(0x67743C, vival) && vival == 0xE1A07002)
            {
				Process::Write32(0x655E44, 0xE3A01017);
                Process::Write32(0x67743C, 0xE3A07006);
                Process::Write32(0x68DC3C, 0xE1A00000);
                OSD::Notify("Visibility: Stationary", Color::Blue);
				Message = true;
            }
            else if (vival == 0xE3A07006)
            {
				Process::Write32(0x655E44, 0xE3A01016);
                Process::Write32(0x67743C, 0xE3A07000);
                Process::Write32(0x68DC3C, 0x1BFF021A);
                OSD::Notify("Visibility: Invisible", Color::Yellow);
				Message = true;
            }
			else if (vival == 0xE3A07000)
            {
				Process::Write32(0x655E44, 0xE3A01017);
                Process::Write32(0x67743C, 0xE1A07002);
                Process::Write32(0x68DC3C, 0x1BFF021A);
                OSD::Notify("Visibility: Default", Color::Green);
				Message = true;
            }
        }
		
		if (!entry->Hotkeys[0].IsDown())
			Message = false;
    }

	bool widget(const Screen &screen)
	{
		u16 isloading;
		u8 hour;
		u8 minute;
		u8 month;
		u8 day;
		
		Process::Read8(0x9515BB, hour);
		Process::Read8(0x9515BA, minute);
		Process::Read8(0x98B06D, day);
		Process::Read8(0x98B06C, month);
		Process::Read16(0x9515BA, isloading);
		
		if (isloading != 0xFFFF)
		{
			if (screen.IsTop)
			{
				screen.DrawSysfont(std::to_string(hour),  6, 225, Color::White);
				screen.DrawSysfont(" : ", 23, 224, Color::White);
				screen.DrawSysfont(std::to_string(minute),  32, 225, Color::White);
				screen.DrawSysfont(std::to_string(month),  6, 214, Color::White);
				screen.DrawSysfont("/", 23, 213, Color::White);
				screen.DrawSysfont(std::to_string(day),  30, 214, Color::White);
			
			}
		}
		else if (isloading == 0xFFFF)
		{
			if (screen.IsTop)
			{
				screen.DrawSysfont("Loading clock, Please wait...", 6, 225, Color::Yellow);
			}
		}
	}
	
	bool timetravelr(const Screen &screen)
	{
		u8 isonline;
		u32 x, y;
		UIntVector Position = Touch::GetPosition();

		Process::Read8(0x31FF59C9, isonline);
		
		if (*Mapbool == 1) //OSD Coding for touch screen. please jump to the next instruction for animation DATA.
		{
			if (isonline == 0x00)
			{
				if (!screen.IsTop)
				{
					screen.DrawRect(7, 35, 48, 22, Color::Orange);
					screen.DrawSysfont("Time", 10, 39, Color::White);
				}
			}
			else if (isonline == 0x01 | 0x02)
			{
				if (!screen.IsTop)
				{
					screen.DrawRect(7, 35, 48, 22, Color::Orange);
					screen.DrawSysfont("Time", 10, 39, Color::Red);
				}
			}
			if (Touch::IsDown()) //actual time travel data
			{
				if (*Mapbool == 1)
				{
					if (isonline == 0x00)
					{
						if (Position.x >= 7 && Position.x < 57 && Position.y >= 34 && Position.y < 54)
						{	
							if (Player::GetWorldCoords(&x, &y, 4, 1))
							{
								Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x3F, 0, 1, 1, 1, 0, x, y, true);
							}
						}
					}
				}
			}
		}
	}
	
	bool balloon(const Screen &screen)
	{
		u32 x, y;
		u16 balloon = Utils::Random(0x336C, 0x3383);
		UIntVector Position = Touch::GetPosition();
		
		if (*Mapbool == 1) //OSD Coding for touch screen. please jump to the next instruction for animation DATA.
		{
			if (!screen.IsTop)
			{
				screen.DrawRect(7, 64, 48, 22, Color::Green);
				screen.DrawSysfont("Trip", 10, 68, Color::White);
			}
		}

		if (Touch::IsDown()) //actual time travel data
		{
			if (*Mapbool == 1)
			{
				if (Position.x >= 7 && Position.x < 55 && Position.y >= 64 && Position.y < 85)
				{	
					if (Player::GetWorldCoords(&x, &y, 4, 1))
					{
						Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x38, balloon, 1, 1, 1, 0, x, y, true);
						Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x9F, 0, 1, 1, 1, 0, x, y, true);
					}
				}
			}
		}
	}
	
	bool Reload(const Screen &screen)
	{
		u32 x, y;
		u16 animSnake = Utils::Random(0x000D, 0x01FF);
		UIntVector Position = Touch::GetPosition();
		
		if (*Mapbool == 1) //OSD Coding for touch screen. please jump to the next instruction for animation DATA.
		{
			if (!screen.IsTop)
			{
				screen.DrawRect(7, 93, 48, 22, Color::Yellow);
				screen.DrawSysfont("Anim", 10, 97, Color::White);
			}
		}

		if (Touch::IsDown()) //actual time travel data
		{
			if (*Mapbool == 1)
			{
				if (Position.x >= 7 && Position.x < 55 && Position.y >= 93 && Position.y < 113)
				{	
					if (Player::GetWorldCoords(&x, &y, 4, 1))
					{
						for (u8 i = 0; i < 4; i++)
						{
							Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0xC4, animItem, emotion, animSnake, soundID, 0, x, y, true);
						}
					}
				}
			}
		}
	}
	
	bool catchbug(const Screen &screen)
	{
		u16 bug = Utils::Random(0x228E, 0x22D5);
		u32 x, y;
		UIntVector Position = Touch::GetPosition();
		
		if (*Mapbool == 1) //OSD Coding for touch screen. please jump to the next instruction for animation DATA.
		{
			if (!screen.IsTop)
			{
				screen.DrawRect(7, 125, 48, 22, Color::Blue);
				screen.DrawSysfont("Bug", 10, 129, Color::White);
			}
		}

		if (Touch::IsDown()) //actual time travel data
		{
			if (*Mapbool == 1)
			{
				if (Position.x >= 7 && Position.x < 55 && Position.y >= 125 && Position.y < 145)
				{	
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x71, bug, 1, 1, 1, 0, x, y, true);
				}
			}
		}
	}
	
	bool catchfish(const Screen &screen)
	{
		u16 fish = Utils::Random(0x22E1, 0x2328);
		u32 x, y;
		UIntVector Position = Touch::GetPosition();
		
		if (*Mapbool == 1) //OSD Coding for touch screen. please jump to the next instruction for animation DATA.
		{
			if (!screen.IsTop)
			{
				screen.DrawRect(7, 154, 48, 22, Color::Purple);
				screen.DrawSysfont("Fish", 10, 158, Color::White);
			}
		}

		if (Touch::IsDown()) //actual time travel data
		{
			if (*Mapbool == 1)
			{
				if (Position.x >= 7 && Position.x < 55 && Position.y >= 154 && Position.y < 174)
				{	
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0xB7, fish, 1, 1, 1, 0, x, y, true);
				}
			}
		}
	}
	
	bool catchmarine(const Screen &screen)
	{
		u16 Marine = Utils::Random(0x232D, 0x234A);
		u32 x, y;
		UIntVector Position = Touch::GetPosition();
		
		if (*Mapbool == 1) //OSD Coding for touch screen. please jump to the next instruction for animation DATA.
		{
			if (!screen.IsTop)
			{
				screen.DrawRect(7, 183, 48, 22, Color::SkyBlue);
				screen.DrawSysfont("Sea", 10, 187, Color::White);
			}
		}
		if (Touch::IsDown()) //actual time travel data
		{
			if (*Mapbool == 1)
			{
				if (Position.x >= 7 && Position.x < 55 && Position.y >= 183 && Position.y < 203)
				{	
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x24, Marine, 1, 1, 1, 0, x, y, true);
				}
			}
		}
	}
	
	bool crasheveryone(const Screen &screen)
	{
		u32 x, y;
		u16 roomcheck;
		UIntVector Position = Touch::GetPosition();
		
		Process::Read16(0x33077504, roomcheck);

		if (!screen.IsTop)
		{
			screen.DrawRect(7, 212, 48, 22, Color::Orange);
			screen.DrawSysfont("Crash", 10, 216, Color::Red);
		}
		if (Touch::IsDown()) //actual time travel data
		{
			if (Position.x >= 7 && Position.x < 55 && Position.y >= 212 && Position.y < 232)
			{
				if (roomcheck == 0xFFFF)
				{
					for (u8 i = 0; i < 4; i++)
					{
						Player::ExecuteAnimationWrapper(i, 0x41, 0x2117, emotion, snake, soundID, 0, x, y, true);
					}
					{
						Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x6, 0, 1, 1, 1, 0, x, y, true);
					}
				}
				else if (roomcheck != 0xFFFF)
				{
					for (u8 i = 0; i < 4; i++)
					{
						Player::ExecuteAnimationWrapper(i, 0x5F, 0x2117, emotion, snake, soundID, 0, x, y, true);
					}
					{
						Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x6, 0, 1, 1, 1, 0, x, y, true);
					}
				}
			}
		}
	}
	
	void instantbuttons(MenuEntry *entry)
	{
		bool timetravelr(const Screen &screen);
		u8 roomloc;
		
		Process::Read8(0x9513D3, roomloc);
		
		if (entry->Hotkeys[0].IsDown())
		{
			OSD::Run(crasheveryone);
		}
		else if (roomloc != 0)
		{
			OSD::Stop(crasheveryone);
		}
		
		if (entry->IsActivated())
		{
			OSD::Run(balloon);
			OSD::Run(Reload);
			OSD::Run(timetravelr);
			OSD::Run(catchbug);
			OSD::Run(catchfish);
			OSD::Run(catchmarine);
			Process::Write32(0x6D3CB4, 0xE3A0002E);
			Process::Write32(0x6578B0, 0xE3A0101A);
			Process::Write32(0x68B57C, 0xE3A01006);
			Process::Write32(0x671F50, 0xE3A01006);
		}
		if (!entry->IsActivated())
		{
			OSD::Stop(balloon);
			OSD::Stop(Reload);
			OSD::Stop(timetravelr);
			OSD::Stop(catchbug);
			OSD::Stop(catchfish);
			OSD::Stop(catchmarine);
			Process::Write32(0x6D3CB4, 0xE3A0004A);
			Process::Write32(0x6578B0, 0xE3A01017);
			Process::Write32(0x68B57C, 0xE3A01006);
			Process::Write32(0x671F50, 0xE3A01042);
		}
	}

	void animmod(MenuEntry *entry)
    {   
		u16 randy = Utils::Random(0x22E1, 0x234B);
		u16 fish1;
		u16 fish2;
		
        if (entry->Hotkeys[0].IsDown()) //to set anim id
        {
            static u8 animID = 0;
            Keyboard    keyboard("Type an Animation ID");
            keyboard.IsHexadecimal(true);
            if (keyboard.Open(animID, animID) != -1)
            {
                if (animID != 0)
                {
                    Process::Patch(0x67CB98, 0xE3A01000 + animID);
                    Process::Patch(0x67CB84, 0xE1A00000);
                    Process::Patch(0x67C9E4, 0xE1A00000);
                }
                else
                {
                    Process::Patch(0x67CB98, 0xE3A0101E);
                    Process::Patch(0x67CB84, 0x1A00003C);
                    Process::Patch(0x67C9E4, 0x0A000071);
                }
            }
        }
		if (entry->Hotkeys[1].IsDown()) //to set seconday anim id
        {
            static u8 animID2 = 0;
            Keyboard    keyboard("Type an Animation ID");
            keyboard.IsHexadecimal(true);
            if (keyboard.Open(animID2, animID2) != -1)
            {
                if (animID2 != 0)
                {
                    Process::Patch(0x683FA8, 0xE3A01000 + animID2);
                    Process::Patch(0x683F28, 0xE1A00000);
					Process::Patch(0x683F70, 0xE1A00000);
					Process::Patch(0x683F78, 0xE1A00000);
					Process::Patch(0x683F94, 0xE1A00000);
                }
                else
                {
                    Process::Patch(0x683FA8, 0xE3A01070);
                    Process::Patch(0x683F28, 0x1A000008);
					Process::Patch(0x683F70, 0x0A000026);
					Process::Patch(0x683F78, 0x1A000015);
					Process::Patch(0x683F94, 0x1A000008);
                }
            }
        }
		if (Controller::IsKeysPressed(Key::A))
		{
			if (0x33077C84 == 80000000)
			{
				Process::Write32(0x33077C84, 0x00002117);
			}
		}
        if (entry->Hotkeys[2].IsDown())
        {
            {
                    Process::Write16(0x33077C86, randy);
            }
        }
		if (entry->Hotkeys[3].IsDown())
		{
			Process::Read16(0x3200C20A, fish1);
			Process::Read16(0x3200C208, fish2);
			Process::Write16(0x33077C8C, fish1);
			Process::Write16(0x33077C8A, fish2);
		}
		if (entry->Hotkeys[4].IsDown())
		{
			Process::Read16(0xAAE9CE, fish1);
			Process::Read16(0xAAE9CC, fish2);
			Process::Write16(0x33077C8C, fish1);
			Process::Write16(0x33077C8A, fish2);
		}
		if (entry->Hotkeys[5].IsDown())
		{
			Process::Read16(0xAAE9F4, fish1);
			Process::Read16(0xAAE9F2, fish2);
			Process::Write16(0x33077C8C, fish1);
			Process::Write16(0x33077C8A, fish2);
		}
		if (entry->Hotkeys[6].IsDown())
		{
			Process::Read16(0xAAEA1A, fish1);
			Process::Read16(0xAAEA18, fish2);
			Process::Write16(0x33077C8C, fish1);
			Process::Write16(0x33077C8A, fish2);
		}
    }

	/*void cng_item(MenuEntry *entry)//Item ID setter for items held by player, e.g. Eating
    {
        u16 randy = Utils::Random(0x22E1, 0x234B);
		u16 fish1;
		u16 fish2;

		if (Controller::IsKeysPressed(Key::A))
		{
			if (0x33077C84 == 80000000)
			{
				Process::Write32(0x33077C84, 0x00002117);
			}
		}
        if (entry->Hotkeys[0].IsDown())
        {
            {
                    Process::Write16(0x33077C86, randy);
            }
        }
		if (entry->Hotkeys[1].IsDown())
		{
			Process::Read16(0x3200C20A, fish1);
			Process::Read16(0x3200C208, fish2);
			Process::Write16(0x33077C8C, fish1);
			Process::Write16(0x33077C8A, fish2);
		}
		if (entry->Hotkeys[2].IsDown())
		{
			Process::Read16(0xAAE9CE, fish1);
			Process::Read16(0xAAE9CC, fish2);
			Process::Write16(0x33077C8C, fish1);
			Process::Write16(0x33077C8A, fish2);
		}
		if (entry->Hotkeys[3].IsDown())
		{
			Process::Read16(0xAAE9F4, fish1);
			Process::Read16(0xAAE9F2, fish2);
			Process::Write16(0x33077C8C, fish1);
			Process::Write16(0x33077C8A, fish2);
		}
		if (entry->Hotkeys[4].IsDown())
		{
			Process::Read16(0xAAEA1A, fish1);
			Process::Read16(0xAAEA18, fish2);
			Process::Write16(0x33077C8C, fish1);
			Process::Write16(0x33077C8A, fish2);
		}
    }*/
	
	void forcelag(MenuEntry *entry)
	{
		u32 lagon;
		u32 lagoff;
		u8 lagms;
		static bool Message = false;
		
		if (entry->Hotkeys[0].IsDown())
		{
            Keyboard    keyboard("Enter lag timer: \nDon't lag it for more than 50 Seconds. \nThe shorter the lag time, the less chance you have of it erroring.");
            keyboard.IsHexadecimal(true);
			if (keyboard.Open(lagms, lagms) == -1)
            return;
			Process::Read32(0x31FF599C, lagon);
			Process::Write32(0x9EA000, lagon);
			OSD::Notify("Starting Network Lagger!", Color::Green);
			Message = true;
			Sleep(Seconds(1.30));
			Process::Write32(0x31FF599C, 0x00000000);
			OSD::Notify("Lagging Session!", Color::Blue);
			Message = true;
			Sleep(Seconds(lagms));
			Process::Read32(0x9EA000, lagoff);
			Process::Write32(0x31FF599C, lagoff);
			OSD::Notify("Lagging Session Over!", Color::Red);
			Message = true;
        }
		if (entry->Hotkeys[1].IsDown())
		{
			Process::Read32(0x31FF599C, lagon);
			Process::Write32(0x9EA000, lagon);
			OSD::Notify("Dropping Lagspike!", Color::Blue);
			Message = true;
			Sleep(Seconds(0.08));
			Process::Write32(0x31FF599C, 0x00000000);
			Sleep(Seconds(0.12));
			Process::Read32(0x9EA000, lagoff);
			Process::Write32(0x31FF599C, lagoff);
			OSD::Notify("Done!", Color::Blue);
			Message = true;
		}
		
		if (!entry->Hotkeys[0].IsDown())
			Message = false;
	}
	
	void maxturbo(MenuEntry *entry)
	{
        Sleep(Seconds(0.0085f));
        Process::Write8(0x32922CA4, 0); //abxy
        Process::Write8(0x32922CA6, 0); //dpad
    }
	
	void TouchCoord(MenuEntry *entry)
    {
        u8 room;

		UIntVector Position = Touch::GetPosition();
        UIntVector  touchCoord = Touch::GetPosition();

		if (Touch::IsDown())
        {
			if (Position.x >= 67 && Position.x < 250 && Position.y >= 30 && Position.y < 210)
			{
				u8 Mapped;
				Process::Read8(0x950C30, Mapped);
				if (Mapped == 1)
				{
					Process::Read8(0x958342, room);
					if(room == 0)
					{
						FloatVector  fPos(touchCoord);
						float xCoord;
						float zCoord;
	
						xCoord = (fPos.x - 77.f) * 14.94f + 527.f;
						zCoord = (fPos.y - 50.f) * 15.21f + 527.f;
						Process::WriteFloat(PlayerHelper::GetInstance()->OffsetReturn() + 0x14, xCoord);
						Process::WriteFloat(PlayerHelper::GetInstance()->OffsetReturn() + 0x1C, zCoord);
					}
				}
			}
		}
		else if (!Touch::IsDown())
		{
			Process::Write8(0x9E0015, 0x01);
		}
	}
	
	void inf_expression(MenuEntry *entry)
    {
        if (entry->Hotkeys[0].IsDown())
		{
			Process::Write32(0x65E9B0, 0xE3A010FF);
		}
		if (!entry->Hotkeys[0].IsDown())
		{
			Process::Write32(0x65E9B0, 0xE1D010B0);
		}
	}

	void hostDrop(MenuEntry *entry)
	{
		optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case 0:
			//Process::Write32(0x5A03F4, 0xE1A00000);
			Process::Write32(0x5A1454, 0xE1A00000); //create locked spot pkt send
			Process::Write32(0x5A12F4, 0xE1A00000); //clear locked spot pkt send
			Process::Write32(0x59FF5C, 0xEA000010); //pkt is from host
			Process::Write32(0x59FD98, 0xEA000003); //bypass check in 0x59FC7C
			Process::Write32(0x5983F4, 0xE1A00000); //unnecessary?
			Process::Write32(0x5984DC, 0xE1A00000); //unnecessary?
			//Process::Write32(0x5A2FC4, 0xE1A00000); //free pkt, may cause locked spots?
			Process::Write32(0x59F8A0, 0xE1A00000);
			Process::Write32(0x5A09B0, 0xEA000022); //item tree drop
			Process::Write32(0x5A0DF4, 0xEA00002D); //fruit tree drop
			break;
		case 1:
			//Process::Write32(0x5A03F4, 0x0A00000B);
			Process::Write32(0x5A1454, 0x0A00002C);
			Process::Write32(0x5A12F4, 0x0A00002C);
			Process::Write32(0x59FF5C, 0x0A000010);
			Process::Write32(0x59FD98, 0x0A000003);
			Process::Write32(0x5983F4, 0x0A00001D);
			Process::Write32(0x5984DC, 0x0A000025);
			Process::Write32(0x5A2FC4, 0x0A00000B);
			Process::Write32(0x59F8A0, 0x0A00003B);
			Process::Write32(0x5A09B0, 0x0A000022);
			Process::Write32(0x5A0DF4, 0x0A00002D);
			break;
		default:
			break;
		}
	}

    void	nocrash(MenuEntry *entry)
    {
        optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case 0:
            Process::Patch(0x5980F8, 0xE3A00001);
			Process::Patch(0x5A26B4, 0xE1A00000);
			Process::Patch(0x5A26F4, 0xE3A00001);
			Process::Patch(0x6517CC, 0x03500001);
			Process::Patch(0x6517E4, 0xE1A00000);
			Process::Patch(0x651A9C, 0x01000540);
			Process::Patch(0x665E50, 0xE3A01006);
			Process::Patch(0x68B57C, 0xE3A01016);
			break;
		case 1:
            Process::Patch(0x5980F8, 0xE3500000);
			Process::Patch(0x5A26B4, 0xE3500000);
			Process::Patch(0x5A26F4, 0xE5D4200F);
			Process::Patch(0x6517CC, 0x13500001);
			Process::Patch(0x6517E4, 0xE3500000);
			Process::Patch(0x651A9C, 0x0100054E);
			Process::Patch(0x665E50, 0xE3A0105F);
			Process::Patch(0x68B57C, 0xE3A0103E);
			break;
		default:
			break;
		}
    }

	void bypass(MenuEntry *entry)
	{
		optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case 0:
			Process::Write32(0x5A11C8, 0xE3E00000);
			Process::Write32(0x5A11CC, 0xEA000012);
			Process::Write32(0x5A13C8, 0xE3A00000);
			Process::Write32(0x5A13CC, 0xE8BD83F0);
			bypassing = true;
			break;
		case 1:
			Process::Write32(0x5A11C8, 0xE1A05001);
			Process::Write32(0x5A11CC, 0x1A000001);
			Process::Write32(0x5A13C8, 0xE24DD01C);
			Process::Write32(0x5A13CC, 0xE1A07001);
			bypassing = false;
			break;
		default:
			break;
		}

	}

	/*void swap(MenuEntry *entry) 
	{
		u32 ppObjA, ppObjB, temp;
		ppObjA = GameHelper::GetOnlinePlayerIndex() * 4 + 0xAAE994;
		optKb->Populate(playerOpt);
		ppObjB = optKb->Open();
		if (ppObjB != -1)
		{
			ppObjB = ppObjB * 4 + 0xAAE994;
			temp = *(u32 *)ppObjA;
			*(u32 *)ppObjA = *(u32 *)ppObjB;
			*(u32 *)ppObjB = temp;
		}
	}*/

	void countrySpoof(MenuEntry *entry)
	{
		u8 val;

		SetUpKB("Enter dec country ID\n(255 to restore original):\nUSA: 49\nFrance: 77", false, 2, val, 0);
		if (val == 0xFF)
		{
			Process::Write32(0x350B18, 0xE1A00C20);
		}
		else
		{
			Process::Write32(0x350B18, 0xE3A00000 + val);
		}
	}

	void soundSpeed(MenuEntry *entry)
	{
		u32 SSP;
		if (SetUpKB("Enter Float! \n Default: 3F800000", true, 8, SSP, *(u32 *)0x4C4B4C))
		{
			Process::Write32(0x4C4B4C, SSP);
		}
	}

	void fovMod(MenuEntry *entry)
	{
		u32 FOV;
		if (SetUpKB("Enter Float! \n Default: 3F800000", true, 8, FOV, *(u32 *)0x47E48C))
		{
			Process::Write32(0x47E48C, FOV);
		}
	}
	
	void panadj(MenuEntry *entry)
	{
		u32 PANSPEED;
		if (SetUpKB("Enter Float! \n Default: 3F800000", true, 8, PANSPEED, *(u32 *)0x1A5430))
		{
			Process::Write32(0x1A5430, PANSPEED);
		}
	}

	void cameraUp(MenuEntry *entry) 
	{ 
		u8 *ptr = Player::GetAnimation();
		if (ptr != nullptr && *ptr == 6) Process::Write32(Camera::GetInstance() + 0xF0, 0x500); 
	}
	
	void noshot(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
            Process::Patch(0x5B41A8, 0xE3A00001);

        if (!entry->IsActivated())
            Process::Patch(0x5B41A8, 0xE3500000);
    }

	//void menuTracker(MenuEntry *entry) { SetUpKB("Enter addr:", true, 8, addr, addr); }
	
	//void freezer(MenuEntry *entry) { Process::Write32(freezeAddr, freezeVal); }
	//movement
	void coordMod(MenuEntry *entry)
	{
		volatile float *pCoords = Player::GetCoordinates();
		if (pCoords != nullptr && !selecting) // if not in tile selection mode & valid player obj
		{
			if (Controller::IsKeysDown(Key::CStickRight)) *pCoords += coordSpeed;
			if (Controller::IsKeysDown(Key::CStickLeft)) *pCoords -= coordSpeed;
			if (Controller::IsKeysDown(Key::CStickDown)) *((float *)((vu32)pCoords + 8)) += coordSpeed;
			if (Controller::IsKeysDown(Key::CStickUp)) *((float *)((vu32)pCoords + 8)) -= coordSpeed;
		}
	}

	void menuCoordMod(MenuEntry *entry)
	{
		kb->GetMessage() = "Enter speed:";
		kb->IsHexadecimal(false);
		kb->SetMaxLength(4);
		kb->Open(coordSpeed);
	}

	void speedMod(MenuEntry *entry)
	{
		Process::WriteFloat(0x887880, walkSpeed);
		Process::WriteFloat(0x887888, walkSpeed); 
		Process::WriteFloat(0x887958, walkSpeed);
		Process::WriteFloat(0x5D4C80, 1.8f * walkSpeed * 4096.0f); //swim speed
		Process::Write16(0x8879B8, 0x16BC); //fast 0x20 rotate
		Process::Write16(0x887C68, 0xB5E); //fast 0x1F rotate
		Process::Write16(0x94EF34, 0xB5E); //fast 0x1F rotate
		Process::Write8(0x8878A4, 0x14 * walkSpeed); //slide
	}

	void menuSpeedMod(MenuEntry *entry)
	{
		kb->GetMessage() = "Enter speed:";
		kb->IsHexadecimal(false);
		kb->SetMaxLength(4);
		kb->Open(walkSpeed);
	}

	void wtw(MenuEntry *entry)
	{
		u8 val;

		if (Controller::IsKeysPressed(Key::L + Key::DPadUp))
		{
			if (Process::Read8(0x6503FF, val) && val == 0x0A)
			{
				Process::Write8(0x6503FF, 0xEA);
				Process::Write8(0x650417, 0xEA);
				Process::Write8(0x65057B, 0xEA);
				Process::Write8(0x6505F3, 0xEA);
				Process::Write32(0x6506A4, 0xE1A00000);
				Process::Write32(0x6506BC, 0xE1A00000);
				Process::Write8(0x6506C3, 0xEA);
				Process::Write8(0x6506EF, 0xEA);
				OSD::Notify("Walk over objects: Enabled!", Color::Green);
			}
			else if (val == 0xEA)
			{
				Process::Write8(0x6503FF, 0x0A);
				Process::Write8(0x650417, 0x0A);
				Process::Write8(0x65057B, 0x0A);
				Process::Write8(0x6505F3, 0xDA);
				Process::Write32(0x6506A4, 0xED841A05);
				Process::Write32(0x6506BC, 0xED840A07);
				Process::Write8(0x6506C3, 0x0A);
				Process::Write8(0x6506EF, 0x0A);
				OSD::Notify("Walk over objects: Disabled!", Color::Red);
			}
		}
	}

	void moonjump(MenuEntry *entry)
	{
		if (entry->Hotkeys[0].IsDown())
		{
			Process::Write32(0x33077C82, 0x007FFFFF);
			Process::Write32(0x33077DAE, 0x007FFFFF);
		}
		if (entry->Hotkeys[1].IsDown())
		{
			Process::Write32(0x33077C82, 0x00019D5D);
			Process::Write32(0x33077DAE, 0x00019D5D);
		}
	}
	
	void forceerror(MenuEntry *entry)
	{
		if (entry->Hotkeys[0].IsDown())
		{
			Process::Write32(0x31FF599C, 0xFEDFEDFF);
		}
	}

	void playerColMod(MenuEntry *entry)
	{
		volatile float *pCol = Player::GetCollisionSize(4);
		if (pCol != nullptr) // if valid player obj
		{
			if (Controller::IsKeysPressed(Key::Y + Key::DPadUp)) OSD::Notify("Increasing Collision Size");
			if (Controller::IsKeysPressed(Key::Y + Key::DPadDown)) OSD::Notify("Decreasing Collision Size");
			if (Controller::IsKeysDown(Key::Y + Key::DPadUp)) *pCol += 1.0f; *((float *)((vu32)pCol + 4)) += 1.0f;
			if (Controller::IsKeysDown(Key::Y + Key::DPadDown)) *pCol -= 1.0f; *((float *)((vu32)pCol + 4)) -= 1.0f;
		}
	}

	void stalk(MenuEntry *entry)
	{
		if (Controller::IsKeysPressed(Key::L + Key::DPadLeft))
		{
			OSD::Notify("Stalking player " << Hex(playerindex));
		}
		if (Controller::IsKeysDown(Key::L + Key::DPadLeft))
		{
			if ((u32)Player::GetCoordinates(playerindex) != 0 && (u32)Player::GetCoordinates() != 0)
			{
				*Player::GetCoordinates() = *Player::GetCoordinates(playerindex);
				*((float *)((vu32)Player::GetCoordinates() + 8)) = *((float *)((vu32)Player::GetCoordinates(playerindex) + 8));
			}
		}
		if (Controller::IsKeysPressed(Key::L + Key::DPadRight))
		{
			u32 x, y;
			if (Player::GetWorldCoords(&x, &y, 4, 1))
			{
				for (u8 i = 0; i < 4; i++)
				{
					Player::ExecuteAnimationWrapper(i, 0x34, 1, 1, 1, 1, 0, x, y, true);
				}
				OSD::Notify("Teleported players to you.");
			}
		}
	}

	void slmoanms(MenuEntry *entry)
	{
		u32 saval;

		if (Controller::IsKeysPressed(Key::ZL + Key::DPadLeft))
		{
			if (Process::Read32(0x654578, saval) && saval == 0x0A000004)
			{
				Process::Write32(0x654578, 0xE3A00001);
				Process::Write32(0x652C10, 0x40ff0000);
				Process::Write32(0x887880, 0x40C00000);
				OSD::Notify("Slow Animations: On", Color::Blue);
			}
			else if (saval == 0xE3A00001)
			{
				Process::Write32(0x654578, 0x0A000004);
				Process::Write32(0x652C10, 0x3F800000);
				Process::Write32(0x887880, 0x3F800000);
				OSD::Notify("Slow Animations: Off", Color::Orange);
			}
		}
	}
	
	void doonall(MenuEntry *entry)
	{
		u8 DATAIndexRandom = Utils::Random(0x00, 0x03);
		u8 dataforplayer;
		
		if (entry->Hotkeys[0].IsDown())
		{
			u32 x, y;
			if (Player::GetWorldCoords(&x, &y, 4, 1))
			{
				for (u8 i = 0; i < 4; i++)
				{
					Player::ExecuteAnimationWrapper(i, anim1, animItem, emotion, snake, soundID, 0, x, y, true);
				}
			}
		}
		if (entry->Hotkeys[1].IsDown())
		{
			u32 x, y;
			if (Player::GetWorldCoords(&x, &y, 4, 1))
			{
				for (u8 i = 0; i < 4; i++)
				{
					Player::ExecuteAnimationWrapper(i, 0x06, 1, 1, 1, 1, 0, x, y, true);
				}
			}
		}
		if (entry->Hotkeys[2].IsDown())
        {
            u32 player = Player::GetInstance(4, 1);
            u32 animInstance;
            u8 target = GameHelper::GetOnlinePlayerIndex();
            if (player != 0)
            {
				Process::Write32(0x677504, 0xE1A00000);
				Process::Write32(0x628B54, 0xE3A01000 + DATAIndexRandom);
                for (u8 i = 0; i < 4; i++)
                {
                    animInstance = Player::GetAnimationInstance(player, 0, 0, 0);
                    Process::Write16(animInstance + 0xA, 0xFFFF);
                    Process::Write16(animInstance + 0xC, 0x7F);
                    Process::Write16(animInstance + 0xE, 0x0C);
                    Process::Write16(animInstance + 0x10, 0x38C);
                    Process::Write8(animInstance + 0x13, 0);
                    Process::Write16(animInstance + 0x15, 0xFFFE);
                    Process::Write8(animInstance + 0x17, 4);
                    Process::Write8(animInstance + 0x18, *(u8 *)(player + 0x2F));
                    if (GameHelper::GetOnlinePlayerIndex() == GameHelper::GetActualPlayerIndex()) Player::ExecuteAnimation(player, 0xC4, animInstance, 0);
                    else Player::SendAnimPacket(i, animInstance, 0xC4, Player::GetRoom(i), i);
					Process::Write8(0x31FF59C9, 0x02);
                }
            }
        }
		if (!entry->Hotkeys[2].IsDown())
		{
			Process::Write32(0x628B54, 0xE5D11268);
            Process::Write32(0x677504, 0x1A000017);
		}
	}
	
	/*
	void doonall(MenuEntry *entry)
	{
		u8 DATAIndexRandom = Utils::Random(0x00, 0x03);
		u8 dataforplayer;
		
		if (entry->Hotkeys[0].IsDown())
		{
			u32 x, y;
			if (Player::GetWorldCoords(&x, &y, 4, 1))
			{
				for (u8 i = 0; i < 4; i++)
				{
					Player::ExecuteAnimationWrapper(i, anim1, animItem, emotion, snake, soundID, 0, x, y, true);
				}
			}
		}
		if (entry->Hotkeys[1].IsDown())
		{
			u32 x, y;
			if (Player::GetWorldCoords(&x, &y, 4, 1))
			{
				for (u8 i = 0; i < 4; i++)
				{
					Player::ExecuteAnimationWrapper(i, 0x06, 1, 1, 1, 1, 0, x, y, true);
				}
			}
		}
		if (entry->Hotkeys[2].IsDown())
        {
            u32 player = Player::GetInstance(4, 1);
            u32 animInstance;
            u8 target = GameHelper::GetOnlinePlayerIndex();
            if (player != 0)
            {
				Process::Write32(0x677504, 0xE1A00000);
				Process::Write32(0x628B54, 0xE3A01000 + DATAIndexRandom);
                for (u8 i = 0; i < 4; i++)
                {
                    animInstance = Player::GetAnimationInstance(player, 0, 0, 0);
                    Process::Write16(animInstance + 0xA, 0xFFFF);
                    Process::Write16(animInstance + 0xC, 0x7F);
                    Process::Write16(animInstance + 0xE, 0x0C);
                    Process::Write16(animInstance + 0x10, 0x38C);
                    Process::Write8(animInstance + 0x13, 0);
                    Process::Write16(animInstance + 0x15, 0xFFFE);
                    Process::Write8(animInstance + 0x17, 4);
                    Process::Write8(animInstance + 0x18, *(u8 *)(player + 0x2F));
                    if (GameHelper::GetOnlinePlayerIndex() == GameHelper::GetActualPlayerIndex()) Player::ExecuteAnimation(player, 0xC4, animInstance, 0);
                    else Player::SendAnimPacket(i, animInstance, 0xC4, Player::GetRoom(i), i);
					Process::Write8(0x31FF59C9, 0x02);
                }
            }
        }
		if (!entry->Hotkeys[2].IsDown())
		{
			Process::Write32(0x628B54, 0xE5D11268);
            Process::Write32(0x677504, 0x1A000017);
		}
	}*/

	void dropfromall(MenuEntry *entry)
	{
		u8 playerswapper = Utils::Random(0x00, 0x03);
		
		if (entry->Hotkeys[0].IsDown())
		{
			if (0x31FF59C9 != 0x00)
				Process::Write8(0x75F020, playerswapper);
				Process::Write32(0x764504, 0xEA000020);
				Process::Write32(0x75F010, 0xE1500000);
		}
		if (!entry->Hotkeys[0].IsDown())
		{
			Process::Write32(0x764504, 0x2A000020);
			Process::Write32(0x75F010, 0xE3520000);
			Process::Write32(0x75F020, 0x03A00000);
		}
	}
	
	void itemrandom(MenuEntry *entry)
    {
		u32 item1send;
		u32 item2send;
        u16 item1;
        u16 item2;
		u16 flag1;
		u16 flag2;
		u8 droploc1;
		//u8 range1;
		//u8 range2;
		//u8 range3;
		//u8 range4;
		//static bool Message = false;

		if (entry->Hotkeys[1].IsDown())
        {
			Sleep(Milliseconds(100));
            Keyboard Choices("Start Item");
            int UserChoice = Choices.Open(item1send);
			Process::Write32(0x9E0004, item1send);
			Sleep(Milliseconds(100));
            Keyboard Choices1("End Item");
            int UserChoice1 = Choices1.Open(item2send);
			Process::Write32(0x9E0008, item2send);
        }
        if (entry->Hotkeys[0].IsDown())
        {
			Process::Read16(0x9E0004, item1);
			Process::Read16(0x9E0008, item2);
			if (item1 != 0x0000)
			{
				if (item2 != 0x0000)
				{
					u16 itemidtoplace = Utils::Random(item1, item2);
					Process::Write16(0x31f2dbf0, itemidtoplace);
					Process::Write16(0x9E0000, itemidtoplace);
					Process::Write16(0x9E0020, itemidtoplace);
					//Process::Write16(selectedItem, itemidtoplace);
				}
			}
			else if (item1 == 0x0000)
			{
				if (item2 == 0x0000)
				{
					u16 nonzero = Utils::Random(0x009F, 0x00C8);
					Process::Write16(0x31f2dbf0, nonzero);
					Process::Write16(0x9E0000, nonzero);
					//Process::Write16(selectedItem, nonzero);
				}
			}
			//flags
			Process::Read16(0x9E0006, flag1);
			Process::Read16(0x9E000A, flag2);
			if (flag1 != 0x0000)
			{
				if (flag2 != 0x0000)
				{
					u16 flagtoset = Utils::Random(flag1, flag2);
					Process::Write16(0x31F2DBF2, flagtoset);
					Process::Write16(0x9E0002, flagtoset);
					Process::Write16(0x9E0022, flagtoset);
				}
			}
			else if (flag1 == 0x0000)
			{
				if (flag2 == 0x0000)
				{
					Process::Write16(0x31F2DBF2, 0x0000);
					Process::Write16(0x9E0002, 0x0000);
					Process::Write16(0x9E0022, 0x0000);
				}
				else if (flag2 != 0x0000)
				{
					u16 flagtoset = Utils::Random(0x0000, flag2);
					Process::Write16(0x31F2DBF2, flagtoset);
					Process::Write16(0x9E0002, flagtoset);
					Process::Write16(0x9E0022, flagtoset);
				}
			}
		}
		if (entry->Hotkeys[2].IsDown())
		{
			u8 droploc1 = Utils::Random(0x01, 0xFF);
			//u8 range1 = Utils::Random(0xE0, 0xFF);
			//u8 range2 = Utils::Random(0xE4, 0xFF);
			//u8 range3 = Utils::Random(0xE8, 0xFF);
			//u8 range4 = Utils::Random(0xE8, 0xFF);
			Process::Write8(0x59915C, droploc1);
			Process::Write8(0x599348, droploc1);
			Process::Write32(0x85FE58, 0xFBFAF9F8); //range1 essentially, but no longer random as the items would drop too far from you. Maybe if you weren't such a little fucking cum whore, this wouldnt be a problem.
			Process::Write32(0x5991E8, 0xE1A01000);
			Process::Write32(0x599248, 0xEA000006);
        }
    }
	
	void slattzylove(MenuEntry *entry)
	{
		Process::Write32(0x1D43A4, 0xE3A00001);
		Process::Write32(0x1D43C0, 0xE3A00001);
		Process::Write32(0x1D43D0, 0xE3A00001);
		Process::Write32(0x759024, 0xE1A00005);
	}
	
	void alltour(MenuEntry *entry)
    {
        u32		toury;
        static bool beendone = false;

        Process::Read32(0x95D734, toury);
        if (toury != 0 && !beendone)
        {
            for (int tr = 0; tr < 64; tr++)
                Process::Write8(toury + 0x10 + (0x1 * tr), 0x01);

            OSD::Notify("All Tours Open");
            beendone = true;
        }
        else if (toury == 0 && beendone)
        {
            beendone = false;
        }
    }
	
	void autoPickup(MenuEntry *entry)
	{
		u32 PickupLoop;
		static bool Message = false;
		
		if (entry->Hotkeys[0].IsDown() && !Message)
		{
			if (Process::Read32(0x67CCB8, PickupLoop) && PickupLoop == 0x0A000039)
			{
				Process::Write32(0x67CCB8, 0xE1A00000); //Loop Pickup Code, This line was made by Brume, not Nico.
				Process::Write32(0x59A0D0, 0xE1A00000);
				Process::Write32(0x59A1BC, 0xE1A00000);
				Process::Write32(0x59A268, 0x31F2DBF0);
				OSD::Notify("Auto-Pickup: On", Color:: Blue);
				Message = true;
			}
			else if (PickupLoop == 0xE1A00000)
			{
				Process::Write32(0x67CCB8, 0x0A000039); //Loop Pickup Code, This line was made by Brume, not Nico.
				Process::Write32(0x59A0D0, 0x1A000041);
				Process::Write32(0x59A1BC, 0x0A000006);
				Process::Write32(0x59A268, 0x0095CFFC);
				OSD::Notify("Auto-Pickup: Off", Color::Orange);
				Message = true;
			}
		}
		if (!entry->Hotkeys[0].IsDown())
			Message = false;
	}
	
	void pickupmod(MenuEntry *entry)
	{
		static bool Message = false;
		u8 mode3;
		u16 pickupID;
		u8 pickupanimm;
		
		Process::Write32(0x59A268, 0x9E0000);
		Process::Patch(0x59A0C4, 0xEA00003D);
		
		if (entry->Hotkeys[0].IsDown() && !Message)
		{
			if (Process::Read8(0x598d9c, mode3) && mode3 == 0x07)
			{
				Process::Write32(0x65D410, 0xE3A01006); //START OF ENABLER
				Process::Write32(0x598D9C, 0xE3A00001); //type
				Process::Write32(0x599FA8, 0xEA000008);
				Process::Write32(0x599FD0, 0xE3A00001);
				Process::Write32(0x59A1C0, 0xE3A04001);
				Process::Write32(0x59A24C, 0xE3A00001);
				Process::Write32(0x661894, 0xE5840000); //important data
				Process::Write32(0x661898, 0xE5C48004);
				Process::Write32(0x66189C, 0xE5CA9013);
				Process::Write32(0x6618C0, 0xE3A0103D); //animation
				Process::Write32(0x670814, 0xE3A0103D);
				OSD::Notify("Pickup Type: Pickup", Color::Red);
				Message = true;
			}
			else if (Process::Read8(0x598d9c, mode3) && mode3 == 0x01)
			{
				Process::Write32(0x65D410, 0xE3A01006); //START OF ENABLER
				Process::Write32(0x598D9C, 0xE3A00003); //type
				Process::Write32(0x599FD0, 0xE3A00003);
				Process::Write32(0x59A1C0, 0xE3A04003);
				Process::Write32(0x59A24C, 0xE3A00003);
				Process::Write32(0x661894, 0xE5840000); //important data
				Process::Write32(0x661898, 0xE5C48004);
				Process::Write32(0x66189C, 0xE5CA9013);
				Process::Write32(0x6618C0, 0xE3A01040); //animation
				Process::Write32(0x670814, 0xE3A01040);
				OSD::Notify("Pickup Type: Pluck", Color::Orange);
				Message = true;
			}
			else if (Process::Read8(0x598d9c, mode3) && mode3 == 0x03)
			{
				Process::Write32(0x65D410, 0xE3A01006); //START OF ENABLER
				Process::Write32(0x598D9C, 0xE3A0000A); //type
				Process::Write32(0x599FD0, 0xE3A0000A);
				Process::Write32(0x59A1C0, 0xE3A0400A);
				Process::Write32(0x59A24C, 0xE3A0000A);
				Process::Write32(0x661894, 0xE5840002); //important data
				Process::Write32(0x661898, 0xE5C48000);
				Process::Write32(0x66189C, 0xE5CA900F);
				Process::Write32(0x6618C0, 0xE3A01061); //animation
				Process::Write32(0x670814, 0xE3A01061);
				OSD::Notify("Pickup Type: FlipDrop", Color::Green);
				Message = true;
			}
			else if (Process::Read8(0x598d9c, mode3) && mode3 == 0x0A)
			{
				Process::Write32(0x65D410, 0xE3A01006); //START OF ENABLER
				Process::Write32(0x598D9C, 0xE3A00012); //type
				Process::Write32(0x599FA8, 0xEA000008); //okay, this better fix the crash or else I'm gonna kick some ass.
				Process::Write32(0x599FD0, 0xE3A00012);
				Process::Write32(0x59A1C0, 0xE3A04012);
				Process::Write32(0x59A24C, 0xE3A00012);
				Process::Write32(0x661894, 0xE5840002); //important data
				Process::Write32(0x661898, 0xE5C48000);
				Process::Write32(0x66189C, 0xE5CA900F);
				Process::Write32(0x6618C0, 0xE3A0105A); //animation
				Process::Write32(0x670814, 0xE3A0105A);
				OSD::Notify("Pickup Type: RockBreak", Color::Blue);
				Message = true;
			}
			else if (Process::Read8(0x598d9c, mode3) && mode3 == 0x12)
			{
				Process::Write32(0x65D410, 0xE3A01006); //START OF ENABLER
				Process::Write32(0x598D9C, 0xE3A00013); //type
				Process::Write32(0x599FD0, 0xE3A00013);
				Process::Write32(0x59A1C0, 0xE3A04013);
				Process::Write32(0x59A24C, 0xE3A00013);
				Process::Write32(0x661894, 0xE5840002); //important data
				Process::Write32(0x661898, 0xE5C48000);
				Process::Write32(0x66189C, 0xE5CA900F);
				Process::Write32(0x6618C0, 0xE3A0104F); //animation
				Process::Write32(0x670814, 0xE3A0104F);
				OSD::Notify("Pickup Type: Dig Hole", Color::Purple);
				Message = true;
			}
			else if (Process::Read8(0x598d9c, mode3) && mode3 == 0x13)
			{
				Process::Write32(0x65D410, 0xE3A01006); //START OF ENABLER
				Process::Write32(0x598D9C, 0xE3A00007); //type
				Process::Write32(0x599FD0, 0xE3A00007);
				Process::Write32(0x59A1C0, 0xE3A04007);
				Process::Write32(0x59A24C, 0xE3A00007);
				Process::Write32(0x661894, 0xE5840002); //important data
				Process::Write32(0x661898, 0xE5C48000);
				Process::Write32(0x66189C, 0xE5CA900F);
				Process::Write32(0x6618C0, 0xE3A0105D); //animation
				Process::Write32(0x670814, 0xE3A0105D);
				OSD::Notify("Pickup Type: Fill Hole", Color::Orange);
				Message = true;
			}
		}
		if (entry->Hotkeys[1].IsDown())
        {
            if (SetUpKB("Enter Item ID:", true, 8, pickupID, *(u16 *)0x9E0000))
            {
                {
                    Process::Write16(0x9E0000, pickupID);
                }
            }	
        }
		if (entry->Hotkeys[2].IsDown())
        {
            if (SetUpKB("Enter Animation ID for Pickup Mod:", true, 8, pickupanimm, *(u8 *)0x6618C0))
            {
                {
					Process::Write8(0x6618C0, pickupanimm); //animation
					Process::Write8(0x670814, pickupanimm);
                }
            }	
        }
		if (!entry->Hotkeys[0].IsDown())
			Message = false;
	}

    void MovementChanger(MenuEntry *entry)
        {
        u32 waval;
		static bool Message = false;

        if (entry->Hotkeys[0].IsDown() && !Message)
        {
			if (Process::Read32(0x64E82C, waval) && waval == 0xE3A00000)
            {
                Process::Write32(0x64E824, 0x03A00001);
                Process::Write32(0x64E82C, 0xE3A00001);
                Process::Write32(0x653154, 0xE1A00000);
                Process::Write32(0x653530, 0xE3A00000);
                Process::Write32(0x763ABC, 0xE3A00000);
                OSD::Notify("Movement Mode: Swimming", Color::Blue);
				Message = true;
            }
            else if (waval == 0xE3A00001)
            {
                Process::Write32(0x64E824, 0x03A00000);
                Process::Write32(0x64E82C, 0xE3A00000);
                Process::Write32(0x653154, 0xEBFC6348);
                Process::Write32(0x653530, 0xEB00AFB7);
                Process::Write32(0x763ABC, 0xE3A00001);
                OSD::Notify("Movement Mode: Walking", Color::Green);
				Message = true;
            }
        }
		if (!entry->Hotkeys[0].IsDown())
			Message = false;
    }
	
	void noPush(MenuEntry *entry)
	{
		toggle2(0x652288, 0xEA00002D, 0x2A00002D);
	}

	//inventory
	bool itemOSD(const Screen &screen)
	{
		u32 y;
		u32 item;
		if (screen.IsTop && Player::GetInstance(4, 1) != 0)
		{
			if (ItemSequence::Enabled()) item = ItemSequence::PeekNext();
			else if (selecting) item = selectedItem;
			else if (!ItemSequence::Enabled()) item = *(u32 *)inv;
			y = screen.Draw("Drop: " << Hex(item), 270, 0, Color::Green);
			if (itemIDToReplace != 0x7FFE) y = screen.Draw("Replace: " << (itemIDToReplace == 0xFFFFFFFF ? "everything" : Hex(itemIDToReplace)), 270, y, Color::Blue);
		}
	}

	void onItemChange(Keyboard &k, InputChangeEvent &e)
	{
		std::string s = k.GetInput();
		k.GetMessage() = "Enter ID:\n\n" << IDList::GetItemName(s != "" ? std::stoi(s, nullptr, 16) : 0);
	}

	void t2i(MenuEntry *entry)
	{
		u32 val;

		if (entry->WasJustActivated())
		{
			OSD::Run(itemOSD);
		}
		if (Controller::IsKeysDown(Key::X + Key::DPadRight))
		{
			if (SetUpKB("Enter ID:", true, 8, val, 0x7FFE, onItemChange) && val != 0)
			{
				writeItem(val, 0);
			}
		}
		else if (Controller::IsKeysDown(Key::X + Key::DPadUp))
		{
			if (SetUpKB("Enter ID to start the set:", true, 8, val, 0x7FFE) && val != 0)
			{
				for (int i = 0; i < 0x10; i++)
				{
					writeItem(val + i, i * 4);
				}
			}
		}
		else if (Controller::IsKeysPressed(Key::X + Key::DPadDown))
		{
			u32 x, y;
			
			if (Player::GetWorldCoords(&x, &y, 4, 1))
			{
				val = (u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0);
				if (val != 0)
				{
					writeItem(*(u32 *)val, 0);
					OSD::Notify("Read ID: " << Hex(*(u32 *)val));
					OSD::Notify("Address: " << Hex(val));
				}
			}
		}
		else if (Controller::IsKeysPressed(Key::X + Key::DPadLeft))
		{
			if (SetUpKB("Enter ID to replace\n(0xFFFFFFFF to replace everything):", true, 8, itemIDToReplace, 0x7FFE))
			OSD::Notify("Now replacing: " << (itemIDToReplace == 0xFFFFFFFF ? "everything" : Hex(itemIDToReplace)));
		}
	}

	void duplicate(MenuEntry *entry)
	{
		u32 val;

		if (Controller::IsKeyPressed(Key::R))
		{
			if (Process::Read32(inv, val) && val != 0) Process::Write32(inv + 4, val);
			if (Process::Read32(online_inv0, val) && val != 0) Process::Write32(online_inv0 + 4, val);
			if (Process::Read32(online_inv1, val) && val != 0) Process::Write32(online_inv1 + 4, val);
			if (Process::Read32(online_inv2, val) && val != 0) Process::Write32(online_inv2 + 4, val);
			if (Process::Read32(online_inv3, val) && val != 0) Process::Write32(online_inv3 + 4, val);
		}
		else if (Controller::IsKeysPressed(Key::R + Key::X))
		{
			if (Process::Read32(inv, val) && val != 0) for (int i = 1; i <= 0xF; i++) Process::Write32(inv + i * 4, val);
			if (Process::Read32(online_inv0, val) && val != 0) for (int i = 1; i <= 0xF; i++) Process::Write32(online_inv0 + i * 4, val);
			if (Process::Read32(online_inv1, val) && val != 0) for (int i = 1; i <= 0xF; i++) Process::Write32(online_inv1 + i * 4, val);
			if (Process::Read32(online_inv2, val) && val != 0) for (int i = 1; i <= 0xF; i++) Process::Write32(online_inv2 + i * 4, val);
			if (Process::Read32(online_inv3, val) && val != 0) for (int i = 1; i <= 0xF; i++) Process::Write32(online_inv3 + i * 4, val);
			OSD::Notify("Items duplicated!");
		}
	}
	
	void instantchop(MenuEntry *entry)
	{
        if (entry->WasJustActivated())
            Process::Write32(0x59945C, 0xE1A00000);

        if (!entry->IsActivated())
            Process::Write32(0x59945C, 0xCA000005);
    }
	
	void shakechop(MenuEntry *entry)
	{
        optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case 0:
            Process::Write32(0x5971D4, 0xE1A00000);
			Process::Write32(0x5971DC, 0xE1A00000);
			Process::Write32(0x5971E4, 0x1A00001B);
			Process::Write32(0x5971EC, 0xEA000080);
			break;
		case 1:
            Process::Write32(0x5971D4, 0x0A000008);
			Process::Write32(0x5971DC, 0x0A00005B);
			Process::Write32(0x5971E4, 0x0A00001B);
			Process::Write32(0x5971EC, 0x0A000080);
			break;
		default:
			break;
		}
    }

	void dropMod(MenuEntry *entry)
	{
		u8 val;
		if (entry->WasJustActivated()) ItemSequence::init(0x31F2DBF0);
		if (Controller::IsKeysPressed(Key::R + Key::DPadDown))
		{
			if (Process::Read8(0x19C4D0, val) && val == 0x06)
			{
				Process::Write8(0x19C4D0, 0);
				Process::Write8(0x19C574, 0);
				Process::Write8(0x19B02C, 0); //bury
				Process::Write8(0x1A00F0, 0);//display
				OSD::Notify("Infinite Drop: Enabled!");
			}
			else
			{
				Process::Write8(0x19C4D0, 0x06);
				Process::Write8(0x19C574, 0x06);
				Process::Write8(0x19B02C, 0x06);
				Process::Write8(0x1A00F0, 0x06);
				OSD::Notify("Infinite Drop: Disabled!");
			}
		}
		else if (Controller::IsKeysPressed(Key::R + Key::DPadLeft))
		{
			if (Process::Read8(0x26F0F8, val) && val == 0xDA)
			{
				Process::Write8(0x1A00F0, 0);
				Process::Write8(0x19B02C, 0);
				Process::Write8(0x19C4D0, 0);
				Process::Write8(0x19C574, 0); //infdrop enable for safety
				Process::Write32(0x26F0F8, 0xE1A00000); //plant, safe
				Process::Write32(0x19C5E8, 0xE1A00000); //drop
				Process::Write32(0x19DE84, 0xE1A00000); 
				Process::Write32(0x19DFA8, 0xE1A00000); 
				OSD::Notify("DragDrop Spam: Enabled!");
			}
			else
			{
				Process::Write32(0x26F0F8, 0xEB00B0DA); //plant
				Process::Write32(0x19C5E8, 0xEB03FB9E); //drop
				Process::Write32(0x19DE84, 0xEB03F577); 
				Process::Write32(0x19DFA8, 0xEB110036); 
				OSD::Notify("DragDrop Spam: Disabled!");
			}
		}
		else if (Controller::IsKeysPressed(Key::R + Key::DPadUp))
		{
			if (*(u32 *)0x75F010 == 0xE3520000)
			{
				Process::Write32(0x75F010, 0xE1500000);
				Process::Write8(0x75F020, playerindex);
				OSD::Notify("Controlling player " << Hex(playerindex) << ": Enabled!");
			}
			else
			{
				Process::Write32(0x75F010, 0xE3520000);
				Process::Write8(0x75F020, 0);
				OSD::Notify("Controlling player: Disabled!");
			}
		}
		else if (Controller::IsKeysPressed(Key::R + Key::DPadRight))
		{
			int8_t index;
			Process::Write32(0x19CFFC, 0xE1A00000);
			Process::Write8(0x660430, 0x06); //set anim after bury
			optKb->Populate(dropOpt);
			index = optKb->Open();
			if (index == -1) return;
			if (index == 0x24)
			{
				Sleep(Milliseconds(100));
				optKb->Populate(dropAnimOpt);
				optKb->GetMessage() = "Choose animation:\n(Current: 0x" << Hex(waitAnim) << ")";
				index = optKb->Open();
				optKb->GetMessage() = "Choose option:";
				if (index == -1) return;
				waitAnim = dropAnims[index];
				//SetUpAnimKB(waitAnim, waitAnim);
				return;
			}
			DropType = index + 1;
			Process::Write8(0x59771C, DropType);
			return;
		}
		else if (Controller::IsKeysPressed(Key::R + Key::CPadUp))
		{
			/*"Full square",
			"Circle",
			"Horizontal line",
			"Vertical line",
			"Square",
			"NE to SW diagonal",
			"NW to SE diagonal",
			"Reset",
			"Item array",*/
			int8_t index;
			u8 range, count;
			u8 val;
			const u32 addr = 0x85FE58;
			optKb->Populate(shapeOpt);
			index = optKb->Open();
			Sleep(Milliseconds(100));
			switch (index)
			{
			case -1:
				return;
			case 0:
				if (SetUpKB("Enter radius:", true, 1, val, 0))
				{
					val = val & 0xF;
					range = val * 2 + 1;
					count = range * range;
					Process::Write8(0x599348, count);
					Process::Write8(0x59915C, count);
					for (int8_t i = 0 - (int8_t)val; i <= (int8_t)val; i++)
					{
						for (u8 j = 0; j < range; j++)
						{
							Process::Write8(addr + j + ((i + val) * range), (0 - (int8_t)val) + j);
							Process::Write8(addr + count + j + ((i + val) * range), i);
						}
					}
					
				}
				break;
			case 1:
				if (SetUpKB("Enter radius:", true, 1, val, 0))
				{
					val = val & 0xFF;
					count = 2 * val * val - 6 * val + 16; //val * 6;
					Process::Write8(0x599348, count);
					Process::Write8(0x59915C, count);
					for (u8 i = 0; i < count; i++)
					{
						Process::Write8(addr + i, (int8_t)nearbyint((float)(val * cos((6.28318530 / count) * i))));
						Process::Write8(addr + count + i, (int8_t)nearbyint((float)(val * sin((6.28318530 / count) * i))));
					}
				}
				break;
			case 2:
				if (SetUpKB("Enter radius:", true, 2, val, 0))
				{
					range = val * 2 + 1;
					Process::Write8(0x599348, range);
					Process::Write8(0x59915C, range);
					for (int8_t i = 0 - (int8_t)val; i <= (int8_t)val; i++)
					{
						Process::Write8(addr + (i + val), i);
						Process::Write8(addr + range + (i + val), 0);
					}

				}
				break;
			case 3:
				if (SetUpKB("Enter radius:", true, 2, val, 0))
				{
					range = val * 2 + 1;
					Process::Write8(0x599348, range);
					Process::Write8(0x59915C, range);
					for (int8_t i = 0 - (int8_t)val; i <= (int8_t)val; i++)
					{
						Process::Write8(addr + (i + val), 0);
						Process::Write8(addr + range + (i + val), i);
					}

				}
				break;
			case 4:
				if (SetUpKB("Enter radius:", true, 2, val, 0))
				{
					range = val * 2 + 1;
					count = range * 4;
					Process::Write8(0x599348, count);
					Process::Write8(0x59915C, count);
					for (u8 i = 0; i < range; i++)
					{
						Process::Write8(addr + i * 4, i - val);
						Process::Write8(addr + count + i * 4, 0 - val);
						Process::Write8(addr + 1 + i * 4, 0 - val);
						Process::Write8(addr + 1 + count + i * 4, i - val);
						Process::Write8(addr + 2 + i * 4, i - val);
						Process::Write8(addr + 2 + count + i * 4, val);
						Process::Write8(addr + 3 + i * 4, val);
						Process::Write8(addr + 3 + count + i * 4, i - val);
					}
				}
				break;
			case 5:
				if (SetUpKB("Enter radius:", true, 2, val, 0))
				{
					range = val * 2 + 1;
					Process::Write8(0x599348, range);
					Process::Write8(0x59915C, range);
					for (int8_t i = 0 - (int8_t)val; i <= (int8_t)val; i++)
					{
						Process::Write8(addr + (i + val), -i);
						Process::Write8(addr + range + (i + val), i);
					}
				}
				break;
			case 6:
				if (SetUpKB("Enter radius:", true, 2, val, 0))
				{
					range = val * 2 + 1;
					Process::Write8(0x599348, range);
					Process::Write8(0x59915C, range);
					for (int8_t i = 0 - (int8_t)val; i <= (int8_t)val; i++)
					{
						Process::Write8(addr + (i + val), i);
						Process::Write8(addr + range + (i + val), i);
					}
				}
				break;
			case 8:
				break;
			default:
				Process::Write8(0x599348, 0x09);
				Process::Write8(0x59915C, 0x09);
				//Process::Write32(0x5991E8, 0xE7971100);
				Process::Write32(0x599248, 0x0A000006);
				int8_t def[18]{ 0, 0, -1, 1, -1, 1, -1, 1, 0, 0, -1, -1, -1, 0, 0, 1, 1, 1 };
				for (u8 i = 0; i < 18; i++)
					Process::Write8(addr + i, (u8)def[i]);
				return;
			}
			Process::Write32(0x5991E8, 0xE1A01000);
			Process::Write32(0x599248, 0xEA000006);
			return;
		}
		else if (Controller::IsKeysPressed(Key::R + Key::CPadRight)) SetUpKB("Enter room ID to drop into\n(For instant drop only,\n0xA5 to restore):", true, 2, DropRoom, DropRoom);
		else if (Controller::IsKeysPressed(Key::R + Key::Start)) ItemSequence::openKB();
		if (GameHelper::GetActualPlayerIndex() == 0) *(u32 *)0x30B83C = 0x13A00001; else *(u32 *)0x30B83C = 0x13A00000;
	}
	void useChain(MenuEntry *entry)
	{
		optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case 0:
			ItemSequence::Switch(true);
			break;
		case 1:
			ItemSequence::Switch(false);
			break;
		default:
			break;
		}
	}
	void dragDrop(MenuEntry *entry) { toggle2(0x19B694, 0xE1A00000, 0x0A00000C); }
	void validate(MenuEntry *entry) { toggle2(0x76A894, 0xE3A00001, 0xE3A00000); }
	void eat(MenuEntry *entry) { toggle2(0x19C1F0, 0xE1A00000, 0x0A000009); }
	void display(MenuEntry *entry) {toggle2(0x19C068, 0xE1A00000,0x0A00001A); }

	//environment
	void flwTrample(MenuEntry *entry) { toggle2(0x597F58, 0xE3A0001D, 0xEBF5935C); }

	void trampleSeeder(MenuEntry *entry)
	{ 
		if (Controller::IsKeysPressed(Key::R + Key::B))
		{
			if (*(u32 *)0x597F64 == 0xE1A00000)
			{
				Process::Write32(0x64E4D4, 0x0A000032);
				Process::Write32(0x597F38, 0x0A000056);
				Process::Write32(0x597F58, 0xE3A0001D);
				Process::Write32(0x597F64, 0x0A00004B);
				Process::Write32(0x597FA0, 0x1A00003C);
				Process::Write32(0x597FE8, 0x0A00002A);
				Process::Write32(0x597FAC, 0x0A000039);
				OSD::Notify("RESTORUS");
			}
			else
			{
				Process::Write32(0x64E4D4, 0xE1A00000);
				Process::Write32(0x597F38, 0xE1A00000);
				Process::Write32(0x597F58, 0xEBF5935C);
				Process::Write32(0x597F64, 0xE1A00000);
				Process::Write32(0x597FA0, 0xE1A00000);
				Process::Write32(0x597FE8, 0xE1A00000);
				Process::Write32(0x597FAC, 0xE1A00000);
				OSD::Notify("DELETUS");
			}
		}
	}

	void treecrusher(MenuEntry *entry)
	{
		u32 val;
		
		if (Controller::IsKeysPressed(Key::ZR + Key::DPadUp))
		{
			u32 x, y;
			
			if (Player::GetWorldCoords(&x, &y, 4, 1))
			{
				val = (u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0);
				if (val != 0)
				{
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x6B, 0x2B, 1, 1, 1, 0, x, y, true);
				}
			}
		}
	}
	
	void treeItemMod(MenuEntry *entry)
	{
		toggle(new u32[2]{ 0x5A0DB8, 0x5A0EF4 }, new u32[2]{ 0xE59F1134, 0x31F2DBF0 }, new u32[2]{ 0xE59D104C, 0x95CFFC }, 2);
	}
	
	void itemontreemod(MenuEntry *entry)
	{
		u32 invindex;
		
		Process::Read32(0x31F2DBF0, invindex);
		Process::Write32(0x2FE6A0, 0xE59F0020);
		Process::Write32(0x2FE6AC, 0xE3500000);
		Process::Write32(0x2FE6C8, invindex);
	}
	
	void fruitStays(MenuEntry *entry)
	{
		toggle(new u32[2]{ 0x5972D4, 0x5972CC }, new u32[2]{ 0xE58D101C, 0xE5961000 }, new u32[2]{ 0xEB00054D, 0xE1A01006 }, 2);
	}

	void envMod(MenuEntry *entry)
	{
		u8 val;

		kb->GetMessage() = "Enter behavior ID\n(0xFF to restore game's functionality):";
		kb->IsHexadecimal(true);
		kb->SetMaxLength(2);
		if (kb->Open(val) == 0)
		{
			if (val == 0xFF)
			{
				Process::Write32(0x2FCCF8, 0xE350001D);
				Process::Write32(0x2FCCFC, 0x23A00000);
			}
			else
			{
				Process::Write32(0x2FCCF8, 0xE1A00000);
				Process::Write32(0x2FCCFC, 0xE3A00000 + val);
			}
		}
	}

	void noTrap(MenuEntry *entry) { toggle(new u32[2]{ 0x6789E4, 0x65A668 }, new u32[2]{ 0xEA00002D, 0xEA000014 }, new u32[2]{ 0x1A00002D, 0x1A000014}, 2); }

	void infRock(MenuEntry *entry)
	{
		//if (*(vu16 *)0x9536C0 < 0x60)
		//{
		Process::Write32(0x9536C0, 0xFFFF);
		Process::Write32(0x9536C4, 0);
		//}
	}

	void menuRock(MenuEntry *entry)
	{
		u32 item;

		if (SetUpKB("Enter item ID:", true, 8, item, 0x20AC))
		{
			if (item == 0x20AC)
			{
				Process::Write32(0x860284, 0x20AC20AC);
				Process::Write32(0x860288, 0x20AE20AD);
				Process::Write32(0x86028C, 0x20B520B0);
				Process::Write32(0x860290, 0x20B820B6);
				Process::Write32(0x860294, 0x20C420BC);
			}
			else
			{
				for (int i = 0; i < 10; i++)
				{
					Process::Write16(0x860284 + i * 2, item & 0xFFFF);
				}
			}
		}
	}
	
    void BuryAnim(MenuEntry *entry)
	{
		u32 BuryA;
		if (SetUpKB("Enter Animation ID:", true, 8, BuryA, *(u8 *)0x65EDF8))
		{
			Process::Write8(0x65EDF8, BuryA);
		}
	}
	
	void tooltype(MenuEntry *entry)
	{
		static bool Message = false;
		u32 ToolT;
		if (SetUpKB("Enter tool animation ID:", true, 8, ToolT, *(u8 *)0x65AA54))
		{
			Process::Write8(0x65AA54, ToolT);
			Process::Write8(0x66B230, ToolT);
			Process::Write8(0x66B254, ToolT);
			Process::Write8(0x66B2AC, ToolT);
			Process::Write8(0x66B30C, ToolT);
			Process::Write8(0x66B3B0, ToolT);
			Process::Write8(0x66B3D4, ToolT);
			Process::Write8(0x66B41C, ToolT);
			Process::Write8(0x66B4E4, ToolT);
			Process::Write8(0x66B508, ToolT);
			Process::Write8(0x66B52C, ToolT);
			Process::Write8(0x670890, ToolT);
			Process::Write8(0x674594, ToolT);
			Process::Write8(0x678918, ToolT);
			Process::Write8(0x67B2F8, ToolT);
			Process::Write8(0x67FD48, ToolT);
			Process::Write8(0x683FA8, ToolT);
			Process::Write8(0x68A354, ToolT);
			OSD::Notify("Tool animation changed!");
            Message = true;
		}
	}

	void    EncChat(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
			Process::Patch(0x32C5B384, 0x06);
		
		if (!entry->IsActivated())
			Process::Patch(0x32C5B384, 0x02);
    }

	void fountainItem(MenuEntry *entry)
	{
		u32 item;
		if (SetUpKB("Enter item ID:", true, 8, item, *(u32 *)0x597870))
		{
			Process::Write32(0x597870, item);
		}
	}

	void Shakethatass(MenuEntry *entry)
	{
		u32 TShake;
		if (SetUpKB("Enter Interaction ID: (01 - 23)", true, 8, TShake, *(u8 *)0x5972C8))
		{
			Process::Write8(0x5972C0, TShake);
		}
	}
	
	void emotreez(MenuEntry *entry)
	{
		u32 TCut;
		if (SetUpKB("Enter Interaction ID: (01 - 23)", true, 8, TCut, *(u8 *)0x599400))
		{
			Process::Write8(0x599400, TCut);
		}
	}
	
	void PocketDropDrag(MenuEntry *entry)
	{
		u32 PDI;
		if (SetUpKB("Enter Interaction ID: (01 - 23) !!WARNING!! READ NOTE!!", true, 8, PDI, *(u8 *)0x597840))
		{
			Process::Write8(0x597840, PDI);
		}
	}
	
	std::vector<std::string> gametype{
		"Offline",
		"Online (Mode 1)",
		"Online (Mode 2)",
		"Dream",
	};
	
	void mgtype(MenuEntry *entry)
    {
        Keyboard    keyboard("Choose Game Type", gametype);
        int         gametchoice = keyboard.Open();

        if (gametchoice < 0)
            return;

        Process::Write8(0x31FF59C9, gametchoice);
    }
	
	void rockparticle(MenuEntry *entry)
	{
		u32 rp_type;
		if (SetUpKB("Enter Particle ID:", true, 8, rp_type, *(u16 *)0x5A2D20))
		{
			Process::Write16(0x5A2D20, rp_type);
		}
	}

	void shader1(MenuEntry *entry)
	{
		u8 grass_col;
		if (SetUpKB("Set Value:", true, 8, grass_col, *(u8 *)0x4B10AC))
		{
			Process::Write32(0x4B10AC, 0xE3A00000 + grass_col);
		}
	}
	
	std::vector<std::string> lightsource{
		"Unused (Glitched)",
		"Dark World",
		"Overhead Light",
		"Overhead Light 2",
		"Dark Backdrop",
		"Lighter Backdrop",
		"Bright Light 1",
		"Bright Light 2",
		"Blue Tint",
		"Blue Tint 2",
		"Light Blue Tint",
		"Brighter Overall",
		"Sea Blue Hue",
		"Natural Cleanliness",
		"Double Brightness",
		"Double Brightness 2",
		"Dark Grey World",
		"Dark Grey World 2",
		"Dark Green Backdrop",
	};
	
	void shader2(MenuEntry *entry)
    {

        Keyboard    keyboard("Choose Light Source", lightsource);
        int         lightchoice = keyboard.Open();

        if (lightchoice < 0)
            return;

        Process::Write8(0x4B10C8, lightchoice);
    }
	
	void shader3(MenuEntry *entry)
	{
		u32 BRIGHT;
		if (SetUpKB("Enter Float! \n Default: 3F800000", true, 8, BRIGHT, *(u32 *)0x1E6D58))
		{
			Process::Write32(0x1E6D58, BRIGHT);
		}
	}
	
	void shader4(MenuEntry *entry)
	{
		u16 cols;
		if (SetUpKB("Set Value: \nDefault: 1521 ", true, 8, cols, *(u16 *)0x4B10AC))
		{
			Process::Write32(0x4B10A4, 0xE3A00000 + cols);
		}
	}
	
	void exportshader(MenuEntry *entry)
    {
        std::string ShaderName;

        Keyboard keyboard("Name the shader");
        keyboard.SetMaxLength(30);
        if (keyboard.Open(ShaderName) != -1) {
            Sleep(Seconds(0.1f));
            Directory SaveDir;
            if (Directory::IsExists("/FOXXY/Shader") == 0)
                Directory::Create("/FOXXY/Shader");
            
            File SaveFile;
            File::Open(SaveFile,"/FOXXY/Shader/"+ShaderName+".fasha", File::RWC);
            SaveFile.Dump(0x4B1090, 0x40);
            SaveFile.Close();
            MessageBox("Current shader dumped!")();
        }
    }
	
	void importshader(MenuEntry *entry)
    {
        std::vector<std::string> FileList;
        Directory ShaderDir;
      
        Directory::Open(ShaderDir,"/FOXXY/Shader", true);
        ShaderDir.ListFiles(FileList, ".fasha");
        Keyboard keyboard("Choose a shader!", FileList);
        int FileChoice = keyboard.Open();

        if(FileChoice < 0)
            return;

        Sleep(Seconds(0.1f));
        File SaveFile;    
        ShaderDir.OpenFile(SaveFile, FileList[FileChoice], File::READ);
        SaveFile.Inject(0x4B1090, 0x40);
        MessageBox("Shader loaded!")();
    }
	
	void	hatz(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
            Process::Patch(0x68C630, 0xE1A00000);

        if (!entry->IsActivated())
            Process::Patch(0x68C630, 0x3AFFFFD6);
    }
	
	void pullSpeedMod(MenuEntry *entry)
	{
		optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case -1:
			break;
		case 0:
			*(float *)0x67C220 = 200.0f; //pull speed
			*(float *)0x67C224 = 200.0f; //pull display item speed
			break;
		case 1:
			*(float *)0x67C220 = 6.0f;
			*(float *)0x67C224 = 1.0f;
			break;
		}
	}

	void pickSpeedMod(MenuEntry *entry)
	{
		optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case -1:
			break;
		case 0:
			*(float *)0x660F2C = 200.0f; //pickup speed
			*(float *)0x660F30 = 200.0f; //pickup display item speed
			break;
		case 1:
			*(float *)0x660F2C = 6.0f;
			*(float *)0x660F30 = 1.0f;
			break;
		}
	}

	//room
	void onRoomChange(Keyboard &k, InputChangeEvent &e)
	{
		std::string s = k.GetInput();
		k.GetMessage() = "Enter room ID:\n\n" << IDList::GetRoomName(s != "" ? std::stoi(s, nullptr, 16) : 0);
	}

	void roomWarp(MenuEntry *entry)
	{
		if (Controller::IsKeysPressed(Key::L + Key::B) && !selecting)
		{
			u8 val;
			u32 nop = 0xE1A00000;
			/*"Arrive at island",
			"Arrive at tourdesk",
			"Arrive at town",
			"Leave from island",*/

			// allow warp for host
			Process::Write32(0x61B568, nop);
			Process::Write32(0x61B908, nop);
			Process::Write32(0x61BA10, nop);
			Process::Write32(0x61B980, nop);
			// fix checks
			Process::Write32(0x75F830, 0xE3A00001);

			optKb->Populate(warpOpt);
			switch (optKb->Open())
			{
			case 0:
				Sleep(Milliseconds(100));
				if (SetUpKB("Enter room ID:", true, 2, val, 0, onRoomChange))
				{
					OSD::Notify("Warping to room " << Hex(val));
					//Process::Write32(0x61E7A8, 0xE3A05000 | val); //set default room
					//Sleep(Milliseconds(200));
					//GameHelper::Warp0(); //execute warp func
					//Sleep(Milliseconds(2000));
					//Process::Write32(0x61E7A8, 0xE1A05000); //restore game's behaviour
					Process::Write32((u32)&pfunction04, 0x304A60);
					pfunction04(val, 1, 1, 0);
				}
				break;

			case 1:
				//GameHelper::ArriveAtIsland();
				Process::Write8(0x31FE4ECC, 0x02);
				break;

			case 2:
				//GameHelper::ArriveAtTourdesk();
				Process::Write32(0x61B800, nop);
				Process::Write8(0x31FE4ECC, 0x05);
				Sleep(Milliseconds(5));
				break;

			case 3:
				//GameHelper::ArriveAtTown();
				Process::Write8(0x31FE4ECC, 0x07);
				break;

			case 4:
				//GameHelper::LeaveIsland();
				Process::Write8(0x31FE4ECC, 0x06);
				break;

			default:
				break;
			}
			Sleep(Milliseconds(80));
			Process::Write32(0x75F830, nop);
		}
	}

	void roomSeeder(MenuEntry *entry)
	{
        Process::Write32(0x998C7A, 0x10101010);
        Process::Write8(0x998C7E, 0x10);
        if (entry->WasJustActivated())
        {
			//Process::Patch(0x5B4B60, 0xE3A00001); //allow wallpaper change
            Process::Patch(0x5B5268, 0xE1A00000);
            Process::Patch(0x5B5284, 0xEA000026);
			Process::Patch(0x5B7558, 0xE3A00001);
        }
        else if (!entry->IsActivated())
        {
			//Process::Patch(0x5B4B60, 0xE3A00000); //allow wallpaper change
            Process::Patch(0x5B5268, 0x0A00000D);
            Process::Patch(0x5B5284, 0x0A000026);
			Process::Patch(0x5B7558, 0xE3A00000);
        }
    }

	void invSeeder(MenuEntry *entry) { toggle2(0x19AE90, 0xEA000008, 0x0A000008); }

	void roomTrack(MenuEntry *entry)
	{
		u8 val;

		if (Process::Read8(0x958343, val) && val != roomID && val != 0xA5)
		{
			roomID = val;
			OSD::Notify("Room: " << Hex(roomID));
		}
	}

	void roomSwap(MenuEntry *entry)
	{
		u8 roomToSwap;
		u32 ptr;
		if (SetUpKB("Enter ID of room whose model you would like in your current room:", true, 2, roomToSwap, 0));
		{
			Process::Write32(0x958364 + (*(u8 *)room * 4), *(u32 *)(0x958364 + roomToSwap * 4));
		}
	}
	
	//anim
	void idle(MenuEntry *entry)
	{
		if (entry->Hotkeys[0].IsDown())
		{
			Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), 6, 0, 0, 0, 0, 0, 0, 0);
		}
	}

	void onSnakeChange(Keyboard &k, InputChangeEvent &e)
	{
		std::string s = k.GetInput();
		k.GetMessage() = snakeKB << "\n\n" << IDList::GetSnakeName( s != "" ? std::stoi(s, nullptr, 16) : 0);
	}

	void execAnim(MenuEntry *entry)
	{
		static s8 offsetX, offsetY;
		static u32 wX, wY;
		if (entry->Hotkeys[0].IsDown())
		{
			u8 temp;
			s8 index;
			optKb->Populate(animOpt);
			index = optKb->Open();
			Sleep(Milliseconds(100));
			switch (index)
			{
			case 0:
				SetUpAnimKB(anim1, anim1);
				break;
			case 1:
				SetUpKB("Enter item ID to assign:", true, 8, animItem, animItem);
				break;
			case 2:
				SetUpKB(snakeKB, true, 3, snake, snake, onSnakeChange);
				break;
			case 3:
				SetUpKB("Enter emotion ID\n(Use with animation 0xAF or 0x88)", true, 2, emotion, emotion);
				break;
			case 4:
				SetUpKB("Enter sound ID\n(Use with animation 0x45 or 0xC4)", true, 8, soundID, soundID);
				break;
			case 5:
				SetUpKB("Enter hair style ID\n(Use with animation 0xB9)", true, 2, appearance[0], appearance[0]);
				Sleep(Milliseconds(100));
				SetUpKB("Enter hair color\n(Use with animation 0xB9)", true, 2, appearance[1], appearance[1]);
				Sleep(Milliseconds(100));
				SetUpKB("Enter eye color\n(Use with animation 0xB9)", true, 2, appearance[2], appearance[2]);
				break;
			case 6:
				if (SetUpKB("Enter online animation ID\n(0x6 to disable)", true, 2, temp, 6))
				{
					if (temp == 0x6 || temp == 0xFF) Process::Write32(0x67743C, 0xE1A07002);
					else Process::Write32(0x67743C, 0xE3A07000 + temp);
				}
				break;
			default:
				break;
			}
		}
		if (Controller::IsKeysPressed(Key::A))
		{
			Player::GetWorldCoords(&wX, &wY, GameHelper::GetActualPlayerIndex(), 1);
		}
		if (turbo ? Controller::IsKeysDown(Key::A) : Controller::IsKeysPressed(Key::A))
		{
			animExecuting = true;
			Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, anim1, animItem, emotion, snake, soundID, 0, wX + offsetX, wY + offsetY, false, appearance);
			if (Controller::IsKeyDown(Key::CPadRight)) offsetX++;
			if (Controller::IsKeyDown(Key::CPadLeft)) offsetX--;
			if (Controller::IsKeyDown(Key::CPadDown)) offsetY++;
			if (Controller::IsKeyDown(Key::CPadUp)) offsetY--;
		}
		else
		{
			animExecuting = false;
			offsetX = 0;
			offsetY = 0;
		}
	}

	void openAnimMod(MenuEntry *entry) { u8 val;  if (SetUpAnimKB(val, 0x16)) Process::Write8(0x65A99C, val); }

	void speedUpAnim(MenuEntry *entry)
	{
		u8 animID;
		u32 funcTable = 0x844634;
		if (SetUpAnimKB(animID, 0))
		{
			if (animID != 0)
			{
				u32 i;
				u32 funcAddr = ((*(u32 *)(funcTable + animID * 0x1C + 4)) & 1) == 0 ? *(u32 *)(funcTable + animID * 0x1C) : 0;
				if (funcAddr == 0)
				{
					OSD::Notify("Could not speed up animation.");
					return;
				}
				u32 frameFuncAddr = *(u32 *)(funcTable + animID * 0x1C + 8);
				while (true)
				{
					if (*(u32 *)(funcAddr + i * 4) == 0x3F800000)
					{
						Process::Write32(funcAddr + i * 4, 0x45000000);
						break;
					}
					/*if (*(u32 *)(funcAddr + i * 4) == 0)
					{
						break;
					}*/
					i++;
				}
				i = 0;
				while (true)
				{
					if (*(u32 *)(frameFuncAddr + i * 4) == 0x3F800000)
					{
						Process::Write32(funcAddr + i * 4, 0x45000000);
						break;
					}
					/*if (*(u32 *)(frameFuncAddr + i * 4) == 0)
					{
						break;
					}*/
					i++;
				}
			}
		}
	}

	void digany(MenuEntry *entry)
	{
		{
			Process::Write32(0x5999F8, 0xE3A00001);
			Process::Write32(0x599A0C, 0xE3A04013);
			Process::Write32(0x6761B8, 0xE3A01050);
			Process::Write32(0x676214, 0xE3A01050);
			Process::Write32(0x67A2F4, 0x00003414);
		}
	}
	
	void asmpresses(MenuEntry *entry)
		{
		u32 pasm;

		if (Controller::IsKeysPressed(Key::ZL + Key::DPadRight))
		{
			if (Process::Read32(0x5C5BEC, pasm) && pasm == 0x0A000028)
			{
				Process::Write32(0x5C5BEC, 0xE1A00000);
				OSD::Notify("Multi-Presses: On!", Color::Green);
			}
			else if (pasm == 0xE1A00000)
			{
				Process::Write32(0x5C5BEC, 0x0A000028);
				OSD::Notify("Multi-Presses: Off!", Color::Red);
			}
		}
	}
	
	void forceIf(MenuEntry *entry)
	{
		static bool alreadyExecuted = false;
		u32 wX, wY;
		for (u8 i = 0; i < 4; i++)
		{
			if (Player::GetWorldCoords(&wX, &wY, i, 1) && !anims.empty())
			{
				if (std::find(anims.begin(), anims.end(), *Player::GetAnimation(i)) != anims.end())
				{
					if (!alreadyExecuted)
					{
						Player::ExecuteAnimationWrapper(i, animToForce, animItem, emotion, snake, soundID, 0, wX, wY, true, appearance);
						alreadyExecuted = true;
					}
					//*Player::GetAnimation(i) = animToForce;
				}
				else
				{
					alreadyExecuted = false;
				}
			}
		}
	}

	void menuForceIf(MenuEntry *entry)
	{
		std::vector<std::string> elements;
		int8_t index;
		u8 newAnim;
		elements.clear();
		for (u8 i = 0; i < anims.size(); i++) elements.push_back(Hex(anims.at(i)));
		elements.push_back("Add...");
		optKb->GetMessage() = "Set triggering animations:";
		optKb->Populate(elements);
		Sleep(Milliseconds(100));
		index = optKb->Open();
		Sleep(Milliseconds(100));
		if (index == elements.size() - 1)
		{
			if (SetUpAnimKB(newAnim, 0)) anims.push_back(newAnim);
		}
		else if (index == -1) goto n;
		else
		{
			if (!SetUpKB("Modify anim ID (B to erase):", true, 2, anims.at(index), anims.at(index))) anims.erase(anims.begin() + index);
		}
		menuForceIf(entry);
		return;
		n:
		optKb->GetMessage() = "Choose option:";
		Sleep(Milliseconds(100));
		SetUpKB("Enter anim ID to force:", true, 2, animToForce, animToForce);
	}
	//island
	void acreMod(MenuEntry *entry)
	{
		for (u8 i = 0; i < 16; i++)
		{
			Process::Write8(islandAcres + i * 2, isl.acres[i]);
		}
	}

	void menuAcreMod(MenuEntry *entry)
	{
		kb->SetMaxLength(2);
		kb->IsHexadecimal(true);
		for (u8 i = 0; i < 16; i++)
		{
			kb->GetMessage() = "Enter ID for acre number " << std::to_string(i + 1) << ":" ;
			kb->Open(isl.acres[i], isl.acres[i]);
		}
	}

	void buildingMod(MenuEntry *entry)
	{
		for (u8 i = 0; i < 2; i++)
		{
			Process::Write16(islandBuildings + i * 4, isl.b[i].id);
			Process::Write8(islandBuildings + 2 + i * 4, isl.b[i].x);
			Process::Write8(islandBuildings + 3 + i * 4, isl.b[i].y);
		}
	}

	void menuBuildingMod(MenuEntry *entry)
	{
		kb->SetMaxLength(2);
		kb->IsHexadecimal(true);
		for (u8 i = 0; i < 2; i++)
		{
			kb->GetMessage() = "Enter building ID: " << std::to_string(i + 1);
			kb->Open(isl.b[i].id, isl.b[i].id);
			kb->GetMessage() = "Enter building coord X: " << std::to_string(i + 1);
			kb->Open(isl.b[i].x, isl.b[i].x);
			kb->GetMessage() = "Enter building coord Y: " << std::to_string(i + 1);
			kb->Open(isl.b[i].y, isl.b[i].y);
		}
	}

	void numbers(MenuEntry *entry)
	{
		Process::Write8(0xAD7158, 2);
	}
	
	void	tourend(MenuEntry *entry)
    {
        static bool button = false;

        if (entry->Hotkeys[0].IsDown() && !button)
        {
            Process::Write8(0x95171D, 1);
            button = true;
        }
        if (!entry->Hotkeys[0].IsDown())
            button = false;
    }

	//npcs
	void npcMenu(MenuEntry  *entry)
	{
		if (Controller::IsKeysPressed(Key::Y + Key::DPadRight))
		{
			s8 sPkt;
			s8 index;
			optKb->Populate(npcOpt);
			index = optKb->Open();
			Sleep(Milliseconds(100));
			switch (index)
			{
			case 0:
				optKb->Populate(npcNames);
				sPkt = optKb->Open();
				if (sPkt != -1)
				{
					if (Player::GetInstance(4, 1) != 0)
					{
						u32 cmp;
						u32 start = 0x33000000;
						if (sPkt == 0) cmp = 0xBF383C;
						if (sPkt == 1) cmp = 0xBF3CBC;
						if (sPkt == 2) cmp = 0x8F0FDC;
						if (sPkt == 3) cmp = 0xC5D250;
						if (sPkt == 4) cmp = 0xC2A3C4;
						if (sPkt == 5) cmp = 0xC2AF7C;
						if (sPkt == 6) cmp = 0xC2AE10;
						if (sPkt >= 7) cmp = 0xBA12B8;
						if (cmp == 0) goto no;
						Process::Write32(0x57B9C0, 0xE1A00000);
						while (*(u32 *)start != cmp)
						{
							start += 4;
							if (start > 0x33100000) goto no;
						}
						npc = start;
						OSD::Notify("NPC found at " << Hex(npc));
					}
					else goto no;
					return;
				no:
					OSD::Notify("NPC was not found.");
					return;
				}
				break;
			case 1:
				SetUpKB("Enter NPC animation ID:", true, 2, npcAnim, npcAnim);
				break;
			case 2:
				SetUpKB("Enter item ID to assign to the NPC animation:", true, 8, npcAnimItem, npcAnimItem);
				break;
			case 3:
				SetUpKB("Enter NPC snake ID\n(Use with NPC animation 0xD):", true, 3, npcSnake, npcSnake);
				break;
			case 4:
				SetUpKB("Enter NPC emote ID\n(Use with NPC animation 0xC):", true, 2, npcEmote, npcEmote);
				break;
			default:
				break;
			}
		}
	}

	void npcCoordinates(MenuEntry *entry)
	{
		if (npc != 0 && *((float *)((vu32)npc + 0x14)) != 0)
		{
			if (Controller::IsKeysDown(Key::A + Key::CPadRight)) *((float *)((vu32)npc + 0x14)) += coordSpeed;
			if (Controller::IsKeysDown(Key::A + Key::CPadLeft)) *((float *)((vu32)npc + 0x14)) -= coordSpeed;
			if (Controller::IsKeysDown(Key::A + Key::CPadDown)) *((float *)((vu32)npc + 0x1C)) += coordSpeed;
			if (Controller::IsKeysDown(Key::A + Key::CPadUp)) *((float *)((vu32)npc + 0x1C)) -= coordSpeed;
		}
		return;
	}

	void npcAnimation(MenuEntry *entry)
	{
		if (npc != 0)
		{
			if (turbo ? Controller::IsKeysDown(Key::Y + Key::A) : Controller::IsKeysPressed(Key::Y + Key::A))
			{
				u32 null[]{ 0 };
				switch (npcAnim)
				{
				case 0xC:
					Process::Write32((u32)&pfunction03, 0x6EC4E0);
					pfunction03(npc + 0x78, 0, npcEmote);
					break;
				case 0xD:
					Process::Write32((u32)&pfunction05, 0x6EB384);
					pfunction05(npc + 0x78, 0, npcSnake, 0, 0);
					break;
				case 0xE:
					Process::Write32((u32)&pfunction05, 0x6EE68C);
					pfunction05(npc + 0x78, 0, npcSnake, 0, 0);
					break;
				case 0x10:
					Process::Write32((u32)&pfunction06, 0x6EAECC);
					pfunction06(npc + 0x78, 0, npcSnake, npc + 0x14, 0x8816CC, 2/*GameHelper::PlaceItem(DropType, itemIDToReplace == 0xFFFFFFFF ? new u32{0x26} : &itemIDToReplace, (u32 *)inv, (u32 *)inv, *(u32 *)(npc + 0x78 + 0xA74), *(u32 *)(npc + 0x78 + 0xA78), 0, 0, 0, 0, 0)*/);
					Sleep(Milliseconds(40));
					break;
				case 0x12:
					Process::Write32((u32)&pfunction03, 0x6EE778);
					pfunction03(npc + 0x78, 0, (u32)&npcAnimItem);
					break;
				case 0x19:
					Process::Write32((u32)&pfunction03, 0x6EDBCC);
					pfunction03(npc + 0x78, 0, npcSnake);
					break;
				default:
					Process::Write32((u32)&pfunction08, 0x6E7D54);
					pfunction08(npc + 0x78, npcAnim, 0, 0xAE6864, (u32)null, (u32)null, 0, 0x8816C4);
					break;
				}
				//Process::Write8(npc + 0x78 + 0x474, npcAnim);
				//Process::Write8(npc + 0x78 + 0x4DC, 1);
			}
		}
	}

	//tour
	void bonusOre(MenuEntry *entry)
	{
		u16 item;
		if (Process::Read16(0x33038DD4, item))
			for(int i = 0; i < 0x10; i++)
				writeItem(item, i * 4);
	}

	void instantFruit(MenuEntry *entry)
	{
		u32 val;

		val = GameHelper::GetOnlinePlayerIndex();
		if (val <= 3 && val >= 0)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					writeItem(*(u16 *)(0x33038DD4 + 2 * (val * 3 + i)), 4 * (i * 3 + j));
				}
			}
		}
	}

	void tourCorrupter(MenuEntry *entry)
	{
		//0x33037660: tour data
		u32 addr = 0x33037668;
		u8 number;
		for (u32 i = 0; i < 0x500; i++)
		{
			number = Utils::Random(0, 6);
			if (number == 4) number = 0x64;
			if (number == 3) number = 0xC;
			if (number == 2) number = 4;
			if (number == 5) number = 1;
			Process::Write8(addr + i, number);
		}
		Sleep(Milliseconds(100));
		MessageBox("Tours have been corrupted.")();
	}
	
	//mods
	void playerSelector(MenuEntry *entry)
	{
		if (Controller::IsKeysPressed(Key::B + Key::DPadRight) && !animExecuting)
		{
			playerindex = (playerindex + 1) % 4;
			OSD::Notify("Player " << Hex(playerindex) << " selected!");
		}
	}

	void fast(MenuEntry *entry)
	{
		if (Controller::IsKeysPressed(Key::R + Key::CPadDown))
		{
			turbo = !turbo;
			if (turbo) OSD::Notify("Fast mode: Enabled!");
			else OSD::Notify("Fast mode: Disabled!");
		}
	}

	bool isReplaceable(u32 *item)
	{
		if (itemIDToReplace == 0xFFFFFFFF || *item == itemIDToReplace || (*item == 0x80007FFE && itemIDToReplace == 0x7FFE)) return true;
		else return false;
	}

    void walletfix(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
        {
            Process::Write16(0x19F6F8, 0x2117);
            Process::Write32(0x2C02C0, 0x000F423F);
        }
        else if (!entry->IsActivated())
        {
            Process::Write16(0x19F6F8, 0x20BE);
            Process::Write32(0x2C02C0, 0x0001869F);
        }
    }
	
   void	patternedit(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
            Process::Patch(0x2FEC44, 0xE3A00001);

        if (!entry->IsActivated())
            Process::Patch(0x2FEC44, 0xE3A00000);
    }

	void instantDrop(MenuEntry *entry)
	{
		Hook h;
		if (entry->WasJustActivated())
		{
			Process::Write32(0x165528, 0xE1A00000);
			h.Initialize(0x165528, (u32)&isReplaceable);
			h.Enable();
		}
		if (turbo ? Controller::IsKeysDown(Key::L + Key::DPadDown) : Controller::IsKeysPressed(Key::L + Key::DPadDown))
		{
			u32 wX, wY, u0;
			if (DropRoom == 0xA5 ? GameHelper::dropCheck(&wX, &wY, &u0, 0, 0) : Player::GetWorldCoords(&wX, &wY, 4, 1))
			{
				GameHelper::PlaceItemWrapper(DropType, itemIDToReplace, *(u16 *)inv == 0x339F ? (u32 *)0x597870 : (u32 *)inv, (u32 *)inv, wX, wY, 0, 0, 0, 0, 0, waitAnim, DropRoom);
			}
		}
	}

	void touchDrop(MenuEntry *entry)
	{
		if ((turbo ? Touch::IsDown() : Controller::IsKeyPressed(Key::Touchpad)) && *Mapbool == 1)
		{
			UIntVector pos = Touch::GetPosition();
			u32 worldx, worldy; 
			switch (*(u8 *)room)
			{
			case 0:
				if (pos.x >= 68 && pos.y >= 50 && pos.x <= 245 && pos.y <= 192)
				{
					worldx = ((pos.x - 68) / 2.2125) + 0x10;
					worldy = ((pos.y - 50) / 2.2125) + 0x10;
				}
				else return;
				break;
			case 0x68:
				if (pos.x >= 116 && pos.y >= 70 && pos.x <= 201 && pos.y <= 157)
				{
					worldx = ((pos.x - 116) / 2.65) + 0x10;
					worldy = ((pos.y - 70) / 2.65) + 0x10;
				}
				else return;
				break;
			default:
				return;
			}
			u32 player = Player::GetInstance(4, 1);
			if (player != 0) 
			{
				GameHelper::PlaceItemWrapper(DropType, itemIDToReplace, (u32 *)inv, (u32 *)inv, worldx, worldy, 0, 0, 0, 0, 0, waitAnim, 0xA5);
			}
		}
	}

	void menuTouchDrop(MenuEntry *entry)
	{
		/*optKb->Populate(cmnOpt);
		optKb->GetMessage() = "Bypass replace check?";
		switch (optKb->Open())
		{
		case 0:
			Process::Write32(0x5A036C, 0xE3A00001);
			break;
		case 1:
			Process::Write32(0x5A036C, 0xEB0002E1);
			break;
		default:
			break;
		}
		optKb->GetMessage() = "Choose option:";*/
	}

    void	noklok(MenuEntry *entry)
    {
		if (entry->IsActivated())
		{
			OSD::Run(widget);
			Process::Patch(0x2936DC, 0xE3A02001);
		}
		if (!entry->IsActivated())
		{
			OSD::Stop(widget);
			Process::Patch(0x2936DC, 0xE3A02000);
		}
    }

	void nonesave(MenuEntry *entry)
    {
        optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case 0:
            Process::Patch(0x1A0968, 0xE1A00000);
            Process::Patch(0x1A096C, 0xE3A00000);
            Process::Patch(0x1A0970, 0xEB0E069C);
			break;
		case 1:
            Process::Patch(0x1A0968, 0xE3A01040);
            Process::Patch(0x1A096C, 0xE5900000);
            Process::Patch(0x1A0970, 0xEB14D0AB);
			break;
		default:
			break;
		}
    }

    void MusicOFF(MenuEntry *entry)
    {
        u32 SVA;
        if (SetUpKB("Enter Float! \n Default: 3F800000", true, 8, SVA, *(u32 *)0x27A318))
        {
            Process::Write32(0x27A318, SVA);
        }
    }
	
	void bgmchange(MenuEntry *entry)
	{
		static u8 bgmout = 0;
		Keyboard *kb = new Keyboard("Enter Music ID:");
		kb->IsHexadecimal(true);
		if (kb->Open(bgmout, bgmout) != -1)
		{
        OSD::Notify("Playing BGM 0x" << Utils::ToHex(bgmout));
        for (int i = 0x846784; i < 0x8468A0; i += 4)
            *(u8 *)i = bgmout;
		}
	}
	
	void OSVA(MenuEntry *entry)
    {
        u32 OVA;
        if (SetUpKB("Enter Float! \n Default: 3F800000", true, 8, OVA, *(u32 *)0x4C4D68))
        {
            Process::Write32(0x4C4D68, OVA);
        }
    }
	
	void MenuSoundAdjust(MenuEntry *entry)
    {
        u32 SAN;
        if (SetUpKB("Enter Float! \n Default: 3F800000", true, 8, SAN, *(u32 *)0x4CC95C))
        {
            Process::Write32(0x4CC95C, SAN);
        }
    }

	void TunePitch(MenuEntry *entry)
	{
		u32 TPI;
		if (SetUpKB("Enter value!", true, 8, TPI, *(u32 *)0x4D44B8))
		{
			Process::Write32(0x4D44B8, TPI);
		}
	}

	std::vector<std::string> g_player{
		"Player 1 (Island)",
		"Player 2 (Online)",
		"Player 3 (Online)",
		"Player 4 (Online)",
		"Player 1 (Town)",
		"Player 2 (Town)",
		"Player 3 (Town)",
		"Player 4 (Town)",
		"Back Up",
		"Restore",
	};

    void	playercopy(MenuEntry *entry)
    {
        
        u8 PlayerSlot;
    
        if (entry->Hotkeys[0].IsDown())
        {
            Keyboard    keyboard("Select a player to copy:", g_player);
            int         playerchoice = keyboard.Open();
            if (playerchoice < 0)
                return;
            
            Process::Read8(0x31FF5994, PlayerSlot);
            if (PlayerSlot == 4)
                PlayerSlot = 0;

            if (playerchoice == 8)
            {
                u32 savelocation;
                int checkexist;
                int	errorcheck;

                Sleep(Seconds(0.1f));
                checkexist = Directory::IsExists("/FOXXY");
                if (checkexist == 0)
                    Directory::Create("/FOXXY");

                Directory backup;
                Directory::Open(backup, "/FOXXY", false);
                File backupfile;
                errorcheck = File::Open(backupfile, "/FOXXY/playerdump.bin", File::RWC);
                if (errorcheck != 0)
                {
                    MessageBox("Error!") ();
                    return;
                }
                Process::Read32(0xAB014C, savelocation);
                backupfile.Dump(savelocation, 0x6BD0);
                backupfile.Close();
                MessageBox("Backup created!")();
            }
            else if (playerchoice == 9)
            {
                u32 savelocation;
                int	errorcheck;

                Sleep(Seconds(0.1f));
                Directory backup;
                Directory::Open(backup, "FOXXY", false);
                File backupfile;
                errorcheck = File::Open(backupfile, "/FOXXY/playerdump.bin", File::READ);
                if (errorcheck != 0)
                {
                    MessageBox("Error!")();
                    return;
                }
                Process::Read32(0xAB014C, savelocation);
                backupfile.Inject(savelocation, 0x6BD0);
                backupfile.Close();
                MessageBox("Backup restored!")();
            }
            else if (playerchoice == 0)
            {
                Process::Write32(0xAB014C, 0xAA4510);
                Process::Write8(0xAAE990 + PlayerSlot, 7);
            }
            else if (playerchoice > 3)
            {
                playerchoice -= 4;
                Process::Write32(0xAB014C, 0x31F27020 + (0xA480 * playerchoice));
                Process::Write8(0xAAE990 + PlayerSlot, playerchoice);
            }
            else if (playerchoice < 4)
            {
                playerchoice -= 1;
                Process::Write32(0xAB014C, 0xA85790 + (0xA480 * playerchoice));
                Process::Write8(0xAAE990 + PlayerSlot, playerchoice + 4);
            }
        }
    }

	void itemClipboard(MenuEntry *entry)
	{
		if (Controller::IsKeysPressed(Key::R + Key::CPadLeft))
		{
			int8_t index;
			static u8 width = 1, length = 1;
			static u8 pasteMethod = 1;
			u32 playerX, playerY;
			u8 outX, outY;
			u32 player = Player::GetInstance(4, 1);
			u32 count = 0;
			std::string name;
			std::vector<std::string> files;
			Directory folder;

			if (/*Player::GetLocation() != 0xFF ||*/player == 0) { OSD::Notify(/*"You have to be outside for this!"*/"Error"); return; }
			Player::GetWorldCoords(&playerX, &playerY, 4, 1);
			optKb->Populate(clipBoardOpt);
			index = optKb->Open();
			Sleep(Milliseconds(100));
			switch (index)
			{
			case -1:
				return;
			case 0:
			case 1:
				if (SetUpKB("Enter width:", false, 2, outX, width) && SetUpKB("Enter length:", false, 2, outY, length))
				{
					u16 *pItemAtCoords;

					width = outX & 0x3F; length = outY & 0x3F;
					if (outX == 0) width = 1;
					if (outY == 0) length = 1;

					//delete[] clipboard;
					//Sleep(Milliseconds(50));
					//clipboard = new u16[width * length];
					clipboard.clear();

					for (u8 x = 0; x < width; x++)
					{
						for (u8 y = 0; y < length; y++)
						{
							count++;
							pItemAtCoords = (u16 *)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x + playerX, y + playerY, 0);
							clipboard.push_back(pItemAtCoords != nullptr ? *pItemAtCoords : 0x7FFE);
						}
					}
					OSD::Notify(std::to_string(count) << " items copied!");
					Sleep(Milliseconds(50));
					if (index == 1) GameHelper::RemoveItems(false, playerX & 0xFF, playerY & 0xFF, width, length, false, false);
				}
				break;
			case 2:
				u32 item;

				if (clipboard.empty()) { OSD::Notify("Clipboard is empty!"); return; }
				if (pasteMethod == 1) // remove items, then paste
				{
					GameHelper::RemoveItems(true, playerX & 0xFF, playerY & 0xFF, width, length, false, false);
				}
				if (!bypassing)
				{
					Process::Write32(0x5A11C8, 0xE3E00000);
					Process::Write32(0x5A11CC, 0xEA000012);
					Process::Write32(0x5A13C8, 0xE3A00000);
					Process::Write32(0x5A13CC, 0xE8BD83F0);
				}
				Sleep(Milliseconds(200));
				//Process::Write32(0x5A13C8, 0xE3A00000); // do not create locked spots
				//Process::Write32(0x5A13CC, 0xE8BD83F0);
				for (u8 x = 0; x < width; x++)
				{
					for (u8 y = 0; y < length; y++)
					{
						if (GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x + playerX, y + playerY, 0) != 0)
						{
							
							item = clipboard.at(x * length + y);
							if (GameHelper::GetOnlinePlayerCount() > 1)
							{
								if (item != 0x7FFE) { GameHelper::PlaceItemWrapper(0x1, 0x7FFE, &item, &item, x + playerX, y + playerY, 0, 0, 0, 0, 0, 0x3D, 0xA5); count++; }
								if (count % 400 == 0) Sleep(Milliseconds(500));//Sleep(Milliseconds(40)); // wait for pkt
							}
							else if (item != 0x7FFE)
							{
								Process::Write32((u32)&pfunction05, 0x168E20);
								pfunction05(x + playerX, y + playerY, 0, *(u8 *)room, (u32)&item);
								Process::Write32((u32)&pfunction03, 0x59F144);
								pfunction03(x + playerX, y + playerY, 1);
								count++;
							}
						}
					}
				}
				//Process::Write32(0x5A13C8, 0xE24DD01C);
				//Process::Write32(0x5A13CC, 0xE1A07001);
				OSD::Notify(std::to_string(count) << " items pasted!"); Process::Write32(0x5A11C8, 0xE3E00000);
				if (!bypassing)
				{
					Process::Write32(0x5A11C8, 0xE1A05001);
					Process::Write32(0x5A11CC, 0x1A000001);
					Process::Write32(0x5A13C8, 0xE24DD01C);
					Process::Write32(0x5A13CC, 0xE1A07001);
				}
				break;
			case 3:
				optKb->Populate(cmnOpt);
				optKb->GetMessage() = "Enable paste with overwrite?\n(Enabled by default)";
				switch (optKb->Open())
				{
				default:
					break;
				case 0:
					pasteMethod = 1;
					break;
				case 1:
					pasteMethod = 0;
					break;
				}
				optKb->GetMessage() = "Choose option:";
				break;
			case 4:
				if (clipboard.empty()) { OSD::Notify("Clipboard is empty!"); return; }
				kb->SetMaxLength(16);
				kb->GetMessage() = "Enter name without extension:";
				if (kb->Open(name) == 0)
				{
					if (Directory::IsExists(clipBoardsPath) == 0) Directory::Create(clipBoardsPath);
					if (File::Exists(clipBoardsPath << name << ".bin") == 0) File::Create(clipBoardsPath << name << ".bin");
					File::Open(*file, clipBoardsPath << name << ".bin", File::Mode::WRITE);
					file->Flush();
					file->Write(&width, 1);
					file->Write(&length, 1);
					for (u32 i = 0; i < width * length; i++)
					{
						file->Write(&clipboard.at(i), 2);
					}
					file->Close();
					OSD::Notify("Saved as " << name << ".bin");
				}
				break;
			case 5:
				files.clear();
				clipboard.clear();
				Directory::Open(folder, clipBoardsPath, Directory::IsExists(clipBoardsPath) == 0);
				if (folder.ListFiles(files) > 0)
				{
					optKb->Populate(files);
					index = optKb->Open();
					if (index != -1)
					{
						File::Open(*file, clipBoardsPath << files.at(index), File::Mode::READ);
						file->Read(&width, 1);
						file->Read(&length, 1);
						for (u32 i = 0; i < width * length; i++)
						{
							file->Read(&count, 2);
							clipboard.push_back(count & 0xFFFF);
						}
						file->Close();
						OSD::Notify(files.at(index) << " loaded!");
					}
				}
				else OSD::Notify("No files found.");
				folder.Close();
				break;
			case 6:
				files.clear();

				Directory::Open(folder, clipBoardsPath, Directory::IsExists(clipBoardsPath) == 0);
				if (folder.ListFiles(files) > 0)
				{
					optKb->Populate(files);
					index = optKb->Open();
					if (index != -1)
					{
						File::Open(*file, clipBoardsPath << files.at(index), File::Mode::WRITE);
						file->Flush();
						file->Close();
						File::Remove(clipBoardsPath << files.at(index));
						OSD::Notify(files.at(index) << " deleted!");
					}
				}
				else OSD::Notify("No files found.");
				folder.Close();
				break;
			}
		}
	}

	bool players(const Screen &screen)
	{
		if(!screen.IsTop && *Mapbool == 1)
		{
			/*u32 x, y;
			if ((*(u8 *)room == 0 || *(u8 *)room == 0x6D) && *Mapbool == 1)
			{
				if (Player::GetWorldCoords(&x, &y, 0, 1)) screen.DrawRect((u32)(34.3f + x * 2.2225f), (u32)(15.3f + y * 2.2225f), 6, 6, Color::Blue);
				if (Player::GetWorldCoords(&x, &y, 1, 1)) screen.DrawRect((u32)(34.3f + x * 2.2225f), (u32)(15.3f + y * 2.2225f), 6, 6, Color::Red);
				if (Player::GetWorldCoords(&x, &y, 2, 1)) screen.DrawRect((u32)(34.3f + x * 2.2225f), (u32)(15.3f + y * 2.2225f), 6, 6, Color::Green);
				if (Player::GetWorldCoords(&x, &y, 3, 1)) screen.DrawRect((u32)(34.3f + x * 2.2225f), (u32)(15.3f + y * 2.2225f), 6, 6, Color::Yellow);
			}
			if (*(u8 *)room == 0x68 && *Mapbool == 1)
			{
				if (Player::GetWorldCoords(&x, &y, 0, 1)) screen.DrawRect((u32)(74.6f + x * 2.65f), (u32)(29.8f + y * 2.65f), 6, 6, Color::Blue);
				if (Player::GetWorldCoords(&x, &y, 1, 1)) screen.DrawRect((u32)(74.6f + x * 2.65f), (u32)(29.8f + y * 2.65f), 6, 6, Color::Red);
				if (Player::GetWorldCoords(&x, &y, 2, 1)) screen.DrawRect((u32)(74.6f + x * 2.65f), (u32)(29.8f + y * 2.65f), 6, 6, Color::Green);
				if (Player::GetWorldCoords(&x, &y, 3, 1)) screen.DrawRect((u32)(74.6f + x * 2.65f), (u32)(29.8f + y * 2.65f), 6, 6, Color::Yellow);
			}*/

			float X, Z;
			u32 *i = (u32 *)0x330B522C;

			if ((*(u8 *)room == 0 || (*(u8 *)room >= 0x6D && *(u8 *)room < 0x80)))
			{
				if (Player::GetCoordinatesRef(0, X, Z)) screen.DrawRect((u32)(32.0f + (X / 14.2f)), (u32)(13.0f + (Z / 14.2f)), 6, 6, Color::Blue);
				if (Player::GetCoordinatesRef(1, X, Z)) screen.DrawRect((u32)(32.0f + (X / 14.2f)), (u32)(13.0f + (Z / 14.2f)), 6, 6, Color::Red);
				if (Player::GetCoordinatesRef(2, X, Z)) screen.DrawRect((u32)(32.0f + (X / 14.2f)), (u32)(13.0f + (Z / 14.2f)), 6, 6, Color::Green);
				if (Player::GetCoordinatesRef(3, X, Z)) screen.DrawRect((u32)(32.0f + (X / 14.2f)), (u32)(13.0f + (Z / 14.2f)), 6, 6, Color::Yellow);
				if (selecting) screen.DrawRect((u32)(33.0f + ((selectedX * 0x20 + 0x10) / 14.2f)), (u32)(14.0f + ((selectedY * 0x20 + 0x10) / 14.2f)), 4, 4, Color::Black);
				if (NPC::GetCoordinatesRef(npc, X, Z)) screen.DrawRect((u32)(33.0f + (X / 14.2f)), (u32)(14.0f + (Z / 14.2f)), 4, 4, Color::Orange);
			}
			if (*(u8 *)room == 0x68)
			{
				if (Player::GetCoordinatesRef(0, X, Z)) screen.DrawRect((u32)(73.0f + (X / 12.1f)), (u32)(30.6f + (Z / 12.1f)), 6, 6, Color::Blue);
				if (Player::GetCoordinatesRef(1, X, Z)) screen.DrawRect((u32)(73.0f + (X / 12.1f)), (u32)(30.6f + (Z / 12.1f)), 6, 6, Color::Red);
				if (Player::GetCoordinatesRef(2, X, Z)) screen.DrawRect((u32)(73.0f + (X / 12.1f)), (u32)(30.6f + (Z / 12.1f)), 6, 6, Color::Green);
				if (Player::GetCoordinatesRef(3, X, Z)) screen.DrawRect((u32)(73.0f + (X / 12.1f)), (u32)(30.6f + (Z / 12.1f)), 6, 6, Color::Yellow);
				if (selecting) screen.DrawRect((u32)(74.0f + ((selectedX * 0x20 + 0x10) / 12.1f)), (u32)(31.6f + ((selectedY * 0x20 + 0x10) / 12.1f)), 4, 4, Color::Black);
			}
		}
	}

	void map(MenuEntry *entry)
	{
		optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case 0:
			OSD::Run(players);
			break;
		case 1:
			OSD::Stop(players);
			break;
		default:
			break;
		}
	}

	void miscFunctions(MenuEntry *entry)
	{
		u32 x, y;
		u16 shirt = Utils::Random(0x2493, 0x26f5);
		u16 pants = Utils::Random(0x26f8, 0x2776);
		u16 socks = Utils::Random(0x2777, 0x279e);
		u16 shoes = Utils::Random(0x279f, 0x27e5);
		u16 headgear = Utils::Random(0x280b, 0x28f3);
		u16 glasses = Utils::Random(0x28f5, 0x295b);
		u16 tool = Utils::Random(0x334C, 0x3380);
		u16 animS = Utils::Random(0x000D, 0x01FF);
		u16 soundR = Utils::Random(0x0500, 0x0E50);
		Process::Write32((u32)&pfunction04, 0x304A60);
		u8 readroom;
		
		if (Controller::IsKeysPressed(Key::B + Key::DPadDown) && !selecting && !animExecuting)
		{
			s8 val;
			static u32 lastExecuted;
			std::vector<std::string> buildingOpt
			{
				"Place at your location...",
				"Move to your location...",
				"Remove building...",
			};
			std::vector<u8> index;
			u32 player, targetPlayer, animInstance, x, y;
			u32 param[11], res;
			u8 roomID;


			optKb->Populate(funcOpt);
			switch (optKb->Open())
			{
			case 0:
				if (SetUpKB("Enter address of function:", true, 8, lastExecuted, lastExecuted))
				{
					u8 val;
					if (SetUpKB("Enter parameter count (max 11):", false, 2, val, 0))
					{
						val = val % 12;
						kb->SetMaxLength(8);
						kb->IsHexadecimal(true);
						for (int i = 0; i < val; i++)
						{
							Sleep(Milliseconds(100));
							kb->GetMessage() = "Enter value for param " << std::to_string(i) << ":\n(0xABCDEF for current player instance)";
							if (kb->Open(param[i]) != 0) return;
							if (param[i] == 0xABCDEF) param[i] = Player::GetInstance(4, 1);
						}
						Sleep(Milliseconds(100));
						switch (val)
						{
						case 0:
							Process::Write32((u32)&pfunction00, lastExecuted);
							res = pfunction00();
							break;
						case 1:
							Process::Write32((u32)&pfunction01, lastExecuted);
							res = pfunction01(param[0]);
							break;
						case 2:
							Process::Write32((u32)&pfunction02, lastExecuted);
							res = pfunction02(param[0], param[1]);
							break;
						case 3:
							Process::Write32((u32)&pfunction03, lastExecuted);
							res = pfunction03(param[0], param[1], param[2]);
							break;
						case 4:
							Process::Write32((u32)&pfunction04, lastExecuted);
							res = pfunction04(param[0], param[1], param[2], param[3]);
							break;
						case 5:
							Process::Write32((u32)&pfunction05, lastExecuted);
							res = pfunction05(param[0], param[1], param[2], param[3], param[4]);
							break;
						case 6:
							Process::Write32((u32)&pfunction06, lastExecuted);
							res = pfunction06(param[0], param[1], param[2], param[3], param[4], param[5]);
							break;
						case 7:
							Process::Write32((u32)&pfunction07, lastExecuted);
							res = pfunction07(param[0], param[1], param[2], param[3], param[4], param[5], param[6]);
							break;
						case 8:
							Process::Write32((u32)&pfunction08, lastExecuted);
							res = pfunction08(param[0], param[1], param[2], param[3], param[4], param[5], param[6], param[7]);
							break;
						case 9:
							Process::Write32((u32)&pfunction09, lastExecuted);
							res = pfunction09(param[0], param[1], param[2], param[3], param[4], param[5], param[6], param[7], param[8]);
							break;
						case 10:
							Process::Write32((u32)&pfunction010, lastExecuted);
							res = pfunction010(param[0], param[1], param[2], param[3], param[4], param[5], param[6], param[7], param[8], param[9]);
							break;
						case 11:
							Process::Write32((u32)&pfunction011, lastExecuted);
							res = pfunction011(param[0], param[1], param[2], param[3], param[4], param[5], param[6], param[7], param[8], param[9], param[10]);
							break;
						default:
							return;
						}
						OSD::Notify("Returned value: " << Hex(res));
					}
				}
				break;
				
			case 2:
				Sleep(Milliseconds(100));
				{
					Keyboard    keyboard("Choose Game Type", gametype);
					int         gametchoice2 = keyboard.Open();

					if (gametchoice2 < 0)
					return;

					Process::Write8(0x31FF59C9, gametchoice2);
				}
				break;

			case 1:
				Sleep(Milliseconds(100));
				optKb->Populate(buildingOpt);
				switch (optKb->Open())
				{
				case 0:
					Sleep(Milliseconds(100));
					Player::GetWorldCoords(&x, &y, 4, 1);
					if (SetUpKB("Enter ID:", 1, 2, player, 0))
					{
						player &= 0xFFFF;
						GameHelper::PlaceBuildingUpdateCollisions(x, y, player);
					}
					break;
				case 1:
					Sleep(Milliseconds(100));
					animInstance = 0x31F72E08;
					buildingOpt.clear();
					for (player = 0; player < 58; player++)
					{
						targetPlayer = animInstance + player * 4;
						if (*(u8 *)(targetPlayer) != 0xFC)
						{
							index.push_back(player);
							buildingOpt.push_back(IDList::GetBuildingName(*(u8 *)(targetPlayer)).erase(0, 5) << " X" << Hex(*(u8 *)(targetPlayer + 2)) << "Y" << Hex(*(u8 *)(targetPlayer + 3)));
						}
					}
					optKb->Populate(buildingOpt);
					val = optKb->Open();
					if (val == -1) return;
					Player::GetWorldCoords(&x, &y, 4, 1);
					Process::Write8(animInstance + index.at(val) * 4 + 2, x & 0xFF);
					Process::Write8(animInstance + index.at(val) * 4 + 3, y & 0xFF);
					break;
				case 2:
					Sleep(Milliseconds(100));
					animInstance = 0x31F72E08;
					buildingOpt.clear();
					for (player = 0; player < 58; player++)
					{
						targetPlayer = animInstance + player * 4;
						if (*(u8 *)(targetPlayer) != 0xFC)
						{
							index.push_back(player);
							buildingOpt.push_back(IDList::GetBuildingName(*(u8 *)(targetPlayer)).erase(0, 5) << " X" << Hex(*(u8 *)(targetPlayer + 2)) << "Y" << Hex(*(u8 *)(targetPlayer + 3)));
						}
					}
					optKb->Populate(buildingOpt);
					val = optKb->Open();
					if (val == -1) return;
					Process::Write32(animInstance + index.at(val) * 4, 0xFC);
					*(u8 *)(animInstance - 4) = *(u8 *)(animInstance - 4) - 1;
					break;
				default:
					break;
				}
				break;

			case 3:
				GameHelper::RemoveItems(true, 0, 0, 0xFF, 0xFF, true, true);
				break;

			case 4:
				if (!bypassing)
				{
					Process::Write32(0x5A11C8, 0xE3E00000);
					Process::Write32(0x5A11CC, 0xEA000012);
					Process::Write32(0x5A13C8, 0xE3A00000);
					Process::Write32(0x5A13CC, 0xE8BD83F0);
				}
				Sleep(Milliseconds(100));
				player = 0;
				if (!SetUpKB("Enter the ID of the item you\nwould like to search\n(0xFFFFFFFF to replace everything):", true, 8, targetPlayer, 0x7FFE)) return;
				Sleep(Milliseconds(100));
				if (!SetUpKB("Enter the ID of the item you\nwould like to place:", true, 8, animInstance, *(u32 *)inv)) return;
				Sleep(Milliseconds(100));
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
				
				res = 1;
				while (res == 1)
				{
					while (res == 1)
					{
						if ((u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) != 0)
						{
							if (GameHelper::PlaceItemWrapper(0x1, targetPlayer, &animInstance, &animInstance, x, y, 0, 0, 0, 0, 0, 0x3D, 0xA5))
							{
								player++;
								if (player % 400 == 0) Sleep(Milliseconds(500));
							}
						}
						else
						{
							res = 0;
						}
						y++;
					}
					res = 1;
					if (Player::GetLocation() == 0xFF) y = 0x10;
					else y = 0;
					x++;
					if ((u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) == 0)
					{
						res = 0;
					}
				}
				OSD::Notify(std::to_string(player) << " items replaced!");
				if (!bypassing)
				{
					Process::Write32(0x5A11C8, 0xE1A05001);
					Process::Write32(0x5A11CC, 0x1A000001);
					Process::Write32(0x5A13C8, 0xE24DD01C);
					Process::Write32(0x5A13CC, 0xE1A07001);
				}
				break;

			case 5:
				Process::Read8(0x94F4C5, readroom);
				
				{
					pfunction04(readroom, 1, 1, 0);
				}
				break;

			case 6:
				Player::GetWorldCoords(&x, &y, 4, 1);
				if (bypassing)
				{
					Process::Write32(0x5A11C8, 0xE1A05001);
					Process::Write32(0x5A11CC, 0x1A000001);
					Process::Write32(0x5A13C8, 0xE24DD01C);
					Process::Write32(0x5A13CC, 0xE1A07001);
				}
				Sleep(Milliseconds(5));
				if (GameHelper::createLockedSpot(0x12, x, y, *(u8 *)room, true) == 0xFFFFFFFF)
				{
					OSD::Notify("Error: Too many locked spots are already existing!");
				}
				else OSD::Notify("Success!");
				if (bypassing)
				{
					Process::Write32(0x5A11C8, 0xE3E00000);
					Process::Write32(0x5A11CC, 0xEA000012);
					Process::Write32(0x5A13C8, 0xE3A00000);
					Process::Write32(0x5A13CC, 0xE8BD83F0);
				}
				break;

			case 7:
				if (bypassing)
				{
					Process::Write32(0x5A11C8, 0xE1A05001);
					Process::Write32(0x5A11CC, 0x1A000001);
					Process::Write32(0x5A13C8, 0xE24DD01C);
					Process::Write32(0x5A13CC, 0xE1A07001);
				}
				Sleep(Milliseconds(5));
				Player::GetWorldCoords(&x, &y, 4, 1);
				GameHelper::clearLockedSpot(x, y, *(u8 *)room, 4);
				OSD::Notify("Unlocked.");
				if (bypassing)
				{
					Process::Write32(0x5A11C8, 0xE3E00000);
					Process::Write32(0x5A11CC, 0xEA000012);
					Process::Write32(0x5A13C8, 0xE3A00000);
					Process::Write32(0x5A13CC, 0xE8BD83F0);
				}
				break;

			case 8:
				if (bypassing)
				{
					Process::Write32(0x5A11C8, 0xE1A05001);
					Process::Write32(0x5A11CC, 0x1A000001);
					Process::Write32(0x5A13C8, 0xE24DD01C);
					Process::Write32(0x5A13CC, 0xE1A07001);
				}
				res = 0;
				roomID = 0;
				Sleep(Milliseconds(5));
				while (GameHelper::createLockedSpot(0x12, 0x10 + roomID, 0x10 + res, *(u8 *)room, true) != 0xFFFFFFFF)
				{
					roomID++;
					if (roomID % 6 == 2) { res++; roomID = 0; }
					Sleep(Milliseconds(40));
				}
				OSD::Notify("Done.");
				if (bypassing)
				{
					Process::Write32(0x5A11C8, 0xE3E00000);
					Process::Write32(0x5A11CC, 0xEA000012);
					Process::Write32(0x5A13C8, 0xE3A00000);
					Process::Write32(0x5A13CC, 0xE8BD83F0);
				}
				break;

			case 9:
				if (bypassing)
				{
					Process::Write32(0x5A11C8, 0xE1A05001);
					Process::Write32(0x5A11CC, 0x1A000001);
					Process::Write32(0x5A13C8, 0xE24DD01C);
					Process::Write32(0x5A13CC, 0xE1A07001);
				}
				Sleep(Milliseconds(5));
				x = 0x10;
				y = 0x10;
				res = 1;
				while (res == 1)
				{
					while (res == 1)
					{
						if ((u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) != 0)
						{
							if (GameHelper::GetLockedSpotIndex(x, y, *(u8 *)room) != 0xFFFFFFFF)
							{
								animInstance++;
								GameHelper::clearLockedSpot(x, y, *(u8 *)room, 4);
								Sleep(Milliseconds(40));
							}
						}
						else
						{
							res = 0;
						}
						y++;
					}
					res = 1;
					y = 0x10;
					x++;
					if ((u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) == 0)
					{
						res = 0;
					}
				}
				OSD::Notify("Done.");
				Sleep(Milliseconds(5));
				if (bypassing)
				{
					Process::Write32(0x5A11C8, 0xE3E00000);
					Process::Write32(0x5A11CC, 0xEA000012);
					Process::Write32(0x5A13C8, 0xE3A00000);
					Process::Write32(0x5A13CC, 0xE8BD83F0);
				}
				break;

			case 10:
				Sleep(Milliseconds(100));
				if (Player::GetWorldCoords(&x, &y, 4, 1))
				{
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x38, tool, 1, 1, 1, 0, x, y, true);
					OSD::Notify("Tool: " << Hex(tool), Color::Green);
				}
				break;

			case 11:
				Sleep(Milliseconds(100));
				if (Player::GetWorldCoords(&x, &y, 4, 1))
				{
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x36, headgear, 0, 0, 1, 0, x, y, true);
					OSD::Notify("Headgear: " << Hex(headgear), Color::Red);
				}
				if (Player::GetWorldCoords(&x, &y, 4, 1))
				{
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x36, glasses, 1, 1, 1, 0, x, y, true);
					OSD::Notify("Glasses: " << Hex(glasses), Color::Orange);
				}
				if (Player::GetWorldCoords(&x, &y, 4, 1))
				{
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x36, shirt, 2, 2, 1, 0, x, y, true);
					OSD::Notify("Shirt: " << Hex(shirt), Color::Yellow);
				}
				if (Player::GetWorldCoords(&x, &y, 4, 1))
				{
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x36, pants, 4, 4, 1, 0, x, y, true);
					OSD::Notify("Pants: " << Hex(pants), Color::Green);
				}
				if (Player::GetWorldCoords(&x, &y, 4, 1))
				{
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x36, socks, 5, 5, 1, 0, x, y, true);
					OSD::Notify("Socks: " << Hex(socks), Color::Blue);
				}
				if (Player::GetWorldCoords(&x, &y, 4, 1))
				{
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x36, shoes, 6, 6, 1, 0, x, y, true);
					OSD::Notify("Shoes: " << Hex(shoes), Color::Purple);
				}
				if (Player::GetWorldCoords(&x, &y, 4, 1))
				{
					Player::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex() & 3, 0x6, 1, 1, 1, 1, 0, x, y, true);
				}
				break;

			default:
				break;
			}
		}
		
	}

	void tileSelector(MenuEntry *entry)
	{
		if (Player::GetInstance(playerindex, 1) == 0) return;
		static u32 keyPressedTicks = 0, DPadKeyPressedTicks = 0;
		static u8 size;
		static bool removal = false;
		float Y = *(float *)((u32)Player::GetCoordinates(playerindex) + 4);
		float particleCoords[3]{ 0, Y, 0 };
		u32 pItem;

		if (!selecting) Player::GetWorldCoords(&selectedX, &selectedY, playerindex, 1);
		pItem = (u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), selectedX, selectedY, 0);
		if (Controller::IsKeysPressed(Key::B + Key::DPadUp) && !animExecuting)
		{
			if (selecting)
			{
				//Process::Write32(0x764504, 0x2A000020); //lock camera
				Process::Write32(0x1A51C8, 0xE2805C01);
				OSD::Notify("Editor: Off",  Color::Red);
				selecting = false;
			}
			else
			{
				//Process::Write32(0x764504, 0xEA000020); //unlock camera
				Process::Write32(0x1A51C8, 0xE8BD81F0);
				OSD::Notify("Editor: On", Color::Green);
				selecting = true;

				*(float *)(Camera::GetInstance() + 4) = (float)(selectedX * 0x20 + 0x10);
				Camera::AddToY(90.0f);
				*(float *)(Camera::GetInstance() + 0xC) = (float)(selectedY * 0x20 + 0x70);
				Camera::AddToYRotation(0x700);
			}
		}

		if (selecting)
		{
			if (Controller::IsKeyDown(Key::DPadRight) || Controller::IsKeyPressed(Key::DPadRight))
			{
				DPadKeyPressedTicks++;
				if ((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100)
				{
					selectedX += 1;
					Camera::AddToX(32.0f);
				}
			}
			if (Controller::IsKeyDown(Key::DPadLeft) || Controller::IsKeyPressed(Key::DPadLeft))
			{
				DPadKeyPressedTicks++;
				if ((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100)
				{
					selectedX -= 1;
					Camera::AddToX(-32.0f);
				}
			}
			if (Controller::IsKeyDown(Key::DPadDown) || Controller::IsKeyPressed(Key::DPadDown))
			{
				DPadKeyPressedTicks++;
				if ((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100)
				{
					selectedY += 1;
					Camera::AddToZ(32.0f);
				}
			}
			if (Controller::IsKeyDown(Key::DPadUp) || Controller::IsKeyPressed(Key::DPadUp))
			{
				DPadKeyPressedTicks++;
				if ((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100)
				{
					selectedY -= 1;
					Camera::AddToZ(-32.0f);
				}
			}
			if (Controller::IsKeyReleased(Key::L) || Controller::IsKeyReleased(Key::R)) keyPressedTicks = 0;
			if (Controller::IsKeyReleased(Key::DPadRight) || Controller::IsKeyReleased(Key::DPadLeft) || Controller::IsKeyReleased(Key::DPadDown) || Controller::IsKeyReleased(Key::DPadUp)) DPadKeyPressedTicks = 0;
			if (Controller::IsKeyDown(Key::L))
			{
				keyPressedTicks++;
				if ((keyPressedTicks < 90 ? (keyPressedTicks % 10) == 1 : (keyPressedTicks % 3) == 1) || keyPressedTicks > 220) selectedItem = (selectedItem - 1 == 0x1FFF ? 0xFD : selectedItem - 1) % 0x4000;
			}
			if (Controller::IsKeyDown(Key::R))
			{
				keyPressedTicks++;
				if ((keyPressedTicks < 90 ? (keyPressedTicks % 10) == 1 : (keyPressedTicks % 3) == 1) || keyPressedTicks > 220) selectedItem = (selectedItem + 1 == 0xFE ? 0x2000 : selectedItem + 1) % 0x4000;
			}
			if (Controller::IsKeysPressed(Key::B + Key::DPadDown))
			{
				size++;
				if (size >= 4) size = 0;
				OSD::Notify("Size set to " << std::to_string(size));
			}
			if (Controller::IsKeysPressed(Key::B + Key::DPadLeft))
			{
				if (removal) { OSD::Notify("Removal mode: Disabled!"); removal = false; }
				else { OSD::Notify("Removal mode: Enabled!"); removal = true; }
			}
			if (turbo ? Controller::IsKeyDown(Key::A) : Controller::IsKeyPressed(Key::A))
			{
				if (pItem == 0) return;
				for (int8_t i = -size; i <= size; i++)
				{
					for (int8_t j = -size; j <= size; j++)
					{
						if (!removal) GameHelper::PlaceItemWrapper(DropType, itemIDToReplace, &selectedItem, &selectedItem, (selectedX + j), (selectedY + i), 0, 0, 0, 0, 0, waitAnim, 0xA5);
						else GameHelper::TrampleAt((selectedX + j), (selectedY + i));
					}
				}
			}
			for (int8_t i = -size; i <= size; i++)
			{
				for (int8_t j = -size; j <= size; j++)
				{
					particleCoords[0] = (selectedX + j) * 0x20 + 0x10;
					particleCoords[2] = (selectedY + i) * 0x20 + 0x10;
					GameHelper::Particles(particleID, particleCoords);
				}
			}
		}
	}

	void autoDrop(MenuEntry *entry)
	{
		
		static bool enabled = false;
		Hook h;
		u32 wX, wY, u0;
		if (entry->WasJustActivated())
		{
			Process::Write32(0x165528, 0xE1A00000);
			h.Initialize(0x165528, (u32)&isReplaceable);
			h.Enable();
		}
		if (Controller::IsKeysPressed(Key::B + Key::DPadLeft) && !selecting && !animExecuting)
		{
			if (enabled)
			{
				enabled = false;
				OSD::Notify("Auto drop: Disabled!");
				//Process::Write32(0x70B68C, 0xEA00EDC3);
			}
			else
			{
				enabled = true;
				OSD::Notify("Auto drop: Enabled!");
				//Process::Write32(0x70B68C, 0xE3A00001);
			}
		}
		if (enabled)
		{
			if (GameHelper::dropCheck(&wX, &wY, &u0, 0, 0))
			{
				GameHelper::PlaceItemWrapper(DropType, itemIDToReplace, *(u16 *)inv == 0x339F ? (u32 *)0x597870 : (u32 *)inv, (u32 *)inv, wX, wY, 0, 0, 0, 0, 0, waitAnim, 0xA5);
			}
		}
	}

	bool debugOSD(const Screen &screen)
	{
		Color playerColor;
		u32 player = Player::GetInstance(playerindex, 1);

		switch (playerindex)
		{
		case 0:
			playerColor = Color::Blue;
			break;
		case 1:
			playerColor = Color::Red;
			break;
		case 2:
			playerColor = Color::Green;
			break;
		case 3:
			playerColor = Color::Yellow;
			break;
		}

		if (screen.IsTop && player != 0)
		{
			/*static u32 wX, wY, selectedItem = 0x10, keyPressedTicks = 0, DPadKeyPressedTicks = 0;
			static u8 size;
			static bool removal = 0;*/
			u32 y = 0, pItem;
			float X = *Player::GetCoordinates(playerindex), Z = *(float *)((u32)Player::GetCoordinates(playerindex) + 8);
			//float Y = *(float *)((u32)Player::GetCoordinates(playerindex) + 4);
			//float particleCoords[3]{ 0, Y, 0 };*/

			if (!selecting) Player::GetWorldCoords(&selectedX, &selectedY, playerindex, 1);
			pItem = (u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), selectedX, selectedY, 0);
			
			/*
			if (Controller::IsKeysPressed(Key::B + Key::DPadUp)) 
			{ 
				if (selecting)
				{
					Process::Write32(0x764504, 0x2A000020); //lock camera
					OSD::Notify("Stopped selection.");
					selecting = false;
				}
				else
				{
					Process::Write32(0x764504, 0xEA000020); //unlock camera
					OSD::Notify("Started selection.");
					selecting = true;

					
					*(float *)0x9866F4 = wX * 0x20 + 0x10;
					*(float *)0x9866FC = wY * 0x20 + 0x10;
				}
			}
			
			if (selecting)
			{
				if (Controller::IsKeyDown(Key::DPadRight) || Controller::IsKeyPressed(Key::DPadRight)) 
				{ 
					DPadKeyPressedTicks++;
					if (DPadKeyPressedTicks % 10 == 2)
					{
						wX += 1;
						*(float *)0x9866F4 += 32.0f;
					}
				}
				if (Controller::IsKeyDown(Key::DPadLeft) || Controller::IsKeyPressed(Key::DPadLeft))
				{ 
					DPadKeyPressedTicks++; 
					if (DPadKeyPressedTicks % 10 == 2)
					{
						wX -= 1;
						*(float *)0x9866F4 -= 32.0f;
					}
				}
				if (Controller::IsKeyDown(Key::DPadDown) || Controller::IsKeyPressed(Key::DPadDown))
				{ 
					DPadKeyPressedTicks++;
					if (DPadKeyPressedTicks % 10 == 2)
					{
						wY += 1;
						*(float *)0x9866FC += 32.0f;
					}
				}
				if (Controller::IsKeyDown(Key::DPadUp) || Controller::IsKeyPressed(Key::DPadUp))
				{ 
					DPadKeyPressedTicks++;
					if (DPadKeyPressedTicks % 10 == 2)
					{
						wY -= 1;
						*(float *)0x9866FC -= 32.0f;
					}
				}
				if (Controller::IsKeyReleased(Key::L) || Controller::IsKeyReleased(Key::R)) keyPressedTicks = 0;
				if (Controller::IsKeysReleased(Key::DPadRight | Key::DPadLeft | Key::DPadDown | Key::DPadUp)) DPadKeyPressedTicks = 0;
				if (Controller::IsKeyDown(Key::L))
				{
					keyPressedTicks++;
					if ((keyPressedTicks < 110 ? (keyPressedTicks % 10) == 1 : (keyPressedTicks % 3) == 1) || keyPressedTicks > 250) selectedItem = (selectedItem - 1 == 0x1FFF ? 0xFD : selectedItem - 1) % 0x4000;
				}
				if (Controller::IsKeyDown(Key::R)) 
				{
					keyPressedTicks++;
					if ((keyPressedTicks < 110 ? (keyPressedTicks % 10) == 1 : (keyPressedTicks % 3) == 1) || keyPressedTicks > 250) selectedItem = (selectedItem + 1 == 0xFE ? 0x2000 : selectedItem + 1) % 0x4000;
				}
				if (Controller::IsKeysPressed(Key::B + Key::DPadDown))
				{
					size++;
					if (size >= 3) size = 0;
					OSD::Notify("Size set to " << std::to_string(size));
				}
				if (Controller::IsKeysPressed(Key::B + Key::DPadLeft))
				{
					if (removal) { OSD::Notify("Removal mode: Disabled!"); removal = false; }
					else { OSD::Notify("Removal mode: Enabled!"); removal = true; }
				}
				if (turbo ? Controller::IsKeyDown(Key::A) : Controller::IsKeyPressed(Key::A))
				{
					if (pItem == 0) return 0;
					for (int8_t i = -size; i <= size; i++)
					{
						for (int8_t j = -size; j <= size; j++)
						{
							GameHelper::PlaceItemSupportedDropIDs(DropType, itemIDToReplace, removal ? (u32 *)0x95CFFC : &selectedItem, removal ? (u32 *)0x95CFFC : &selectedItem, (wX + j), (wY + i), 0, 0, 0, 0, 0, waitAnim, 0xA5);
						}
					}	
				}
				for (int8_t i = -size; i <= size; i++)
				{
					for (int8_t j = -size; j <= size; j++)
					{
						particleCoords[0] = (wX + j) * 0x20 + 0x10;
						particleCoords[2] = (wY + i) * 0x20 + 0x10;
						GameHelper::Particles(particleID, particleCoords);
					}
				}
			}*/
			

			y = screen.Draw("Player " << Hex(playerindex), 0, y, playerColor);
			y = screen.Draw("Coordinates: " << std::to_string(X).erase(4) << "|" << std::to_string(Z).erase(4), 0, y);
			y = screen.Draw("X|Y: " << Hex((u8)(selectedX & 0xFF)) << "|" << Hex((u8)(selectedY & 0xFF)), 0, y);
			y = screen.Draw("Speed: " << std::to_string(*Player::GetVelocity(playerindex)).erase(4), 0, y);
			y = screen.Draw("Animation: " << Hex(*Player::GetAnimation(playerindex)), 0, y);
			y = screen.Draw("Standing on: " << (pItem != 0 ? Hex(*(u32 *)pItem) : "N/A") << (GameHelper::GetLockedSpotIndex(selectedX, selectedY) != 0xFFFFFFFF ? " (locked)" : ""), 0, y);
			y = screen.Draw("MAP: " << Hex(*(u8 *)room) << " / " << Hex(GameHelper::GetCurrentMap()), 0, y);
			//y = screen.Draw("+0x171: " << Hex((u8)(*(u8 *)(player + 0x171))), 0, y);
			//y = screen.Draw("Ptr: " << Hex(player), 0, y);
			//y = screen.Draw("Index in instance: " << Hex(*(u8 *)(player + 0x1AC)), 0, y);
		}
		else if (screen.IsTop)
		{
			u32 y;
			u32 animObjPtr;
			Process::Write32((u32)&pfunction01, 0x5C3DDC);
			animObjPtr = pfunction01(playerindex);
			
			if (animObjPtr != 0 && *(u8 *)animObjPtr != 0xFF)
			{
				y = screen.Draw("Player " << Hex(playerindex), 0, y, playerColor);
				y = screen.Draw("Animation: " << Hex(*(u8 *)(animObjPtr + 1)), 0, y);
				y = screen.Draw("MAP: " << Hex(playerindex == GameHelper::GetOnlinePlayerIndex() ? *(u8 *)room : *(u8 *)animObjPtr), 0, y);
			}
		}
	}

	void debug(MenuEntry *entry)
	{
		optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case 0:
			OSD::Run(debugOSD);
			break;
		case 1:
			OSD::Stop(debugOSD);
			break;
		default:
			break;
		}
	}

	//useless
	void playerSize(MenuEntry *entry)
    {
        u32 psize;
        if (SetUpKB("Enter Float! \n Default: 3F800000", true, 8, psize, *(u32 *)0x1ACE00))
        {
            Process::Write32(0x1ACE00, psize);
        }
    }

	void bugSize(MenuEntry *entry)
    {
        u32 bsize;
        if (SetUpKB("Enter Float! \n Default: 3F800000", true, 8, bsize, *(u32 *)0x1F557C))
        {
            Process::Write32(0x1F557C, bsize);
        }
    }

	void rotatingFurn(MenuEntry *entry)
	{
		toggle2(0x571494, 0xE1A00000, 0xEA000020);
	}

	void bubblesDisappear(MenuEntry *entry)
	{
		toggle2(0x2145F8, 0xE1A00000, 0x0A000006);
	}


	//helper
	void writeItem(u32 item, u32 offset)
	{
		u64 cmp64;
		Process::Write32(inv + offset, item);
		if (Process::Read64(online_inv0, cmp64) && cmp64 != 0) Process::Write32(online_inv0 + offset, item);
		if (Process::Read64(online_inv1, cmp64) && cmp64 != 0) Process::Write32(online_inv1 + offset, item);
		if (Process::Read64(online_inv2, cmp64) && cmp64 != 0) Process::Write32(online_inv2 + offset, item);
		if (Process::Read64(online_inv3, cmp64) && cmp64 != 0) Process::Write32(online_inv3 + offset, item);
	}

	void toggle(u32 addr[], u32 enableVal[], u32 disableVal[], u8 Length)
	{
		optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case -1:
			break;
		case 0:
			for (u8 i = 0; i < Length; i++)
				Process::Write32(addr[i], enableVal[i]);
			break;
		case 1:
			for (u8 i = 0; i < Length; i++)
				Process::Write32(addr[i], disableVal[i]);
			break;
		}
	}

	void toggle2(u32 addr, u32 enableVal, u32 disableVal)
	{
		optKb->Populate(cmnOpt);
		switch (optKb->Open())
		{
		case -1:
			break;
		case 0:
			Process::Write32(addr, enableVal);
			break;
		case 1:
			Process::Write32(addr, disableVal);
			break;
		}
	}

	bool SetUpKB(const std::string &msg, bool hex, const int length, u32 &output, u32 def, OnInputChange cb)
	{
		Sleep(Milliseconds(100));
		kb->GetMessage() = msg;
		kb->IsHexadecimal(hex);
		kb->SetMaxLength(length);
		kb->OnInputChange(cb);
		return kb->Open(output, def) == 0;
	}

	bool SetUpKB(const std::string &msg, bool hex, const int length, u16 &output, u16 def, OnInputChange cb)
	{
		Sleep(Milliseconds(100));
		kb->GetMessage() = msg;
		kb->IsHexadecimal(hex);
		kb->SetMaxLength(length);
		kb->OnInputChange(cb);
		return kb->Open(output, def) == 0;
	}

	bool SetUpKB(const std::string &msg, bool hex, const int length, u8 &output, u8 def, OnInputChange cb)
	{
		Sleep(Milliseconds(100));
		kb->GetMessage() = msg;
		kb->IsHexadecimal(hex);
		kb->SetMaxLength(length);
		kb->OnInputChange(cb);
		return kb->Open(output, def) == 0;
	}

	bool SetUpAnimKB(u8 &output, u8 def)
	{
		Sleep(Milliseconds(100));
		kb->GetMessage() = "Enter Animation ID:";
		kb->IsHexadecimal(true);
		kb->SetMaxLength(2);
		kb->OnInputChange(nullptr);
		return kb->Open(output, def) == 0;
	}

	void cleanC()
	{
		delete kb;
		delete optKb;
		delete &isl;
		delete file;
	}
}