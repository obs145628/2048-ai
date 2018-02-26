#pragma once

#include <algorithm>
#include "grid.hh"


namespace to48
{

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

    void heur_init_monoticity();
    void heur_init_empty();
    void heur_init_smoothness();
    void heur_init_sum();
    void heur_init();
    heur_t heur_eval1(grid_t grid);
    heur_t heur_eval2(grid_t grid);

}

#include "heur.hxx"
