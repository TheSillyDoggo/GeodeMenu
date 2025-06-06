#pragma once

#include <imgui-cocos.hpp>

class ColourUtility
{
	public:

		static cocos2d::ccColor3B cc3x(int hexValue) {
			if (hexValue <= 0xf)
				return geode::prelude::ccColor3B{
					static_cast<GLubyte>(hexValue * 17),
					static_cast<GLubyte>(hexValue * 17),
					static_cast<GLubyte>(hexValue * 17)};
			if (hexValue <= 0xff)
				return geode::prelude::ccColor3B{
					static_cast<GLubyte>(hexValue),
					static_cast<GLubyte>(hexValue),
					static_cast<GLubyte>(hexValue)};
			if (hexValue <= 0xfff)
				return geode::prelude::ccColor3B{
					static_cast<GLubyte>((hexValue >> 8 & 0xf) * 17),
					static_cast<GLubyte>((hexValue >> 4 & 0xf) * 17),
					static_cast<GLubyte>((hexValue >> 0 & 0xf) * 17)};
			else
				return geode::prelude::ccColor3B{
					static_cast<GLubyte>(hexValue >> 16 & 0xff),
					static_cast<GLubyte>(hexValue >> 8 & 0xff),
					static_cast<GLubyte>(hexValue >> 0 & 0xff)};
		}
		
		enum ClientColour
		{
			WindowBG,
			Accent,
			Text,
			Hovered,
			Pressed,
			InputField,
			InputCarot,
			GradientLeft,
			GradientRight,
		};

		#ifdef QOLMOD_IMGUI
		static ImColor GetColour(ClientColour colour);
		#endif

		static inline int pastel = 0;

		static inline float va = 0.0f;
		static inline float totalSessionTime = 0.0f;

		/*static inline std::vector<int> pastelV = { 0x6464ff,0x6464ff,0x6465ff,0x6465ff,0x6466ff,0x6467ff,0x6467ff,0x6468ff,0x6468ff,0x6469ff,0x646aff,0x646aff,0x646bff,0x646bff,0x646cff,0x646dff,0x646dff,0x646eff,0x646eff,0x646fff,0x6470ff,0x6470ff,0x6471ff,0x6471ff,0x6472ff,0x6473ff,0x6473ff,0x6474ff,0x6475ff,0x6475ff,0x6476ff,0x6476ff,0x6477ff,0x6478ff,0x6478ff,0x6479ff,0x6479ff,0x647aff,0x647bff,0x647bff,0x647cff,0x647cff,0x647dff,0x647eff,0x647eff,0x647fff,0x647fff,0x6480ff,0x6481ff,0x6481ff,0x6482ff,0x6483ff,0x6483ff,0x6484ff,0x6484ff,0x6485ff,0x6486ff,0x6486ff,0x6487ff,0x6487ff,0x6488ff,0x6489ff,0x6489ff,0x648aff,0x648aff,0x648bff,0x648cff,0x648cff,0x648dff,0x648dff,0x648eff,0x648fff,0x648fff,0x6490ff,0x6490ff,0x6491ff,0x6492ff,0x6492ff,0x6493ff,0x6494ff,0x6494ff,0x6495ff,0x6495ff,0x6496ff,0x6497ff,0x6497ff,0x6498ff,0x6498ff,0x6499ff,0x649aff,0x649aff,0x649bff,0x649bff,0x649cff,0x649dff,0x649dff,0x649eff,0x649eff,0x649fff,0x64a0ff,0x64a0ff,0x64a1ff,0x64a2ff,0x64a2ff,0x64a3ff,0x64a3ff,0x64a4ff,0x64a5ff,0x64a5ff,0x64a6ff,0x64a6ff,0x64a7ff,0x64a8ff,0x64a8ff,0x64a9ff,0x64a9ff,0x64aaff,0x64abff,0x64abff,0x64acff,0x64acff,0x64adff,0x64aeff,0x64aeff,0x64afff,0x64afff,0x64b0ff,0x64b1ff,0x64b1ff,0x64b2ff,0x64b3ff,0x64b3ff,0x64b4ff,0x64b4ff,0x64b5ff,0x64b6ff,0x64b6ff,0x64b7ff,0x64b7ff,0x64b8ff,0x64b9ff,0x64b9ff,0x64baff,0x64baff,0x64bbff,0x64bcff,0x64bcff,0x64bdff,0x64bdff,0x64beff,0x64bfff,0x64bfff,0x64c0ff,0x64c1ff,0x64c1ff,0x64c2ff,0x64c2ff,0x64c3ff,0x64c4ff,0x64c4ff,0x64c5ff,0x64c5ff,0x64c6ff,0x64c7ff,0x64c7ff,0x64c8ff,0x64c8ff,0x64c9ff,0x64caff,0x64caff,0x64cbff,0x64cbff,0x64ccff,0x64cdff,0x64cdff,0x64ceff,0x64ceff,0x64cfff,0x64d0ff,0x64d0ff,0x64d1ff,0x64d2ff,0x64d2ff,0x64d3ff,0x64d3ff,0x64d4ff,0x64d5ff,0x64d5ff,0x64d6ff,0x64d6ff,0x64d7ff,0x64d8ff,0x64d8ff,0x64d9ff,0x64d9ff,0x64daff,0x64dbff,0x64dbff,0x64dcff,0x64dcff,0x64ddff,0x64deff,0x64deff,0x64dfff,0x64e0ff,0x64e0ff,0x64e1ff,0x64e1ff,0x64e2ff,0x64e3ff,0x64e3ff,0x64e4ff,0x64e4ff,0x64e5ff,0x64e6ff,0x64e6ff,0x64e7ff,0x64e7ff,0x64e8ff,0x64e9ff,0x64e9ff,0x64eaff,0x64eaff,0x64ebff,0x64ecff,0x64ecff,0x64edff,0x64edff,0x64eeff,0x64efff,0x64efff,0x64f0ff,0x64f1ff,0x64f1ff,0x64f2ff,0x64f2ff,0x64f3ff,0x64f4ff,0x64f4ff,0x64f5ff,0x64f5ff,0x64f6ff,0x64f7ff,0x64f7ff,0x64f8ff,0x64f8ff,0x64f9ff,0x64faff,0x64faff,0x64fbff,0x64fbff,0x64fcff,0x64fdff,0x64fdff,0x64feff,0x64ffff,0x64fffe,0x64fffd,0x64fffd,0x64fffc,0x64fffb,0x64fffb,0x64fffa,0x64fffa,0x64fff9,0x64fff8,0x64fff8,0x64fff7,0x64fff7,0x64fff6,0x64fff5,0x64fff5,0x64fff4,0x64fff4,0x64fff3,0x64fff2,0x64fff2,0x64fff1,0x64fff1,0x64fff0,0x64ffef,0x64ffef,0x64ffee,0x64ffed,0x64ffed,0x64ffec,0x64ffec,0x64ffeb,0x64ffea,0x64ffea,0x64ffe9,0x64ffe9,0x64ffe8,0x64ffe7,0x64ffe7,0x64ffe6,0x64ffe6,0x64ffe5,0x64ffe4,0x64ffe4,0x64ffe3,0x64ffe3,0x64ffe2,0x64ffe1,0x64ffe1,0x64ffe0,0x64ffe0,0x64ffdf,0x64ffde,0x64ffde,0x64ffdd,0x64ffdc,0x64ffdc,0x64ffdb,0x64ffdb,0x64ffda,0x64ffd9,0x64ffd9,0x64ffd8,0x64ffd8,0x64ffd7,0x64ffd6,0x64ffd6,0x64ffd5,0x64ffd5,0x64ffd4,0x64ffd3,0x64ffd3,0x64ffd2,0x64ffd2,0x64ffd1,0x64ffd0,0x64ffd0,0x64ffcf,0x64ffce,0x64ffce,0x64ffcd,0x64ffcd,0x64ffcc,0x64ffcb,0x64ffcb,0x64ffca,0x64ffca,0x64ffc9,0x64ffc8,0x64ffc8,0x64ffc7,0x64ffc7,0x64ffc6,0x64ffc5,0x64ffc5,0x64ffc4,0x64ffc4,0x64ffc3,0x64ffc2,0x64ffc2,0x64ffc1,0x64ffc1,0x64ffc0,0x64ffbf,0x64ffbf,0x64ffbe,0x64ffbd,0x64ffbd,0x64ffbc,0x64ffbc,0x64ffbb,0x64ffba,0x64ffba,0x64ffb9,0x64ffb9,0x64ffb8,0x64ffb7,0x64ffb7,0x64ffb6,0x64ffb6,0x64ffb5,0x64ffb4,0x64ffb4,0x64ffb3,0x64ffb3,0x64ffb2,0x64ffb1,0x64ffb1,0x64ffb0,0x64ffaf,0x64ffaf,0x64ffae,0x64ffae,0x64ffad,0x64ffac,0x64ffac,0x64ffab,0x64ffab,0x64ffaa,0x64ffa9,0x64ffa9,0x64ffa8,0x64ffa8,0x64ffa7,0x64ffa6,0x64ffa6,0x64ffa5,0x64ffa5,0x64ffa4,0x64ffa3,0x64ffa3,0x64ffa2,0x64ffa2,0x64ffa1,0x64ffa0,0x64ffa0,0x64ff9f,0x64ff9e,0x64ff9e,0x64ff9d,0x64ff9d,0x64ff9c,0x64ff9b,0x64ff9b,0x64ff9a,0x64ff9a,0x64ff99,0x64ff98,0x64ff98,0x64ff97,0x64ff97,0x64ff96,0x64ff95,0x64ff95,0x64ff94,0x64ff94,0x64ff93,0x64ff92,0x64ff92,0x64ff91,0x64ff90,0x64ff90,0x64ff8f,0x64ff8f,0x64ff8e,0x64ff8d,0x64ff8d,0x64ff8c,0x64ff8c,0x64ff8b,0x64ff8a,0x64ff8a,0x64ff89,0x64ff89,0x64ff88,0x64ff87,0x64ff87,0x64ff86,0x64ff86,0x64ff85,0x64ff84,0x64ff84,0x64ff83,0x64ff83,0x64ff82,0x64ff81,0x64ff81,0x64ff80,0x64ff7f,0x64ff7f,0x64ff7e,0x64ff7e,0x64ff7d,0x64ff7c,0x64ff7c,0x64ff7b,0x64ff7b,0x64ff7a,0x64ff79,0x64ff79,0x64ff78,0x64ff78,0x64ff77,0x64ff76,0x64ff76,0x64ff75,0x64ff75,0x64ff74,0x64ff73,0x64ff73,0x64ff72,0x64ff71,0x64ff71,0x64ff70,0x64ff70,0x64ff6f,0x64ff6e,0x64ff6e,0x64ff6d,0x64ff6d,0x64ff6c,0x64ff6b,0x64ff6b,0x64ff6a,0x64ff6a,0x64ff69,0x64ff68,0x64ff68,0x64ff67,0x64ff67,0x64ff66,0x64ff65,0x64ff65,0x64ff64,0x64ff64,0x64ff64,0x65ff64,0x65ff64,0x66ff64,0x67ff64,0x67ff64,0x68ff64,0x68ff64,0x69ff64,0x6aff64,0x6aff64,0x6bff64,0x6bff64,0x6cff64,0x6dff64,0x6dff64,0x6eff64,0x6eff64,0x6fff64,0x70ff64,0x70ff64,0x71ff64,0x71ff64,0x72ff64,0x73ff64,0x73ff64,0x74ff64,0x75ff64,0x75ff64,0x76ff64,0x76ff64,0x77ff64,0x78ff64,0x78ff64,0x79ff64,0x79ff64,0x7aff64,0x7bff64,0x7bff64,0x7cff64,0x7cff64,0x7dff64,0x7eff64,0x7eff64,0x7fff64,0x7fff64,0x80ff64,0x81ff64,0x81ff64,0x82ff64,0x83ff64,0x83ff64,0x84ff64,0x84ff64,0x85ff64,0x86ff64,0x86ff64,0x87ff64,0x87ff64,0x88ff64,0x89ff64,0x89ff64,0x8aff64,0x8aff64,0x8bff64,0x8cff64,0x8cff64,0x8dff64,0x8dff64,0x8eff64,0x8fff64,0x8fff64,0x90ff64,0x90ff64,0x91ff64,0x92ff64,0x92ff64,0x93ff64,0x94ff64,0x94ff64,0x95ff64,0x95ff64,0x96ff64,0x97ff64,0x97ff64,0x98ff64,0x98ff64,0x99ff64,0x9aff64,0x9aff64,0x9bff64,0x9bff64,0x9cff64,0x9dff64,0x9dff64,0x9eff64,0x9eff64,0x9fff64,0xa0ff64,0xa0ff64,0xa1ff64,0xa2ff64,0xa2ff64,0xa3ff64,0xa3ff64,0xa4ff64,0xa5ff64,0xa5ff64,0xa6ff64,0xa6ff64,0xa7ff64,0xa8ff64,0xa8ff64,0xa9ff64,0xa9ff64,0xaaff64,0xabff64,0xabff64,0xacff64,0xacff64,0xadff64,0xaeff64,0xaeff64,0xafff64,0xafff64,0xb0ff64,0xb1ff64,0xb1ff64,0xb2ff64,0xb3ff64,0xb3ff64,0xb4ff64,0xb4ff64,0xb5ff64,0xb6ff64,0xb6ff64,0xb7ff64,0xb7ff64,0xb8ff64,0xb9ff64,0xb9ff64,0xbaff64,0xbaff64,0xbbff64,0xbcff64,0xbcff64,0xbdff64,0xbdff64,0xbeff64,0xbfff64,0xbfff64,0xc0ff64,0xc1ff64,0xc1ff64,0xc2ff64,0xc2ff64,0xc3ff64,0xc4ff64,0xc4ff64,0xc5ff64,0xc5ff64,0xc6ff64,0xc7ff64,0xc7ff64,0xc8ff64,0xc8ff64,0xc9ff64,0xcaff64,0xcaff64,0xcbff64,0xcbff64,0xccff64,0xcdff64,0xcdff64,0xceff64,0xceff64,0xcfff64,0xd0ff64,0xd0ff64,0xd1ff64,0xd2ff64,0xd2ff64,0xd3ff64,0xd3ff64,0xd4ff64,0xd5ff64,0xd5ff64,0xd6ff64,0xd6ff64,0xd7ff64,0xd8ff64,0xd8ff64,0xd9ff64,0xd9ff64,0xdaff64,0xdbff64,0xdbff64,0xdcff64,0xdcff64,0xddff64,0xdeff64,0xdeff64,0xdfff64,0xe0ff64,0xe0ff64,0xe1ff64,0xe1ff64,0xe2ff64,0xe3ff64,0xe3ff64,0xe4ff64,0xe4ff64,0xe5ff64,0xe6ff64,0xe6ff64,0xe7ff64,0xe7ff64,0xe8ff64,0xe9ff64,0xe9ff64,0xeaff64,0xeaff64,0xebff64,0xecff64,0xecff64,0xedff64,0xedff64,0xeeff64,0xefff64,0xefff64,0xf0ff64,0xf1ff64,0xf1ff64,0xf2ff64,0xf2ff64,0xf3ff64,0xf4ff64,0xf4ff64,0xf5ff64,0xf5ff64,0xf6ff64,0xf7ff64,0xf7ff64,0xf8ff64,0xf8ff64,0xf9ff64,0xfaff64,0xfaff64,0xfbff64,0xfbff64,0xfcff64,0xfdff64,0xfdff64,0xfeff64,0xffff64,0xfffe64,0xfffd64,0xfffd64,0xfffc64,0xfffb64,0xfffb64,0xfffa64,0xfffa64,0xfff964,0xfff864,0xfff864,0xfff764,0xfff764,0xfff664,0xfff564,0xfff564,0xfff464,0xfff464,0xfff364,0xfff264,0xfff264,0xfff164,0xfff164,0xfff064,0xffef64,0xffef64,0xffee64,0xffed64,0xffed64,0xffec64,0xffec64,0xffeb64,0xffea64,0xffea64,0xffe964,0xffe964,0xffe864,0xffe764,0xffe764,0xffe664,0xffe664,0xffe564,0xffe464,0xffe464,0xffe364,0xffe364,0xffe264,0xffe164,0xffe164,0xffe064,0xffe064,0xffdf64,0xffde64,0xffde64,0xffdd64,0xffdc64,0xffdc64,0xffdb64,0xffdb64,0xffda64,0xffd964,0xffd964,0xffd864,0xffd864,0xffd764,0xffd664,0xffd664,0xffd564,0xffd564,0xffd464,0xffd364,0xffd364,0xffd264,0xffd264,0xffd164,0xffd064,0xffd064,0xffcf64,0xffce64,0xffce64,0xffcd64,0xffcd64,0xffcc64,0xffcb64,0xffcb64,0xffca64,0xffca64,0xffc964,0xffc864,0xffc864,0xffc764,0xffc764,0xffc664,0xffc564,0xffc564,0xffc464,0xffc464,0xffc364,0xffc264,0xffc264,0xffc164,0xffc164,0xffc064,0xffbf64,0xffbf64,0xffbe64,0xffbd64,0xffbd64,0xffbc64,0xffbc64,0xffbb64,0xffba64,0xffba64,0xffb964,0xffb964,0xffb864,0xffb764,0xffb764,0xffb664,0xffb664,0xffb564,0xffb464,0xffb464,0xffb364,0xffb364,0xffb264,0xffb164,0xffb164,0xffb064,0xffaf64,0xffaf64,0xffae64,0xffae64,0xffad64,0xffac64,0xffac64,0xffab64,0xffab64,0xffaa64,0xffa964,0xffa964,0xffa864,0xffa864,0xffa764,0xffa664,0xffa664,0xffa564,0xffa564,0xffa464,0xffa364,0xffa364,0xffa264,0xffa264,0xffa164,0xffa064,0xffa064,0xff9f64,0xff9e64,0xff9e64,0xff9d64,0xff9d64,0xff9c64,0xff9b64,0xff9b64,0xff9a64,0xff9a64,0xff9964,0xff9864,0xff9864,0xff9764,0xff9764,0xff9664,0xff9564,0xff9564,0xff9464,0xff9464,0xff9364,0xff9264,0xff9264,0xff9164,0xff9064,0xff9064,0xff8f64,0xff8f64,0xff8e64,0xff8d64,0xff8d64,0xff8c64,0xff8c64,0xff8b64,0xff8a64,0xff8a64,0xff8964,0xff8964,0xff8864,0xff8764,0xff8764,0xff8664,0xff8664,0xff8564,0xff8464,0xff8464,0xff8364,0xff8364,0xff8264,0xff8164,0xff8164,0xff8064,0xff7f64,0xff7f64,0xff7e64,0xff7e64,0xff7d64,0xff7c64,0xff7c64,0xff7b64,0xff7b64,0xff7a64,0xff7964,0xff7964,0xff7864,0xff7864,0xff7764,0xff7664,0xff7664,0xff7564,0xff7564,0xff7464,0xff7364,0xff7364,0xff7264,0xff7164,0xff7164,0xff7064,0xff7064,0xff6f64,0xff6e64,0xff6e64,0xff6d64,0xff6d64,0xff6c64,0xff6b64,0xff6b64,0xff6a64,0xff6a64,0xff6964,0xff6864,0xff6864,0xff6764,0xff6764,0xff6664,0xff6564,0xff6564,0xff6464,0xff6464,0xff6464,0xff6465,0xff6465,0xff6466,0xff6467,0xff6467,0xff6468,0xff6468,0xff6469,0xff646a,0xff646a,0xff646b,0xff646b,0xff646c,0xff646d,0xff646d,0xff646e,0xff646e,0xff646f,0xff6470,0xff6470,0xff6471,0xff6471,0xff6472,0xff6473,0xff6473,0xff6474,0xff6475,0xff6475,0xff6476,0xff6476,0xff6477,0xff6478,0xff6478,0xff6479,0xff6479,0xff647a,0xff647b,0xff647b,0xff647c,0xff647c,0xff647d,0xff647e,0xff647e,0xff647f,0xff647f,0xff6480,0xff6481,0xff6481,0xff6482,0xff6483,0xff6483,0xff6484,0xff6484,0xff6485,0xff6486,0xff6486,0xff6487,0xff6487,0xff6488,0xff6489,0xff6489,0xff648a,0xff648a,0xff648b,0xff648c,0xff648c,0xff648d,0xff648d,0xff648e,0xff648f,0xff648f,0xff6490,0xff6490,0xff6491,0xff6492,0xff6492,0xff6493,0xff6494,0xff6494,0xff6495,0xff6495,0xff6496,0xff6497,0xff6497,0xff6498,0xff6498,0xff6499,0xff649a,0xff649a,0xff649b,0xff649b,0xff649c,0xff649d,0xff649d,0xff649e,0xff649e,0xff649f,0xff64a0,0xff64a0,0xff64a1,0xff64a2,0xff64a2,0xff64a3,0xff64a3,0xff64a4,0xff64a5,0xff64a5,0xff64a6,0xff64a6,0xff64a7,0xff64a8,0xff64a8,0xff64a9,0xff64a9,0xff64aa,0xff64ab,0xff64ab,0xff64ac,0xff64ac,0xff64ad,0xff64ae,0xff64ae,0xff64af,0xff64af,0xff64b0,0xff64b1,0xff64b1,0xff64b2,0xff64b3,0xff64b3,0xff64b4,0xff64b4,0xff64b5,0xff64b6,0xff64b6,0xff64b7,0xff64b7,0xff64b8,0xff64b9,0xff64b9,0xff64ba,0xff64ba,0xff64bb,0xff64bc,0xff64bc,0xff64bd,0xff64bd,0xff64be,0xff64bf,0xff64bf,0xff64c0,0xff64c1,0xff64c1,0xff64c2,0xff64c2,0xff64c3,0xff64c4,0xff64c4,0xff64c5,0xff64c5,0xff64c6,0xff64c7,0xff64c7,0xff64c8,0xff64c8,0xff64c9,0xff64ca,0xff64ca,0xff64cb,0xff64cb,0xff64cc,0xff64cd,0xff64cd,0xff64ce,0xff64ce,0xff64cf,0xff64d0,0xff64d0,0xff64d1,0xff64d2,0xff64d2,0xff64d3,0xff64d3,0xff64d4,0xff64d5,0xff64d5,0xff64d6,0xff64d6,0xff64d7,0xff64d8,0xff64d8,0xff64d9,0xff64d9,0xff64da,0xff64db,0xff64db,0xff64dc,0xff64dc,0xff64dd,0xff64de,0xff64de,0xff64df,0xff64e0,0xff64e0,0xff64e1,0xff64e1,0xff64e2,0xff64e3,0xff64e3,0xff64e4,0xff64e4,0xff64e5,0xff64e6,0xff64e6,0xff64e7,0xff64e7,0xff64e8,0xff64e9,0xff64e9,0xff64ea,0xff64ea,0xff64eb,0xff64ec,0xff64ec,0xff64ed,0xff64ed,0xff64ee,0xff64ef,0xff64ef,0xff64f0,0xff64f1,0xff64f1,0xff64f2,0xff64f2,0xff64f3,0xff64f4,0xff64f4,0xff64f5,0xff64f5,0xff64f6,0xff64f7,0xff64f7,0xff64f8,0xff64f8,0xff64f9,0xff64fa,0xff64fa,0xff64fb,0xff64fb,0xff64fc,0xff64fd,0xff64fd,0xff64fe,0xff64ff,0xfe64ff,0xfd64ff,0xfd64ff,0xfc64ff,0xfb64ff,0xfb64ff,0xfa64ff,0xfa64ff,0xf964ff,0xf864ff,0xf864ff,0xf764ff,0xf764ff,0xf664ff,0xf564ff,0xf564ff,0xf464ff,0xf464ff,0xf364ff,0xf264ff,0xf264ff,0xf164ff,0xf164ff,0xf064ff,0xef64ff,0xef64ff,0xee64ff,0xed64ff,0xed64ff,0xec64ff,0xec64ff,0xeb64ff,0xea64ff,0xea64ff,0xe964ff,0xe964ff,0xe864ff,0xe764ff,0xe764ff,0xe664ff,0xe664ff,0xe564ff,0xe464ff,0xe464ff,0xe364ff,0xe364ff,0xe264ff,0xe164ff,0xe164ff,0xe064ff,0xe064ff,0xdf64ff,0xde64ff,0xde64ff,0xdd64ff,0xdc64ff,0xdc64ff,0xdb64ff,0xdb64ff,0xda64ff,0xd964ff,0xd964ff,0xd864ff,0xd864ff,0xd764ff,0xd664ff,0xd664ff,0xd564ff,0xd564ff,0xd464ff,0xd364ff,0xd364ff,0xd264ff,0xd264ff,0xd164ff,0xd064ff,0xd064ff,0xcf64ff,0xce64ff,0xce64ff,0xcd64ff,0xcd64ff,0xcc64ff,0xcb64ff,0xcb64ff,0xca64ff,0xca64ff,0xc964ff,0xc864ff,0xc864ff,0xc764ff,0xc764ff,0xc664ff,0xc564ff,0xc564ff,0xc464ff,0xc464ff,0xc364ff,0xc264ff,0xc264ff,0xc164ff,0xc164ff,0xc064ff,0xbf64ff,0xbf64ff,0xbe64ff,0xbd64ff,0xbd64ff,0xbc64ff,0xbc64ff,0xbb64ff,0xba64ff,0xba64ff,0xb964ff,0xb964ff,0xb864ff,0xb764ff,0xb764ff,0xb664ff,0xb664ff,0xb564ff,0xb464ff,0xb464ff,0xb364ff,0xb364ff,0xb264ff,0xb164ff,0xb164ff,0xb064ff,0xaf64ff,0xaf64ff,0xae64ff,0xae64ff,0xad64ff,0xac64ff,0xac64ff,0xab64ff,0xab64ff,0xaa64ff,0xa964ff,0xa964ff,0xa864ff,0xa864ff,0xa764ff,0xa664ff,0xa664ff,0xa564ff,0xa564ff,0xa464ff,0xa364ff,0xa364ff,0xa264ff,0xa264ff,0xa164ff,0xa064ff,0xa064ff,0x9f64ff,0x9e64ff,0x9e64ff,0x9d64ff,0x9d64ff,0x9c64ff,0x9b64ff,0x9b64ff,0x9a64ff,0x9a64ff,0x9964ff,0x9864ff,0x9864ff,0x9764ff,0x9764ff,0x9664ff,0x9564ff,0x9564ff,0x9464ff,0x9464ff,0x9364ff,0x9264ff,0x9264ff,0x9164ff,0x9064ff,0x9064ff,0x8f64ff,0x8f64ff,0x8e64ff,0x8d64ff,0x8d64ff,0x8c64ff,0x8c64ff,0x8b64ff,0x8a64ff,0x8a64ff,0x8964ff,0x8964ff,0x8864ff,0x8764ff,0x8764ff,0x8664ff,0x8664ff,0x8564ff,0x8464ff,0x8464ff,0x8364ff,0x8364ff,0x8264ff,0x8164ff,0x8164ff,0x8064ff,0x7f64ff,0x7f64ff,0x7e64ff,0x7e64ff,0x7d64ff,0x7c64ff,0x7c64ff,0x7b64ff,0x7b64ff,0x7a64ff,0x7964ff,0x7964ff,0x7864ff,0x7864ff,0x7764ff,0x7664ff,0x7664ff,0x7564ff,0x7564ff,0x7464ff,0x7364ff,0x7364ff,0x7264ff,0x7164ff,0x7164ff,0x7064ff,0x7064ff,0x6f64ff,0x6e64ff,0x6e64ff,0x6d64ff,0x6d64ff,0x6c64ff,0x6b64ff,0x6b64ff,0x6a64ff,0x6a64ff,0x6964ff,0x6864ff,0x6864ff,0x6764ff,0x6764ff,0x6664ff,0x6564ff,0x6564ff,0x6464ff };*/

		static void update(float dt)
		{
			va += dt;
		}

		static geode::prelude::ccColor3B hsvToRgb(const geode::prelude::ccHSVValue& hsv) {
			float hue = hsv.h;
			float saturation = hsv.s;
			float value = hsv.v;

			int hi = static_cast<int>(std::floor(hue / 60.0f)) % 6;
			float f = hue / 60.0f - std::floor(hue / 60.0f);

			float p = value * (1 - saturation);
			float q = value * (1 - f * saturation);
			float t = value * (1 - (1 - f) * saturation);

			float r, g, b;

			switch (hi) {
				case 0: r = value; g = t; b = p; break;
				case 1: r = q; g = value; b = p; break;
				case 2: r = p; g = value; b = t; break;
				case 3: r = p; g = q; b = value; break;
				case 4: r = t; g = p; b = value; break;
				case 5: r = value; g = p; b = q; break;
				default: r = g = b = 0; break;
			}

			return geode::prelude::ccc3(static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255));
		}

		static geode::prelude::ccColor3B getChromaColour(float v = -1)
		{
			float a = v == -1 ? va : v;

			return hsvToRgb(geode::prelude::cchsv((a * 180) / 10.0f, 1.0f, 1.0f, true, true));
		}

		static geode::prelude::ccColor3B getPastelColour(float v = -1, int i = 0)
		{
			float a = v == -1 ? va : v;

			return hsvToRgb(geode::prelude::cchsv((a * 180) / 10.0f, 155.0f / 255.0f, 1.0f, true, true));
/*
			int v = round((va * pastelV.size()) / 10.0f);

			v = v % pastelV.size();

			return cc3x(pastelV[v]);*/
		}

		static geode::prelude::ccColor3B lerpColour(const geode::prelude::ccColor3B& color1, const geode::prelude::ccColor3B& color2, float t) {
			if (t < 0)
				t = 0;

			if (t > 1)
				t = 1;

			return geode::prelude::ccc3(
				static_cast<uint8_t>(color1.r + (color2.r - color1.r) * t),
				static_cast<uint8_t>(color1.g + (color2.g - color1.g) * t),
				static_cast<uint8_t>(color1.b + (color2.b - color1.b) * t)
			);
		}

};