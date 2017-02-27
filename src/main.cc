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
#include "player.hh"
#include "replay.hh"
#include "proba.hh"

int main()
{
  grid_init();
  heur_init();
  Proba::init_tables();
  ShellCanvas cvs;


  //auto replay = Replay::load("../replays/173508_1486030462556.data");
  //replay.play(cvs, 10);
  //return 0;

  /*
  std::ifstream is("../tests/grids.txt");
  assert(is.is_open());

  auto grid = grid_load(is);
  grid = grid_load(is);
  
  ShellCanvas cvs;
  cvs.draw_sprite(2, 2, grid_to_sprite(grid));
  cvs.render();
  

  //play_gui(new ExpectimaxAI(heur_eval1));
  */

  ExpectimaxAI agent(heur_eval1);
  Player::play(agent, cvs);

  return 0;
}
