#pragma once

#include "agent.hh"

namespace to48
{


    inline void Agent::init(World& world)
    {
	world.reset();
	world.add_rand();
	world.add_rand();
    }



    inline void Agent::after(World& world)
    {
	world.add_rand();
    }

    inline void Agent::stats()
    {}


}
