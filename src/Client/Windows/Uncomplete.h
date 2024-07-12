#pragma once

#include "../Window.h"

class Uncomplete : public FLAlertLayerProtocol {
public:
    
    virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1);
    
    void onToggle(CCObject* sender);
};