#ifndef CHEATS_H
#define CHEATS_H

#include "CTRPluginFramework.hpp"
#include "Game.hpp"
#include <cmath>
#include <algorithm>


namespace CTRPluginFramework
{
	using OnInputChange = void(*)(Keyboard &keyboard, InputChangeEvent &event);
	void t2i(MenuEntry *entry);
	void duplicate(MenuEntry *entry);
	void dropMod(MenuEntry *entry);
	void useChain(MenuEntry *entry);
	void dragDrop(MenuEntry *entry);
	void validate(MenuEntry *entry);
	void eat(MenuEntry *entry);
	void display(MenuEntry *entry);

	void coordMod(MenuEntry *entry);
	void menuCoordMod(MenuEntry *entry);
	void speedMod(MenuEntry *entry);
	void menuSpeedMod(MenuEntry *entry);
	void wtw(MenuEntry *entry);
	void moonjump(MenuEntry *entry);
	void playerColMod(MenuEntry *entry);
	void stalk(MenuEntry *entry);
	void MovementChanger(MenuEntry *entry);
	void noPush(MenuEntry *entry);
	void MapEditor(MenuEntry *entry);
	void pickupmod(MenuEntry *entry);
	void instantchop(MenuEntry *entry);
	void shakechop(MenuEntry *entry);
	void instantbuttons(MenuEntry *entry);
	void chat_picker(MenuEntry *entry);
	void TouchCoord(MenuEntry *entry);
	void nonesave(MenuEntry *entry);
	void pattern_picker(MenuEntry *entry);
	void allseeding(MenuEntry *entry);
	void itemontreemod(MenuEntry *entry);
	void treecrusher(MenuEntry *entry);
	//void functtest(MenuEntry *entry);

	void pullSpeedMod(MenuEntry *entry);
	void pickSpeedMod(MenuEntry *entry);

	void flwTrample(MenuEntry *entry);
	void trampleSeeder(MenuEntry *entry);
	void treeItemMod(MenuEntry *entry);
	void fruitStays(MenuEntry *entry);
	void fruitRange(MenuEntry *entry);
	void envMod(MenuEntry *entry);
	void noTrap(MenuEntry *entry);
	void infRock(MenuEntry *entry);
	void menuRock(MenuEntry *entry);
	void fountainItem(MenuEntry *entry);
	void rockparticle(MenuEntry *entry);
	void kaomoji(MenuEntry *entry);
	void TranslateJAP(MenuEntry *entry);
	void TranslateKOR(MenuEntry *entry);
	void TranslateGER(MenuEntry *entry);
	void TranslateFRA(MenuEntry *entry);
	void forceerror(MenuEntry *entry);
	void playername(MenuEntry *entry);
	void gender(MenuEntry *entry);
	void tppexport(MenuEntry *entry);
	void tppimport(MenuEntry *entry);
	void playercopy(MenuEntry *entry);
	void hairstylepick(MenuEntry *entry);
	void haircolorpick(MenuEntry *entry);
	void facestylepick(MenuEntry *entry);
	void eyecolorpick(MenuEntry *entry);
	void tpcregn(MenuEntry *entry);
	void onlineplayermod(MenuEntry *entry);
	void animmod(MenuEntry *entry);
	void forcelag(MenuEntry *entry);
	//void cng_item(MenuEntry *entry);
	void EncChat(MenuEntry *entry);
	void Forcesendchat(MenuEntry *entry);
	void dropfromall(MenuEntry *entry);
	void itemrandom(MenuEntry *entry);
	void slattzylove(MenuEntry *entry);
	void alltour(MenuEntry *entry);
	void inf_expression(MenuEntry *entry);
	void maxturbo(MenuEntry *entry);
	void tourend(MenuEntry *entry);
	void censusMenu(MenuEntry *entry);
	void townname(MenuEntry *entry);
	void tpccode(MenuEntry *entry);
	void shader1(MenuEntry *entry);
	void shader2(MenuEntry *entry);
	void shader3(MenuEntry *entry);
	void shader4(MenuEntry *entry);
	void exportshader(MenuEntry *entry);
	void importshader(MenuEntry *entry);
	void OSVA(MenuEntry *entry);
	void MenuSoundAdjust(MenuEntry *entry);
	void panadj(MenuEntry *entry);
	void hatz(MenuEntry *entry);
	void dumpdata(MenuEntry *entry);
	void restoredata(MenuEntry *entry);
	void menuspeed(MenuEntry *entry);
	void TunePitch(MenuEntry *entry);
	void freezeframe(MenuEntry *entry);
	void spamfriendrequests(MenuEntry *entry);

	void roomWarp(MenuEntry *entry);
	void roomSeeder(MenuEntry *entry);
	void invSeeder(MenuEntry *entry);
	void roomTrack(MenuEntry *entry);
	void roomSwap(MenuEntry *entry);

	void fastSpeed(MenuEntry *entry);
	void fixMenu(MenuEntry *entry);
	void extender(MenuEntry *entry);
	void customSymbols(MenuEntry *entry);
	//void timeTravel(MenuEntry *entry);
	void soundPlayer(MenuEntry *entry);
	void particle(MenuEntry *entry);
	void hostDrop(MenuEntry *entry);
	void bypass(MenuEntry *entry);
	void countrySpoof(MenuEntry *entry);
	void dMenu(MenuEntry *entry);
	void soundSpeed(MenuEntry *entry);
	void fovMod(MenuEntry *entry);
	void cameraUp(MenuEntry *entry);
	void noshot(MenuEntry *entry);
	void menuTracker(MenuEntry *entry);
	void tracker(MenuEntry *entry);
	void menuFreezer(MenuEntry *entry);
	void freezer(MenuEntry *entry);

	void idle(MenuEntry *entry);
	void execAnim(MenuEntry *entry);
	void openAnimMod(MenuEntry *entry);
	void speedUpAnim(MenuEntry *entry);
	void forceIf(MenuEntry *entry);
	void menuForceIf(MenuEntry *entry);
	void slmoanms(MenuEntry *entry);
	void doonall(MenuEntry *entry);

	void shopMod(MenuEntry *entry);
	void acreMod(MenuEntry *entry);
	void menuAcreMod(MenuEntry *entry);
	void buildingMod(MenuEntry *entry);
	void menuBuildingMod(MenuEntry *entry);
	void dumpisland(MenuEntry *entry);
	void restoreisland(MenuEntry *entry);
	void numbers(MenuEntry *entry);
	void cameramod(MenuEntry *entry);
	void bgmchange(MenuEntry *entry);
	void displayIslandInfo(MenuEntry *entry);

	void npcMenu(MenuEntry *entry);
	void npcCoordinates(MenuEntry *entry);
	void npcAnimation(MenuEntry *entry);

	void stalkMachine(MenuEntry *entry);
	void bonusOre(MenuEntry *entry);
	void instantFruit(MenuEntry *entry);
	void tourCheat(MenuEntry *entry);
	void tourCorrupter(MenuEntry *entry);

	void playerSelector(MenuEntry *entry);
	void fast(MenuEntry *entry);
	void nocrash(MenuEntry *entry);
	void walletfix(MenuEntry *entry);
	void patternedit(MenuEntry *entry);
	void MusicOFF(MenuEntry *entry);
	void noklok(MenuEntry *entry);
	void instantDrop(MenuEntry *entry);
	void touchDrop(MenuEntry *entry);
	void menuTouchDrop(MenuEntry *entry);
	void itemClipboard(MenuEntry *entry);
	void miscFunctions(MenuEntry *entry);
	void tileSelector(MenuEntry *entry);
	void autoDrop(MenuEntry *entry);
	void autoPickup(MenuEntry *entry);
	void map(MenuEntry *entry);
	void debug(MenuEntry *entry);

	void playerSize(MenuEntry *entry);
	void bugSize(MenuEntry *entry);
	void rotatingFurn(MenuEntry *entry);
	void bubblesDisappear(MenuEntry *entry);
	void BuryAnim(MenuEntry *entry);
	void tooltype(MenuEntry *entry);
	void Shakethatass(MenuEntry *entry);
	void emotreez(MenuEntry *entry);
	void PocketDropDrag(MenuEntry *entry);
	void mgtype(MenuEntry *entry);
	void digany(MenuEntry *entry);
	void asmpresses(MenuEntry *entry);

	// helper functions
	void cleanC(void);
	void writeItem(u32 item, u32 offset);
	void toggle(u32 addr[], u32 enableVal[], u32 disableVal[], u8 Length);
	void toggle2(u32 addr, u32 enableVal, u32 disableVal);
	bool SetUpKB(const std::string &msg, bool hex, const int length, u32 &output, u32 def, OnInputChange cb = nullptr);
	bool SetUpKB(const std::string &msg, bool hex, const int length, u16 &output, u16 def, OnInputChange cb = nullptr);
	bool SetUpKB(const std::string &msg, bool hex, const int length, u8 &output, u8 def, OnInputChange cb = nullptr);
	bool SetUpAnimKB(u8 &output, u8 def);
}

#endif