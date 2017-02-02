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

int main()
{
  grid_init();
  heur_init();
  ShellCanvas cvs;


  //auto replay = Replay::load("../replays/173508_1486030462556.data");
  //replay.play(cvs, 10);
  //return 0;

  ExpectimaxAI agent(heur_eval1);
  Player::play(agent, cvs);

  return 0;
}
