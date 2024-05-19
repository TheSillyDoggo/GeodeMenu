#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class $modify (CCTextInputNode)
{
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
        if (!(this->getID().starts_with("IGNOREBYPASSES"_spr))) {
            return CCTextInputNode::ccTouchBegan(touch, event);
        }

        if (!nodeIsVisible(this)) {
            this->onClickTrackNode(false);
            return false;
        }

        auto const touchPos = touch->getLocation();
        auto const size = this->getContentSize();

        if (this->boundingBox().containsPoint(touchPos))
        {
            this->onClickTrackNode(true);
            this->updateCursorPosition(touchPos, {{0, 0}, size});

            return true;
        }

        if (m_delegate && !m_delegate->allowTextInput(this)) {
            this->onClickTrackNode(false);
            return false;
        }

        this->onClickTrackNode(false);
        return false;
    }
};