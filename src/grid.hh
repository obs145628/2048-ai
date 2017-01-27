#pragma once

#include <cassert>
#include <cstdint>
#include <cmath>
#include "ai/math/random.hh"
#include "ai/shell/grid.hh"
#include "ai/shell/shell.hh"

using grid_t = std::uint64_t;
using index_t = std::uint64_t;
using score_t = std::uint64_t;
using move_t = int;
using move_f = grid_t (*)(grid_t grid, score_t& score);


static const score_t g_pows[] =
  {
    0,
    2,
    4,
    8,
    16,
    32,
    64,
    128,
    256,
    512,
    1024,
    2048,
    4096,
    8192,
    16384,
    32768
  };


static const std::string GRID_LABELS[] =
  {
    "     ",
    "  2  ",
    "  4  ",
    "  8  ",
    " 16  ",
    " 32  ",
    " 64  ",
    " 128 ",
    " 256 ",
    " 512 ",
    "1024 ",
    "2048 ",
    "4096 ",
    "8192 ",
    "16384",
    "32768",
  };


static const char* GRID_FGS[] =
  {
    Shell::FG_DEFAULT,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_WHITE,
    Shell::FG_WHITE,
    Shell::FG_WHITE,
    Shell::FG_WHITE,
  };

static const char* GRID_BGS[] =
  {
    Shell::BG_DEFAULT,
    Shell::BG_LGRAY,
    Shell::BG_DGRAY,
    Shell::BG_LGREEN,
    Shell::BG_GREEN,
    Shell::BG_LYELLOW,
    Shell::BG_YELLOW,
    Shell::BG_LRED,
    Shell::BG_RED,
    Shell::BG_LMAGENTA,
    Shell::BG_MAGENTA,
    Shell::BG_BLUE,
    Shell::BG_BLACK,
    Shell::BG_BLACK,
    Shell::BG_BLACK,
    Shell::BG_BLACK
  };

static constexpr move_t GRID_LEFT = 0;
static constexpr move_t GRID_RIGHT = 1;
static constexpr move_t GRID_TOP = 2;
static constexpr move_t GRID_BOTTOM = 3;


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
  assert((grid | (val << (pos * 4))) == grid_set(grid, pos, val));
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

inline grid_t grid_move_left(grid_t grid, score_t& score)
{
  for (unsigned i = 0; i < 4; ++i)
    {
      unsigned lim = 0;
      unsigned til = 0;
      for (unsigned j = 0; j < 4; ++j)
        {
          grid_t val = grid_get(grid, i, j);
          if (!val)
            continue;
          grid = grid_clear(grid, i, j);

          if (til != lim
              && val == grid_get(grid, i, til - 1))
            {
              lim = til;
              grid = grid_set(grid, i, til - 1, val + 1);
              score += g_pows[val + 1];
            }

          else
            {
              grid = grid_set(grid, i, til, val);
              ++til;
              lim = til - 1;
            }
        }
    }

  return grid;
}

inline grid_t grid_move_right(grid_t grid, score_t& score)
{
  for (unsigned i = 0; i < 4; ++i)
    {
      unsigned lim = 3;
      unsigned til = 3;
      for (unsigned j = 3; j < 4; --j)
        {
          grid_t val = grid_get(grid, i, j);
          if (!val)
            continue;
          grid = grid_clear(grid, i, j);

          if (til != lim
              && val == grid_get(grid, i, til + 1))
            {
              lim = til;
              grid = grid_set(grid, i, til + 1, val + 1);
              score += g_pows[val + 1];
            }

          else
            {
              grid = grid_set(grid, i, til, val);
              --til;
              lim = til + 1;
            }
        }
    }

  return grid;
}

inline grid_t grid_move_top(grid_t grid, score_t& score)
{
  for (unsigned j = 0; j < 4; ++j)
    {
      unsigned lim = 0;
      unsigned til = 0;
      for (unsigned i = 0; i < 4; ++i)
        {
          grid_t val = grid_get(grid, i, j);
          if (!val)
            continue;
          grid = grid_clear(grid, i, j);

          if (til != lim
              && val == grid_get(grid, til - 1, j))
            {
              lim = til;
              grid = grid_set(grid, til - 1, j, val + 1);
              score += g_pows[val + 1];
            }

          else
            {
              grid = grid_set(grid, til, j, val);
              ++til;
              lim = til - 1;
            }
        }
    }

  return grid;
}

inline grid_t grid_move_bottom(grid_t grid, score_t& score)
{
  for (unsigned j = 0; j < 4; ++j)
    {
      unsigned lim = 3;
      unsigned til = 3;
      for (unsigned i = 3; i < 4; --i)
        {
          grid_t val = grid_get(grid, i, j);
          if (!val)
            continue;
          grid = grid_clear(grid, i, j);

          if (til != lim
              && val == grid_get(grid, til + 1, j))
            {
              lim = til;
              grid = grid_set(grid, til + 1, j, val + 1);
              score += g_pows[val + 1];
            }

          else
            {
              grid = grid_set(grid, til, j,val);
              --til;
              lim = til + 1;
            }
        }
    }

  return grid;
}

static const move_f GRID_MOVES_FNS[] =
  {
    grid_move_left,
    grid_move_right,
    grid_move_top,
    grid_move_bottom
  };

inline grid_t grid_move(grid_t grid, score_t& score, move_t move)
{
  assert(move >= 0 && move < 4);
  return GRID_MOVES_FNS[move](grid, score);
}
