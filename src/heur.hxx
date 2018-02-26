#include "heur.hh"
#include <algorithm>

namespace to48
{

    inline void heur_init_monoticity()
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
	    HEUR_VALS[row] -= weight_monoticity * res;
	}
    }

    inline void heur_init_empty()
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

    inline void heur_init_smoothness()
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

    inline void heur_init_sum()
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

	    HEUR_VALS[row] -= weight_sum * res;
	}
    }

    inline void heur_init()
    {
	std::fill_n(HEUR_VALS, GRID_ROW_SIZE, HEUR_BASE);
	heur_init_monoticity();
	heur_init_empty();
	heur_init_smoothness();
	heur_init_sum();
    }

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

}
