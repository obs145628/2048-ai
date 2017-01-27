#include <cassert>
#include <iostream>
#include <fstream>
#include "ai/shell.hh"
#include "grid.hh"
#include "ai.hh"
#include "dump-ai.hh"
#include "expect-ai.hh"
#include "min-max-ai.hh"
#include <unistd.h>

#include "debug-eai.hh"
#include "heur.hh"

long nb_evals = 0;


//test1: 17
long weight_mono = 100;//100
long weight_mono2 = 0;
long weight_smoo = 20;//15
long weight_empty = 400;//150
long weight_max = 0;
long weight_edge = 0;
long weight_sum = 60;//30


void print_grid(ShellCanvas& cvs, grid_t grid, score_t score, long delta)
{
  cvs.clear();
  cvs.draw_string(0, 0, "Delta: " + std::to_string(delta));
  cvs.draw_string(0, 1, "Score: " + std::to_string(score));
  cvs.draw_sprite(2, 2, grid_to_sprite(grid));
  cvs.render();
  //usleep(10 * 10e3);
}

double loop_games(int n)
{
  Timer timer;
  score_t sum_score = 0;

  std::vector<int> maxs(15, 0);
  
  for (int i = 1; i <= n; ++i)
    {
      
      grid_t grid = 0;
      score_t score = 0;
      int moves = 0;
  
      auto agent = DebugEAI<Heur1>{};
      agent.init(grid, score);

      while (!grid_finished(grid))
        {
          grid = grid_move(grid, score, agent.move_get(grid, score));
          agent.after(grid, score);
          ++moves;
        }

      sum_score += score;
      long max_tilde = std::pow(2, heur_max(grid));
      ++maxs[heur_max(grid)];

      continue;
      
      std::cout << i << "/" << n << ": "
                << score << " - " << max_tilde
                << " (" << timer.reset() << "ms, "
                << moves << ")\n";
    }

  double av = double(sum_score) / n;
  std::cout << "----------------------\n";
  std::cout << "Average: " << av << "\n";

  for (std::size_t i = 9; i < maxs.size(); ++i)
    {
      int nb = 0;
      for (std::size_t j = i; j < maxs.size(); ++j)
        nb += maxs[j];
      long val = std::pow(2, i);
      double freq = double(nb) / n;

      std::cout << val << ": " << (freq * 100) << "%\n";
    }
  
  std::cout << "----------------------\n";

  return av;
}

void find_weight(long& weight, long a, long b, long delta)
{
  double max = 0;
  long max_weight = 0;

  {
    std::ofstream os("./hist.data");

    for (weight = a; weight < b; weight += delta)
      {
        std::cout << "Weight: " << weight
                  << "(max: " << max_weight << ")" << std::endl;
        double val = loop_games(10);
        if (val > max)
          {
            max = val;
            max_weight = weight;
          }

        os << weight << " " << val << "\n";
      }

    std::cout << "Best weight: " << max_weight << std::endl;
    std::cout << "Best score : " << max << std::endl;

  }

  Command::exec("perl -lane 'print $F[0], \"\\t\", \"=\" x ($F[1] / 1000)' ./hist.data");
}

void check2()
{
  score_t sum_score = 0;
  score_t min_score = 1e6;
  score_t max_score = 0;
  long min_tilde = 1e6;
  long max_tilde = 0;
  int n = 1;

  while (true)
    {
      nb_evals = 0;
      grid_t grid = 0;
      score_t score = 0;
      auto agent = DebugEAI<Heur1>{};
      agent.init(grid, score);

      while (!grid_finished(grid))
        {
          grid = grid_move(grid, score, agent.move_get(grid, score));
          agent.after(grid, score);
        }

      sum_score += score;
      if (score > max_score)
        max_score = score;
      if (score < min_score)
        min_score = score;

      long tilde = std::pow(2, heur_max(grid));
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

  //find_weight(weight_empty, 0, 600, 30);
  //return 0;

  //check2();
  
  grid_t grid = 0;
  score_t score = 0;
  
  auto agent = DebugEAI<Heur1>{};
  //auto agent = DebugEAI<EvalAlign2>{};
  agent.init(grid, score);


  ShellCanvas cvs;
  long delta = 0;

  print_grid(cvs, grid, score, delta);
  Timer timer;

  while (!grid_finished(grid))
    {
      timer.reset();
      grid_t res = grid_move(grid, score, agent.move_get(grid, score));
      delta = timer.get();
      
      assert(res != grid);
      grid = res;

      agent.after(grid, score);
      print_grid(cvs, grid, score, delta);
    }
  
  std::cout << std::endl;

  agent.stats();
}
