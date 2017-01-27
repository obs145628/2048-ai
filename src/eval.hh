#pragma once

#include "grid.hh"

extern long nb_evals;

extern long weight_mono;
extern long weight_mono2;
extern long weight_smoo;
extern long weight_empty;
extern long weight_max;
extern long weight_edge;
extern long weight_sum;


inline grid_t max_val(grid_t grid)
{
  grid_t res = 0;
  for (index_t i = 0; i < 16; ++i)
    {
      grid_t val = grid_get(grid, i);
      res = std::max(res, val);
    }
  return res;
}

inline grid_t sum_val(grid_t grid)
{
  grid_t res = 0;
  for (index_t i = 0; i < 16; ++i)
    {
      grid_t val = grid_get(grid, i);
      res += std::pow(val, 4);
    }
  return res;
}

inline long edge_val(grid_t grid)
{

  return - (grid_get(grid, 0) != max_val(grid));
  
  return 64 * grid_get(grid, 0)
    + 16 * grid_get(grid, 1)
    + 4 * grid_get(grid, 2)
    + 16 * grid_get(grid, 4)
    + 4 * grid_get(grid, 5)
    + 4 * grid_get(grid, 8);
}

inline long nb_empty(grid_t grid)
{
  grid_t res = 0;
  for (index_t i = 0; i < 16; ++i)
    res += !grid_get(grid, i);
  return res;
}

inline long nb_blocks(grid_t grid)
{
  long res = 0;
  for (index_t i = 0; i < 16; ++i)
    res += !!grid_get(grid, i);
  return res;
}

inline long count_matches(grid_t grid)
{
  long res = 0;
  for (index_t i = 0; i < 4; ++i)
    for (index_t j = 0; j < 3; ++j)
      if (grid_get(grid, i, j) && grid_get(i, j) == grid_get(i, j + 1))
        ++res;

  for (index_t j = 0; j < 4; ++j)
    for (index_t i = 0; i < 3; ++i)
      if (grid_get(grid, i, j) && grid_get(i, j) == grid_get(i + 1, j))
        ++res;

  return res;
}

inline long smoothness(grid_t grid)
{
  grid_t res = 0;
  for (index_t i = 0; i < 4; ++i)
    for (index_t j = 0; j < 4; ++j)
      {
        if (!grid_get(grid, i, j))
          continue;

        index_t k = i + 1;
        while (k < 4 && !grid_get(grid, k, j))
          ++k;
        if (k < 4)
          res += std::abs(grid_get(grid, i, j) - grid_get(grid, k, j));

        k = j + 1;
        while (k < 4 && !grid_get(grid, i, k))
          ++k;

        if (k < 4)
          res += std::abs(grid_get(grid, i, j) - grid_get(grid, i, k));
      }



  long resl = res;
  return -resl;
}

inline long smoothness2(grid_t grid)
{
  long res = 0;
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

inline long monocity(grid_t grid)
{
  long mup = 0;
  long mdown = 0;
  long mleft = 0;
  long mright = 0;

  for (index_t i = 0; i < 4; ++i)
    {
      index_t j1 = 0;
      index_t j2 = 1;

      while (j2 < 4)
        {
          while (j2 < 3 && !grid_get(grid, i, j2))
            ++j2;

          long val = grid_get(grid, i, j1) - grid_get(grid, i, j2);
          if (val < 0)
            mup += std::pow(val, 3);
          else
            mdown -= std::pow(val, 3);

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

          long val = grid_get(grid, i1, j) - grid_get(grid, i2, j);
          if (val < 0)
            mleft += std::pow(val, 3);
          else
            mright -= std::pow(val, 3);

          i1 = i2++;
        }
    }


    
  long res = std::max(mup, mdown) + std::max(mleft, mright);
  //long res = mup + mleft;
  return res;
}

inline long monocity2(grid_t grid)
{
  long mup = 0;
  long mdown = 0;
  long mleft = 0;
  long mright = 0;

  for (index_t i = 0; i < 4; ++i)
    for (index_t j = 0; j < 3; ++j)
      {
        if (grid_get(grid, i, j) > grid_get(grid, i, j + 1))
          mup += (grid_get(grid, i, j) - grid_get(grid, i, j + 1));
        else
          mdown += (grid_get(grid, i, j + 1) - grid_get(grid, i, j));
      }

  for (index_t j = 0; j < 4; ++j)
    for (index_t i = 0; i < 3; ++i)
      {
        if (grid_get(grid, i, j) > grid_get(grid, i + 1, j))
          mleft += (grid_get(grid, i, j) - grid_get(grid, i + 1, j));
        else
          mright += (grid_get(grid, i + 1, j) - grid_get(grid, i, j));
      }


  long res = std::min(mup, mdown) + std::min(mleft, mright);
  //long res = mup + mleft;
  //std::cout << res << std::endl;
  return  - res;
}

struct EvalEmpty
{
  long operator()(grid_t grid) const
  {
    ++nb_evals;
    return 16 - nb_blocks(grid);
  }
};

struct EvalBlocks
{
  long operator()(grid_t grid) const
  {
    ++nb_evals;
    long res = 0;
    for (index_t i = 0; i < 16; ++i)
      res += g_vals[grid_get(grid, i)];
    return res;
  }
};

struct EvalBlocks2
{
  long operator()(grid_t grid) const
  {
    ++nb_evals;
    long res = 0;
    for (index_t i = 0; i < 16; ++i)
      res += g_vals[grid_get(grid, i)];
    return res / nb_blocks(grid);
  }
};

struct EvalAlign
{
  long operator()(grid_t grid) const
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

struct EvalAlign2
{
  long operator()(grid_t grid) const
  {
    ++nb_evals;
    long res = 0;
    long coeff = 65536;
    long prev = g_vals[grid_get(grid, 0)];
    for (index_t i : {0, 1, 2, 3, 7, 6, 5, 4,
          8, 9, 10, 11, 15, 14, 13, 12})
      {
        long val = g_vals[grid_get(grid, i)];
        if (prev >= val)
          res += coeff * val;
        else
          res -= coeff * prev;
        coeff /= 2;
        prev = val;
        prev = prev;
      }

    return res;
  }
};

struct EvalAlign3
{
  long operator()(grid_t grid) const
  {
    ++nb_evals;
    long res = 
       8096 * g_vals[grid_get(grid,  0)]
      + 2048 * g_vals[grid_get(grid,  4)]
      + 2048 * g_vals[grid_get(grid,  1)]
      + 512 * g_vals[grid_get(grid,  8)]
      + 512 * g_vals[grid_get(grid,  5)]
      + 512 * g_vals[grid_get(grid,  2)]
      +  128 * g_vals[grid_get(grid, 12)]
      +  128 * g_vals[grid_get(grid,  9)]
      +  128 * g_vals[grid_get(grid,  6)]
      +  128 * g_vals[grid_get(grid,  3)]
      +  32 * g_vals[grid_get(grid, 13)]
      +  32 * g_vals[grid_get(grid, 10)]
      +  32 * g_vals[grid_get(grid,  7)]
      +  8 * g_vals[grid_get(grid, 14)]
      +  8 * g_vals[grid_get(grid, 11)];

    if (nb_blocks(grid) > 12)
      res /= 2;
    return res;
  }
};

struct EvalMonocity
{
  long operator()(grid_t grid) const
  {
    ++nb_evals;
    long res = 0;

    for (index_t i = 0; i < 3; ++i)
      for (index_t j = 0; j < 3; ++j)
        if (grid_get(grid, i, j) >= grid_get(grid, i, j + 1)
            && grid_get(grid, i, j) >= grid_get(grid, i + 1, j))
          res += std::pow((3 - i) * (3 - j), 3);
        else
          break;
    
    return res;
  }
};

struct EvalMulti1
{
  long operator()(grid_t grid) const
  {
    ++nb_evals;

    return weight_mono * monocity(grid)
      + weight_mono2 * monocity2(grid)
      + weight_empty * nb_empty(grid)
      +  weight_smoo * smoothness2(grid)
      + weight_max * max_val(grid)
      + weight_edge * edge_val(grid)
      + weight_sum * sum_val(grid);
  }
};


struct EvalMonocity2
{
  long operator()(grid_t grid) const
  {
    ++nb_evals;
    long res = 0;
    
    for (index_t i = 0; i < 4; ++i)
      {
        for (index_t j = 0; j < 3; ++j)
          {
            if (grid_get(grid, i, j) >= grid_get(grid, i, j + 1))
              res += grid_get(grid, i, j) * (2 - j) * (2 - j) * (2 - j);
            else
              break;
          }
      }

    for (index_t j = 0; j < 4; ++j)
      {
        for (index_t i = 0; i < 3; ++i)
          {
            if (grid_get(grid, i, j) >= grid_get(grid, i + 1, j))
              res += grid_get(grid, i, j) * (2 - i) * (2 - i) * (2 - i);
            else
              break;
          }
      }

    res += count_matches(grid);
    if (nb_blocks(grid) > 12)
      res /= (nb_blocks(grid) - 12);
    return res;
  }
};
