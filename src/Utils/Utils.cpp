#include "Utils.hpp"
#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

float roundUpToMultipleOf2(float num)
{
	float roundedNum = std::round(num / 2.0f) * 2.0f;
	return roundedNum;
}

float scaleFloat(float v, float min, float max)
{
	return (max - min) * v + min;
}

float unscaleFloat(float v, float min, float max)
{
	float minValue = min;
	float maxValue = max;
	float originalValue = (v - minValue) / (maxValue - minValue);
	return originalValue;
}

geode::Patch* createPatchSafe(void *address, const geode::ByteVector &data)
{
	auto patch = geode::Mod::get()->patch(address, data);

	if (patch.isErr())
		geode::log::error("Failed to apply patch: {}, data: {}, error: {}", address, data, patch.err());

	return patch.unwrapOr(nullptr);
}

cocos2d::CCRect getScreenSafeArea()
{
	cocos2d::CCRect insets;

	#ifdef GEODE_IS_IOS
	insets = getScreenSafeAreaiOS();
	#endif

	return insets;
}

ImVec4 ccc4ToVec(ccColor4B col)
{
    return ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, col.a / 255.0f);
}

ccColor4B vecToCCC4(ImVec4 vec)
{
    return ccc4(vec.x * 255.0f, vec.y * 255.0f, vec.z * 255.0f, vec.w * 255.0f);
}

void ImGuiExt::colouredText(std::string label)
{
	bool isCol = false;
	bool v = false;
	int c = 0;
	ImColor colour = ImColor(1.0f, 1.0f, 1.0f);

	for (auto ch : label)
	{
		bool newLine = ch == '\n';

		if (isCol)
		{
			if (c == 0 && ch == '/')
			{
				colour = ImColor(1.0f, 1.0f, 1.0f);
				v = true;
			}

			if (c == 1 && !v)
			{
				if (ch == 'a')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_a", ccc4(150, 50, 255, 255)));

				if (ch == 'b')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_b", ccc4(74, 82, 225, 255)));

				if (ch == 'c')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_c", ccc4(255, 255, 150, 255)));

				if (ch == 'd')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_d", ccc4(255, 150, 255, 255)));

				if (ch == 'f')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_f", ccc4(150, 255, 255, 255)));

				if (ch == 'g')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_g", ccc4(64, 227, 72, 255)));

				if (ch == 'j')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_j", ccc4(50, 200, 255, 255)));

				if (ch == 'l')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_l", ccc4(96, 171, 239, 255)));

				if (ch == 'o')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_o", ccc4(255, 165, 75, 255)));

				if (ch == 'p')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_p", ccc4(255, 0, 255, 255)));

				if (ch == 'r')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_r", ccc4(255, 90, 90, 255)));

				if (ch == 's')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_s", ccc4(255, 220, 65, 255)));

				if (ch == 'y')
					colour = ccc4ToVec(Client::get()->getThemeColour("Desc_y", ccc4(255, 255, 0, 255)));
			}

			if (c == 2)
			{
				isCol = false;
				v = false;
			}

			c++;

			continue;
		}

		if (ch == '<')
		{
			isCol = true;
			v = false;
			c = 0;

			continue;
		}

		ImGui::TextColored(colour.Value, "%s", fmt::format("{}", ch).c_str());

		if (!newLine)
			ImGui::SameLine();
	}
}