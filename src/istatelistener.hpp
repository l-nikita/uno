#pragma once

#include "state_types.hpp"

class IStateListener
{
public:
    virtual ~IStateListener() = default;
    virtual void OnStateUpdate(const StateUpdate& update) = 0;
};