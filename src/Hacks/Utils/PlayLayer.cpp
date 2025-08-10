#include "PlayLayer.hpp"

PlayLayerUtils* PlayLayerUtils::getUtils()
{
    return as<PlayLayerUtils*>(get());
}

bool PlayLayerUtils::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    m_fields->drawNode = CCDrawNode::create();
    m_objectLayer->addChild(m_fields->drawNode);

    return true;
}

void PlayLayerUtils::postUpdate(float p0)
{
    PlayLayer::postUpdate(p0);

    m_fields->drawNode->clear();
}

void PlayLayerUtils::addObject(GameObject* obj)
{
    PlayLayer::addObject(obj);

    if (obj->m_objectType == GameObjectType::UserCoin || obj->m_objectType == GameObjectType::SecretCoin)
    {
        m_fields->coins.push_back(as<EffectGameObject*>(obj));
    }
}

void PlayLayerUtils::onModify(auto& self)
{
    (void)self.setHookPriorityPost("PlayLayer::addObject", Priority::First);
    (void)self.setHookPriorityPost("PlayLayer::postUpdate", Priority::First);
}