from libcpp cimport bool

import random

__FIX_DEF__ = 0

LEFT = 0

cdef extern from "../src/grid.hh" namespace "to48":
  void grid_init()
  long unsigned int grid_get(long unsigned int grid, long unsigned int pos);
  long unsigned int grid_get(long unsigned int grid, long unsigned int i, long unsigned int j);
  long unsigned int grid_move(long unsigned int grid, long unsigned int& score, int move);
  bool grid_is_finished(long unsigned int grid)
  long unsigned int grid_set(long unsigned int grid, long unsigned int pos, long unsigned int val)
  bool grid_move_valid(long unsigned int grid, int move)
  void grid_dump(long unsigned int grid);
  void grid_dump(long unsigned int grid, long unsigned int score);

cdef class Grid:
  cdef long unsigned int grid_
  cdef long unsigned int score_
  def __cinit__(self):
    self.grid_ = 0
    self.score_ = 0

  def take_action(self, action):
    self.grid_ = grid_move(self.grid_, self.score_, action)

  def set_cell(self, pos, val):
    self.grid_ = grid_set(self.grid_, pos, val)

  def finished(self):
     return grid_is_finished(self.grid_)

  def dump(self):
    grid_dump(self.grid_, self.score_)

  def get_cell1(self, pos):
    return grid_get(self.grid_, pos)

  def get_cell2(self, i, j):
    return grid_get(self.grid_, i, j)

  def valid_action(self, a):
    return grid_move_valid(self.grid_, a)

  def actions_list(self):
    return [a for a in range(4) if grid_move_valid(self.grid_, a)]

  def to_arr(self):
    res = [0] * 16
    for i in range(16):
      res[i] = grid_get(self.grid_, i)
    return res

  def add_rand_cell(self):
    cdef int i, val
    while True:
       i = random.randint(0, 15)
       if grid_get(self.grid_, i) == 0: break
    val = random.randint(1, 2)
    self.grid_ = grid_set(self.grid_, i, val)

  @property
  def score(self): return int(self.score_)
  @score.setter
  def score(self, score): self.score_.y = score

grid_init()