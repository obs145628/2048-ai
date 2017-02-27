#pragma once

class Proba
{
public:
  using proba_t = double;
  static constexpr int MAX = 50;

  //If proba <= LIMIT node value is estimated
  static constexpr proba_t LIMIT = 0.005;

  static bool PROBAS_TABLES[MAX * MAX];

  /**
   * Inithialize the probabilility table
   */
  static void init_tables();

private:

  static long fact_(long n);

  /**
   * Compute the binomial coefficient kCn
   */
  static long bcoeff_(long k, long n);

  /**
   * Compute the probability to get k '4' among n
   */
  static proba_t proba_(long k, long n);
};
