#pragma once

#include "replay-agent.hh"

namespace to48
{

    ReplayAgent::ReplayAgent(const std::vector<int>& actions)
	: actions_(actions)
	, pos_(0)
    {}
    
    void ReplayAgent::init(World& world)
    {
	world.reset();
	world.add_num(actions_[0], actions_[1]);
	world.add_num(actions_[2], actions_[3]);
	pos_ = 4;
    }
    
    World::action_t ReplayAgent::action_get(const World&)
    {
	return actions_[pos_];
    }

    void ReplayAgent::after(World& world)
    {
	world.add_num(actions_[pos_ + 1], actions_[pos_ + 2]);
	pos_ += 3;
    }
    
}
