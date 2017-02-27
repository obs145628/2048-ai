#include "grid.hh"

const score_t GRID_POWS[] =
  {
    0,
    2,
    4,
    8,
    16,
    32,
    64,
    128,
    256,
    512,
    1024,
    2048,
    4096,
    8192,
    16384,
    32768
  };


const std::string GRID_LABELS[] =
  {
    "     ",
    "  2  ",
    "  4  ",
    "  8  ",
    " 16  ",
    " 32  ",
    " 64  ",
    " 128 ",
    " 256 ",
    " 512 ",
    "1024 ",
    "2048 ",
    "4096 ",
    "8192 ",
    "16384",
    "32768",
  };


const char* GRID_FGS[] =
  {
    Shell::FG_DEFAULT,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_BLACK,
    Shell::FG_WHITE,
    Shell::FG_WHITE,
    Shell::FG_WHITE,
    Shell::FG_WHITE,
  };

const char* GRID_BGS[] =
  {
    Shell::BG_DEFAULT,
    Shell::BG_LGRAY,
    Shell::BG_DGRAY,
    Shell::BG_LGREEN,
    Shell::BG_GREEN,
    Shell::BG_LYELLOW,
    Shell::BG_YELLOW,
    Shell::BG_LRED,
    Shell::BG_RED,
    Shell::BG_LMAGENTA,
    Shell::BG_MAGENTA,
    Shell::BG_BLUE,
    Shell::BG_BLACK,
    Shell::BG_BLACK,
    Shell::BG_BLACK,
    Shell::BG_BLACK
  };

grid_t GRID_TABLE_LEFT[GRID_ROW_SIZE];
grid_t GRID_TABLE_RIGHT[GRID_ROW_SIZE];
grid_t GRID_TABLE_SCORE_LEFT[GRID_ROW_SIZE];
grid_t GRID_TABLE_SCORE_RIGHT[GRID_ROW_SIZE];
grid_t GRID_TABLE_TR1[GRID_ROW_SIZE];
grid_t GRID_TABLE_TR2[GRID_ROW_SIZE];
grid_t GRID_TABLE_TR3[GRID_ROW_SIZE];
grid_t GRID_TABLE_TR4[GRID_ROW_SIZE];


void grid_init_left()
{
  for (grid_t row = 0; row < GRID_ROW_SIZE; ++row)
    {
      grid_t data[4] =
        {
          (row >> 0) & 0xF,
          (row >> 4) & 0xF,
          (row >> 8) & 0xF,
          (row >> 12) & 0xF,
        };

      GRID_TABLE_SCORE_LEFT[row] = 0;

      unsigned lim = 0;
      unsigned til = 0;
      for (unsigned j = 0; j < 4; ++j)
        {
          grid_t val = data[j];
          if (!val)
            continue;
          data[j] = 0;

          if (til != lim
              && val == data[til - 1])
            {
              lim = til;
              data[til - 1] = val + 1;
              GRID_TABLE_SCORE_LEFT[row] += GRID_POWS[val + 1];
            }

          else
            {
              data[til] = val;
              ++til;
              lim = til - 1;
            }
        }

      GRID_TABLE_LEFT[row] = (data[0] << 0)
        | (data[1] << 4)
        | (data[2] << 8)
        | (data[3] << 12);
    }
}

void grid_init_right()
{
  for (grid_t row = 0; row < GRID_ROW_SIZE; ++row)
    {
      grid_t data[4] =
        {
          (row >> 0) & 0xF,
          (row >> 4) & 0xF,
          (row >> 8) & 0xF,
          (row >> 12) & 0xF,
        };

      GRID_TABLE_SCORE_RIGHT[row] = 0;

      unsigned lim = 3;
      unsigned til = 3;
      for (unsigned j = 3; j < 4; --j)
        {
          grid_t val = data[j];
          if (!val)
            continue;
          data[j] = 0;

          if (til != lim
              && val == data[til + 1])
            {
              lim = til;
              data[til + 1] = val + 1;
              GRID_TABLE_SCORE_RIGHT[row] += GRID_POWS[val + 1];
            }

          else
            {
              data[til] = val;
              --til;
              lim = til + 1;
            }
        }

      GRID_TABLE_RIGHT[row] = (data[0] << 0)
        | (data[1] << 4)
        | (data[2] << 8)
        | (data[3] << 12);
    }
}

void grid_init_transpose()
{
  for (grid_t row = 0; row < GRID_ROW_SIZE; ++row)
    {
      grid_t data[4] =
        {
          (row >> 0) & 0xF,
          (row >> 4) & 0xF,
          (row >> 8) & 0xF,
          (row >> 12) & 0xF,
        };

      GRID_TABLE_TR1[row] = 0
        | (data[0] << 0)
        | (data[1] << 16)
        | (data[2] << 32)
        | (data[3] << 48);

      GRID_TABLE_TR2[row] = 0
        | (data[0] << 4)
        | (data[1] << 20)
        | (data[2] << 36)
        | (data[3] << 52);

      GRID_TABLE_TR3[row] = 0
        | (data[0] << 8)
        | (data[1] << 24)
        | (data[2] << 40)
        | (data[3] << 56);

      GRID_TABLE_TR4[row] = 0
        | (data[0] << 12)
        | (data[1] << 28)
        | (data[2] << 44)
        | (data[3] << 60);

    }
}



void grid_init()
{
  grid_init_left();
  grid_init_right();
  grid_init_transpose();
}
