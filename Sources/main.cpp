#include "3ds.h"
#include "CTRPluginFramework.hpp"
#include "cheats.hpp"
#include "text_color.h"
#include "PlayerHelper.hpp"


namespace CTRPluginFramework
{
	void	PatchProcess(FwkSettings &settings)
	{
		settings.AllowSearchEngine = true;
		if (Process::GetTitleID() == 0x0004000000086300)
		{
			const u32 nop = 0xE1A00000;
			const u32 dropReplaceSwitchAddr = 0x5A1120;
			//inventory base
			Process::Write32(0x19B694, nop);
			Process::Write32(0x19BF2C, nop);
			Process::Write32(0x19BA78, nop);
			Process::Write32(0x19B988, nop);
			Process::Write32(0x19C050, nop);
			Process::Write32(0x725124, nop);
			Process::Write32(0x19B5BC, 0xE3A06001); //items2money
			//pitfall
			Process::Write32(0x6789E4, 0xEA00002D);
			Process::Write32(0x65A668, 0xEA000014);
			//validate
			Process::Write32(0x76A894, 0xE3A00001);
			//no trample
			Process::Write32(0x597F58, 0xE3A0001D);
			//pickup
			Process::Write32(0x59A0D0, nop);
			Process::Write32(0x59A120, 0xEA000004);
			Process::Write32(0x59A268, 0x31F2DBF0);
			//drop
			Process::Write32(0x59FCA4, 0xEA000004);
			//catch animation speed
			Process::Write32(0x6737AC, 0x45000000);
			Process::Write32(0x66FFA0, 0x45000000);
			//drop replace
			for (int i = 0; i <= 0x21; i++) Process::Write32(0x5A0F54 + i * 4, dropReplaceSwitchAddr);
			// prevent particle crash
			Process::Write32(0x5506D4, 0xE3A0C000);
			Process::Write32(0x5509CC, 0xE3A0C000);
			Process::Write32(0x721418, 0xE3A0C000);
			// idle after tree shake/cut
			Process::Write8(0x660600, 6);
			Process::Write8(0x662328, 6);
			//Process::Write8(0x651FCC, 6); // idle after anim 0x36
			// shovel animations
			Process::Write32(0x66479C, 0xEA000028);
			// drop everywhere
			Process::Write32(0x1655EC, 0xE3A00001);
			Process::Write32(0x1655F8, 0xEA000006);
			Process::Write32(0x1654EC, 0xEA000005);
			Process::Write32(0x165580, 0xEA000010); // on other players
			// host drop
			Process::Write32(0x5A1454, nop); //create locked spot pkt send
			Process::Write32(0x5A12F4, nop); //clear locked spot pkt send
			Process::Write32(0x59FF5C, 0xEA000010); //pkt is from host
			Process::Write32(0x59FD98, 0xEA000003); //bypass check in 0x59FC7C
			Process::Write32(0x5983F4, nop); //unnecessary?
			Process::Write32(0x5984DC, nop); //unnecessary?
			Process::Write32(0x59F8A0, nop);
			Process::Write32(0x5A09B0, 0xEA000022); //item tree drop
			Process::Write32(0x5A0DF4, 0xEA00002D); //fruit tree drop
			//fast pull
			Process::WriteFloat(0x67C220, 200.0f); 
			Process::WriteFloat(0x67C224, 200.0f); 
			//item to inv after pick up
			Process::Write32(0x323514, 0xE3A00001);
			//prevent rockbreak crash
			Process::Write32(0x5A2D20, 0x000000F3);
			//Prevent others from crashing you
			Process::Write32(0x5980F8, 0xE3A00001); //Disable Sprite Spawn for Anim 0x5f and 0x60
			Process::Write32(0x5A26B4, nop); //Makes Burying spawn toss sprite on the ground
			Process::Write32(0x5A26F4, 0xE3A00001); //Disable Eat Item Sprite Spawn part 1
			Process::Write32(0x6517CC, 0x03500001); //Disable Eat Item Sprite Spawn part 2
			Process::Write32(0x6517E4, nop); //Another fix for buring items
			Process::Write32(0x651A9C, 0x01000540); //Fixes annoying as fuck sound
			Process::Write32(0x665E50, 0xE3A01006); //Replaces Animation for falling into hole after sit, lay down. Prevents you from accidentally crashing others.
			Process::Write32(0x68B57C, 0xE3A01016); //Changes "putinpocket" anim after eat
			//FIX ROOM EDIT MENU CRASH ONLINE
			Process::Write32(0x6D2B44, 0xE3A0008A); //Menu Change to error
			Process::Write32(0x6708A0, 0xE3A01016); //In-Pocket Animation
			//SANTA BAG FIX
			Process::Write16(0x68D0EC, 0x7FFE);
            Process::Write16(0x689574, 0x7FFE);
			//FAST TALK (NPC)
			Process::Write32(0x5FC6B0, 0xE3A00001);
			//BYPASS CHECKS
			Process::Write32(0x1D43A4, 0xE3A00001);
			Process::Write32(0x1D43C0, 0xE3A00001);
			Process::Write32(0x1D43D0, 0xE3A00001);
			Process::Write32(0x759024, 0xE1A00005);
		}
	}

    static MenuEntry *EntryWithHotkey(MenuEntry *entry, const Hotkey &hotkey)
    {
        if (entry != nullptr)
        {
            entry->Hotkeys += hotkey;
            entry->SetArg(new std::string(entry->Name()));
            entry->Name() += " " + hotkey.ToString();
            entry->Hotkeys.OnHotkeyChangeCallback([](MenuEntry *entry, int index)
            {
                std::string *name = reinterpret_cast<std::string *>(entry->GetArg());

                entry->Name() = *name + " " + entry->Hotkeys[0].ToString();
            });
        }

        return (entry);
    }

    static MenuEntry *EntryWithHotkey(MenuEntry *entry, const std::vector<Hotkey> &hotkeys)
    {
        if (entry != nullptr)
        {
            for (const Hotkey &hotkey : hotkeys)
                entry->Hotkeys += hotkey;
        }

        return (entry);
    }

	int	main()
	{
		if (Process::GetTitleID() == 0x0004000000086300)
		{
			PluginMenu *menu = new PluginMenu(Color(LoveBlue) << "\uE015Foxxy\uE015 by \uE017Jay\uE017", 2, 3, 1, "wat teh fek, im a pizza pie");
			menu->SynchronizeWithFrame(true);
			
			const std::vector<MenuEntry *> essentialEntries
			{
				new MenuEntry(Color(Blue01) << "Improved Wallet", walletfix, "Store up to 999,999 bells. Rich bitch."),
				new MenuEntry(Color(Blue01) << "Edit All Patterns", patternedit, "Edit any pattern as if they're your own."),
				new MenuEntry(Color(Blue01) << "Duplicate \uE053 (+ \uE002)", duplicate, "\uE053: Copy item from first slot to second slot\n\uE053 + \uE002: Copy item from first slot to all slots"),
				new MenuEntry(Color(Blue01) << "Wear helmet and accesory", hatz, "Wear a mask with a accesory. Make sure the accesory is on first! Thanks to Levi for making this awesome code! <3"),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Item Randomizer", itemrandom, "Randomize items, flags, and coordinates for items. When you're done, be sure to reset the drop area by pressing the defined hotkeys. "),{ Hotkey(Key::ZR, "Apply Items"), Hotkey(Key::ZR | Key::X, "Set items"), Hotkey(Key::L | Key::DPadDown, "Random Drop Range") }),
				new MenuEntry(Color(Blue01) << "Item Sequencer", nullptr, useChain, "Read drop modifier info to find out how to use ID's. This lets items drop in a pattern, for example, Tree, Bell, rock, tree bell rock."),
				new MenuEntry(Color(Blue01) << "Eat everything", nullptr, eat),
			};
			const std::vector<MenuEntry *> movEntries{
				new MenuEntry(Color(Blue01) << "Coordinates modifier \uE000 + \uE006", coordMod, menuCoordMod),
				new MenuEntry(Color(Blue01) << "Touch Coordinates", TouchCoord, "Touch the map to go to the spot. Due to a diffrent algorithm, items are placed in a different spot when used wit this."),
				new MenuEntry(Color(Blue01) << "Multi-Presses", asmpresses, "Used for actual game things like using tools and whatnot."),
				new MenuEntry(Color(Blue01) << "Speed modifier", speedMod, menuSpeedMod, "Touch the white keyboard icon to enter a float factor for your walking, running and swimming speed"),
				new MenuEntry(Color(Blue01) << "Walk over objects \uE052 + \uE079", wtw),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Online moonjump", moonjump, "Press the hotkeys to go up!"),{ Hotkey(Key::ZL | Key::DPadUp, "Go up"), Hotkey(Key::ZL | Key::DPadDown, "Go down") }),
				new MenuEntry(Color(Blue01) << "Player Collision modifier \uE003 + \uE07D", playerColMod),
				new MenuEntry(Color(Blue01) << "Player teleporter \uE052 + \uE07E", stalk, "\uE052 + \uE07B: Stalk selected player\n\uE052 + \uE07C: Teleport players to your location"),
				new MenuEntry(Color(Blue01) << "Room warping \uE052 + \uE001", roomWarp, "Press the buttons to open up a menu where you can choose a room ID to warp to, or a warping method to warp all the players."),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Movement Changer", MovementChanger, "Press the hotkeys to set the movement to swimming or walking. (Let's you walk in the ocean.)"),{ Hotkey(Key::ZL | Key::B, "Movement Changer") }),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Visibility Modifier", onlineplayermod, "Press the hotkeys to change the effect that others see on you. You can go fully invisible, or appear as if you're not moving at all. Useful if you want to explore somewhere without someone following you. Remember what you used. This does work online."),{ Hotkey(Key::ZL | Key::A, "Change Appearence") }),
				new MenuEntry(Color(Blue01) << "Other players can't push you", nullptr, noPush, "Bone dry."),
			};
			const std::vector<MenuEntry *> dropEntries{
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "All Seeder", allseeding, "Every item you interact with will be what you set the ID to. Any item. This will remind you that it's on. Use ID 0x00 to disable. Credit to Levi for this."),{ Hotkey(Key::Select | Key::DPadUp, "Change ID") }),
				new MenuEntry(Color(ccLightPurple) << "Drop Items", instantDrop, "Use with dropping modifier to change drop range/type/drop from other players etc."),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Drop from everyone", dropfromall, "Similar to force animations on everybody at once, this allows you to drop from everyone at once. Simply press your hotkeys while auto/touch/instant dropping to do so. Code is under heavy work rn. owo Had to prevent the camera from following others."),{ Hotkey(Key::ZR, "Drop from all") }),
				new MenuEntry(Color(ccLightPurple) << "Touch drop \uE058", touchDrop, "Touch that screen to drop shit like a true nigga."),
				new MenuEntry(Color(ccLightPurple) << "Item clipboard \uE053 + \uE077LEFT", itemClipboard, "Allows you to copy, cut and paste rectangle-like item structures. Make sure to always be located at the top-left corner of the structure you'd like to copy/cut/paste. You can also save and load clipboards from your sd cart."),
				new MenuEntry(Color(ccLightPurple) << "Quick Menu \uE001 + \uE07A", miscFunctions, "This has a small set of tools for quick hacks. Everything but building modifier works online. Please be very careful with this. \n\nCall Function: This is used for quickly executng a games function. Generally you won't need to use this. \n\nBuilding Modifier: It's easy to understand what this does. \n\nChange Gametype: This is the same cheat as seen in the Online Tools folder. Look at the about info on it there for details. \n\nRemove All Items: This tramples everything at once. This works online, once you do this, there's no going back. \n\nSearch and Replace: Type an item ID to find, and then type the item id to replace it with. This also works online, be cautious with it."),
				new MenuEntry(Color(ccLightPurple) << "Map Editor \uE001 + \uE079", tileSelector, "\uE001 + \uE079: Toggle selection\n\nWhile selecting:\n\uE006: Select world coordinates\n\uE000: Place item at world coordinates\n\uE052/\uE053: Scroll item\n\uE001 + \uE07B: Toggle removal mode\n\uE001 + \uE07A: Scroll size"),
				new MenuEntry(Color(ccLightPurple) << "Auto drop \uE001 + \uE07B", autoDrop, "Too lazy to drag your stylus across the screen? Or just wanna make it look like you're using an online walking seeder? Well, this has you covered!"),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Auto Pickup", autoPickup, "Press the hotkeys to automatically pickup. Press it again to turn it off."),{ Hotkey(Key::ZL | Key::Y, "Auto Pickup") }),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Pickup Modifier", pickupmod, "Press the hotkeys to change the type of pickup you want to do. WARNING!!! This cheat changes a lot of assembly within the game, and this cheat hasn't been fully tested. With that in mind, it may crash. Just a heads up."),{ Hotkey(Key::Y | Key::DPadDown, "Pickup Changer"), Hotkey(Key::Y | Key::DPadLeft, "Change Item ID"), Hotkey(Key::Y | Key::DPadUp, "Change Animation ID") }),
				new MenuEntry(Color(ccLightPurple) << "Item Changer \uE002 + \uE006", t2i, "\uE002 + \uE07C: Enter ID for a single item (also used for dropper)\n\uE002 + \uE079: Start a set\n\uE002 + \uE07A: Copy item on ground\n\uE002 + \uE07B: Set item for replacing"),
				new MenuEntry(Color(ccLightPurple) << "Drop Modifiers \uE053 + \uE006|\uE077", dropMod, "\uE053 + \uE07A: Infdrop\n\uE053 + \uE07B: Drag drop spam\n\uE053 + \uE079: Drop from selected player\n\uE053 + \uE07C: Drop type mod\n\uE053 + \uE077UP: Drop range mod\n\uE053 + \uE077RIGHT: Set room to drop into\n\uE053 + START: Item sequence editor"),
				new MenuEntry(Color(ccLightPurple) << "Edit Every Room", roomSeeder, "Change furniture, wallpaper, flooring, music, everything inside another players house!"),
				new MenuEntry(Color(ccLightPurple) << "AC:NH Tree Digger", digany, "Well, the name sucks, doesn't it? But seriously, you can dig anything up like it's buried! Use animation 49! :P"),
				new MenuEntry(Color(ccLightPurple) << "Disable Item Locks", nullptr, bypass, "Did that little cunt bitch Briclyn lock the island and you can't do anything anymore? Well tell her lock to go fuck itself. Seriously. Also can make some drops looks cool as hell!"),
			};
			const std::vector<MenuEntry *> InteractionModEntries{
				new MenuEntry(Color(Blue01) << "Tree Shake", nullptr, Shakethatass),
				new MenuEntry(Color(Blue01) << "Tree Chop", nullptr, emotreez),
				new MenuEntry(Color(Blue01) << "Inventory Drop Type", nullptr, PocketDropDrag, "ONLY DROP TYPES 0x0A TO 0x0D ARE VALID!! ANY OTHER VALUE WILL DROP THE ITEM INVISIBLY AND WON'T APPEAR UNTIL IT SAVES!!! Don't be a cunt, don't brick others."),
				new MenuEntry(Color(Blue01) << "Axe Tree Shake", nullptr, shakechop, "Shaking trees chops them down as if you're using an axe."),
				new MenuEntry(Color(Blue01) << "Instant Tree Chop", instantchop, "Trees chop down with one hit. What kind of sourcery is this? Got arms like the hulk."),
				new MenuEntry(Color(Blue01) << "Tree Crusher", treecrusher, "Walk over trees to BREAK THEM DOWN! This was Tristans idea."),
			};
			const std::vector<MenuEntry *> envEntries{
				new MenuEntry(Color(ccLightPurple) << "Real Time Acre Editor", MapEditor, "Touch a square on the map to change it's acres. HUGE thanks to brume for helping me with this! We worked together to make this and the online island acre editor. :) \nWhen you edited the map to your liking, save and close the game. Then, reopen it to get the results."),
				new MenuEntry(Color(ccLightPurple) << "No trample", nullptr, flwTrample),
				new MenuEntry(Color(ccLightPurple) << "Trampler \uE053 + \uE001", trampleSeeder, "Run over an item to remove it."),
				new MenuEntry(Color(ccLightPurple) << "Fruit Tree Item Modifier", nullptr, treeItemMod, "Changes all fruit trees to the item id."),
				new MenuEntry(Color(ccLightPurple) << "Tree Item Modifier", itemontreemod, "Changes item sprites on fruit trees to the drop item."),
				new MenuEntry(Color(ccLightPurple) << "Infinite fruit tree", nullptr, fruitStays, "Fruit will drop infinitely from fruit trees."),
				new MenuEntry(Color(ccLightPurple) << "Environment item behaviour mod", nullptr, envMod),
				new MenuEntry(Color(ccLightPurple) << "Cannot fall in holes or pitfalls", nullptr, noTrap, "Enabled by default."),
				new MenuEntry(Color(ccLightPurple) << "Infinite money rock + set item", infRock, menuRock, "Money will drop from the money rock infinitely. Touch the white keyboard icon to modify the item that is dropped from the money rock. Works with drop type 0x11. May not work online."),
				new MenuEntry(Color(ccLightPurple) << "Fountain item changer", nullptr, fountainItem, "Touch the white keyboard icon to modify the item that is placed when a fountain firework runs out. Only´works with instant drop."),
			};
			const std::vector<MenuEntry *> animEntries{
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Idle", idle, "Press the buttons to reset your animation. Also works on other players."),{ Hotkey(Key::R, "Idle") }),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Animation modifier \uE000 + ", execAnim, "Krazy. Do anything with this."),{ Hotkey(Key::A | Key::X, "Animation Settings") }),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Animation Modifier (ASM)", animmod, "ASM Alternative to the above cheat! Use this if the above code annoys you. owo Also allows two animations to be used at once!"),{ Hotkey(Key::ZR | Key::A, "Set Animation:"), Hotkey(Key::ZR | Key::B, "Set Secondary Animation:"), Hotkey(Key::A, "Set Fish"), Hotkey(Key::DPadUp, "Drop fish at player 1"), Hotkey(Key::DPadLeft, "Drop fish at player 2"), Hotkey(Key::DPadRight, "Drop fish at player 3"), Hotkey(Key::DPadDown, "Drop fish at player 4") }),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Force Animations (Everyone)", doonall, "Force specified animation on every player at once, or force everyone to moonjump with you. This does set the game to online mode to prevent issues when playing online. If you want to time travel in your town, set the game type to offline. NOTE: This does not work with ASM animations."),{ Hotkey(Key::ZR | Key::A, "Force animation on everyone"), Hotkey(Key::ZR | Key::R, "Idle Everyone"), Hotkey(Key::ZL | Key::DPadUp, "Moonjump Everyone") }),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Infinite Expressions", inf_expression, "Press B to keep an expression lasting forever in a loop. You can smile forever! If only this worked irl... Thanks to Levi for this!"),{ Hotkey(Key::B, "Infinite Expression") }),
				new MenuEntry(Color(ccLightPurple) << "Slow-Motion Animations", slmoanms, "Press \uE054 + \uE07B to enable or disable this. It makes animations act weird and do cool things! :)  Press ZL while walking around to change clothes while you move!"),
				new MenuEntry(Color(ccLightPurple) << "Max Turbo Presses", maxturbo, "Use this to spam ASM Animations. Just hold the A, B, X or Y button."),
				new MenuEntry(Color(ccLightPurple) << "Change Bury Animation", nullptr, BuryAnim, "Change the animation for drop type 0x0B."),
				new MenuEntry(Color(ccLightPurple) << "Change Tool Animation", nullptr, tooltype, "Use X tool as Y tool. This carries over to all tools."),
				new MenuEntry(Color(ccLightPurple) << "Fast pull", nullptr, pullSpeedMod),
				new MenuEntry(Color(ccLightPurple) << "Fast pick up", nullptr, pickSpeedMod),
				new MenuEntry(Color(ccLightPurple) << "Speed up specified animation", nullptr, speedUpAnim),
				new MenuEntry(Color(ccLightPurple) << "Force specified animation if...", forceIf, menuForceIf, "Touch the white keyboard icon to open up a keyboard to set the triggering animations. Then, press B to open a keyboard to set an animation that will be executed when one of the triggering animations is detected. Works on other players in the same room."),
			};
			const std::vector<MenuEntry *> islEntries{
				new MenuEntry(Color(Blue01) << "Dump Island Layout", nullptr, dumpisland, "Dumps island to FOXXY/Islands."),
				new MenuEntry(Color(Blue01) << "Restore Island Layout", nullptr, restoreisland, "Restore island from FOXXY/Islands."),
				new MenuEntry(Color(Blue01) << "Island shop modifier", nullptr, shopMod, "Only works on the (online) island."),
				new MenuEntry(Color(Blue01) << "Island acre modifier", acreMod, menuAcreMod, "Touch the white keyboard icon to open up a keyboard to enter 16 acre IDs. Acres are counted left to right, top to bottom. Enable this cheat before you arrive on a new island. Only works as host."),
				new MenuEntry(Color(Blue01) << "Island buildings modifier", buildingMod, menuBuildingMod, "Touch the white keyboard icon to enter building data for the 2 island buildings. Enable this cheat before you arrive on a new island. Only works as host."),
				new MenuEntry(Color(Blue01) << "Allow writing more than 3 numbers", numbers),
				new MenuEntry(Color(Blue01) << "Fill inv with bonus ore", nullptr, bonusOre),
				new MenuEntry(Color(Blue01) << "Fill inv with needed fruit", nullptr, instantFruit),
				new MenuEntry(Color(Blue01) << "Tour corrupter", nullptr, tourCorrupter),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Force End Tour (Host only)", tourend, "Meh. ._."),{ Hotkey(Key::R, "End tour") }),
				new MenuEntry(Color(Blue01) << "All Tours Selectable", alltour, "Season pass to all the tours"),
			};
			const std::vector<MenuEntry *> npcEntries{
				new MenuEntry(Color(ccLightPurple) << "NPC Menu \uE003 + \uE07C", npcMenu),
				new MenuEntry(Color(ccLightPurple) << "NPC Coordinates \uE000 + \uE077", npcCoordinates),
				new MenuEntry(Color(ccLightPurple) << "NPC Animation Axecuter \uE000 + \uE003", npcAnimation),
			};
			const std::vector<MenuEntry *> miscEntries{
				new MenuEntry(Color(Blue01) << "Fast game speed \uE052 + \uE000", fastSpeed),
				new MenuEntry(Color(Blue01) << "Fix Menu \uE053 + \uE003", fixMenu),
				//new MenuEntry(Color(Blue01) << "Time travel", timeTravel, "Press the TT button on the touch screen to change the date and time! This is disabled for online use for obvious reasons. It breaks the game."),
				new MenuEntry(Color(Blue01) << "Bypass Checksums", slattzylove, "Bypasses region checks, save checks, and secure value checks! Thank you Slattz for this! <3 I LOVE YOU!!!"),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Touch Buttons", instantbuttons, "Displays little buttons on the touch screen that does little things. \nTime: Change Date & Time (Offline only) \n \nTrip: Make yourself fall over with a balloon. \n \nAnim: Does random animation. \n \nBug: Catch Random Bug \n \nFish: Catch Random Fish \n \nSea: Catch Random Seacreature \n \nCrash: Crashes everyone but yourself. (Or someone else using the plugin.) \nPRESS ZL AND ZR TO DISPLAY THE CRASH BUTTON. THIS IS SO YOU CAN BE SURE YOU WANT TO DO IT."),{ Hotkey(Key::ZL | Key::ZR, "Show Crash Button") }),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Give Patterns to Others", pattern_picker, "The source pattern is what pattern you use when giving to others, while the destination pattern is where the pattern will end up. Enter design IDs to choose where it goes. Do not do this without the permission of others (you can if they are a cunt.)  CREDIT TO LEVI FOR THIS!"),{ Hotkey(Key::Select | Key::DPadLeft, "Pattern Picker") }),
				//EntryWithHotkey(new MenuEntry(Color(Blue01) << "TEST FUNCTION", functtest, "Used for testing functions. This will not be enabled in the plugin so I don't care what goes here. :p"),{ Hotkey(Key::B, "Execute function") }),
			};
			const std::vector<MenuEntry *> keyboardEntries{
				new MenuEntry(Color(ccLightPurple) << "Chat bubbles don't disappear", nullptr, bubblesDisappear),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Talk through anyone", chat_picker, "Press the hotkeys to select a player to talk through. Alternative to the drop from anyone cheat, as this let's you see when the targeted person replies to you."),{ Hotkey(Key::L | Key::X, "Chat Picker") }),
				new MenuEntry(Color(ccLightPurple) << "Encyclopedia Chatter", EncChat, "Use the Encyclopedia to chat. Prevents others from leaving when you do."),
				new MenuEntry(Color(ccLightPurple) << "Friend Request Spammer", spamfriendrequests, "Set the game type to 'offline' or \n'Online (Mode 1)' to allow friend requests to be sent from the island, then enable this to be able to send them over and over again! \nOnly works on club tortimer on people who aren't friends."),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Force Send Chat", Forcesendchat, "Blah blah."),{ Hotkey(Key::ZR, "Force Send Chat") }),
				new MenuEntry(Color(ccLightPurple) << "Keyboard Extender", customSymbols, "Lots of text."),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Keyboard Unlocker", extender, "This unlocks arobase and allows line breaking."),{ Hotkey(Key::ZR, "Extendo-keys") }),
				new MenuEntry(Color(ccLightPurple) << "Kaomoji Shortcuts", kaomoji, "Use Kaomoji's!! (Most will crash if used of best friend chat.)"),
				new MenuEntry(Color(ccLightPurple) << "Japanese Communicator", TranslateJAP, "Refer to the plugin GUIDE button for tips on using this. Thanks to a stupid bitch for help with some sentences."),
				new MenuEntry(Color(ccLightPurple) << "Korean Communicator", TranslateKOR, "Refer to the plugin GUIDE button for tips on using this."),
				new MenuEntry(Color(ccLightPurple) << "German Communicator", TranslateGER, "Refer to the plugin GUIDE button for tips on using this."),
				new MenuEntry(Color(ccLightPurple) << "French Communicator", TranslateFRA, "Refer to the plugin GUIDE button for tips on using this."),
			};
			const std::vector<MenuEntry *> modEntries{
				new MenuEntry(Color(Blue01) << "Player selector \uE001 + \uE07C", playerSelector, "Select the player you'd like to fuck with."),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Player Copier", playercopy, "Uh oh, someone call Mat016 to make a shitty YouTube video... \n (C++ source for this cheat was made by Levi.)"),{ Hotkey(Key::R | Key::Select, "Player Copy") }),
				new MenuEntry(Color(Blue01) << "Fast mode \uE053 + \uE077DOWN", fast, "Functions are executed twice as fast. Basically a non ACNL Max Turbo, but better."),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Force Error", forceerror, "Set the hotkeys before enabling this cheat or else I'll come over there and be- I mean show you mad affection. Calm yo' ass down."),{ Hotkey(Key::ZL | Key::ZR | Key::B, "ErrorQueer") }),
				EntryWithHotkey(new MenuEntry(Color(Blue01) << "Network Lagger", forcelag, "Use this to temporarily slow down any online game you're in, then wait a few seconds and the game will start running normally again. Alternatively, you can press the second set of hotkeys to drop a quick lagspike. When it says 'Done!', you can drop another lagspike."),{ Hotkey(Key::ZL | Key::ZR, "Network Lagger Commencer!"), Hotkey(Key::L | Key::ZL, "Drop lag spike"), }),
				new MenuEntry(Color(Blue01) << "Change Gametype", nullptr, mgtype, "This let's you change what kind of AC:NL you're playing. For example, make the game load in DREAM mode, or set a club tortimer game mode to offline or town, which allows you to send friend requests to other players from the public islands."),
				new MenuEntry(Color(Blue01) << "Country spoofer", nullptr, countrySpoof, "Changes the North America island region to an island of your choice!"),
				new MenuEntry(Color(Blue01) << "Show players on the map", nullptr, map, "A feature that should have been built into the fucking game."),
				new MenuEntry(Color(Blue01) << "Show player info", nullptr, debug, "Displays current status of other players. Speed, animation, item, coordinates, room, you name it."),
				new MenuEntry(Color(Blue01) << "Crash Prevention", nullptr, nocrash, "Prevents others from crashing you. Having this on can cause some bugs with digging and patten kicking, so disable it if you don't want those issues. This cheat is enabled by default."),
				new MenuEntry(Color(Blue01) << "Remove Lag", nullptr, hostDrop, "Tired of the lag when trying to do something online? Fuck that stupid shit, this will fix that for you. Enabled by default."),
			};
			const std::vector<MenuEntry *> SeemenotEntries{
				new MenuEntry(Color(ccLightPurple) << "Modern Clock", noklok, "Replace the games clock with a more modern one!"),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Freeze Frame", freezeframe, "Press the hotkey to freeze the screen. \nThis let's you lock the screen, and for instance warp to the photobooth and take a picture of the frozen screen! \nYou can also use this with Amiibo camera to take screenshots while in the camera to post on swapdoodle."),{ Hotkey(Key::A | Key::ZR, "Take picture"), Hotkey(Key::ZR, "Freeze Frame") }),
				EntryWithHotkey(new MenuEntry(Color(ccLightPurple) << "Camera Mod", cameramod, "See the world from all sorts of angles! It's beautiful! Rotate with R & CirclePad, pan using ZR + DPad & R & L CREDIT TO LEVI FOR THIS!!"),{ Hotkey(Key::B | Key::X, "Reset Camera"), Hotkey(Key::ZR | Key::DPadLeft, "Pan Left"), Hotkey(Key::ZR | Key::DPadRight, "Pan Right"), Hotkey(Key::ZR | Key::DPadDown, "Pan Back"), Hotkey(Key::ZR | Key::DPadUp, "Pan Foward"), Hotkey(Key::ZR | Key::R, "Pan Up"), Hotkey(Key::ZR | Key::L, "Pan Down") }),
				new MenuEntry(Color(ccLightPurple) << "Camera doesn't ascend", cameraUp),
				new MenuEntry(Color(ccLightPurple) << "Disable L + R Screenshots", noshot, "Disable L and R screenshots! Use the plugin screenshot tool instead, it's better! This is my own version of the cheat. :p its a one liner. uwu"),
				new MenuEntry(Color(ccLightPurple) << "Fast Menus", menuspeed, "Makes the menus xtra speedy. Thanks to Levi for this sweet cheat!"),
				new MenuEntry(Color(ccLightPurple) << "Disable Save Menus", nullptr, nonesave, "Disable save menus so that stupid ass menu doesn't pop up when you use any hotkeys! Credit to Levi for this."),
				new MenuEntry(Color(ccLightPurple) << "FOV Adjuster", nullptr, fovMod, "Used on some of my YouTube Videos."),
				new MenuEntry(Color(ccLightPurple) << "Camera Pan Speed Adjuster", nullptr, panadj, "Loop de loop and pull, and ya shoes are lookin' kool!"),
				new MenuEntry(Color(ccLightPurple) << "Player size", nullptr, playerSize),
				new MenuEntry(Color(ccLightPurple) << "Bug/Fish size", nullptr, bugSize),
				new MenuEntry(Color(ccLightPurple) << "Rotating furniture", nullptr, rotatingFurn),
				new MenuEntry(Color(ccLightPurple) << "Change Rockbreak Particle", nullptr, rockparticle, "This will let you spam the rockbreak seeder, however crashes other players.. Good for if they're an asshole!"),
			};
			const std::vector<MenuEntry *> shaderEntries{
				new MenuEntry(Color(Blue01) << "Grass Shine Modifier", nullptr, shader1, "How gay do you want to make the shiny grass look?"),
				new MenuEntry(Color(Blue01) << "Light Source Modifier", nullptr, shader2, "Lights, camera, action!"),
				new MenuEntry(Color(Blue01) << "Brightness Modifier", nullptr, shader3, "Shiny"),
				new MenuEntry(Color(Blue01) << "General Color Modifier", nullptr, shader4, "Oily! Maximum value is 2FFF or something like that. Keep The value between 0000 and 02D0 and you'll be fine. Refer to the guide for color codes."),
				new MenuEntry(Color(Blue01) << "Export Shader Data", nullptr, exportshader, "Export your current shader configuration to /FOXXY/Shaders/. This does not export brightness levels."),
				new MenuEntry(Color(Blue01) << "Import Shader Data", nullptr, importshader, "Import your saved shader configurations from /FOXXY/Shaders/. This does not import brightness levels."),
			};
			const std::vector<MenuEntry *> AudioEntries{
				new MenuEntry(Color(ccLightPurple) << "Change Outside BGM", nullptr, bgmchange, "Change the music to be whatever you want it to be!"),
				new MenuEntry(Color(ccLightPurple) << "Sound Pitch", nullptr, soundSpeed, "Change the overall pitch of all the sounds in the game."),
				new MenuEntry(Color(ccLightPurple) << "Music Volume Adjuster", nullptr, MusicOFF, "Change the overall volume of all the music in the game."),
				new MenuEntry(Color(ccLightPurple) << "Overall Sound Volume Adjuster", nullptr, OSVA, "Change the overall volume of all the sounds in the game."),
				new MenuEntry(Color(ccLightPurple) << "Menu Sound Pitch Adjuster", nullptr, MenuSoundAdjust, "Change the overall pitch of all the menu sounds in the game."),
				new MenuEntry(Color(ccLightPurple) << "Tune Pitch Modifier", nullptr, TunePitch, "EEEK!! Credit for this goes to Levi."),
			};
			const std::vector<MenuEntry *> playerEntries{
				new MenuEntry(Color(Blue01) << "Backup Save File", nullptr, dumpdata, "Backup your save data!"),
				new MenuEntry(Color(Blue01) << "Restore Save File", nullptr, restoredata, "Restore your save data!"),
				new MenuEntry(Color(Blue01) << "Change Player Name", nullptr, playername, "Change your name!"),
				new MenuEntry(Color(Blue01) << "Change Town Name", nullptr, townname, "Change your town name!"),
				new MenuEntry(Color(Blue01) << "Export TPC Picture", nullptr, tppexport, "Export your photo on the fly! All photos will be saved as a .jpg file."),
				new MenuEntry(Color(Blue01) << "Import TPC Picture", nullptr, tppimport, "Import your photo on the fly!"),
				new MenuEntry(Color(Blue01) << "Change Dream Code", nullptr, tpccode, "Change your dream address!"),
				new MenuEntry(Color(Blue01) << "Change Gender", nullptr, gender, "Change your gender!"),
				new MenuEntry(Color(Blue01) << "Enable Census Menu", nullptr, censusMenu, "Unused stats menu! Thanks Slattz for finding this! Open the TPC menu, and click the new red tab with no icon. OOOhhhh."),
				new MenuEntry(Color(Blue01) << "Change Hair Style", nullptr, hairstylepick, "Change your villagers hair style! \n (C++ source for this cheat was made by Levi.)"),
				new MenuEntry(Color(Blue01) << "Change Hair Color", nullptr, haircolorpick, "Change your villagers hair color! \n (C++ source for this cheat was made by Levi.)"),
				new MenuEntry(Color(Blue01) << "Change Face Type", nullptr, facestylepick, "Professional plastic surgion for hire! Just dial 1-800-555-5555! \n (C++ source for this cheat was made by Levi.)"),
				new MenuEntry(Color(Blue01) << "Change Eye Color", nullptr, eyecolorpick, "I wanted to make a nipple eye reference to AJPCrosser, but that little cunt Jake would catch a bug up his ass. So instead, have some more fuckin' negativity."),
				new MenuEntry(Color(Blue01) << "Change TPC Region", nullptr, tpcregn, "I'm from here, not there. If ya wanna dox me, make sure you do it right. Don't be a pansy ass bitch with your paid people finders app, you dumbass hoe."),
			};
			
			// ítems
			menu->Append(new MenuFolder(Color(Blue01) << "\uE016Inventory\uE016", "indeed", essentialEntries));
			menu->Append(new MenuFolder(Color(ccLightPurple) << "\uE017Animation\uE017", animEntries));
			menu->Append(new MenuFolder(Color(Blue01) << "\uE008Movement\uE008", movEntries));
			menu->Append(new MenuFolder(Color(ccLightPurple) << "\uE058Item Mods\uE058", "Drop items up that way..! Wait, they're going over there! WOAH!! Hack pls?", dropEntries));
			menu->Append(new MenuFolder(Color(Blue01) << "\uE00CItem Interaction Mods\uE00C", InteractionModEntries));
			menu->Append(new MenuFolder(Color(ccLightPurple) << "\uE018Environment\uE018", envEntries));
			menu->Append(new MenuFolder(Color(Blue01) << "\uE00BOnline Island\uE00B", islEntries));
			menu->Append(new MenuFolder(Color(ccLightPurple) << "\uE00BNPCs\uE00B", npcEntries));
			menu->Append(new MenuFolder(Color(Blue01) << "\uE017Misc.\uE017", miscEntries));
			menu->Append(new MenuFolder(Color(ccLightPurple) << "\uE00DKeyboard and Text\uE00D", keyboardEntries));
			menu->Append(new MenuFolder(Color(Blue01) << "\uE009Online Tools\uE009", modEntries));
			menu->Append(new MenuFolder(Color(ccLightPurple) << "\uE017Visual Effects\uE017", SeemenotEntries));
			menu->Append(new MenuFolder(Color(Blue01) << "\uE00CShader Modifications\uE00C", shaderEntries));
			menu->Append(new MenuFolder(Color(ccLightPurple) << "\uE018Audio Effects\uE018", AudioEntries));
			menu->Append(new MenuFolder(Color(Blue01) << "\uE015Player Modifiers\uE015", playerEntries));
			//menu->Append(new MenuFolder(Color(ccLightPurple) << "DUMMY", DUMMYentries));
			
			menu->Run();
			delete menu;
		}
		else MessageBox("DON'T STICK THE MACORONI DOWN MY PANTS!! NO!!!! NOT THE MACORONI!! NOT THE MACORONI!!!!!!! Oh, also your stupid version isn't supported, so FUCK OFF!! Jk! :p")();
		return 0;
	}
}
