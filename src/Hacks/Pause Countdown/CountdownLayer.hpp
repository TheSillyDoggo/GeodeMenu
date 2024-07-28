#include <Geode/Geode.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class CountdownLayer : public CCLayer
{
    public:
        CCLabelBMFont* label;
        int count = 3;

        void onDecrement(float);
        void applyAnimation();

        virtual bool init();
        virtual void onCountReachedZero();
        virtual void keyBackClicked();

        CREATE_FUNC(CountdownLayer);
};