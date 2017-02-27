#pragma once

#include "heur.hh"

inline heur_t heur_eval1(grid_t grid)
{
  ++heur_nb_evals;
  grid_t tr = grid_transpose(grid);
  return HEUR_VALS[grid_get_row(grid, 0)]
    + HEUR_VALS[grid_get_row(grid, 1)]
    + HEUR_VALS[grid_get_row(grid, 2)]
    + HEUR_VALS[grid_get_row(grid, 3)]
    + HEUR_VALS[grid_get_row(tr, 0)]
    + HEUR_VALS[grid_get_row(tr, 1)]
    + HEUR_VALS[grid_get_row(tr, 2)]
    + HEUR_VALS[grid_get_row(tr, 3)];
}

inline heur_t heur_eval2(grid_t grid)
{
  ++heur_nb_evals;
  return 1024 * GRID_POWS[grid_get(grid, 0)]
    + 512 * GRID_POWS[grid_get(grid, 1)]
    + 256 * GRID_POWS[grid_get(grid, 2)]
    + 128 * GRID_POWS[grid_get(grid, 3)]
    + 64 * GRID_POWS[grid_get(grid, 7)]
    + 32 * GRID_POWS[grid_get(grid, 6)]
    + 16 * GRID_POWS[grid_get(grid, 5)]
    + 8 * GRID_POWS[grid_get(grid, 4)]
    + 7 * GRID_POWS[grid_get(grid, 8)]
    + 6 * GRID_POWS[grid_get(grid, 9)]
    + 5 * GRID_POWS[grid_get(grid, 10)]
    + 4 * GRID_POWS[grid_get(grid, 11)]
    + 3 * GRID_POWS[grid_get(grid, 14)]
    + 2 * GRID_POWS[grid_get(grid, 13)]
    + 1 * GRID_POWS[grid_get(grid, 12)];
}
