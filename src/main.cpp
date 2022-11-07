#include <Dolphin/OS.h>
#include <Dolphin/math.h>
#include <Dolphin/string.h>

#include <JSystem/J2D/J2DOrthoGraph.hxx>
#include <JSystem/J2D/J2DTextBox.hxx>

#include <SMS/System/Application.hxx>

#include <BetterSMS/game.hxx>
#include <BetterSMS/module.hxx>
#include <BetterSMS/stage.hxx>
#include <BetterSMS/loading.hxx>

/*
/ Example module that logs to the console and draws to the screen during gameplay
*/

static int getTextWidth(J2DTextBox *textbox) {
    const size_t textLength = strlen(textbox->mStrPtr);

    size_t textWidth = 0;
    for (int i = 0; i < textLength; ++i) {
        JUTFont::TWidth width;
        textbox->mFont->getWidthEntry(textbox->mStrPtr[i], &width);
        textWidth += width.mWidth;
    }

    return textWidth + (Max(textLength - 1, 0) * textbox->mCharSpacing);
}

static J2DTextBox *sOurTextBox = nullptr;
static J2DTextBox *sOurTextBoxBackDrop = nullptr;
static s32 sCoordX, sCoordY = 0;
static bool sXTravelsRight, sYTravelsDown = true;

static void onStageInit(TMarDirector *director) {
    sOurTextBox = new J2DTextBox(gpSystemFont->mFont, "Hello Screen!");
    {
        sOurTextBox->mGradientTop    = {160, 210, 10, 255};  // RGBA
        sOurTextBox->mGradientBottom = {240, 150, 10, 255};  // RGBA
    }

    sOurTextBoxBackDrop = new J2DTextBox(gpSystemFont->mFont, "Hello Screen!");
    {
        sOurTextBoxBackDrop->mGradientTop    = {0, 0, 0, 255};  // RGBA
        sOurTextBoxBackDrop->mGradientBottom = {0, 0, 0, 255};  // RGBA
    }

    sCoordX = -BetterSMS::getScreenRatioAdjustX();
    sCoordY = 32;

    OSReport("Textbox initialization successful!\n");
}

static void onStageUpdate(TMarDirector *director) {
    if (sXTravelsRight)
        sCoordX += 1;
    else
        sCoordX -= 1;

    if (sYTravelsDown)
        sCoordY += 1;
    else
        sCoordY -= 1;

    if (sCoordX >= BetterSMS::getScreenRenderWidth() - getTextWidth(sOurTextBox))
        sXTravelsRight = false;
    else if (sCoordX <= -BetterSMS::getScreenRatioAdjustX())
        sXTravelsRight = true;

    if (sCoordY >= (480 - sOurTextBox->mCharSizeY))
        sYTravelsDown = false;
    else if (sCoordY <= 32)
        sYTravelsDown = true;
}

static void onStageDraw2D(TMarDirector *director, const J2DOrthoGraph *ortho) {
    sOurTextBoxBackDrop->draw(sCoordX + 1, sCoordY + 2);  // Draw backdrop text to the screen
    sOurTextBox->draw(sCoordX, sCoordY);  // Draw text to the screen
}

// Module definition

static void initModule() {
    OSReport("Initializing Module...\n");

    // Register callbacks
    BetterSMS::Stage::registerInitCallback("OurModule_StageInitCallBack", onStageInit);
    BetterSMS::Stage::registerUpdateCallback("OurModule_StageUpdateCallBack", onStageUpdate);
    BetterSMS::Stage::registerDraw2DCallback("OurModule_StageDrawCallBack", onStageDraw2D);
}

static void deinitModule() {
    OSReport("Deinitializing Module...\n");

    // Cleanup callbacks
    BetterSMS::Stage::deregisterInitCallback("OurModule_StageInitCallBack");
    BetterSMS::Stage::deregisterUpdateCallback("OurModule_StageUpdateCallBack");
    BetterSMS::Stage::deregisterDraw2DCallback("OurModule_StageDrawCallBack");
}

// Definition block
KURIBO_MODULE_BEGIN("OurModule", "JoshuaMK", "v1.0") {
    // Set the load and unload callbacks to our registration functions
    KURIBO_EXECUTE_ON_LOAD { initModule(); }
    KURIBO_EXECUTE_ON_UNLOAD { deinitModule(); }
}
KURIBO_MODULE_END()
