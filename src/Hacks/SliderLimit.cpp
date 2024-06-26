#include <Geode/Geode.hpp>
#include <Geode/modify/SliderTouchLogic.hpp>
#include <Geode/modify/SliderThumb.hpp>
#include <Geode/modify/GJScaleControl.hpp>
#include "../Client/Client.h"

class $modify (SliderTouchLogic)
{
    virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
    {
        if (this->m_thumb->m_vertical)
            this->m_thumb->setPositionY(this->convertToNodeSpace(p0->getLocation()).y);
        else
            this->m_thumb->setPositionX(this->convertToNodeSpace(p0->getLocation()).x);

        this->m_slider->updateBar();

        this->m_thumb->activate();
    }

    static void onModify(auto& self) {
        auto hook = self.getHook("SliderTouchLogic::ccTouchMoved");

        Loader::get()->queueInMainThread([hook]
        {
            auto modu = Client::GetModule("slider-limit");
            modu->addHookRaw(hook);
        });
    }
};