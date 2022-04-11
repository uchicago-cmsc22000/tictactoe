#ifndef __GAME_H
#define __GAME_H

/* Represents a player */
typedef enum {
    NONE = 0,
    X = 1,
    O = 2
} player_t;

/* Represents a game of tic-tac-toe */
typedef struct {
    /* The tic-tac-toe board */
    player_t board[3][3];

    /* Number of moves played so far*/
    unsigned char num_moves;

    /* Who made the latest move? */
    player_t last_player;

    /* Where was the latest move? */
    struct {
        unsigned char x;
        unsigned char y;
    } last_move;
} game_t;

/* Function pointer for strategies */
typedef int (*strategy_func)(game_t *g, player_t p);

/* Produce a char representation of a player_t */
#define PLAYER_CHAR(p) (p==NONE?' ':(p==X?'X':(p==O?'O':'?')))

/* Initialize a game */
int game_init(game_t *g);

/* Determine the winner of a game */
player_t game_winner(game_t *g);

/* Print the state of a game */
void game_print(game_t *g);

/* Make a move in the game */
int game_move(game_t *g, player_t p, unsigned char x, unsigned char y);

/* Play a full game, using the provided strategies for X and O */
player_t game_play(game_t *g, strategy_func stratX, strategy_func stratO);

#endif /* __GAME_H */

