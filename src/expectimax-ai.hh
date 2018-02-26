#pragma once

#include <map>
#include <unordered_map>
#include <utility>
#include <ai/misc/timer.hh>
#include "agent.hh"
#include "heur.hh"

namespace to48
{

    class ExpectimaxAI : public Agent
    {

    public:
	using val_t = std::pair<heur_t, int>; //<value, depth>
	using cache_t = std::unordered_map<grid_t, val_t>;

	ExpectimaxAI(heur_f eval);
	heur_t min(grid_t grid, int depth);
	heur_t max(grid_t grid, int depth);
	move_t move_depth(grid_t grid, int depth);
	virtual World::action_t action_get(const World& world) override;
	virtual void stats() override;

    private:
	heur_f eval_;
  
	int max_depth_;
	int sum_depth_;
	int nmoves_;
	Timer timer_;

	cache_t cache_min_;
	cache_t cache_max_;
  
    };

}

#include "expectimax-ai.hxx"
