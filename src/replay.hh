#pragma once

#include "ai/shell.hh"
#include "grid.hh"
#include <string>
#include <vector>

/**
 * Object that contains all grids and moves of a 2048 game
 */
class Replay
{
public:

  /**
   * load a Replay object from a file
   */
  static Replay load(const std::string& path);

  Replay(const std::vector<grid_t>& grids = {},
         const std::vector<move_t>& moves = {});

  const std::vector<grid_t>& grids_get() const;
  const std::vector<move_t>& moves_get() const;

  void add_move(grid_t grid, move_t move);

  /**
   * Save the replay into a file
   */
  void save(const std::string& path);

  /**
   * Play the game and display board
   * @param wait - time in ms between each move
   */
  void play(ShellCanvas& cvs, long wait);

private:
  std::vector<grid_t> grids_;
  std::vector<move_t> moves_;
};
