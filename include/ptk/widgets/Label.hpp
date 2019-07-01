//
//  widgets/Label.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-30.
//

#ifndef PTK_WIDGETS_LABEL_HPP
#define PTK_WIDGETS_LABEL_HPP

// Local Headers
#include "ptk/core/Shape.hpp"
#include "ptk/core/Types.hpp"

// C++ Headers
#include <string>

namespace pTK
{
    class Label : public Shape
    {
    public:
        Label();
        ~Label() = default;
        
        void setFont(const std::string& font);
        
        void setFontSize(uint fontSize);
        
        void setText(const std::string& text);
        
        /** Draw function.
         Function is called when it is time to draw.
         
         Derived from Drawable.
         */
        void onDraw(const Canvas& canvas) override;
        
    private:
        std::string m_fontName;
        uint m_fontSize;
        std::string m_text;
        
        void calculateBounds();
    };
}

#endif // PTK_WIDGETS_LABEL_HPP
