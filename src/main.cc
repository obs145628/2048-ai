#include <cassert>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "ai/shell.hh"
#include "ai.hh"
#include "expectimax-ai.hh"
#include "grid.hh"
#include "heur.hh"
#include "min-max-ai.hh"

long heur_nb_evals = 0;

heur_t weight_monoticity = 100;
heur_t weight_smoothness = 20;
heur_t weight_empty = 200;
heur_t weight_sum = 30;


void print_grid(ShellCanvas& cvs, grid_t grid, score_t score, long delta)
{
  cvs.clear();
  cvs.draw_string(0, 0, "Delta: " + std::to_string(delta));
  cvs.draw_string(0, 1, "Score: " + std::to_string(score));
  cvs.draw_sprite(2, 2, grid_to_sprite(grid));
  cvs.render();
}

void play_gui(AI* agent)
{
  grid_t grid = 0;
  score_t score = 0;
  agent->init(grid, score);

  ShellCanvas cvs;
  long delta = 0;

  print_grid(cvs, grid, score, delta);
  Timer timer;

  while (!grid_is_finished(grid))
    {
      timer.reset();
      grid_t res = grid_move(grid, score, agent->move_get(grid));
      delta = timer.get();

      assert(res != grid);
      grid = res;

      agent->after(grid, score);
      print_grid(cvs, grid, score, delta);
    }

  std::cout << "\n";

  agent->stats();
}

template <class T>
void play_stats(heur_f heur)
{
  score_t sum_score = 0;
  score_t min_score = 1e6;
  score_t max_score = 0;
  long min_tilde = 1e6;
  long max_tilde = 0;
  int n = 1;

  while (true)
    {
      heur_nb_evals = 0;
      grid_t grid = 0;
      score_t score = 0;
      T agent{heur};
      agent.init(grid, score);

      while (!grid_is_finished(grid))
        {
          grid = grid_move(grid, score, agent.move_get(grid));
          agent.after(grid, score);
        }

      sum_score += score;
      if (score > max_score)
        max_score = score;
      if (score < min_score)
        min_score = score;

      long tilde = std::pow(2, grid_get_max(grid));
      if (tilde > max_tilde)
        max_tilde = tilde;
      if (tilde < min_tilde)
        min_tilde = tilde;

      agent.stats();

      std::cout << "----------------------\n";
      std::cout << "Run #" << n << ":\n";
      std::cout << "Score: " << score << "( " << tilde << ")\n"; 
      std::cout << "Min: " << min_score << " (" << min_tilde << ")\n";
      std::cout << "Max: " << max_score << " (" << max_tilde << ")\n";

      double av = double(sum_score) / n;
      std::cout << "Average: " << av << "\n";
      std::cout << "----------------------\n\n";

      ++n;
    }

}

int main()
{ 
  grid_init();
  heur_init();

  play_gui(new ExpectimaxAI(heur_eval1));

  return 0;
}
