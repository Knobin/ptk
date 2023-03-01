//
//  platform/KeyMapFactory.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

// Local Headers
#include "KeyMapFactory.hpp"

namespace pTK::Platform
{
    namespace KeyMapFactoryImpl
    {
        // Function to implement by the platform.
        std::map<int32_t, KeyCode> Make();
    } // namespace KeyMapFactoryImpl

    std::map<int32_t, KeyCode> KeyMapFactory::Make()
    {
        return KeyMapFactoryImpl::Make();
    }

} // namespace pTK::Platform
