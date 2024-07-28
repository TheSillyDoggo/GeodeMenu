//#include "PCGuiNode.hpp"
//
//PCGuiNode* PCGuiNode::create()
//{
//    auto pRet = new PCGuiNode();
//
//    if (pRet && pRet->init())
//    {
//        pRet->blur = CCBlurLayer::create();
//        pRet->blur->retain();
//        pRet->autorelease();
//        return pRet;
//    }
//
//    CC_SAFE_DELETE(pRet);
//    return nullptr;
//}
//
//PCGuiNode::~PCGuiNode()
//{
//    this->release();
//    blur->release();
//}
//
//PCGuiNode* PCGuiNode::get()
//{
//    if (!instance)
//    {
//        instance = PCGuiNode::create();
//        instance->retain();
//    }
//
//    return instance;
//}
//
//void PCGuiNode::update(float delta)
//{
//    Client::instance->animStatus += delta * (Client::instance->open ? 1 : -1) / 0.5f;
//    Client::instance->animStatus = clampf(Client::instance->animStatus, 0, 1);
//
//    Client::instance->delta = delta;
//}
//
//void PCGuiNode::visit()
//{
//    blur->setOpacity(Client::instance->animStatus * 50);
//    blur->visit();
//
//    CCNode::visit();
//}
//
//void PCGuiNode::draw()
//{
//    CCNode::draw();
//    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    if (!Client::instance)
//        return;
//
//    for (auto window : Client::instance->windows)
//    {
//        window->preDraw();
//        window->drawWindow();
//        window->postDraw();
//    }
//}
//
//class $modify (CCEGLView)
//{
//    virtual void swapBuffers()
//    {
//        PCGuiNode::get()->visit();
//
//        CCEGLView::swapBuffers();
//    }
//
//    static void onModify(auto& self) {
//        auto it = self.m_hooks.begin();
//        std::advance(it, 0);
//
//        if (auto h = it->second.get())
//        {
//            if (!Mod::get()->getSettingValue<bool>("use-new-ui"))
//                h->disable();
//        }
//    }
//};
//
////class $modify (CCDirector)
////{
////    void drawScene()
////    {
////        CCDirector::drawScene();
////
////        PCGuiNode::get()->visit();
////    }
////
////    static void onModify(auto& self) {
////        auto it = self.m_hooks.begin();
////        std::advance(it, 0);
////
////        if (auto h = it->second.get())
////        {
////            if (!Mod::get()->getSettingValue<bool>("use-new-ui"))
////                h->disable();
////        }
////    }
////};
//
//class $modify (CCScheduler)
//{
//    virtual void update(float delta)
//    {
//        PCGuiNode::get()->update(delta);
//
//        CCScheduler::update(delta);
//    }
//
//    static void onModify(auto& self) {
//        auto it = self.m_hooks.begin();
//        std::advance(it, 0);
//
//        if (auto h = it->second.get())
//        {
//            if (!Mod::get()->getSettingValue<bool>("use-new-ui"))
//                h->disable();
//        }
//    }
//};
//
//class $modify (CCTouchDispatcher)
//{
//    void touches(CCSet* touches, CCEvent* event, unsigned int type)
//    {
//        bool disableTouch = false;
//
//        if (auto touch = as<CCTouch*>(touches->anyObject()))
//        {
//            for (auto window : Client::instance->windows)
//            {
//                if (type == CCTOUCHBEGAN)
//                    disableTouch = disableTouch || window->touchBegan(touch->getLocation(), touch);
//
//                if (type == CCTOUCHMOVED)
//                    disableTouch = disableTouch || window->touchMoved(touch->getLocation(), touch);
//
//                if (type == CCTOUCHENDED)
//                    disableTouch = disableTouch || window->touchEndedOrCancelled(touch->getLocation(), touch, false);
//
//                if (type == CCTOUCHCANCELLED)
//                    disableTouch = disableTouch || window->touchEndedOrCancelled(touch->getLocation(), touch, true);
//            }
//        }
//
//        if (!disableTouch)
//            return CCTouchDispatcher::touches(touches, event, type);
//	}
//};

/*
CCTOUCHBEGAN = 0,
CCTOUCHMOVED = 1,
CCTOUCHENDED = 2,
CCTOUCHCANCELLED = 3,

ccTouchMax = 4,
*/