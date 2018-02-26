#pragma once

#include <ai/misc/random-mt.hh>
#include "world.hh"

namespace to48
{

    class Agent
    {
    public:

	static constexpr long TIME_LIMIT = 10;
	static constexpr int MAX_DEPTH = 50;

	virtual ~Agent() = default;
	virtual void init(World& world);
	virtual World::action_t action_get(const World& world) = 0;
	virtual void after(World& word);
	virtual void stats();
    };

}

#include "agent.hxx"
