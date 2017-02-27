#include "player.hh"
#include <cassert>
#include <iostream>
#include "replay.hh"

void Player::play(AI& agent)
{
  grid_t grid = 0;
  score_t score = 0;
  agent.init(grid, score);

  Replay replay;

  while (!grid_is_finished(grid))
    {
      move_t move = agent.move_get(grid);
      grid_t res = grid_move(grid, score, move);
      replay.add_move(grid, move);

      assert(res != grid);
      grid = res;

      agent.after(grid, score);
    }

  agent.stats();
  replay.save("../replays/" + std::to_string(score) + "_" + std::to_string(Date::now()) + ".data");
}

void Player::play(AI& agent, ShellCanvas& cvs)
{
  grid_t grid = 0;
  score_t score = 0;
  agent.init(grid, score);
  long delta = 0;

  print_grid(cvs, grid, score, delta);
  Timer timer;

  Replay replay;

  while (!grid_is_finished(grid))
    {
      timer.reset();
      move_t move = agent.move_get(grid);
      grid_t res = grid_move(grid, score, move);
      replay.add_move(grid, move);
      delta = timer.get();

      assert(res != grid);
      grid = res;

      agent.after(grid, score);
      print_grid(cvs, grid, score, delta);
    }

  std::cout << "\n";
  agent.stats();
  replay.save("../replays/" + std::to_string(score) + "_" + std::to_string(Date::now()) + ".data");
}

void Player::print_grid(ShellCanvas& cvs, grid_t grid, score_t score, long delta)
{
  cvs.clear();
  cvs.draw_string(0, 0, "Delta: " + std::to_string(delta));
  cvs.draw_string(0, 1, "Score: " + std::to_string(score));
  cvs.draw_sprite(2, 2, grid_to_sprite(grid));
  cvs.render();
}
