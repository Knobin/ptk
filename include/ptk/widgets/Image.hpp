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

        Takes a Texture and wraps it in a widget format for displaying
        2D arrays of drawable data.
    */
    class PTK_API Image : public Widget
    {
    public:
        /** Constructs Image with default values.

            @return    default initialized Image
        */
        Image() = default;

        /** Constructs Image with texture.

            Takes a copy of the shared pointer to a texture and tries to display
            it if the texture is valid.

            @param texture  texture to display
            @return         initialized Image with texture
        */
        explicit Image(std::shared_ptr<Texture> texture);

        /** Constructs Image with buffer.

            Creates a texture from the buffer.
            Look at Texture constructor that takes ImmutableBuffer for more info.

            @param buffer   buffer to create texture from
            @return         initialized Image with buffer
        */
        explicit Image(const ImmutableBuffer& buffer);

        /** Constructs Image with pixmap.

            Creates a texture from the pixmap.
            Look at Texture constructor that takes Pixmap for more info.

            @param pixmap   pixmap to create texture from
            @return         initialized Image with pixmap
        */
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
