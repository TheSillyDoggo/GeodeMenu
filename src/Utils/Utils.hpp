#pragma once

#include <Geode/Geode.hpp>

enum kCCTransition
{
    kCCTransitionFade,
    kCCTransitionCrossFade,
    kCCTransitionFadeBL,
    kCCTransitionFadeTR,
    kCCTransitionFadeUp,
    kCCTransitionFadeDown,
    kCCTransitionFlipAngular,
    kCCTransitionFlipX,
    kCCTransitionFlipY,
    kCCTransitionZoomFlipAngular,
    kCCTransitionZoomFlipX,
    kCCTransitionZoomFlipY,
    kCCTransitionJumpZoom,
    kCCTransitionMoveInT,
    kCCTransitionMoveInB,
    kCCTransitionMoveInL,
    kCCTransitionMoveInR,
    kCCTransitionRotoZoom,
    kCCTransitionShrinkGrow,
    kCCTransitionSlideInT,
    kCCTransitionSlideInB,
    kCCTransitionSlideInL,
    kCCTransitionSlideInR,
    kCCTransitionSplitRows,
    kCCTransitionSplitCols,
    kCCTransitionTurnOffTiles,
    kCCTransitionProgressRadialCW,
    kCCTransitionProgressRadialCCW,
    kCCTransitionProgressHorizontal,
    kCCTransitionProgressVertical,
    kCCTransitionProgressInOut,
    kCCTransitionProgressOutIn,
    kCCTransitionPageTurn,
};

namespace qolmod::utils
{
    GJGameLevel* getCurrentLevel(bool requireLevelData = false);
    std::wstring toWideString(std::string str);
    std::string toUTF8String(std::wstring ws);
    void addCircleToNode(cocos2d::CCNode* node , float scale = 1.0f);
    cocos2d::CCScene* createTransitionForEnum(kCCTransition trans, float time, cocos2d::CCScene* scene);
    std::string sizeToPretty(unsigned int size);
};