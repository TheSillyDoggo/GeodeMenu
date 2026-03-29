#pragma once

#include <Geode/Geode.hpp>
#include <ColourConfig.hpp>
#include <EasyBG.hpp>
#include <BetterInputNode.hpp>
#include <FloatingButton/FloatingUIButton.hpp>
#include <SetupColourConfigUI.hpp>
#include <Button.hpp>

namespace qolmod
{
    enum class PaintTool
    {
        Brush = 0,
        Rubber = 1,
        Clear = 2,
    };

    class PaintControl : public FloatingUIButton, public TextInputDelegate
    {
        protected:
            struct PaintToolData
            {
                PaintTool tool = PaintTool::Brush;
                cocos2d::CCSprite* sprite = nullptr;
                qolmod::Button* button = nullptr;
            };
            std::unordered_map<PaintTool, PaintToolData> toolSprs = {};
            std::unordered_map<PaintTool, int> toolSizes = {};
            PaintTool selectedTool = PaintTool::Brush;
            Ref<EasyBG> bg = nullptr;
            geode::Ref<BetterInputNode> input = nullptr;
            geode::Ref<CCMenu> menu = nullptr;
            cocos2d::CCSprite* colourSpr = nullptr;
            ColourConfig colour = { ccc3(255, 255, 255) };
            int isMenuSelected = 0;
            bool isExpanded = false;
            geode::Ref<SetupColourConfigUI> ui = nullptr;

            virtual void textChanged(CCTextInputNode* node);

            virtual bool ccTouchBegan(cocos2d::CCTouch* touch);
            virtual void ccTouchMoved(cocos2d::CCTouch* touch);
            virtual void ccTouchEnded(cocos2d::CCTouch* touch);

            virtual bool init();
            virtual void update(float dt);
            virtual void setupChildren();
            virtual void updatePosition(cocos2d::CCPoint point);

            void addToolButton(PaintTool tool);

        public:
            CREATE_FUNC(PaintControl);
            static PaintControl* get();

            void onChangeTool(CCObject* sender);
            void onChangeColour(CCObject* sender);

            cocos2d::ccColor3B getColour();

            void updateOrientation(bool horizontal = true);
            void toggleExpanded(bool value);

            void setTool(PaintTool tool);
            PaintTool getTool();

            void setToolSize(PaintTool tool, int size);
            int getSelectedSize();

            bool getExpanded();
            bool isActive();

            virtual void preVisit();
            virtual void visit();
    };
};