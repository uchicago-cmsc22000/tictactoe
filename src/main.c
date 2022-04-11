#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

#include "simulator.h"
#include "strategy.h"


int main(int argc, char *argv[])
{
    int opt;
    int num_games_opt = 1000;
    char *strategyX = NULL, *strategyO = NULL;

    while ((opt = getopt(argc, argv, "X:O:n:vh")) != -1)
        switch (opt)
        {
        case 'X':
            strategyX = strdup(optarg);
            break;
        case 'O':
            strategyO = strdup(optarg);
            break;
        case 'n':
            num_games_opt = strtol(optarg, NULL, 10);
            break;
        case 'h':
            printf("Usage: tictactoe -X STRATEGY -O STRATEGY [-n NUM_GAMES]\n");
            exit(0);
            break;
        default:
            printf("ERROR: Unknown option -%c\n", opt);
            exit(-1);
        }


    /* Validate parameters and add simulation parameters to simulation_t struct */

    simulation_t sim;

    if(strategyX == NULL || strategyO == NULL)
    {
        printf("You must specify strategies with the -X and -O options\n");
        exit(-1);
    }

    sim.stratX = get_strategy(strategyX);
    if(sim.stratX == NULL)
    {
        printf("Unknown strategy: %s\n", strategyX);
        exit(-1);
    }

    sim.stratO = get_strategy(strategyO);
    if(sim.stratO == NULL)
    {
        printf("Unknown strategy: %s\n", strategyO);
        exit(-1);
    }

    if(num_games_opt <= 0)
    {
        printf("ERROR: Invalid number of games %s\n", optarg);
        exit(-1);
    }
    sim.num_games = (unsigned int) num_games_opt;

    /* Run simulation */
    simulate(&sim);

    /* Print results */
    float pct_X, pct_O, pct_ties, avg_mov_X, avg_mov_O;

    pct_X = (sim.results.wins_X * 100.0) / sim.num_games;
    pct_O = (sim.results.wins_O * 100.0) / sim.num_games;
    pct_ties = (sim.results.ties * 100.0) / sim.num_games;

    avg_mov_X = sim.results.num_moves_X / (float) sim.results.wins_X;
    avg_mov_O = sim.results.num_moves_O / (float) sim.results.wins_O;

    printf("X wins: %.2f%% (avg moves to win: %.2f)\n", pct_X, avg_mov_X);
    printf("O wins: %.2f%% (avg moves to win: %.2f)\n", pct_O, avg_mov_O);
    printf("Ties: %.2f%%\n", pct_ties);
}
