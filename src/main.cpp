#include <Dolphin/OS.h>
#include <JSystem/J2D/J2DOrthoGraph.hxx>
#include <JSystem/J2D/J2DTextBox.hxx>

#include <SMS/System/Application.hxx>

#include <BetterSMS/game.hxx>
#include <BetterSMS/module.hxx>
#include <BetterSMS/stage.hxx>

/*
/ Example module that logs to the console and draws to the screen during gameplay
*/

static void onGameInit(TApplication *app) { OSReport("Game initialized!\n"); }

static void onGameBoot(TApplication *app) { OSReport("Game booted!\n"); }

static J2DTextBox *sOurTextBox = nullptr;

static void onStageInit(TMarDirector *director) {
    sOurTextBox = new J2DTextBox(gpSystemFont->mFont, "Hello Screen!");
    OSReport("Textbox initialization successful!\n");
}

static void onStageUpdate(TMarDirector *director) { OSReport("Hello world!\n"); }

static void onStageDraw2D(TMarDirector *director, const J2DOrthoGraph *ortho) {
    sOurTextBox->draw(180, 140);  // Draw text to the screen at x=180, y=140
}

// Module definition

static void initModule() {
    OSReport("Initializing Module...\n");

    // Register callbacks
    BetterSMS::Game::registerOnInitCallback("OurModule_GameInitCallBack", onGameInit);
    BetterSMS::Game::registerOnBootCallback("OurModule_GameBootCallBack", onGameBoot);
    BetterSMS::Stage::registerInitCallback("OurModule_StageInitCallBack", onStageInit);
    BetterSMS::Stage::registerUpdateCallback("OurModule_StageUpdateCallBack", onStageUpdate);
    BetterSMS::Stage::registerDraw2DCallback("OurModule_StageDraw2DCallBack", onStageDraw2D);
}

static void deinitModule() {
    OSReport("Deinitializing Module...\n");

    // Cleanup callbacks
    BetterSMS::Game::deregisterOnInitCallback("OurModule_GameInitCallBack");
    BetterSMS::Game::deregisterOnBootCallback("OurModule_GameBootCallBack");
    BetterSMS::Stage::deregisterInitCallback("OurModule_StageInitCallBack");
    BetterSMS::Stage::deregisterUpdateCallback("OurModule_StageUpdateCallBack");
    BetterSMS::Stage::deregisterDraw2DCallback("OurModule_StageDraw2DCallBack");
}

// Definition block
KURIBO_MODULE_BEGIN("OurModule", "JoshuaMK", "v1.0") {
    // Set the load and unload callbacks to our registration functions
    KURIBO_EXECUTE_ON_LOAD { initModule(); }
    KURIBO_EXECUTE_ON_UNLOAD { deinitModule(); }
}
KURIBO_MODULE_END()
