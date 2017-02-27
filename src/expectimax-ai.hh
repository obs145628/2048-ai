#pragma once

#include "ai.hh"
#include <map>
#include <unordered_map>
#include <utility>
#include "ai/shell/timer.hh"
#include "heur.hh"
#include "proba.hh"

class ExpectimaxAI : public AI
{

public:


  using val_t = std::pair<heur_t, int>; //<value, depth>
  using cache_t = std::unordered_map<grid_t, val_t>;

  ExpectimaxAI(heur_f eval)
    : eval_(eval)
    , max_depth_(0)
    , sum_depth_(0)
    , nmoves_(0)
  {}

  heur_t min(grid_t grid, int depth,
             int k, int n)
  {
    if (!depth)
      return eval_(grid);

    auto it = cache_min_.find(grid);
    if (it != cache_min_.end() && it->second.second >= depth)
      return it->second.first;

    heur_t res = 0;
    heur_t div = 0;

    for (index_t i = 0; i < 16; ++i)
      {
        if (grid_get(grid, i))
          continue;

        grid_t grid2 = grid_put(grid, i, 1);
        grid_t grid4 = grid_put(grid, i, 2);


        heur_t val2 = max(grid2, depth - 1, k, n + 1);
        heur_t val4 = max(grid4, depth - 1, k + 1, n + 1);
        res += 10.0f * val2 + val4;
        div += 11.0f;
      }

    res /= div;
    cache_min_[grid] = std::make_pair(res, depth);
    return res;
  }

  heur_t max(grid_t grid, int depth,
             int k, int n)
  {
    if (!depth)
      return eval_(grid);

    if (Proba::PROBAS_TABLES[ k * Proba::MAX + n])
      return eval_(grid);


    auto it = cache_max_.find(grid);
    if (it != cache_max_.end() && it->second.second >= depth)
      return it->second.first;

    heur_t res = HEUR_MIN;

    for (auto m : { GRID_LEFT, GRID_RIGHT, GRID_TOP, GRID_BOTTOM })
      {
        grid_t next = grid_fast_move(grid, m);
        if (next == grid)
          continue;
        res = std::max(res, min(next, depth - 1, k, n));
      }

    if (res == HEUR_MIN)
      res = HEUR_DEATH;

    cache_max_[grid] = std::make_pair(res, depth);
    return res;
  }

  move_t move_depth(grid_t grid, int depth)
  {
    move_t best = GRID_LEFT;
    heur_t res = HEUR_MIN;

    for (auto m : {GRID_LEFT, GRID_RIGHT, GRID_TOP, GRID_BOTTOM })
      {
        grid_t next = grid_fast_move(grid, m);
        if (next == grid)
          continue;
        heur_t val = min(next, depth, 0, 0);
        if (val > res)
          {
            res = val;
            best = m;
          }
      }

    cache_max_[grid] = std::make_pair(res, depth);
    return best;
  }

  virtual move_t move_get(grid_t grid) override
  {
    Timer timer;
    int depth = 0;
    move_t res = GRID_LEFT;

    long max_time = TIME_LIMIT;
    if (grid_count_empty(grid) < 4)
      max_time *= std::pow(4 - grid_count_empty(grid), 1);

    while (depth < MAX_DEPTH)
      {
        res = move_depth(grid, depth);

        long time = timer.reset();
        if (time > max_time)
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
    long evals = heur_nb_evals * 1000 / time;
    std::cout << "------------------------------------\n";
    std::cout << "Number of moves: " << nmoves_ << "\n";
    std::cout << "Max Depth: " << max_depth_ << "\n";
    std::cout << "Average Depth: " << (sum / nmoves_) << "\n";
    std::cout << "Duration: " << time << "ms\n";
    std::cout << "Evalutions / s : " << evals << "\n";
    std::cout << "------------------------------------\n";
  }

private:
  heur_f eval_;
  
  int max_depth_;
  int sum_depth_;
  int nmoves_;
  Timer timer_;

  cache_t cache_min_;
  cache_t cache_max_;
  
};
