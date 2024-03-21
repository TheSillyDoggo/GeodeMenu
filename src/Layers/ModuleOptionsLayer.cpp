#include "ModuleOptionsLayer.h"

void ModuleOptionsLayer::customSetup()
{
    int x = 0;
    int y = 0;

    CCPoint size = ccp(140, 30);

    for (size_t i = 0; i < mod->options.size(); i++)
    {
        mod->options[i]->makeAndroid(baseLayer, ccp(0 + 140 * x, (floorf(mod->options.size() / 2) * size.y) - (size.y * y)) + ccp(this->size.x / 2 - size.x, 0));

        x++;
        if (x == 2)
        {
            x = 0;
            y++;
        }
    }
}