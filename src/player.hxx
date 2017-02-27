#pragma once

#include "player.hh"

template <class T>
void Player::play_stats(heur_f heur)
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
