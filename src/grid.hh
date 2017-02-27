#pragma once

#include <cassert>
#include <cstdint>
#include <cmath>
#include <iostream>
#include "ai/math/random.hh"
#include "ai/shell/grid.hh"
#include "ai/shell/shell.hh"

using grid_t = std::uint64_t;
using index_t = std::uint64_t;
using score_t = std::uint64_t;
using move_t = int;
using move_f = grid_t (*)(grid_t grid, score_t& score);
using fast_move_f = grid_t (*)(grid_t grid);




/** Values of the tildes : 2, 4, 8, ... */
extern const score_t GRID_POWS[];

/**string representation of the tildes */
extern const std::string GRID_LABELS[];

/** Shell foreground color codes */
extern const char* GRID_FGS[];

/** Shell backround color codes */
extern const char* GRID_BGS[];

constexpr move_t GRID_LEFT = 0;
constexpr move_t GRID_RIGHT = 1;
constexpr move_t GRID_TOP = 2;
constexpr move_t GRID_BOTTOM = 3;
constexpr int GRID_ROW_SIZE = 0xFFFF;

extern grid_t GRID_TABLE_LEFT[GRID_ROW_SIZE];
extern grid_t GRID_TABLE_RIGHT[GRID_ROW_SIZE];
extern grid_t GRID_TABLE_SCORE_LEFT[GRID_ROW_SIZE];
extern grid_t GRID_TABLE_SCORE_RIGHT[GRID_ROW_SIZE];
extern grid_t GRID_TABLE_TR1[GRID_ROW_SIZE];
extern grid_t GRID_TABLE_TR2[GRID_ROW_SIZE];
extern grid_t GRID_TABLE_TR3[GRID_ROW_SIZE];
extern grid_t GRID_TABLE_TR4[GRID_ROW_SIZE];

/**
 * Init the tables to move left / up
 */
void grid_init_left();

/**
 * Init the tables to move right / down
 */
void grid_init_right();


/**
 * Init the tables to transpose the grid
 */
void grid_init_transpose();

/**
 * Initialize the tables to perform operations on grid
 */
void grid_init();


/**
 * Returns the value of grid[pos] (up -> down, left -> right)
 */
inline index_t grid_get(grid_t grid, index_t pos);

/**
 * Returns the value of grid[i, j] (row i, col j)
 */
inline index_t grid_get(grid_t grid, index_t i, index_t j);

/**
 * Returns true if 16 tildes are on the grid
 */
inline bool grid_is_full(grid_t grid);

/**
 * Returns true if there is no possible move
 */
inline bool grid_is_finished(grid_t grid);

/**
 * Remove the tilde at grid[pos]
 */
inline grid_t grid_clear(grid_t grid, index_t pos);

/**
 * Remove the tilde at grid[i, j]
 */
inline grid_t grid_clear(grid_t grid, index_t i, index_t j);

/**
 * Return the number of empty cells in grid
 */
inline int grid_count_empty(grid_t grid);

/**
 * Return the value (not power) of the maximum tilde of grid
 */
inline grid_t grid_get_max(grid_t grid);

/**
 * Change the value of grid[pos]
 */
inline grid_t grid_set(grid_t grid, index_t pos, index_t val);

/**
 * Change the value of grid[i, j]
 */
inline grid_t grid_set(grid_t grid, index_t i, index_t j, index_t val);

/**
 * Add a tilde in grid[pos]
 * grid[pos] must be empty
 */
inline grid_t grid_put(grid_t grid, index_t pos, index_t val);

/**
 * Add a tilde in grid[i, j]
 * grid[i, j] must be empty
 */
inline grid_t grid_put(grid_t grid, index_t i, index_t j, index_t val);

inline ShellSprite grid_to_sprite(grid_t grid);

/**
 * Add a tilde at random position in the grid
 * tilde value is 2 with probability 0.9, or 0.4
 */
inline grid_t grid_rand(Random& rand, grid_t grid);

/**
 * Return the i-th row of grid
 */
inline grid_t grid_get_row(grid_t grid, index_t i);

/**
 * Build a grid from 4 rows
 */
inline grid_t grid_join_rows(grid_t r1, grid_t r2, grid_t r3, grid_t r4);

/**
 * Tranpose the grid (rows become columns, and columns become row)
 */
inline grid_t grid_transpose(grid_t grid);

/**
 * Move the grid left and update the score
 */
inline grid_t grid_move_left(grid_t grid, score_t& score);

/**
 * Move the grid right and update the score
 */
inline grid_t grid_move_right(grid_t grid, score_t& score);

/**
 * Move the grid top and update the score
 */
inline grid_t grid_move_top(grid_t grid, score_t& score);

/**
 * Move the grid bottom and update the score
 */
inline grid_t grid_move_bottom(grid_t grid, score_t& score);

/**
 * Move the grid and update the score
 */
inline grid_t grid_move(grid_t grid, score_t& score, move_t move);

/**
 * Move the grid left without updating score
 */
inline grid_t grid_fast_move_left(grid_t grid);

/**
 * Move the grid right without updating score
 */
inline grid_t grid_fast_move_right(grid_t grid);

/**
 * Move the grid top without updating score
 */
inline grid_t grid_fast_move_top(grid_t grid);

/**
 * Move the grid bottom without updating score
 */
inline grid_t grid_fast_move_bottom(grid_t grid);

/**
 * Move the grid without updating score
 */
inline grid_t grid_fast_move(grid_t grid, move_t move);


#include "grid.hxx"
