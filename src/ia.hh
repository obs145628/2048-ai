#include "grid.hh"

class AI
{
public:
  virtual ~AI()
  {}

  virtual Move move_get(grid_t grid, score_t score) = 0;

  virtual void init(grid_t& grid, score_t& score)
  {
    score = 0;
    grid = grid_rand(grid);
  }

  virtual void after(grid_t& grid, score_t& score)
  {
    score = score;
    grid = grid_rand(grid);
  }
};
