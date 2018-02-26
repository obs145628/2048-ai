#pragma once

#include "agent.hh"

namespace to48
{

    class MonteCarlo : public Agent
    {

    public:

	virtual World::action_t action_get(const World& world) override;

    private:


    };

}
