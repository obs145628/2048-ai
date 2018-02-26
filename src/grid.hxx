#pragma once

#include "grid.hh"
#include <algorithm>

namespace to48
{
 
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

    inline grid_t grid_rand(RandomMT& rand, grid_t grid)
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

    inline grid_t grid_rand(XorShift64& rand, grid_t grid)
    {
	grid_t val = 1 + (rand.next_u64() % 10) / 9;

	while (true)
	{
	    grid_t pos = rand.next_u64() % 16;
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

    inline void grid_init_left()
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

	    GRID_TABLE_SCORE_LEFT[row] = 0;
      
	    unsigned lim = 0;
	    unsigned til = 0;
	    for (unsigned j = 0; j < 4; ++j)
	    {
		grid_t val = data[j];
		if (!val)
		    continue;
		data[j] = 0;

		if (til != lim
		    && val == data[til - 1])
		{
		    lim = til;
		    data[til - 1] = val + 1;
		    GRID_TABLE_SCORE_LEFT[row] += GRID_POWS[val + 1];
		}

		else
		{
		    data[til] = val;
		    ++til;
		    lim = til - 1;
		}
	    }

	    GRID_TABLE_LEFT[row] = (data[0] << 0)
		| (data[1] << 4)
		| (data[2] << 8)
		| (data[3] << 12);
	}
    }

    inline void grid_init_right()
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

	    GRID_TABLE_SCORE_RIGHT[row] = 0;
      
	    unsigned lim = 3;
	    unsigned til = 3;
	    for (unsigned j = 3; j < 4; --j)
	    {
		grid_t val = data[j];
		if (!val)
		    continue;
		data[j] = 0;

		if (til != lim
		    && val == data[til + 1])
		{
		    lim = til;
		    data[til + 1] = val + 1;
		    GRID_TABLE_SCORE_RIGHT[row] += GRID_POWS[val + 1];
		}

		else
		{
		    data[til] = val;
		    --til;
		    lim = til + 1;
		}
	    }

	    GRID_TABLE_RIGHT[row] = (data[0] << 0)
		| (data[1] << 4)
		| (data[2] << 8)
		| (data[3] << 12);
	}
    }

    inline void grid_init_transpose()
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

	    GRID_TABLE_TR1[row] = 0
		| (data[0] << 0)
		| (data[1] << 16)
		| (data[2] << 32)
		| (data[3] << 48);

	    GRID_TABLE_TR2[row] = 0
		| (data[0] << 4)
		| (data[1] << 20)
		| (data[2] << 36)
		| (data[3] << 52);

	    GRID_TABLE_TR3[row] = 0
		| (data[0] << 8)
		| (data[1] << 24)
		| (data[2] << 40)
		| (data[3] << 56);

	    GRID_TABLE_TR4[row] = 0
		| (data[0] << 12)
		| (data[1] << 28)
		| (data[2] << 44)
		| (data[3] << 60);

	}

    }



    inline void grid_init()
    {
	grid_init_left();
	grid_init_right();
	grid_init_transpose();
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


    inline void grid_update_world(World& world, grid_t grid,
				  score_t score, long delta)
    {
	for (int i = 0; i < 4; ++i)
	    for (int j = 0; j < 4; ++j)
		world.cell_set(i, j, grid_get(grid, i, j));

	world.score_set(score);
	world.delta_set(delta);
    }

    inline grid_t grid_from_world(const World& world)
    {
	assert(world.size_get() == 4);
	grid_t res = 0;
	for (index_t i = 0; i < 16; ++i)
	{
	    index_t val = world.cell_get(i);
	    res = grid_set(res, i, val);
	}
	return res;
    }


}
