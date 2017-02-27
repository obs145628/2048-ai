#include "heur.hh"

long heur_nb_evals = 0;

heur_t weight_monoticity = - 100.0;
heur_t weight_smoothness = 20.0;
heur_t weight_smoothness2 = - 1.0;
heur_t weight_empty = 200.0;
heur_t weight_sum = - 30.0;

heur_t HEUR_VALS[GRID_ROW_SIZE];

void heur_init_monoticity()
{
  for (grid_t row = 0; row < GRID_ROW_SIZE; ++row)
    {
      grid_t data[4] =
        {
          (row >> 0) & 0xF,
          (row >> 4) & 0xF,
          (row >> 8) & 0xF,
          (row >> 12) & 0xF,
        };

      heur_t mleft = 0;
      heur_t mright = 0;


      index_t i1 = 0;
      index_t i2 = 1;

      while (i2 < 4)
        {
          while (i2 < 3 && !data[i2])
            ++i2;

          if (data[i1] > data[i2])
            mleft +=  (std::pow(data[i1], 4)
                       - std::pow(data[i2], 4));
          else
            mright +=  (std::pow(data[i2], 4)
                        - std::pow(data[i1], 4));

          i1 = i2++;
        }



      heur_t res = std::min(mleft, mright);
      HEUR_VALS[row] += weight_monoticity * res;
    }
}

void heur_init_empty()
{
  for (grid_t row = 0; row < GRID_ROW_SIZE; ++row)
    {
      grid_t data[4] =
        {
          (row >> 0) & 0xF,
          (row >> 4) & 0xF,
          (row >> 8) & 0xF,
          (row >> 12) & 0xF,
        };

      heur_t res = 0;
      for (int i = 0; i < 4; ++i)
        res += !data[i];

      HEUR_VALS[row] += weight_empty * res;
    }
}

void heur_init_smoothness()
{
  for (grid_t row = 0; row < GRID_ROW_SIZE; ++row)
    {
      grid_t data[4] =
        {
          (row >> 0) & 0xF,
          (row >> 4) & 0xF,
          (row >> 8) & 0xF,
          (row >> 12) & 0xF,
        };

      heur_t res = 0;
      for (index_t i = 0; i < 4; ++i)
        {
          if (!data[i])
            continue;

          index_t k = i + 1;
          while (k < 4 && !data[k])
            ++k;
          if (k < 4)
            res += (data[i] == data[k]);
        }

      HEUR_VALS[row] += weight_smoothness * res;
    }
}

void heur_init_smoothness2()
{
    for (grid_t row = 0; row < GRID_ROW_SIZE; ++row)
    {
      grid_t data[4] =
        {
          (row >> 0) & 0xF,
          (row >> 4) & 0xF,
          (row >> 8) & 0xF,
          (row >> 12) & 0xF,
        };

      heur_t res = 0;
      for (index_t i = 0; i < 4; ++i)
        {
          if (!data[i])
            continue;

          index_t k = i + 1;
          while (k < 4 && !data[k])
            ++k;
          if (k < 4)
            res += std::pow(std::abs(data[i] - data[k]), 4);
        }

      HEUR_VALS[row] += weight_smoothness2 * res;
    }
}

void heur_init_sum()
{
  for (grid_t row = 0; row < GRID_ROW_SIZE; ++row)
    {
      grid_t data[4] =
        {
          (row >> 0) & 0xF,
          (row >> 4) & 0xF,
          (row >> 8) & 0xF,
          (row >> 12) & 0xF,
        };

      heur_t res = 0;
      for (int i = 0; i < 4; ++i)
        res += std::pow(data[i], 3);

      HEUR_VALS[row] += weight_sum * res;
    }
}

void heur_init()
{
  std::fill_n(HEUR_VALS, GRID_ROW_SIZE, HEUR_BASE);
  heur_init_monoticity();
  heur_init_empty();
  heur_init_smoothness();
  heur_init_sum();
}
