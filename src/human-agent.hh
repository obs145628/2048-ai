#pragma once

#include "agent.hh"

namespace to48
{

    class HumanAgent : public Agent
    {
    public:
        virtual World::action_t action_get(const World& world) override;
    };

}

#include "human-agent.hxx"
