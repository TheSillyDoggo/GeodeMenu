#include "../../Client/Module.hpp"
#include <Geode/modify/HardStreak.hpp>

using namespace geode::prelude;

class NoWaveTrail : public Module
{
    public:
        MODULE_SETUP(NoWaveTrail)
        {
            setName("No Wave Trail");
            setID("no-wave");
            setCategory("Cosmetic");
            setDescription("Disables the wave trail");
        }
};

SUBMIT_HACK(NoWaveTrail);

class $modify (HardStreak)
{
    void updateStroke(float unused)
    {
        // run original function for other mods that hook it
        HardStreak::updateStroke(unused);

        if (NoWaveTrail::get()->getRealEnabled())
            this->clear();
    }

    static void onModify(auto& self)
    {
        (void)self.setHookPriorityPost("HardStreak::updateStroke", Priority::Last);
    }
};