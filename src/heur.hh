#pragma once

#include "grid.hh"

extern long nb_evals;

using heur_t = grid_t;
constexpr heur_t HEUR_MIN = 0;
constexpr heur_t HEUR_MAX = -1;
constexpr heur_t HEUR_DEATH = 1;
constexpr heur_t HEUR_BASE = 1e9;

extern long weight_mono;
extern long weight_mono2;
extern long weight_smoo;
extern long weight_empty;
extern long weight_max;
extern long weight_edge;
extern long weight_sum;


inline heur_t heur_max(grid_t grid)
{
  heur_t res = 0;
  for (heur_t i = 0; i < 16; ++i)
    {
      heur_t val = grid_get(grid, i);
      res = std::max(res, val);
    }
  return res;
}

inline heur_t heur_sum(grid_t grid)
{
  heur_t res = 0;
  for (index_t i = 0; i < 16; ++i)
    {
      heur_t val = grid_get(grid, i);
      res += std::pow(val, 3);
    }
  return res;
}

inline heur_t heur_edge(grid_t grid)
{

  return - (grid_get(grid, 0) != heur_max(grid));
  
  return 64 * grid_get(grid, 0)
    + 16 * grid_get(grid, 1)
    + 4 * grid_get(grid, 2)
    + 16 * grid_get(grid, 4)
    + 4 * grid_get(grid, 5)
    + 4 * grid_get(grid, 8);
}

inline heur_t heur_empty(grid_t grid)
{
  heur_t res = 0;
  for (heur_t i = 0; i < 16; ++i)
    res += !grid_get(grid, i);
  return res;
}


inline heur_t heur_smoothness(grid_t grid)
{
  heur_t res = 0;
  for (index_t i = 0; i < 4; ++i)
    for (index_t j = 0; j < 4; ++j)
      {
        if (!grid_get(grid, i, j))
          continue;

        index_t k = i + 1;
        while (k < 4 && !grid_get(grid, k, j))
          ++k;
        if (k < 4)
          res += grid_get(grid, i, j) == grid_get(grid, k, j);

        k = j + 1;
        while (k < 4 && !grid_get(grid, i, k))
          ++k;

        if (k < 4)
          res += grid_get(grid, i, j) - grid_get(grid, i, k);
      }

  return res;
}

inline heur_t heur_monocity(grid_t grid)
{ 
  heur_t mup = 0;
  heur_t mdown = 0;
  heur_t mleft = 0;
  heur_t mright = 0;

  for (index_t i = 0; i < 4; ++i)
    {
      index_t j1 = 0;
      index_t j2 = 1;

      while (j2 < 4)
        {
          while (j2 < 3 && !grid_get(grid, i, j2))
            ++j2;

          if (grid_get(grid, i, j1) > grid_get(grid, i, j2))
            mup +=  (std::pow(grid_get(grid, i, j1), 4)
                    - std::pow(grid_get(grid, i, j2), 4));
          else
            mdown += (std::pow(grid_get(grid, i, j2), 4)
                      - std::pow(grid_get(grid, i, j1), 4));

          j1 = j2++;
        }
    }

  for (index_t j = 0; j < 4; ++j)
    {
      index_t i1 = 0;
      index_t i2 = 1;

      while (i2 < 4)
        {
          while (i2 < 3 && !grid_get(grid, i2, j))
            ++i2;

          if (grid_get(grid, i1, j) > grid_get(grid, i2, j))
            mleft +=  (std::pow(grid_get(grid, i1, j), 4)
                      - std::pow(grid_get(grid, i2, j), 4));
          else
            mright +=  (std::pow(grid_get(grid, i2, j), 4)
                       - std::pow(grid_get(grid, i1, j), 4));

          i1 = i2++;
        }
    }


    
  heur_t res = std::min(mup, mdown) + std::min(mleft, mright);
  //heur_t res = mup + mleft;
  return res;
}

struct Heur1
{
  heur_t operator()(grid_t grid) const
  {
    ++nb_evals;

    return HEUR_BASE
      - weight_mono * heur_monocity(grid)
      + weight_empty * heur_empty(grid)
      + weight_smoo * heur_smoothness(grid)
      + weight_max * heur_max(grid)
      + weight_edge * heur_edge(grid)
      - weight_sum * heur_sum(grid);
  }
};

struct Heur2
{
  heur_t operator()(grid_t grid) const
  {
    ++nb_evals;
    return 1024 * g_vals[grid_get(grid, 0)]
      + 512 * g_vals[grid_get(grid, 1)]
      + 256 * g_vals[grid_get(grid, 2)]
      + 128 * g_vals[grid_get(grid, 3)]
      + 64 * g_vals[grid_get(grid, 7)]
      + 32 * g_vals[grid_get(grid, 6)]
      + 16 * g_vals[grid_get(grid, 5)]
      + 8 * g_vals[grid_get(grid, 4)]
      + 7 * g_vals[grid_get(grid, 8)]
      + 6 * g_vals[grid_get(grid, 9)]
      + 5 * g_vals[grid_get(grid, 10)]
      + 4 * g_vals[grid_get(grid, 11)]
      + 3 * g_vals[grid_get(grid, 14)]
      + 2 * g_vals[grid_get(grid, 13)]
      + 1 * g_vals[grid_get(grid, 12)];
  }
};


