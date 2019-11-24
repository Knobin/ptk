//
//  core/Sizable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-11.
//

#ifndef PTK_CORE_SIZABLE_HPP
#define PTK_CORE_SIZABLE_HPP

// Local Headers
#include "ptk/util/Size.hpp"
#include "ptk/core/Types.hpp"

namespace pTK
{
    /** Sizable class implementation.
     
     Sizable class for handling size.
     */
    class Sizable
    {
    public:
        /** Constructs Sizable with default values.
         
         @return  default initialized Sizable
         */
        Sizable();
        
        /** Constructs Sizable with default values with size.
         
         @return  default initialized Sizable
         */
        Sizable(const Size& size);
        virtual ~Sizable() = default;
        
        /** Funtion for setting minimal size.
         
         @param size    minimal size
         */
        virtual void setMinSize(const Size& size);
        
        /** Funtion for getting the minimal size.
         
         @param size    minimal size
         */
        Size getMinSize() const;
        
        /** Funtion for setting size.
         
         @param size    size
         */
        virtual void setSize(const Size& size);
        
        /** Funtion for setting size.
         
         @param size    size
         */
        virtual void setConstSize(const Size& size);
        
        /** Funtion for getting the size.
         
         @param size    size
         */
        const Size& getSize() const;
        
        /** Funtion for setting maximal size.
         
         @param size    maximal size
         */
        virtual void setMaxSize(const Size& size);
        
        /** Funtion for getting the maximal size.
         
         @param size    maximal size
         */
        Size getMaxSize() const;
        
    private:
        Size m_minSize;
        Size m_size;
        Size m_maxSize;
        
        bool m_minLock;
        bool m_maxLock;
    };
    
    // Comparison operators.
    bool operator==(const Sizable& lhs, const Sizable& rhs);
    bool operator!=(const Sizable& lhs, const Sizable& rhs);
}


#endif // PTK_CORE_SIZABLE_HPP

