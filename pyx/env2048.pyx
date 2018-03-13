from libcpp cimport bool

__FIX_DEF__ = 0

cdef extern from "../src/grid.hh" namespace "to48":
  void grid_init()
  long unsigned int grid_get(long unsigned int grid, long unsigned int pos);
  long unsigned int grid_get(long unsigned int grid, long unsigned int i, long unsigned int j);
  long unsigned int grid_move(long unsigned int grid, long unsigned int& score, int move);
  bool grid_is_finished(long unsigned int grid)
  long unsigned int grid_set(long unsigned int grid, long unsigned int pos, long unsigned int val)
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

  @property
  def score(self): return int(self.score_)
  @score.setter
  def score(self, score): self.score_.y = score

'''
cdef class MyVector:
  cdef Vector ptr_
  def __cinit__(self, double x = 0, double y = 0):
    self.ptr_ = Vector(x, y)
  def norm(self):
    return self.ptr_.norm()
  def dump(self):
    self.ptr_.dump()

  @property
  def x(self): return self.ptr_.x
  @x.setter
  def x(self, x): self.ptr_.x = x

  @property
  def y(self): return self.ptr_.y
  @y.setter
  def y(self, y): self.ptr_.y = y
'''

grid_init()