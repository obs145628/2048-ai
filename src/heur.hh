#pragma once

#include "grid.hh"

extern long heur_nb_evals;

using heur_t = grid_t;
using heur_f = heur_t (*)(grid_t grid);
constexpr heur_t HEUR_MIN = 0;
constexpr heur_t HEUR_MAX = -1;
constexpr heur_t HEUR_DEATH = 1000;
constexpr heur_t HEUR_BASE = 1e8;

extern heur_t weight_monoticity;
extern heur_t weight_smoothness;
extern heur_t weight_empty;
extern heur_t weight_sum;

extern heur_t HEUR_VALS[GRID_ROW_SIZE];

/**
 * Initialize the values of the heuristic monoticity
 * Test if values are in ascending or descending order
 */
void heur_init_monoticity();

/**
 * Initialize the values of the heuristic empty
 * Count the number of empty cells
 */
void heur_init_empty();

/**
 * Initialize the values of the heuristic smoothness
 * Count the number of adjacent equal cells
 */
void heur_init_smoothness();

/**
 * Initialize the values of the heuristic of sum
 * Sum of the power of all cells
 */
void heur_init_sum();

/**
 * Initialize the values of the heuristic for each row / column
 */
void heur_init();

/**
 * Returns the heuristic value of a grid using several characteristics :
 * - monoticity
 * - empty
 * - smoothness
 * - sum
 */
inline heur_t heur_eval1(grid_t grid);

/**
 * Returns the heuristic value of a grid using cost-matrix:
 *
 * |1024| 512| 256| 128|
 * |   8|  16|  32|  64|
 * |   7|   6|   5|   4|
 * |   0|   1|   2|   3|
 *
 * Try to align numbers
 */
inline heur_t heur_eval2(grid_t grid);

#include "heur.hxx"
