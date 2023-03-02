//
//  widgets/Image.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-25.
//

#ifndef PTK_WIDGETS_IMAGE_HPP
#define PTK_WIDGETS_IMAGE_HPP

// pTK Headers
#include "ptk/core/Widget.hpp"
#include "ptk/util/Vec2.hpp"

// C++ Headers
#include <string>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkImage.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    /** Image class implementation.

        This class is used for loading and displaying an image.
    */
    class PTK_API Image : public Widget
    {
    public:
        /** Constructs Image with default values.

            @return    default initialized Image
        */
        Image();

        /** Constructs Image with default values with style.

            @return    default initialized Image
        */
        Image(const std::string& path);

        /** Move Constructor for Image.

            @return    initialized Image from value
        */
        Image(Image&& other) = default;

        /** Move Assignment operator for Image.

            @return    Image with value
        */
        Image& operator=(Image&& other) = default;

        /** Destructor for Image.

        */
        virtual ~Image() = default;

        /** Function for loading an image from disk.

            @param path    file path
            @return        status
        */
        bool loadFromFile(const std::string& path);

        /** Function for checking if an image is loaded.

            @return        status
        */
        bool isLoaded() const;

        /** Function is called when it is time to draw.

            @param canvas   Canvas to draw to.
        */
        void onDraw(Canvas& canvas) override;

        /** Function for getting the scale of an image.

            @return        scale
        */
        [[nodiscard]] const Vec2f& getScale() const;

        /** Function for setting the scale of an image.

            @param x   scale factor for width
            @param y   scale factor for height
        */
        void setScale(float x, float y);

        /** Function for setting the scale of an image.

            @param scale   scale factor for both width and height
        */
        void setScale(const Vec2f& scale);

    private:
        void applyScale(float x, float y);

    private:
        std::string m_path;
        sk_sp<SkImage> m_image;
        Vec2f m_scale;
    };
} // namespace pTK

#endif // PTK_WIDGETS_IMAGE_HPP
