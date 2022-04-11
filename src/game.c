#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "game.h"

/* See game.h */
int game_init(game_t *g)
{
    /* Zero out memory */
    memset(g, 0, sizeof(game_t));

    return 0;
}

/* See game.h */
player_t game_winner(game_t *g)
{
    if(g->num_moves < 5)
    {
        /* There can't be a winner until there's been, at least, five moves */
        return NONE;
    }

    /* Check row */
    unsigned char x = g->last_move.x;
    for(unsigned char y=0; y < 3; y++)
    {
        /* If any position doesn't match the last player's, we don't
           have a winning row */
        if(g->board[x][y] != g->last_player)
            break;

        /* If we get to the end of the row and all positions are the
           same as the last player, the last player has won */
        if(y == 2)
            return g->last_player;
    }

    /* Check column */
    unsigned char y = g->last_move.y;
    for(unsigned char x=0; x < 3; y++)
    {
        /* If any position doesn't match the last player's, we don't
           have a winning column */
        if(g->board[x][y] != g->last_player)
            break;

        /* If we get to the end of the column and all positions are the
           same as the last player, the last player has won */
        if(x == 2)
            return g->last_player;
    }

    /* Check diagonals, if applicable */
    if(g->last_move.x == g->last_move.y || (g->last_move.x + g->last_move.y == 2))
    {
        /* Easier to check by rote than with loops */
        if(g->board[0][0] == g->last_player &&
           g->board[1][1] == g->last_player &&
           g->board[2][2] == g->last_player)
        {
            return g->last_player;
        }

        if(g->board[0][2] == g->last_player &&
           g->board[1][1] == g->last_player &&
           g->board[2][0] == g->last_player)
        {
            return g->last_player;
        }
    }


    /* If we get to the end of the function and haven't found a winner,
       there is no winner */
    return NONE;
}

/* See game.h */
void game_print(game_t *g)
{
    printf(" %c | %c | %c \n", PLAYER_CHAR(g->board[0][0]), PLAYER_CHAR(g->board[0][1]), PLAYER_CHAR(g->board[0][2]));
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", PLAYER_CHAR(g->board[1][0]), PLAYER_CHAR(g->board[1][1]), PLAYER_CHAR(g->board[1][2]));
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", PLAYER_CHAR(g->board[2][0]), PLAYER_CHAR(g->board[2][1]), PLAYER_CHAR(g->board[2][2]));
}

/* See game.h */
int game_move(game_t *g, player_t p, unsigned char x, unsigned char y)
{
    /* Check that provided values are valid */
    assert(p == X || p == O);
    assert(g->last_player != p);
    assert(g->board[x][y] == NONE);
    assert(g->num_moves < 9);

    /* Update the game */
    g->board[x][y] = p;
    g->num_moves++;
    g->last_move.x = x;
    g->last_move.y = y;
    g->last_player = p;

    return 0;
}

/* See game.h */
player_t game_play(game_t *g, strategy_func stratX, strategy_func stratO)
{
    game_init(g);

    player_t winner = NONE;
    player_t cur_p = X;

    while(winner == NONE && g->num_moves < 9)
    {
        if (cur_p == X)
            stratX(g, cur_p);
        else if (cur_p == O)
            stratO(g, cur_p);

        winner = game_winner(g);

        cur_p = (cur_p == X? O: X);
    }

    return winner;
}

