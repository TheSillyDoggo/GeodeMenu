#include "../../Client/InputModule.hpp"
#include <Geode/modify/CCMotionStreak.hpp>
#include "../../Utils/Num.hpp"

using namespace geode::prelude;

class TrailCutting : public Module
{
    public:
        MODULE_SETUP(TrailCutting)
        {
            setID("trail-cutting");
            setCategory("Cosmetic");
        }

        virtual void onToggle();
};

class TrailCuttingFPS : public InputModule
{
    public:
        MODULE_SETUP(TrailCuttingFPS)
        {
            setID("trail-cutting/fps");

            setDefaultString("20");
            setPlaceholderString("FPS");
            setHint("fps");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }

        virtual void onToggle()
        {
            TrailCutting::get()->onToggle();
        }
};

SUBMIT_HACK(TrailCutting)
SUBMIT_OPTION(TrailCutting, TrailCuttingFPS)

class $modify (CuttingMotionStreak, CCMotionStreak)
{
    bool initWithFade(float fade, float minSeg, float stroke, ccColor3B const& color, CCTexture2D* texture)
    {
        if (!CCMotionStreak::initWithFade(fade, minSeg, stroke, color, texture))
            return false;

        if (TrailCutting::get()->getRealEnabled())
            updateMaxPoints(TrailCuttingFPS::get()->getStringFloat());

        return true;
    }

    void updateMaxPoints(float fps)
    {
        CC_SAFE_DELETE(m_pPointState);
        CC_SAFE_DELETE(m_pPointVertexes);
        CC_SAFE_DELETE(m_pVertices);
        CC_SAFE_DELETE(m_pTexCoords);
        CC_SAFE_DELETE(m_pColorPointer);

        m_uMaxPoints = (int)((1.0f / m_fFadeDelta)*fps)+2;
        m_uNuPoints = 0;
        m_pPointState = (float *)malloc(sizeof(float) * m_uMaxPoints);
        m_pPointVertexes = (CCPoint*)malloc(sizeof(CCPoint) * m_uMaxPoints);

        m_pVertices = (ccVertex2F*)malloc(sizeof(ccVertex2F) * m_uMaxPoints * 2);
        m_pTexCoords = (ccTex2F*)malloc(sizeof(ccTex2F) * m_uMaxPoints * 2);
        m_pColorPointer = (GLubyte*)malloc(sizeof(GLubyte) * m_uMaxPoints * 2 * 4);

        reset();
    }
};

void TrailCutting::onToggle()
{
    std::vector<CCNode*> childs = {};
    getAllChildrenRecursive(CCScene::get(), childs);

    for (auto node : childs)
    {
        if (typeinfo_cast<CCMotionStreak*>(node))
        {
            static_cast<CuttingMotionStreak*>(node)->updateMaxPoints(TrailCutting::get()->getRealEnabled() ? TrailCuttingFPS::get()->getStringFloat() : 60 * 5);
        }
    }
}