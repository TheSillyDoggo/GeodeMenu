#include "HitboxColours.hpp"
#include "ShowHitboxes.hpp"
#include "../HitboxUtils.hpp"
#include "Hooks.hpp"

SUBMIT_HACK(ShowHitboxes);
SUBMIT_HACK(ShowHitboxesOnDeath);
SUBMIT_HACK(HitboxTrail);

SUBMIT_OPTION(ShowHitboxes, HitboxSolid);
SUBMIT_OPTION(ShowHitboxes, HitboxHazard);
SUBMIT_OPTION(ShowHitboxes, HitboxPassable);
SUBMIT_OPTION(ShowHitboxes, HitboxInteractable);
SUBMIT_OPTION(ShowHitboxes, HitboxPlayer);
SUBMIT_OPTION(ShowHitboxes, HitboxPlayerRot);

SUBMIT_OPTION(ShowHitboxes, HitboxThickOutline);
SUBMIT_OPTION(ShowHitboxes, HitboxFill);
SUBMIT_OPTION(ShowHitboxes, HitboxFillOpacity);

SUBMIT_OPTION(HitboxTrail, HitboxTrailMaxPositions);
SUBMIT_OPTION(HitboxTrail, HitboxTrailResetOnDeath);
SUBMIT_OPTION(HitboxTrail, HitboxTrailDarkenByAge);
SUBMIT_OPTION(HitboxTrail, HitboxTrailDoClickColours);
SUBMIT_OPTION(HitboxTrail, HitboxTrailStartClickCol);
SUBMIT_OPTION(HitboxTrail, HitboxTrailEndClickCol);
SUBMIT_OPTION(HitboxTrail, HitboxTrailHoldClickColours);
SUBMIT_OPTION(HitboxTrail, HitboxTrailMidClickCol);

SUBMIT_OPTION(ShowHitboxesOnDeath, ShowHitboxesOnDeathDeathObjOnly);
SUBMIT_OPTION(ShowHitboxesOnDeath, ShowHitboxesOnDeathTrail);

void ShowHitboxes::onToggle()
{
    if (auto gjbgl = GJBaseGameLayer::get())
    {
        base_cast<HitboxBaseGameLayer*>(gjbgl)->onTick(false);
    }
}