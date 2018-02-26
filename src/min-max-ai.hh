#pragma once

#include <climits>
#include <map>
#include <utility>
#include <ai/misc/timer.hh>
#include "agent.hh"
#include "heur.hh"

namespace to48
{

    class MinMaxAI : public Agent
    {

    public:

	using val_t = std::pair<heur_t, int>; //<value, depth>
	using cache_t = std::map<grid_t, val_t>;

	MinMaxAI(heur_f eval);
	heur_t min(grid_t grid, int depth, heur_t alpha, heur_t beta);
	heur_t max(grid_t grid, int depth, heur_t alpha, heur_t beta);
	move_t move_depth(grid_t grid, int depth);
	virtual World::action_t action_get(const World& world) override;
	void stats() override;

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

#include "min-max-ai.hxx"
