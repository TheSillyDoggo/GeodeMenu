#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class $modify (CCTextInputNode)
{
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
        if (!this->getUserObject("fix-text-input") || !(this->getID().starts_with("IGNOREBYPASSES"_spr))) {
            return CCTextInputNode::ccTouchBegan(touch, event);
        }

        if (!nodeIsVisible(this)) {
            this->onClickTrackNode(false);
            return false;
        }

        auto const touchPos = touch->getLocation();
        auto const size = this->getContentSize();
        auto const pos = this->convertToNodeSpace(touchPos) + m_textField->getAnchorPoint() * size;

        if (pos.x < 0 || pos.x > size.width || pos.y < 0 || pos.y > size.height) {
            this->onClickTrackNode(false);
            return false;
        }
        if (m_delegate && !m_delegate->allowTextInput(this)) {
            this->onClickTrackNode(false);
            return false;
        }

        this->onClickTrackNode(true);
        this->updateCursorPosition(touchPos, {{0, 0}, size});

        return true;
    }
};