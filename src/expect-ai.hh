#pragma once

#include "ai.hh"
#include <climits>
#include <map>

template <class E>
class ExpectAI : public AI
{

public:

  ExpectAI(E eval = {}, int depth = 4)
    : eval_(eval)
    , depth_(depth)
    , tmp_(0)
  {}

  long exp(grid_t grid, int depth)
  {
    long total = 0;
    long weight = 0;

    for (index_t i = 0; i < 16; ++i)
      {
        if (grid_get(grid, i))
          continue;

        grid_t grid2 = grid_set(grid, i, 1);
        grid_t grid4 = grid_set(grid, i, 2);

        total += 10 * max(grid2, depth - 1);
        total += 1 * max(grid4, depth - 1);
        weight += 11;
      }

    long res = total / weight;
    return res;
  }

  long max(grid_t grid, int depth)
  {
    auto it = table_.find(grid);
    if (it != table_.end())
      return it->second;

    long res = LONG_MIN;
    if (grid_finished(grid))
      res = -10e14L;
    if (!depth)
      res = eval_(grid);

    else
      {
        for (auto m : { Move::LEFT, Move::RIGHT, Move::TOP, Move::BOTTOM })
          {
            grid_t next = grid_move(grid, tmp_, m);
            if (next == grid)
              continue;
            res = std::max(res, exp(next, depth));
          }
      }

    table_[grid] = res;
    return res;
  }

  virtual move_t move_get(grid_t grid, score_t) override
  {
    Move best = Move::LEFT;
    long res = LONG_MIN;

    for (auto m : { Game::LEFT, Game::RIGHT, Game::TOP, Game::BOTTOM })
      {
        grid_t next = grid_move(grid, tmp_, m);
        if (next == grid)
          continue;
        long val = exp(next, depth_);
        if (val > res)
          {
            res = val;
            best = m;
          }
      }

    return best;
  }

private:
  E eval_;
  int depth_;
  score_t tmp_;
  std::map<grid_t, long> table_;
  
};
