#include "grid.hh"

namespace to48
{
    const score_t GRID_POWS[] =
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


    grid_t GRID_TABLE_LEFT[GRID_ROW_SIZE];
    grid_t GRID_TABLE_RIGHT[GRID_ROW_SIZE];
    grid_t GRID_TABLE_SCORE_LEFT[GRID_ROW_SIZE];
    grid_t GRID_TABLE_SCORE_RIGHT[GRID_ROW_SIZE];
    grid_t GRID_TABLE_TR1[GRID_ROW_SIZE];
    grid_t GRID_TABLE_TR2[GRID_ROW_SIZE];
    grid_t GRID_TABLE_TR3[GRID_ROW_SIZE];
    grid_t GRID_TABLE_TR4[GRID_ROW_SIZE];
}

