#include "replay.hh"
#include <cassert>
#include <fstream>
#include <unistd.h>

Replay Replay::load(const std::string& path)
{
  std::ifstream is(path);
  assert(is.is_open());

  std::vector<grid_t> grids;
  std::vector<move_t> moves;

  while (true)
    {
      grid_t grid = 0;
      for (index_t i = 0; i < 16; ++i)
        {
          int val;
          is >> val;
          grid = grid_put(grid, i, val);
        }

      move_t move;
      is >> move;

      if (!is.good())
        break;

      grids.push_back(grid);
      moves.push_back(move);
    }

  return Replay(grids, moves);
}

Replay::Replay(const std::vector<grid_t>& grids,
               const std::vector<move_t>& moves)
  : grids_(grids)
  , moves_(moves)
{
  assert(grids.size() == moves.size());
}

const std::vector<grid_t>& Replay::grids_get() const
{
  return grids_;
}

const std::vector<move_t>& Replay::moves_get() const
{
  return moves_;
}

void Replay::add_move(grid_t grid, move_t move)
{
  assert(move >= 0 && move <= 3);
  grids_.push_back(grid);
  moves_.push_back(move);
}

void Replay::save(const std::string& path)
{
  std::ofstream os(path);
  assert(os.is_open());

  for (std::size_t i = 0; i < moves_.size(); ++i)
    {
      grid_t grid = grids_[i];
      move_t move = moves_[i];

      for (index_t j = 0; j < 16; ++j)
        {
          int val = grid_get(grid, j);
          os << val << " ";
        }

      os << move << "\n";
    }
}

void Replay::play(ShellCanvas& cvs, long wait)
{
  for (auto grid : grids_)
    {
      cvs.clear();
      cvs.draw_sprite(2, 2, grid_to_sprite(grid));
      cvs.render();
      usleep(wait * 1e3);
    }
}
