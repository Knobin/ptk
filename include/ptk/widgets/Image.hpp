//
//  widgets/Image.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-25.
//

// Local Headers
#include "ptk/core/Widget.hpp"

// C++ Headers
#include <string>

// Skia Headers
#include "include/core/SkImage.h"

namespace pTK
{
    class Image : public Widget
    {
    public:
        Image();
        Image(const std::string& path);
        virtual ~Image() = default;
        
        bool loadFromFile(const std::string& path);
        void onDraw(SkCanvas* canvas) override;
        
    private:
        std::string m_path;
        sk_sp<SkImage> m_image;
    };
}
