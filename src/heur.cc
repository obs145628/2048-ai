#include "heur.hh"

namespace to48
{
    long heur_nb_evals = 0;
    heur_t weight_monoticity = 100;
    heur_t weight_smoothness = 20;
    heur_t weight_empty = 200;
    heur_t weight_sum = 30;
    heur_t HEUR_VALS[GRID_ROW_SIZE];
}
