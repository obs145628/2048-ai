#pragma once

#include "grid.hh"

class AI
{
public:

  static constexpr long TIME_LIMIT = 10;
  static constexpr int MAX_DEPTH = 50;

  virtual ~AI()
  {}

  virtual void init(grid_t& grid, score_t& score)
  {
    score = 0;
    grid = grid_rand(rand_, grid);
    grid = grid_rand(rand_, grid);
  }

  virtual move_t move_get(grid_t grid) = 0;

  virtual void after(grid_t& grid, score_t&)
  {
    grid = grid_rand(rand_, grid);
  }

  virtual void stats()
  {}

private:
  Random rand_;
};
