#ifndef CLOCK_H
#define CLOCK_H

#include <inttypes.h>

struct Clock
{
  int32_t start;
};

void startClock(struct Clock* clock);
int elapsedClock(struct Clock* clock);
void resetClock(struct Clock* clock);

#endif // CLOCK_H
