#include "monte-carlo.hh"
#include <ai/misc/xor-shift.hh>
#include "grid.hh"


namespace to48
{

    namespace
    {

        score_t play_rand(grid_t grid)
        {
            XorShift64 rand;

            score_t score = 0;

            grid = grid_rand(rand, grid);

            while (!grid_is_finished(grid))
            {
                move_t move = rand.next_int(4);
                grid_t next = grid_move(grid, score, move);
                if (next != grid)
                    next = grid_rand(rand, next);
                grid = next;
            }

            return score;
        }

    }

    World::action_t MonteCarlo::action_get(const World& world)
    {
	grid_t grid = grid_from_world(world);
        move_t best_move = 0;
        score_t max = 0;

        for (move_t i = 0; i < 4; ++i)
        {
            score_t temp;
            score_t av = 0;
            grid_t grid2 = grid_move(grid, temp, i);

            if (grid2 == grid)
                continue;

            score_t NPLAYS = 65536;
            for (score_t j = 0; j < NPLAYS; ++j)
                av += play_rand(grid2);
            av /= NPLAYS;

            if (av > max)
            {
                max = av;
                best_move = i;
            }
        }

        return best_move;
    }

}
