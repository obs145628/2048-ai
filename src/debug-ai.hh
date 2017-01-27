#pragma once

#include "ai.hh"
#include <climits>
#include <map>
#include <utility>
#include "ai/shell/timer.hh"
#include "eval.hh"


#define LIMIT 1

#define MAX_DEPTH 50

template <class E>
class DebugAI : public AI
{

public:

  using val_t = std::pair<long, int>; //<value, depth>
  using cache_t = std::map<grid_t, val_t>;

  DebugAI(E eval = {})
    : eval_(eval)
    , tmp_(0)
    , max_depth_(0)
    , sum_depth_(0)
    , nmoves_(0)
  {}

  long min(grid_t grid, int depth,
           long alpha, long beta)
  {
    if (!depth)
      return eval_(grid);

    auto it = cache_min_.find(grid);
    if (it != cache_min_.end() && it->second.second >= depth)
      return it->second.first;
  

    long res = LONG_MAX;

    for (index_t i = 0; i < 16; ++i)
      {
        if (grid_get(grid, i))
          continue;

        grid_t grid2 = grid_set(grid, i, 1);
        grid_t grid4 = grid_set(grid, i, 2);


        long val2 = max(grid2, depth - 1, alpha, beta);
        res = std::min(res, val2);
        beta = std::min(beta, res);
        if (beta <= alpha)
          break;
        
        long val4 = max(grid4, depth - 1, alpha, beta);
        res = std::min(res, val4);
        beta = std::min(beta, res);
        if (beta <= alpha)
          break;
      }

    cache_min_[grid] = std::make_pair(res, depth);
    return res;
  }

  long max(grid_t grid, int depth,
           long alpha, long beta)
  {
    if (!depth)
      return eval_(grid);

    auto it = cache_max_.find(grid);
    if (it != cache_max_.end() && it->second.second >= depth)
      return it->second.first;

    long res = LONG_MIN;

    for (auto m : { Move::LEFT, Move::RIGHT, Move::TOP, Move::BOTTOM })
      {
        grid_t next = grid_move(grid, tmp_, m);
        if (next == grid)
          continue;
        res = std::max(res, min(next, depth - 1, alpha, beta));

        alpha = std::max(alpha, res);
        if (beta <= alpha)
          break;
      }

    if (res == LONG_MIN)
      res = -10e14L;

    cache_max_[grid] = std::make_pair(res, depth);
    return res;
  }

  Move move_depth(grid_t grid, int depth)
  {
    Move best = Move::LEFT;
    long res = LONG_MIN;

    long alpha = LONG_MIN;
    long beta = LONG_MAX;

    for (auto m : { Move::LEFT, Move::RIGHT, Move::TOP, Move::BOTTOM })
      {
        grid_t next = grid_move(grid, tmp_, m);
        if (next == grid)
          continue;
        long val = min(next, depth, alpha, beta);
        if (val > res)
          {
            res = val;
            best = m;
          }

        alpha = std::max(alpha, res);
        if (beta <= alpha)
          break;
      }

    cache_max_[grid] = std::make_pair(res, depth);
    return best;
  }

  virtual Move move_get(grid_t grid, score_t) override
  {
    Timer timer;
    int depth = 0;
    Move res = Move::LEFT;

    while (depth < MAX_DEPTH)
      {
        res = move_depth(grid, depth);

        long time = timer.reset();
        if (time > LIMIT)
          break;
        ++depth;
      }

    ++nmoves_;
    max_depth_ = std::max(depth, max_depth_);
    sum_depth_ += depth;
    cache_max_.clear();
    cache_min_.clear();
    return res;
  }

  virtual void stats()
  {
    double sum = sum_depth_;
    long time = timer_.get();
    long evals = nb_evals * 1000 / time;
    std::cout << "------------------------------------\n";
    std::cout << "Number of moves: " << nmoves_ << "\n";
    std::cout << "Max Depth: " << max_depth_ << "\n";
    std::cout << "Average Depth: " << (sum / nmoves_) << "\n";
    std::cout << "Duration: " << time << "ms\n";
    std::cout << "Evalutions / s : " << evals << "\n";
    std::cout << "------------------------------------\n";
  }

private:
  E eval_;
  score_t tmp_;
  
  int max_depth_;
  int sum_depth_;
  int nmoves_;
  Timer timer_;

  cache_t cache_min_;
  cache_t cache_max_;
  
};
