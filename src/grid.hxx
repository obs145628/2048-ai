#pragma once

#include "grid.hh"

inline index_t grid_get(grid_t grid, index_t pos)
{
  assert(pos < 16);
  return (grid >> (pos * 4)) & 0xFUL;
}

inline index_t grid_get(grid_t grid, index_t i, index_t j)
{
  assert(i < 4);
  assert(j < 4);
  return grid_get(grid, i * 4 + j);
}

inline bool grid_is_full(grid_t grid)
{
  for (index_t i = 0; i < 16; ++i)
    if (grid_get(grid, i) == 0)
      return false;
  return true;
}

inline bool grid_is_finished(grid_t grid)
{
  for (index_t i = 0; i < 4; ++i)
    for (index_t j = 0; j < 4; ++j)
      {
        index_t val = grid_get(grid, i, j);
        if (!val)
          return false;
        if (i + 1 < 4 && grid_get(grid, i + 1, j) == val)
          return false;
        if (j + 1 < 4 && grid_get(grid, i, j + 1) == val)
          return false;
      }

  return true;
}

inline grid_t grid_clear(grid_t grid, index_t pos)
{
  assert(pos < 16);
  return grid & ~(0xFUL << (pos * 4));
}

inline grid_t grid_clear(grid_t grid, index_t i, index_t j)
{
  assert(i < 4);
  assert(j < 4);
  return grid_clear(grid, i * 4 + j);
}

inline int grid_count_empty(grid_t grid)
{
  int res = 0;
  for (int i = 0; i < 16; ++i)
    res += !grid_get(grid, i);
  return res;
}

inline grid_t grid_get_max(grid_t grid)
{
  grid_t res = 0;
  for (int i = 0; i < 16; ++i)
    res = std::max(res, grid_get(grid, i));
  return res;
}

inline grid_t grid_set(grid_t grid, index_t pos, index_t val)
{
  assert(pos < 16);
  return grid_clear(grid, pos) | (val << (pos * 4));
}

inline grid_t grid_set(grid_t grid, index_t i, index_t j, index_t val)
{
  assert(i < 4);
  assert(j < 4);
  return grid_set(grid, i * 4 + j, val);
}

inline grid_t grid_put(grid_t grid, index_t pos, index_t val)
{
  assert(pos < 16);
  return grid | (val << (pos * 4));
}

inline grid_t grid_put(grid_t grid, index_t i, index_t j, index_t val)
{
  assert(i < 4);
  assert(j < 4);
  return grid_put(grid, i * 4 + j, val);
}

inline ShellSprite grid_to_sprite(grid_t grid)
{
  Grid res(4, 4);
  for (index_t i = 0; i < 4; ++i)
    for (index_t j = 0; j < 4; ++j)
      {
        index_t val = grid_get(grid, i, j);
        res.at(i, j) = GRID_LABELS[val];
        res.fg_set(i, j, GRID_FGS[val]);
        res.bg_set(i, j, GRID_BGS[val]);
      }
  return res.to_sprite();
}

inline grid_t grid_rand(Random& rand, grid_t grid)
{
  grid_t val = 1 + (rand.int32_get() % 10) / 9;

  while (true)
    {
      grid_t pos = rand.int32_get() % 16;
      if (grid_get(grid, pos))
        continue;
      grid = grid_put(grid, pos, val);
      break;
    }

  return grid;
}

inline grid_t grid_get_row(grid_t grid, index_t i)
{
  assert(i < 4);
  return (grid >> (16 * i)) & 0xFFFF;
}

inline grid_t grid_join_rows(grid_t r1, grid_t r2, grid_t r3, grid_t r4)
{
  return (r1 << (16 * 0))
    | (r2 << (16 * 1))
    | (r3 << (16 * 2))
    | (r4 << (16 * 3));
}


inline grid_t grid_transpose(grid_t grid)
{
  return 0
    | GRID_TABLE_TR1[grid_get_row(grid, 0)]
    | GRID_TABLE_TR2[grid_get_row(grid, 1)]
    | GRID_TABLE_TR3[grid_get_row(grid, 2)]
    | GRID_TABLE_TR4[grid_get_row(grid, 3)];
}

inline grid_t grid_move_left(grid_t grid, score_t& score)
{
  score += GRID_TABLE_SCORE_LEFT[grid_get_row(grid, 0)]
    + GRID_TABLE_SCORE_LEFT[grid_get_row(grid, 1)]
    + GRID_TABLE_SCORE_LEFT[grid_get_row(grid, 2)]
    + GRID_TABLE_SCORE_LEFT[grid_get_row(grid, 3)];
  return grid_join_rows(
                        GRID_TABLE_LEFT[grid_get_row(grid, 0)],
                        GRID_TABLE_LEFT[grid_get_row(grid, 1)],
                        GRID_TABLE_LEFT[grid_get_row(grid, 2)],
                        GRID_TABLE_LEFT[grid_get_row(grid, 3)]
                        );
}

inline grid_t grid_move_right(grid_t grid, score_t& score)
{
  score += GRID_TABLE_SCORE_RIGHT[grid_get_row(grid, 0)]
    + GRID_TABLE_SCORE_RIGHT[grid_get_row(grid, 1)]
    + GRID_TABLE_SCORE_RIGHT[grid_get_row(grid, 2)]
    + GRID_TABLE_SCORE_RIGHT[grid_get_row(grid, 3)];
  return grid_join_rows(
                        GRID_TABLE_RIGHT[grid_get_row(grid, 0)],
                        GRID_TABLE_RIGHT[grid_get_row(grid, 1)],
                        GRID_TABLE_RIGHT[grid_get_row(grid, 2)],
                        GRID_TABLE_RIGHT[grid_get_row(grid, 3)]
                        );
}

inline grid_t grid_move_top(grid_t grid, score_t& score)
{
  grid_t tr = grid_transpose(grid);
  tr = grid_move_left(tr, score);
  return grid_transpose(tr);
}

inline grid_t grid_move_bottom(grid_t grid, score_t& score)
{
  grid_t tr = grid_transpose(grid);
  tr = grid_move_right(tr, score);
  return grid_transpose(tr);
}

inline grid_t grid_move(grid_t grid, score_t& score, move_t move)
{
  static const move_f GRID_MOVES_FNS[] =
    {
      grid_move_left,
      grid_move_right,
      grid_move_top,
      grid_move_bottom
    };
  assert(move >= 0 && move < 4);
  return GRID_MOVES_FNS[move](grid, score);
}


inline grid_t grid_fast_move_left(grid_t grid)
{
  return grid_join_rows(
                        GRID_TABLE_LEFT[grid_get_row(grid, 0)],
                        GRID_TABLE_LEFT[grid_get_row(grid, 1)],
                        GRID_TABLE_LEFT[grid_get_row(grid, 2)],
                        GRID_TABLE_LEFT[grid_get_row(grid, 3)]
                        );
}

inline grid_t grid_fast_move_right(grid_t grid)
{
  return grid_join_rows(
                        GRID_TABLE_RIGHT[grid_get_row(grid, 0)],
                        GRID_TABLE_RIGHT[grid_get_row(grid, 1)],
                        GRID_TABLE_RIGHT[grid_get_row(grid, 2)],
                        GRID_TABLE_RIGHT[grid_get_row(grid, 3)]
                        );
}

inline grid_t grid_fast_move_top(grid_t grid)
{
  grid_t tr = grid_transpose(grid);
  tr = grid_fast_move_left(tr);
  return grid_transpose(tr);
}

inline grid_t grid_fast_move_bottom(grid_t grid)
{
  grid_t tr = grid_transpose(grid);
  tr = grid_fast_move_right(tr);
  return grid_transpose(tr);
}

inline grid_t grid_fast_move(grid_t grid, move_t move)
{
  static const fast_move_f GRID_FAST_MOVES_FNS[] =
    {
      grid_fast_move_left,
      grid_fast_move_right,
      grid_fast_move_top,
      grid_fast_move_bottom
    };
  assert(move >= 0 && move < 4);
  return GRID_FAST_MOVES_FNS[move](grid);
}
