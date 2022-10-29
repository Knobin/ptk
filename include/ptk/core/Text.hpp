//
//  core/Text.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-10-09.
//

#ifndef PTK_CORE_TEXT_HPP
#define PTK_CORE_TEXT_HPP

// pTK Headers
#include "ptk/Core.hpp"
#include "ptk/core/Drawable.hpp"
#include "ptk/util/Color.hpp"
#include "ptk/util/Size.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkFont.h"
#include "include/core/SkTypeface.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    /** Text class implementation.

     */
    class PTK_API Text
    {
    public:
        // Encoding of the text.
        enum class Encoding : uint8_t
        {
            NONE = 0,
            UTF8,
            UTF16,
            UTF32
        };

        struct StrData
        {
            const void* text;
            std::size_t size;
            Encoding encoding;
        };

    public:
        /** Constructs Text with default values.

            @return    default initialized Text
        */
        Text();

        /** Destructor for Text.

        */
        virtual ~Text() = default;

        /** Function for setting the font from a given file.

            @param path    font file to load
            @return        status
        */
        bool setFontFromFile(const std::string& path);

        /** Function for setting the font family.

            @param font    font family to use.
        */
        bool setFontFamily(const std::string& fontFamily);

        /** Function for retrieving the font family.

            @return  current font family
        */
        [[nodiscard]] std::string getFontFamily() const;

        /** Function for setting the font size.

            @param fontSize    size of the font
        */
        void setFontSize(uint32_t fontSize);

        /** Function for retrieving the font size.

            @return  current font size
        */
        [[nodiscard]] uint32_t getFontSize() const;

        /** Function for retrieving the distance between ascent and descent.

            @return  distance between ascent and descent.
        */
        [[nodiscard]] float ascentToDescent() const;

        /** Function for retrieving the cap height.

            @return  cap height
         */
        [[nodiscard]] float capHeight() const;

        /** Function for retrieving the bounds of the text.

            @param  str     string to measure
            @return         bounds of the str
        */
        [[nodiscard]] Vec2f getBoundsFromStr(const std::string& str) const;

        /** Function for retrieving the raw SkFont.

            @return  raw SkFont
         */
        [[nodiscard]] const SkFont& skFont() const;

        /** Function for drawing a line of text.

            @param canvas   pointer to SkCanvas
            @param data     str to draw, size of the ptr and encoding
            @param color    color of the text
            @param pos      draw text at
            @return         advance
         */
        float drawTextLine(SkCanvas* canvas, const StrData& data, const Color& color, const Vec2f& pos);

        /** Function for drawing a line of text.

            @param canvas       pointer to SkCanvas
            @param data         str to draw, size of the ptr and encoding
            @param color        color of the text
            @param pos          draw text at
            @param outlineSize  outline size
            @param outColor     outline color
            @return             advance
         */
        float drawTextLine(SkCanvas* canvas, const StrData& data, const Color& color, const Vec2f& pos,
                           float outlineSize, const Color& outColor);

        /** Function for drawing a line of text.

            @param canvas       pointer to SkCanvas
            @param data         str to draw, size of the ptr and encoding
            @param color        color of the text
            @param pos          draw text at
            @param paint        use custom SkPaint
            @return             advance
         */
        float drawTextLineWithPaint(SkCanvas* canvas, const StrData& data, const Vec2f& pos, const SkPaint& paint);

    private:
        // Callback for when the text updates.
        virtual void onTextUpdate() {}

        // Internal function that updates member variables.
        void updateFontInfo();

    private:
        SkFont m_font;
        float m_capHeight{0.0f};
        float m_ascentToDescent{0.0f};
    };
} // namespace pTK

#endif // PTK_CORE_TEXT_HPP
