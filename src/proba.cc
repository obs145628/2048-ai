#include "proba.hh"
#include <cassert>
#include <cmath>

bool Proba::PROBAS_TABLES[MAX * MAX];


void Proba::init_tables()
{
  for (long n = 1; n < MAX; ++n)
    for (long k = 0; k <= n; ++k)
      PROBAS_TABLES[k * MAX  + n] = proba_(k, n) <= LIMIT;
}

long Proba::fact_(long n)
{
  return n < 2 ? 1 : n * fact_(n - 1);
}


long Proba::bcoeff_(long k, long n)
{
  return fact_(n) / (fact_(k) * fact_(n - k));
}


Proba::proba_t Proba::proba_(long k, long n)
{
  assert(k <= n);
  return bcoeff_(k, n) * std::pow(0.9, n - k) * std::pow(0.1, k);
}
