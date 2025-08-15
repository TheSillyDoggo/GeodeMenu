#include "LabelUtils.hpp"

LabelUtils* LabelUtils::get()
{
    static LabelUtils* instance = nullptr;

    if (!instance)
        instance = new LabelUtils();

    return instance;
}

CCRect LabelUtils::getSafeZone()
{
    // left, top, right, bottom
    return CCRectMake(3, 0, 3, 0);
}