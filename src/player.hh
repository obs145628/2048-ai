#pragma once
#include "ai/shell.hh"
#include "ai.hh"
#include "grid.hh"
#include "heur.hh"

class Player
{
public:

  /**
   * Play a 2048 game without displaying board
   * Print stats at the end of the game
   */
  static void play(AI& agent);

  /**
   * Play a 2048 game and display board
   * Print stats at the end of the game
   */
  static void play(AI& agent, ShellCanvas& cvs);

  /**
   * Play an infinite number of 2048 games without displaying board
   * At the end of each game stats about current and all games are displayed
   */
  template <class T>
  void play_stats(heur_f heur);

  static void print_grid(ShellCanvas& cvs, grid_t grid, score_t score, long delta);
};

#include "player.hxx"
