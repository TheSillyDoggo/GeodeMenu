/*#include "Window.h"

void Window::cocosCreate(CCMenu* menu)
{
    auto back = CCScale9Sprite::create("square02_small.png");
    back->setContentSize(menu->getContentSize() / 0.5f);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    back->setScale(0.5f);
    back->setOpacity(100);

    menu->addChild(back);

    int y = 0;

    for (size_t m = 0; m < modules.size(); m++)
    {
        float x = 20;

        if (!(m % 2 == 0))
            x = 188;

        modules[m]->makeAndroid(menu, {x, (menu->getContentSize().height - 20 - 20) - 7 - (28.0f * (y - 1.0f)) });

        if ((m - 1) % 2 == 0 && m != 0)
            y++;
    }
}*/