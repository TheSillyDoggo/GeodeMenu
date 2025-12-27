#pragma once

#include "../../../Client/Module.hpp"

using namespace geode::prelude;

class NoParticlesAdv : public Module
{
    public:
        MODULE_SETUP(NoParticlesAdv)
        {
            setName("No Particles");
            setID("no-particles-adv");
            setCategory("Cosmetic");
            setDescription("Disable specific particles in the game");
        }
};

class ParticleObjects : public Module
{
    public:
        MODULE_SETUP(ParticleObjects)
        {
            setName("Particle Objects");
            setID("no-particles-adv/particle-objects");
            setDescription("Disable advanced particle objects that creators placed in the editor");
            setPriority(1);
        }
};

class PlayerGroundParticles : public Module
{
    public:
        MODULE_SETUP(PlayerGroundParticles)
        {
            setName("Player movement particles");
            setID("no-particles-adv/player-ground-particles");
        }
};

class PlayerLandParticles : public Module
{
    public:
        MODULE_SETUP(PlayerLandParticles)
        {
            setName("Player land particles");
            setID("no-particles-adv/player-land-particles");
        }
};

class PlayerTrailParticles : public Module
{
    public:
        MODULE_SETUP(PlayerTrailParticles)
        {
            setName("Ship trail particles");
            setID("no-particles-adv/player-trail-particles");
        }
};

class ShipHoldParticles : public Module
{
    public:
        MODULE_SETUP(ShipHoldParticles)
        {
            setName("Ship boost particles");
            setID("no-particles-adv/player-hold-particles");
        }
};

class UfoClickParticles : public Module
{
    public:
        MODULE_SETUP(UfoClickParticles)
        {
            setName("Ufo Click particles");
            setID("no-particles-adv/ufo-click-particles");
        }
};

class ShipGroundParticles : public Module
{
    public:
        MODULE_SETUP(ShipGroundParticles)
        {
            setName("Vehicle ground particles");
            setID("no-particles-adv/ship-ground-particles");
        }
};

class RobotHoldParticles : public Module
{
    public:
        MODULE_SETUP(RobotHoldParticles)
        {
            setName("Robot boost particles");
            setID("no-particles-adv/robot-hold-particles");
        }
};

class GlitterParticles : public Module
{
    public:
        MODULE_SETUP(GlitterParticles)
        {
            setName("Glitter particles");
            setID("no-particles-adv/glitter-particles");
        }
};

class RingParticles : public Module
{
    public:
        MODULE_SETUP(RingParticles)
        {
            setName("Orb particles");
            setID("no-particles-adv/ring-particles");
        }
};

class PadParticles : public Module
{
    public:
        MODULE_SETUP(PadParticles)
        {
            setName("Pad particles");
            setID("no-particles-adv/pad-particles");
        }
};

class GravityPortalParticles : public Module
{
    public:
        MODULE_SETUP(GravityPortalParticles)
        {
            setName("Gravity Portal Particles");
            setID("no-particles-adv/gravity-portal-particles");
        }
};

class GamemodePortalParticles : public Module
{
    public:
        MODULE_SETUP(GamemodePortalParticles)
        {
            setName("Gamemode Portal Particles");
            setID("no-particles-adv/gamemode-portal-particles");
        }
};

class DualPortalParticles : public Module
{
    public:
        MODULE_SETUP(DualPortalParticles)
        {
            setName("Dual Portal Particles");
            setID("no-particles-adv/dual-portal-particles");
        }
};

class SizePortalParticles : public Module
{
    public:
        MODULE_SETUP(SizePortalParticles)
        {
            setName("Size Portal Particles");
            setID("no-particles-adv/size-portal-particles");
        }
};

class MirrorPortalParticles : public Module
{
    public:
        MODULE_SETUP(MirrorPortalParticles)
        {
            setName("Mirror Portal Particles");
            setID("no-particles-adv/mirror-portal-particles");
        }
};

class SpeedPortalParticles : public Module
{
    public:
        MODULE_SETUP(SpeedPortalParticles)
        {
            setName("Speed Portal Particles");
            setID("no-particles-adv/speed-portal-particles");
        }
};

class SpeedChangeParticles : public Module
{
    public:
        MODULE_SETUP(SpeedChangeParticles)
        {
            setName("Speed Change Particles");
            setID("no-particles-adv/speed-change-particles");
            setDescription("Disables the particles that play in the background when the players speed changes");
        }
};

class EndPortalParticles : public Module
{
    public:
        MODULE_SETUP(EndPortalParticles)
        {
            setName("End Portal Particles");
            setID("no-particles-adv/end-portal-particles");
            setDescription("Disables the particles that come out of the end portal");
        }
};

class SwingFireParticles : public Module
{
    public:
        MODULE_SETUP(SwingFireParticles)
        {
            setName("Swing Fire Particles");
            setID("no-particles-adv/swing-fire-particles");
        }
};

class CoinParticles : public Module
{
    public:
        MODULE_SETUP(CoinParticles)
        {
            setName("Collectables Particles");
            setID("no-particles-adv/coin-particles");
            setDescription("Disables particles on collectible objects such as coins");
        }
};

class DashOrbParticles : public Module
{
    public:
        MODULE_SETUP(DashOrbParticles)
        {
            setName("Dash Orb Particles");
            setID("no-particles-adv/dash-orb-particles");
            setDescription("Disables particles when using a dash orb");
        }
};

class TeleportPortalParticles : public Module
{
    public:
        MODULE_SETUP(TeleportPortalParticles)
        {
            setID("no-particles-adv/teleport-portal-particles");
        }
};

SUBMIT_HACK(NoParticlesAdv);
SUBMIT_OPTION(NoParticlesAdv, ParticleObjects);
SUBMIT_OPTION(NoParticlesAdv, PlayerGroundParticles);
SUBMIT_OPTION(NoParticlesAdv, PlayerLandParticles);
SUBMIT_OPTION(NoParticlesAdv, PlayerTrailParticles);
SUBMIT_OPTION(NoParticlesAdv, ShipHoldParticles);
SUBMIT_OPTION(NoParticlesAdv, UfoClickParticles);
SUBMIT_OPTION(NoParticlesAdv, ShipGroundParticles);
SUBMIT_OPTION(NoParticlesAdv, RobotHoldParticles);
SUBMIT_OPTION(NoParticlesAdv, GlitterParticles);
SUBMIT_OPTION(NoParticlesAdv, RingParticles);
SUBMIT_OPTION(NoParticlesAdv, PadParticles);
SUBMIT_OPTION(NoParticlesAdv, GravityPortalParticles);
SUBMIT_OPTION(NoParticlesAdv, GamemodePortalParticles);
SUBMIT_OPTION(NoParticlesAdv, DualPortalParticles);
SUBMIT_OPTION(NoParticlesAdv, SizePortalParticles);
SUBMIT_OPTION(NoParticlesAdv, MirrorPortalParticles);
SUBMIT_OPTION(NoParticlesAdv, SpeedPortalParticles);
SUBMIT_OPTION(NoParticlesAdv, SpeedChangeParticles);
SUBMIT_OPTION(NoParticlesAdv, EndPortalParticles);
SUBMIT_OPTION(NoParticlesAdv, SwingFireParticles);
SUBMIT_OPTION(NoParticlesAdv, CoinParticles);
SUBMIT_OPTION(NoParticlesAdv, DashOrbParticles);
SUBMIT_OPTION(NoParticlesAdv, TeleportPortalParticles);