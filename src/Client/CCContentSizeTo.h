#include <Geode/Geode.hpp>

//definetly not ai generated

using namespace geode::prelude;

class CCContentSizeTo : public cocos2d::CCActionInterval
{
public:
    static CCContentSizeTo* create(float duration, const cocos2d::CCSize& size)
    {
        CCContentSizeTo* action = new CCContentSizeTo();
        if (action && action->initWithDuration(duration, size))
        {
            action->autorelease();
            return action;
        }

        delete action;
        return nullptr;
    }

    virtual void update(float time)
    {
        if (_target)
        {
            cocos2d::CCSize newSize(_startSize.width + (_endSize.width - _startSize.width) * time,
                                    _startSize.height + (_endSize.height - _startSize.height) * time);
            _target->setContentSize(newSize);
        }
    }

protected:

    bool initWithDuration(float duration, const cocos2d::CCSize& size)
    {
        if (CCActionInterval::initWithDuration(duration))
        {
            _endSize = size;
            return true;
        }

        return false;
    }

    void startWithTarget(cocos2d::CCNode* target)
    {
        CCActionInterval::startWithTarget(target);
        _target = target;
        _startSize = target->getContentSize();
    }


    cocos2d::CCSize _endSize;
    cocos2d::CCSize _startSize;
    cocos2d::CCNode* _target;
};