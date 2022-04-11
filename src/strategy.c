#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "strategy.h"
#include "game.h"


/*** HELPER FUNCTIONS ***/

/* Make a random move */
int do_random_move(game_t *g, player_t p)
{
    int num_empties = 9 - g->num_moves;

    /* Choose random position */
    int next_pos = rand() % num_empties;

    int empty_pos = 0;
    for(int i=0; i<9; i++)
    {
        int x = i / 3;
        int y = i % 3;

        if(g->board[x][y] == NONE && empty_pos == next_pos)
        {
            game_move(g, p, x, y);
            break;
        }
        else if (g->board[x][y] == NONE)
        {
            empty_pos++;
        }
    }

    return 0;
}

#define ROW (0)
#define COL (1)

#define BLOCKED (0)
#define NOT_BLOCKED (1)

/* Find out whether a blocking move is possible in a row or
   column, and make that blocking move (and return BLOCKED). 
   Otherwise, do nothing and return NOT_BLOCKED */
int do_blocking_move_rowcol(game_t *g, player_t p, int rowcol)
{
    assert(rowcol == ROW || rowcol == COL);

    for (int i = 0; i < 3; i++)
    {
        int num_other_player = 0;
        int num_empty = 0;
        int x_empty = -1, y_empty = -1;

        for (int j = 0; j < 3; j++)
        {
            int x, y;

            if (rowcol == ROW)
            {
                x = i;
                y = j;
            }
            else if (rowcol == COL)
            {
                x = j;
                y = i;
            }
                
            if (g->board[x][y] == g->last_player)
                num_other_player++;
            else if (g->board[x][y] == NONE)
            {
                num_empty++;
                x_empty = x;
                y_empty = y;
            }
        }

        if (num_other_player == 2 && num_empty == 1)
        {
            assert(x_empty != -1 && y_empty != -1);

            /* Block! */
            game_move(g, p, x_empty, y_empty);
            return BLOCKED;
        }
    }

    /* Did not block */
    return NOT_BLOCKED;
}

/* Find out whether a blocking move is possible and 
   make that blocking move (and return BLOCKED). 
   Otherwise, do nothing and return NOT_BLOCKED */
int do_blocking_move(game_t *g, player_t p)
{
    /* Can't do a blocking move before there's, at least,
       three moves in the game */
    if (g->num_moves < 3)
        return NOT_BLOCKED;

    /* Check rows */
    if (do_blocking_move_rowcol(g, p, ROW) == BLOCKED)
        return BLOCKED;

    /* Check columns */
    if (do_blocking_move_rowcol(g, p, COL) == BLOCKED)
        return BLOCKED;

    /* Check diagonals */
    if(g->board[0][0] == NONE &&
       g->board[1][1] == g->last_player &&
       g->board[2][2] == g->last_player)
    {
        game_move(g, p, 0, 0);
        return BLOCKED;
    }

    if(g->board[0][0] == g->last_player &&
       g->board[1][1] == g->last_player &&
       g->board[2][2] == NONE)
    {
        game_move(g, p, 2, 2);
        return BLOCKED;
    }

    if(g->board[0][2] == NONE &&
       g->board[1][1] == g->last_player &&
       g->board[2][0] == g->last_player)
    {
        game_move(g, p, 0, 2);
        return BLOCKED;
    }

    if(g->board[0][2] == g->last_player &&
       g->board[1][1] == g->last_player &&
       g->board[2][0] == NONE)
    {
        game_move(g, p, 2, 0);
        return BLOCKED;
    }

    if(g->board[1][1] == NONE &&
         ((g->board[0][0] == g->last_player && g->board[2][2] == g->last_player) ||
          (g->board[0][2] == g->last_player && g->board[2][0] == g->last_player)))
    {
        game_move(g, p, 1, 1);
        return BLOCKED;
    }

    return NOT_BLOCKED;
}


/* Make a smart opening */
int do_smart_opening(game_t *g, player_t p)
{
    /* Verify this is our opening move */
    assert(g->num_moves < 2);

    if(g->num_moves == 0)
    {
        /* Choose a corner or the center */
        int next_pos = rand() % 5;

        switch(next_pos)
        {
            case 0:
                game_move(g, p, 1, 1); break;
            case 1:
                game_move(g, p, 0, 0); break;
            case 2:
                game_move(g, p, 0, 2); break;
            case 3:
                game_move(g, p, 2, 0); break;
            case 4:
                game_move(g, p, 2, 2); break;
        }
    }
    else if (g->num_moves == 1)
    {
        /* Always take the center if it is available */
        if (g->board[1][1] == NONE)
            game_move(g, p, 1, 1);
        else
        {
            /* Otherwise, take a corner */
            int next_pos = rand() % 4;

            switch(next_pos)
            {
                case 0:
                    game_move(g, p, 0, 0); break;
                case 1:
                    game_move(g, p, 0, 2); break;
                case 2:
                    game_move(g, p, 2, 0); break;
                case 3:
                    game_move(g, p, 2, 2); break;
            }
        }
    }

    return 0;
}


/*** STRATEGY FUNCTIONS ***/

/* Random strategy: Always make a random move */
int strategy_random(game_t *g, player_t p)
{
    return do_random_move(g, p);
}

/* Smart random strategy: Make a smart opening and,
   after that, make random moves */
int strategy_smart_random(game_t *g, player_t p)
{
    if(g->num_moves < 2)
        return do_smart_opening(g, p);
    else
        return do_random_move(g, p);
}

/* Blocking strategy: Always block if possible;
   otherwise, make a random move */
int strategy_block(game_t *g, player_t p)
{
    if(do_blocking_move(g, p) == BLOCKED)
        return 0;
    else
        return do_random_move(g, p);
}

/* Smart blocking strategy: Make a smart opening and,
   after that, always block if possible (if not, make
   a random move */  
int strategy_smart_block(game_t *g, player_t p)
{
    if(g->num_moves < 2)
        return do_smart_opening(g, p);
    else if(do_blocking_move(g, p) == BLOCKED)
        return 0;
    else
        return do_random_move(g, p);
}


/*** ARRAY OF STRATEGIES ***/

/* Entry in the array */
struct strategy_entry
{
	char *name;
	strategy_func func;
};

/* Array of strategies */
struct strategy_entry strategies[] =
{
    { "random", strategy_random},
    { "smart-random", strategy_smart_random},
    { "block", strategy_block},
    { "smart-block", strategy_smart_block},
    { NULL, NULL }
};

/* See strategy.h */
strategy_func get_strategy(char *name)
{
    for(int i=0; strategies[i].name != NULL; i++)
        if (!strcmp(name, strategies[i].name))
        {
            return strategies[i].func;
        }

    return NULL;
}


