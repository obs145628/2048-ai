#pragma once

#include "agent.hh"

namespace to48
{

    class ReplayAgent : public Agent
    {
    public:
	ReplayAgent(const std::vector<int>& actions);
	virtual void init(World& world) override;
	virtual World::action_t action_get(const World& world) override;
	virtual void after(World& word) override;

    private:
	std::vector<int> actions_;
	std::size_t pos_;
    };

}

#include "replay-agent.hxx"
