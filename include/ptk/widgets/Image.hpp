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
#include "ptk/util/ImmutableBuffer.hpp"
#include "ptk/util/Pixmap.hpp"
#include "ptk/util/Texture.hpp"
#include "ptk/util/Vec2.hpp"

// C++ Headers
#include <memory>

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
        Image() = default;

        // TODO(knobin): Add documentation.
        explicit Image(std::shared_ptr<Texture> texture);

        // TODO(knobin): Add documentation.
        explicit Image(const ImmutableBuffer& buffer);

        // TODO(knobin): Add documentation.
        explicit Image(const Pixmap& pixmap);

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
        ~Image() override = default;

        /** Function for checking if the image is valid and can be displayed.

            @return        status
        */
        [[nodiscard]] bool isValid() const;

        /** Function is called when it is time to draw.

            @param canvas   valid Canvas pointer to draw to
        */
        void onDraw(Canvas* canvas) override;

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
        std::shared_ptr<Texture> m_texture{};
        Vec2f m_scale{1.0f, 1.0f};
    };
} // namespace pTK

#endif // PTK_WIDGETS_IMAGE_HPP
