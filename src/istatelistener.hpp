#pragma once

#include "state_types.hpp"

namespace client
{
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    class IStateListener
    {
    public:
        virtual ~IStateListener() = default;
        virtual void OnStateUpdate( const shared::StateUpdate& update ) = 0;
    };
}
