#ifndef __STRATEGIES_H
#define __STRATEGIES_H

#include "game.h"

/* Returns the function pointer for the given strategy.
   Returns NULL if no such strategy exists */
strategy_func get_strategy(char *name);



#endif /* __STRATEGIES_H */


