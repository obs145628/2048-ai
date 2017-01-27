#pragma once

#include "ai.hh"

class DumpAI : public AI
{

public:

  virtual Move move_get(grid_t grid, score_t score) override
  {
    Move best = Move::LEFT;
    score_t best_score = score;

    for (auto m : { Move::LEFT, Move::RIGHT, Move::TOP, Move::BOTTOM })
      {
        score_t tmp = score;
        grid_t res = grid_move(grid, tmp, m);
        if (res != grid && tmp >= best_score)
          {
            best_score = tmp;
            best = m;
          }
      }

    return best;
  }
  
};
