//
//  core/Sizable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-11.
//

#ifndef PTK_CORE_SIZABLE_HPP
#define PTK_CORE_SIZABLE_HPP

// pTK Headers
#include "ptk/core/Types.hpp"
#include "ptk/util/Size.hpp"

namespace pTK
{
    /** Limits struct implementation.

        Combines the limits (min and max) into
        an easy format to use.
    */
    struct PTK_API Limits
    {
        Size min{Size::Min};
        Size max{Size::Max};
    };

    /** Sizable class implementation.

        Sizable class for handling size.
    */
    class PTK_API Sizable
    {
    public:
        /** Constructs Sizable with default values.

            @return  default initialized Sizable
        */
        Sizable();

        /** Constructs Sizable with default values with size.

            @return  default initialized Sizable
        */
        explicit Sizable(const Size& size);

        /** Constructs Sizable with default values with size.

            @return  default initialized Sizable
        */
        Sizable(const Size& min, const Size& size, const Size& max);

        /** Destructor for Sizable.

        */
        virtual ~Sizable() = default;

        /** Function for setting minimal size.

            @param size    minimal size
        */
        void setMinSize(const Size& size);

        /** Function for getting the minimal size.

            @return    minimal size
        */
        [[nodiscard]] Size getMinSize() const noexcept;

        /** Function for setting size.

            @param size    size
        */
        void setSize(const Size& size);

        /** Function for getting the size.

            @return    size
        */
        [[nodiscard]] const Size& getSize() const noexcept;

        /** Function for setting maximal size.

            @param size    maximal size
        */
        void setMaxSize(const Size& size);

        /** Function for getting the maximal size.

            @return    maximal size
        */
        [[nodiscard]] Size getMaxSize() const noexcept;

        /** Function for setting both minimal and maximal size.

            @param limits    min size and max size
        */
        void setLimits(const Limits& limits);

        /** Function for setting both minimal and maximal size.

            @param min    minimal size
            @param max    maximum size
        */
        void setLimits(const Size& min, const Size& max);

        /** Function for getting the min and max size.

            @return    min and max size
        */
        [[nodiscard]] Limits getLimits() const noexcept;

    private:
        /** Function for knowing when the size has been changed.

            Will be called when setSize accepts a new size.
            Internal size will be set before calling this, so
            calling getSize will also return the new size.

            @param size    new size
        */
        virtual void onSizeChange(const Size&) {}

        /** Function for knowing when the limits has been changed.

            Will be called when when either setMinSize, setMaxSize or setLimits
            accepts a new size.
            Internal sizes will be set before calling this, so
            calling getMaxSize or getMinSize will also return the new size.

            @param min    current min size
            @param max    current max size
        */
        virtual void onLimitChange(const Size&, const Size&) {}

    protected:
        /** Function for setting minimal size.
            Does not trigger callbacks.

            @param size    minimal size
        */
        void updateMinSize(const Size& size);

        /** Function for setting size.
            Does not trigger callbacks.

            @param size    size
        */
        void updateSize(const Size& size);

        /** Function for setting maximal size.
            Does not trigger callbacks.

            @param size    maximal size
        */
        void updateMaxSize(const Size& size);

    private:
        // Helper functions for updating minimal size.
        byte updateMinWidth(Size::value_type width);
        byte updateMinHeight(Size::value_type height);

        // Helper functions for updating size.
        byte updateWidth(Size::value_type width);
        byte updateHeight(Size::value_type height);

        // Helper functions for updating maximum size.
        byte updateMaxWidth(Size::value_type width);
        byte updateMaxHeight(Size::value_type height);

    private:
        Size m_minSize;
        Size m_size;
        Size m_maxSize;
    };

    // Comparison operators.
    PTK_API bool operator==(const Sizable& lhs, const Sizable& rhs);
    PTK_API bool operator!=(const Sizable& lhs, const Sizable& rhs);
} // namespace pTK

#endif // PTK_CORE_SIZABLE_HPP
