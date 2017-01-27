#pragma once

#include "ai.hh"
#include <climits>
#include <map>

template <class E>
class MinMaxAI : public AI
{

public:

  MinMaxAI(E eval = {}, int depth = 5)
    : eval_(eval)
    , depth_(depth)
    , tmp_(0)
  {}

  long min(grid_t grid, int depth,
           long alpha, long beta)
  {
    auto it = min_table_.find(grid);
    if (it != min_table_.end())
      return it->second;

    long res = LONG_MAX;

    for (index_t i = 0; i < 16; ++i)
      {
        if (grid_get(grid, i))
          continue;

        grid_t grid2 = grid_set(grid, i, 1);
        grid_t grid4 = grid_set(grid, i, 2);

        res = std::min(res, max(grid2, depth - 1, alpha, beta));
        res = std::min(res, max(grid4, depth - 1, alpha, beta));
        beta = std::min(beta, res);
        if (beta <= alpha)
          return alpha;
      }

    min_table_[grid] = res;
    return res;
  }

  long max(grid_t grid, int depth,
           long alpha, long beta)
  {
    auto it = max_table_.find(grid);
    if (it != max_table_.end())
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
            res = std::max(res, min(next, depth, alpha, beta));
            alpha = std::max(alpha, res);
            if (beta <= alpha)
              return beta;
          }
      }

    max_table_[grid] = res;
    return res;
  }

  virtual Move move_get(grid_t grid, score_t) override
  {
    Move best = Move::LEFT;
    long res = LONG_MIN;

    for (auto m : { Move::LEFT, Move::RIGHT, Move::TOP, Move::BOTTOM })
      {
        grid_t next = grid_move(grid, tmp_, m);
        if (next == grid)
          continue;
        long val = min(next, depth_, res, LONG_MAX);
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
  std::map<grid_t, long> max_table_;
  std::map<grid_t, long> min_table_;
  
};
