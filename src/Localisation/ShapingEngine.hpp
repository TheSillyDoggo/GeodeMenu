//https://github.com/DelinWorks/arabic-text-renderer-cpp-axmol/blob/main/source/ShapingEngine.hpp

#pragma once

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <wchar.h>
#include <stdio.h>
#include <codecvt>

/*
* 
* Unicode Spaces and character information
* Sources: https://www.compart.com/en/unicode/
* 
*/

#define RENDER_SYMBOLS L".,<>(){}[]~`!@#$%^&*?\"':;\\\u200C"
#define DISCARD_CHARS L"\u200C"

namespace ShapingEngine {

    namespace Options {
        // converts normal numbers 123 to the Hindu–Arabic or Indo–Arabic numerals automatically.
        inline bool _convertToArabicNumbers = false;
        inline bool _discardChars = false;
    }

    namespace Helper {

        inline void split(const std::string& str, const char* delim, std::vector<std::string>& out)
        {
            size_t start;
            size_t end = 0;

            while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
            {
                end = str.find(delim, start);
                out.push_back(str.substr(start, end - start));
            }
        }

        inline void wsplit(const std::wstring& str, const wchar_t* delim, std::vector<std::wstring>& out)
        {
            size_t start;
            size_t end = 0;

            while ((start = str.find_first_not_of(delim, end)) != std::wstring::npos)
            {
                end = str.find(delim, start);
                out.push_back(str.substr(start, end - start));
            }
        }

        inline bool replace(std::string& str, const std::string& from, const std::string& to) {
            size_t start_pos = str.find(from);
            if (start_pos == std::string::npos)
                return false;
            str.replace(start_pos, from.length(), to);
            return true;
        }

        inline bool wreplace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
            size_t start_pos = str.find(from);
            if (start_pos == std::wstring::npos)
                return false;
            str.replace(start_pos, from.length(), to);
            return true;
        }

        inline std::wstring widen(const std::string& utf8)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
            std::u16string utf16 = convert.from_bytes(utf8);
            std::wstring wstr(utf16.begin(), utf16.end());
            return wstr;
        }

        inline std::string narrow(const std::wstring& utf16) {
            std::u16string u16str(utf16.begin(), utf16.end());
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
            std::string utf8 = convert.to_bytes(u16str);
            return utf8;
        }

        inline bool contains_wchar_t(const wchar_t* text, wchar_t letter) {
            int i = 0;
            while (text[i] != '\0')
            {
                if (text[i] == letter)
                    return true;
                i++;
            }
            return false;
        }

        // Arabic letters range from U+0600 to U+06FF
        // (and U+FB50 to U+FDFF for Arabic Presentation Forms-A)
        // (and U+FE70 to U+FEFF for Arabic Presentation Forms-B)
        inline bool is_arabic_letter(const wchar_t c, bool space = false, bool symbols = false) {
            int asciiValue = (int)c;

            if (asciiValue >= 1536 && asciiValue <= 1791
                || asciiValue >= 64336 && asciiValue <= 65023
                || asciiValue >= 65136 && asciiValue <= 65279 || asciiValue == ' ' && space
                || (symbols && contains_wchar_t(RENDER_SYMBOLS, asciiValue))) {
                return true;
            }

            return false;
        }

        // Arabic letters range from U+0600 to U+06FF
        // (and U+FB50 to U+FDFF for Arabic Presentation Forms-A)
        // (and U+FE70 to U+FEFF for Arabic Presentation Forms-B)
        inline bool is_arabic_word(std::wstring& c) {
            for (int i = 0; i < c.length(); i++)
                if (is_arabic_letter(c[i]))
                    return true;
            return false;
        }

        // check if the character is a vowel.
        inline bool is_arabic_vowel(const wchar_t v) {
            int asciiValue = (int)v;
            if (asciiValue >= 1611 && asciiValue <= 1631)
                return true;
            return false;
        }
    }

    class Glyph;

    static std::map<int, Glyph> glyphs;

    class Glyph {
    public:
        wchar_t glyphInitial;
        wchar_t glyphMiddle;
        wchar_t glyphFinal;

        Glyph(std::wstring three_char_glyph) {
            this->glyphInitial = three_char_glyph[0];
            this->glyphMiddle = three_char_glyph[1];
            this->glyphFinal = three_char_glyph[2];
        }

        Glyph(wchar_t glyphInitial, wchar_t glyphMiddle, wchar_t glyphFinal) {
            this->glyphInitial = glyphInitial;
            this->glyphMiddle = glyphMiddle;
            this->glyphFinal = glyphFinal;
        }

        bool _initial() {
            return glyphInitial == ' ' ? false : true;
        }

        bool _middle() {
            return glyphMiddle == ' ' ? false : true;
        }

        bool _final() {
            return glyphFinal == ' ' ? false : true;
        }

        bool _initial_or_middle() {
            return _initial() || _middle();
        }

        bool _middle_or_final() {
            return _middle() || _final();
        }

        static void init_arabic() {
            if (glyphs.size() != 0)
                return;

            // ****** SPACES ARE IMPORTANT! DO NOT REMOVE!! ****** //

            glyphs.insert({ L'\u0640', Glyph(L"\u0640 \u0640") }); // ـ
            glyphs.insert({ L'\u0627', Glyph(L"  \uFE8E") }); // ا
            glyphs.insert({ L'\u0623', Glyph(L"  \uFE84") }); // أ
            glyphs.insert({ L'\u0625', Glyph(L"  \uFE88") }); // إ
            glyphs.insert({ L'\u0622', Glyph(L"  \uFE82") }); // آ
            glyphs.insert({ L'\u0628', Glyph(L"\uFE91\uFE92\uFE90") }); // ب
            glyphs.insert({ L'\u062A', Glyph(L"\uFE97\uFE98\uFE96") }); // ت
            glyphs.insert({ L'\u062B', Glyph(L"\uFE9B\uFE9C\uFE9A") }); // ث
            glyphs.insert({ L'\u062C', Glyph(L"\uFE9F\uFEA0\uFE9E") }); // ج
            glyphs.insert({ L'\u062D', Glyph(L"\uFEA3\uFEA4\uFEA2") }); // ح
            glyphs.insert({ L'\u062E', Glyph(L"\uFEA7\uFEA8\uFEA6") }); // خ
            glyphs.insert({ L'\u062F', Glyph(L"  \uFEAA") }); // د
            glyphs.insert({ L'\u0630', Glyph(L"  \uFEAC") }); // ذ
            glyphs.insert({ L'\u0631', Glyph(L"  \uFEAE") }); // ر
            glyphs.insert({ L'\u0632', Glyph(L"  \uFEB0") }); // ز
            glyphs.insert({ L'\u0633', Glyph(L"\uFEB3\uFEB4\uFEB2") }); // س
            glyphs.insert({ L'\u0634', Glyph(L"\uFEB7\uFEB8\uFEB6") }); // ش
            glyphs.insert({ L'\u0635', Glyph(L"\uFEBB\uFEBC\uFEBA") }); // ص
            glyphs.insert({ L'\u0636', Glyph(L"\uFEBF\uFEC0\uFEBE") }); // ض
            glyphs.insert({ L'\u0637', Glyph(L"\uFEC3\uFEC4\uFEC2") }); // ط
            glyphs.insert({ L'\u0638', Glyph(L"\uFEC7\uFEC8\uFEC6") }); // ظ
            glyphs.insert({ L'\u0639', Glyph(L"\uFECB\uFECC\uFECA") }); // ع
            glyphs.insert({ L'\u063A', Glyph(L"\uFECF\uFED0\uFECE") }); // غ
            glyphs.insert({ L'\u0641', Glyph(L"\uFED3\uFED4\uFED2") }); // ف
            glyphs.insert({ L'\u0642', Glyph(L"\uFED7\uFED8\uFED6") }); // ق
            glyphs.insert({ L'\u0643', Glyph(L"\uFEDB\uFEDC\uFEDA") }); // ك
            glyphs.insert({ L'\u0644', Glyph(L"\uFEDF\uFEE0\uFEDE") }); // ل
            glyphs.insert({ L'\u0645', Glyph(L"\uFEE3\uFEE4\uFEE2") }); // م
            glyphs.insert({ L'\u0646', Glyph(L"\uFEE7\uFEE8\uFEE6") }); // ن
            glyphs.insert({ L'\u0647', Glyph(L"\uFEEB\uFEEC\uFEEA") }); // ه
            glyphs.insert({ L'\u0648', Glyph(L"  \uFEEE") }); // و
            glyphs.insert({ L'\u064A', Glyph(L"\uFEF3\uFEF4\uFEF2") }); // ي
            glyphs.insert({ L'\u0626', Glyph(L"\uFE8B\uFE8C\uFE8A") }); // ئ
            glyphs.insert({ L'\u0649', Glyph(L"\u0649\u0649\uFEF0") }); // ى
            glyphs.insert({ L'\u0624', Glyph(L"  \uFE86") }); // ؤ
            glyphs.insert({ L'\u0629', Glyph(L"  \uFE94") }); // ة
            glyphs.insert({ L'\uFEFB', Glyph(L"  \uFEFC") }); // لا
            glyphs.insert({ L'\uFEF7', Glyph(L"  \uFEF8") }); // لأ
            glyphs.insert({ L'\uFEF9', Glyph(L"  \uFEFA") }); // لإ
            glyphs.insert({ L'\uFEF5', Glyph(L"  \uFED6") }); // لآ
        }
    };

    inline Glyph _glyph(int glyph) {
        auto pos = glyphs.find(glyph);
        if (pos != glyphs.end())
            return pos->second;
        else return Glyph(L"   ");
    }

    struct vowel_index {
        int index;
        int unicode;
    };

    // Finds and reoders any arabic text in the wstring.
    inline void reorder_glyphs(std::wstring& t, bool symbols, bool correct_num_order) {
        bool f = false;
        int ix = 0;

        // two iterations, cannot be combined since ordering will change indices.
        // and will not work, use correct_num_order when needed for performance.

        if (correct_num_order)
            for (int i = 0; i < t.length(); i++)
            {
                wchar_t l = t[i];
                if ((l >= 48 && l <= 57) || l == 46)
                {
                    ix = i;
                    int iy = i;
                    while ((t[iy] >= 48 && t[iy] <= 57) || t[iy] == 46)
                        iy++;
                    std::reverse(t.begin() + ix, t.begin() + iy);
                    i = iy;
                }
            }

        for (int i = 0; i < t.length(); i++) {
            wchar_t l = t[i];
            if (Helper::is_arabic_letter(l, f, symbols) && !f) {
                f = true;
                ix = i;
            }
            if (!Helper::is_arabic_letter(l, true, symbols) && !((l >= 48 && l <= 57) || (l >= 45 && l <= 46)) && f) {
                f = false;
                int iy = i;
                while (t[ix] == ' ')
                    ix++;
                while (t[iy - 1] == ' ')
                    iy--;
                std::reverse(t.begin() + ix, t.begin() + iy);
            }
        }
        if (f) {
            while (t[ix] == ' ')
                ix++;
            std::reverse(t.begin() + ix, t.end());
        }
    }

    // Finds and shapes any arabic text in the wstring and then returns it.
    // (Converts to arabic presentation forms A-B, Also Takes care of vowels in words).
    inline void shape_glyphs(std::wstring& t) {
        std::vector<vowel_index> vowels;
        for (int i = 0; i < t.length(); i++)
        {
            if (Helper::is_arabic_vowel(t[i]))
                vowels.push_back({ i, (int)t[i] });
        }
        wchar_t ww[2];
        for (int i = 1611; i <= 1631; i++)
        {
            ww[0] = (wchar_t)i;
            ww[1] = '\x0';
            while (Helper::wreplace(t, ww, L""));
        }
        std::wstring c = t;
        for (int i = 0; i < c.length(); i++) {
            if (i == 0) {
                auto g = _glyph(c[i]);
                if (g._initial() && _glyph(c[i + 1])._middle_or_final())
                    t[i] = g.glyphInitial;
            }
            else if (i == c.length() - 1) {
                auto g = _glyph(c[i]);
                if (g._final() && (_glyph(c[i - 1])._initial_or_middle()))
                    t[i] = g.glyphFinal;
            }
            else {
                auto g = _glyph(c[i]);
                if (g._middle() && (_glyph(c[i - 1])._initial()))
                {
                    t[i] = g.glyphFinal;
                    if ((_glyph(c[i + 1])._middle_or_final()))
                        t[i] = g.glyphMiddle;
                }
                else if (g._middle() && (_glyph(c[i + 1])._final()))
                {
                    t[i] = g.glyphInitial;
                    if ((_glyph(c[i - 1])._initial_or_middle()))
                        t[i] = g.glyphMiddle;
                }
                else if (g._final() && (_glyph(c[i - 1])._initial_or_middle()))
                    t[i] = g.glyphFinal;
                else if (g._initial() && (_glyph(c[i + 1])._middle_or_final()))
                    t[i] = g.glyphInitial;
            }
        }

        for (auto i : vowels) {
            ww[0] = (wchar_t)i.unicode;
            ww[1] = '\x0';
            t.insert(i.index, ww);
        }
        vowels.clear();
    }

    inline void discard_chars(std::wstring& t)
    {
        size_t writeIndex = 0;
        for (size_t readIndex = 0; readIndex < t.length(); ++readIndex) {
            if (wcschr(DISCARD_CHARS, t[readIndex]) == nullptr) {
                if (writeIndex != readIndex) {
                    t[writeIndex] = t[readIndex];
                }
                ++writeIndex;
            }
        }
        if (writeIndex < t.length())
            t.resize(writeIndex);
    }

    // Converts normal numbers 123 to the Hindu–Arabic or Indo–Arabic numerals
    // returns a wide string
    inline std::wstring w_arabify_numbers(std::wstring& t) {
        for (int i = 0; i < t.length(); i++)
        {
            if (t[i] == L'0')
                t[i] = L'\u0660';
            else if (t[i] == L'1')
                t[i] = L'\u0661';
            else if (t[i] == L'2')
                t[i] = L'\u0662';
            else if (t[i] == L'3')
                t[i] = L'\u0663';
            else if (t[i] == L'4')
                t[i] = L'\u0664';
            else if (t[i] == L'5')
                t[i] = L'\u0665';
            else if (t[i] == L'6')
                t[i] = L'\u0666';
            else if (t[i] == L'7')
                t[i] = L'\u0667';
            else if (t[i] == L'8')
                t[i] = L'\u0668';
            else if (t[i] == L'9')
                t[i] = L'\u0669';
        }
        return t;
    }

    // Converts normal numbers 123 to the Hindu–Arabic or Indo–Arabic numerals
    // returns a narrowed string
    inline std::string arabify_numbers(std::wstring& t) {
        return Helper::narrow(w_arabify_numbers(t));
    }

    // Converts normal numbers 123 to the Hindu–Arabic or Indo–Arabic numerals
    // returns a narrowed string
    inline std::string arabify_numbers(std::string& t) {
        auto n = Helper::widen(t);
        return Helper::narrow(w_arabify_numbers(n));
    }

    // Render a piece of text containing arabic text. The string passed will not be copied and will be modified directly
    // @param render_with_symbols renders arabic text while treating symbol characters like arabic letter.
    inline void render_ref(std::wstring& t, bool render_with_symbols = false, bool correct_num_order = false) {
        Glyph::init_arabic();
        std::vector<std::wstring> words;
        Helper::wsplit(t, L" ", words);
        // *** For special characters like لا لأ لإ لآ
        while (Helper::wreplace(t, L"\u0644\u0627", L"\uFEFB")); // لا
        while (Helper::wreplace(t, L"\u0644\u0623", L"\uFEF7")); // لأ
        while (Helper::wreplace(t, L"\u0644\u0625", L"\uFEF9")); // لإ
        while (Helper::wreplace(t, L"\u0644\u0622", L"\uFEF5")); // لآ
        // ***************************************
        shape_glyphs(t);
        reorder_glyphs(t, render_with_symbols, correct_num_order);
        if (Options::_discardChars)
            discard_chars(t);
        if (Options::_convertToArabicNumbers)
            t = w_arabify_numbers(t);
    }

    // Render a piece of text containing arabic text. The returned string is NOT narrowed
    // @param render_with_symbols renders arabic text while treating symbol characters like arabic letter.
    // @param correct_num_order retains order of english numbers when rendered with arabic text.
    inline std::wstring wrender(std::wstring t, bool render_with_symbols = false, bool correct_num_order = false) {
        std::wstring out = t;
        render_ref(out, render_with_symbols, correct_num_order);
        return out;
    }

    // Render a piece of text containing arabic text. The returned string IS narrowed
    // @param render_with_symbols renders arabic text while treating symbol characters like arabic letter.
    // @param correct_num_order retains order of english numbers when rendered with arabic text.
    inline std::string render(std::wstring& t, bool render_with_symbols = false, bool correct_num_order = false) {
        return Helper::narrow(wrender(t, render_with_symbols, correct_num_order));
    }

    // Render a piece of text containing arabic text that can may contain numbers or multiple lines.
    // returns a wide string
    // @param tff ttf font definition from a label
    // @param t text to render
    // @param render_with_symbols renders arabic text while treating symbol characters like arabic letter.
    // @param correct_num_order retains order of english numbers when rendered with arabic text.
    // @param wrap_x pixels that a sentence must exceed to go to the next line
    /*inline std::wstring wrender_wrap(ax::TTFConfig ttf, std::wstring& t, bool render_with_symbols = false, bool correct_num_order = false, float wrap_x = FLT_MAX) {
        auto lb = ax::Label::createWithTTF(Helper::narrow(t), ttf.fontFilePath, ttf.fontSize);
        lb->updateContent();
        auto fontAtlas = lb->getFontAtlas();
        std::vector<std::wstring> words;
        Helper::wsplit(t, L" ", words);
        int wordCount = words.size();
        bool s = false;
        int f;
        for (int i = 0; i < words.size(); i++)
        {
            if (!Helper::is_arabic_word(words[i]) && !s)
            {
                f = i;
                s = true;
            }

            if (Helper::is_arabic_word(words[i]) && s)
            {
                s = false;
                std::reverse(words.begin() + f, words.begin() + i);
            }
        }
        std::wstring accString;
        while (true) {
            float accWidth = 0;
            for (int i = 0; i < wordCount; i++) {
                for (int w = 0; w < words[i].length(); w++)
                {
                    ax::FontLetterDefinition letter;
                    fontAtlas->getLetterDefinitionForChar(words[i][w], letter);
                    accWidth += letter.width;
                }
            }
            if (accWidth > wrap_x && wordCount > 1)
                wordCount--;
            else {
                for (int i = wordCount - 1; i >= 0; i--)
                    accString += wrender(words[i], render_with_symbols, correct_num_order) + L" ";
                words.erase(words.begin(), words.begin() + wordCount);
                wordCount = words.size();
                accString += L"\n";
                if (words.empty()) break;
            }
        }
        return (Options::_convertToArabicNumbers ? w_arabify_numbers(accString) : accString);
    }*/
    
    // Render a piece of text containing arabic text that can may contain numbers or multiple lines.
    // returns a narrowed string
    // @param tff ttf font definition from a label
    // @param t text to render
    // @param render_with_symbols renders arabic text while treating symbol characters like arabic letter.
    // @param correct_num_order retains order of english numbers when rendered with arabic text.
    // @param wrap_x pixels that a sentence must exceed to go to the next line
    /*inline std::string render_wrap(ax::TTFConfig ttf, std::wstring& t, bool render_with_symbols = false, bool correct_num_order = false, float wrap_x = FLT_MAX) {
        return Helper::narrow(wrender_wrap(ttf, t, render_with_symbols, correct_num_order, wrap_x));
    }*/

    // Splits the text for each new line it finds, then it reverses the words in all lines
    // then creates a substr of that text, Can be used with scrolling text in games
    // returns a wide string
    inline std::wstring wsubstr(std::wstring& t, int count) {
        std::vector<std::wstring> words;
        Helper::wsplit(t, L"\n", words);
        std::wstring accString;
        int accCount = 0;
        for (auto& s : words) {
            accCount += s.length();
            int c = t.length() - ((t.length() + count) - accCount);
            if (c < 0)
                accString += s + L'\n';
            else if (c < s.length() + 1) {
                accString += s.substr(c, s.length()) + L"\n ";
                break;
            }
        }
        accString.erase(accString.length());
        return accString;
    }

    // Splits the text for each new line it finds, then it reverses the order of everyline
    // then creates a substr of that text, Can be used with scrolling text in games
    // returns a narrowed string
    inline std::string substr(std::wstring& t, int count) {
        return Helper::narrow(wsubstr(t, count));
    }
    
    // Splits the text for each new line it finds, then it reverses the order of everyline
    // then creates a substr of that text, Can be used with scrolling text in games
    // returns a narrowed string
    inline std::string substr(std::string& t, int count) {
        auto n = Helper::widen(t);
        return Helper::narrow(wsubstr(n, count));
    }
}