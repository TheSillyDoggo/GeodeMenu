#include "PlayerDeathAnimation.hpp"

bool PlayerDeathAnimation::init(int id)
{
    if (!CCNode::init())
        return false;

    if (id == 1)
    {
        isDefault = true;

        return true;
    }

    CCTextureCache::get()->addImage(fmt::format("PlayerExplosion_{:02}.png", id - 1).c_str(), false);
    CCSpriteFrameCache::get()->addSpriteFramesWithFile(fmt::format("PlayerExplosion_{:02}.plist", id - 1).c_str());

    for (size_t i = 1; i < 69; i++)
    {
        auto spr = fmt::format("playerExplosion_{:02}_{:03}.png", id - 1, i);

        if (auto frame = CCSpriteFrameCache::get()->spriteFrameByName(spr.c_str()))
        {
            if (auto sprite = CCSprite::createWithSpriteFrame(frame))
            {
                if (sprite->getUserObject("geode.texture-loader/fallback"))
                    break;

                bool b = false;

                for (auto frames : CCDictionaryExt<std::string, CCTexture2D*>(CCTextureCache::get()->m_pTextures))
                {
                    if (frames.second == sprite->getTexture())
                        if (frames.first.find("geode.texture-loader/fallback") != std::string::npos)
                            b = true;
                }

                if (b)
                    break;

                sprite->setVisible(i == 1);
                sprites.push_back(sprite);

                this->addChild(sprite);
            }
        }
        else
            break;
    }

    return true;
}

void PlayerDeathAnimation::run()
{
    if (isDefault)
    {
        if (auto particles = CCParticleSystemQuad::create("explodeEffect.plist", false))
        {
            particles->setPosition(ccp(0, 0));
            particles->resetSystem();
            this->addChild(particles, 2);
        }

        if (auto circle = CCCircleWave::create(20, 100, 0.5f, true))
        {
            this->addChild(circle, 1);
        }

        return;
    }

    this->schedule(schedule_selector(PlayerDeathAnimation::tick), 0.043f);
}

void PlayerDeathAnimation::tick(float)
{
    index++;

    for (size_t i = 0; i < sprites.size(); i++)
    {
        sprites[i]->setVisible(i == index);
    }

    if (index > sprites.size())
        this->removeFromParent();
}

PlayerDeathAnimation* PlayerDeathAnimation::create(int id)
{
    auto pRet = new PlayerDeathAnimation();

    if (pRet && pRet->init(id))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

PlayerDeathAnimation* PlayerDeathAnimation::createAndRun(int id)
{
    auto pRet = create(id);

    pRet->run();

    return pRet;
}