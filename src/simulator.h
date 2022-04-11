#ifndef __SIMULATION_H
#define __SIMULATION_H

#include "strategy.h"

/* Represents the results of a simulation */
typedef struct {
    unsigned int wins_X;
    unsigned int wins_O;
    unsigned int ties;

    unsigned int num_moves_X;
    unsigned int num_moves_O;
} simulation_results_t;


/* Represents a simulation */
typedef struct {
    /* Parameters to the simulation */
    unsigned int num_games;
    strategy_func stratX;
    strategy_func stratO;

    /* Simulation results */
    simulation_results_t results;
} simulation_t;


/* Run a simulation according to the parameters specified in parameter 'sim' */
int simulate(simulation_t *sim);

#endif /* __SIMULATION_H */

