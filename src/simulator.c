#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

#include "simulator.h"
#include "game.h"


int simulate(simulation_t *sim)
{
    /* Initialize random seed */
    srand(time(NULL)); 

    /* Play the games and tally up the winners and ties */
    game_t g;
    sim->results.wins_X = 0;
    sim->results.wins_O = 0;
    sim->results.ties = 0;

    sim->results.num_moves_X = 0;
    sim->results.num_moves_O = 0;

    for(unsigned int i=0; i < sim->num_games; i++)
    {
        player_t winner;

        winner = game_play(&g, sim->stratX, sim->stratO);

        if(winner == X)
        {
            sim->results.wins_X++;
            sim->results.num_moves_X += g.num_moves;
        }
        else if(winner == O)
        {
            sim->results.wins_O++;
            sim->results.num_moves_O += g.num_moves;
        }
        else if(winner == NONE)
        {
            sim->results.ties++;
        }
    }

    assert(sim->results.wins_X + 
           sim->results.wins_O + 
           sim->results.ties   == sim->num_games);

    return 0;
}
