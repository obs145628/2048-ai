#pragma once

#include "grid.hh"

class AI
{
public:
  virtual ~AI()
  {}

  virtual void init(grid_t& grid, score_t& score)
  {
    score = 0;
    grid = grid_rand(rand_, grid);
    grid = grid_rand(rand_, grid);
  }

  virtual move_t move_get(grid_t grid, score_t score) = 0;

  virtual void after(grid_t& grid, score_t& score)
  {
    score = score;
    grid = grid_rand(rand_, grid);
  }

  virtual void stats()
  {}

public:
  Random rand_;
};
