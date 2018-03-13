#pragma once

#include <cassert>
#include <cstdint>
#include <cmath>
#include <ai/misc/random-mt.hh>
#include <ai/misc/xor-shift.hh>
#include "world.hh"

namespace to48
{

    using grid_t = std::uint64_t;
    using index_t = std::uint64_t;
    using score_t = std::uint64_t;
    using move_t = int;
    using move_f = grid_t (*)(grid_t grid, score_t &score);
    using fast_move_f = grid_t (*)(grid_t grid);

    static constexpr move_t GRID_LEFT = 0;
    static constexpr move_t GRID_RIGHT = 1;
    static constexpr move_t GRID_TOP = 2;
    static constexpr move_t GRID_BOTTOM = 3;
    static constexpr move_t  MOVE_NO = 4;
    static constexpr int GRID_ROW_SIZE = 0xFFFF;

    extern const score_t GRID_POWS[];
    extern grid_t GRID_TABLE_LEFT[GRID_ROW_SIZE];
    extern grid_t GRID_TABLE_RIGHT[GRID_ROW_SIZE];
    extern grid_t GRID_TABLE_SCORE_LEFT[GRID_ROW_SIZE];
    extern grid_t GRID_TABLE_SCORE_RIGHT[GRID_ROW_SIZE];
    extern grid_t GRID_TABLE_TR1[GRID_ROW_SIZE];
    extern grid_t GRID_TABLE_TR2[GRID_ROW_SIZE];
    extern grid_t GRID_TABLE_TR3[GRID_ROW_SIZE];
    extern grid_t GRID_TABLE_TR4[GRID_ROW_SIZE];


    index_t grid_get(grid_t grid, index_t pos);
    index_t grid_get(grid_t grid, index_t i, index_t j);
    bool grid_is_full(grid_t grid);
    bool grid_is_finished(grid_t grid);
    grid_t grid_clear(grid_t grid, index_t pos);
    grid_t grid_clear(grid_t grid, index_t i, index_t j);
    int grid_count_empty(grid_t grid);
    grid_t grid_get_max(grid_t grid);
    grid_t grid_set(grid_t grid, index_t pos, index_t val);
    grid_t grid_set(grid_t grid, index_t i, index_t j, index_t val);
    grid_t grid_put(grid_t grid, index_t pos, index_t val);
    grid_t grid_put(grid_t grid, index_t i, index_t j, index_t val);
    grid_t grid_rand(RandomMT& rand, grid_t grid);
    grid_t grid_rand(XorShift64& rand, grid_t grid);
    grid_t grid_get_row(grid_t grid, index_t i);
    grid_t grid_join_rows(grid_t r1, grid_t r2, grid_t r3, grid_t r4);
    void grid_init_left();
    void grid_init_right();
    void grid_init_transpose();
    void grid_init();
    grid_t grid_transpose(grid_t grid);
    grid_t grid_move_left(grid_t grid, score_t& score);
    grid_t grid_move_right(grid_t grid, score_t& score);
    grid_t grid_move_top(grid_t grid, score_t& score);
    grid_t grid_move_bottom(grid_t grid, score_t& score);
    grid_t grid_move(grid_t grid, score_t& score, move_t move);
    grid_t grid_fast_move_left(grid_t grid);
    grid_t grid_fast_move_right(grid_t grid);
    grid_t grid_fast_move_top(grid_t grid);
    grid_t grid_fast_move_bottom(grid_t grid);
    grid_t grid_fast_move(grid_t grid, move_t move);

    void grid_update_world(World& world, grid_t grid,
			   score_t score, long delta = 0);
    grid_t grid_from_world(const World& world);

    void grid_dump(grid_t grid);
    void grid_dump(grid_t grid, score_t score);

}

#include "grid.hxx"
