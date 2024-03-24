#include "ModuleOptionsLayer.h"

void ModuleOptionsLayer::customSetup()
{
    int x = 0;
    int y = 0;

    CCPoint size = ccp(165, 35);

    for (size_t i = 0; i < mod->options.size(); i++)
    {
        CCPoint pos = ccp(this->size.x / 2 + (size.x * (x == 0 ? -1 : 0)), this->size.y - 50 - (size.y * y)) + ccp(15 + 7.5f, 0);

        if (mod->options[i]->description.size() != 0)
            pos.x -= 5;

        mod->options[i]->makeAndroid(baseLayer, pos);

        x++;
        if (x == 2)
        {
            x = 0;
            y++;
        }
    }
}