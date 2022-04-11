# Tic-Tac-Toe Simulator

This program will allow you to simulate multiple games of tic-tac-toe where each player follows one of the following strategies:

* `random`: The player always picks a move at random.
* `block`: If the opposing player is one move away from winning, block the player from winning. Otherwise, pick a move at random.
* `smart-random` and `smart-block`: Same as the above, but making a smart opening move. In tic-tac-toe, it is generally advantageous to pick the center or corners if you're making the first move. If you're making the second move, and the center is open, it is generally better to take it. If it is not available, it is better to pick one of the corners.


## Building

To build the simulator, just run Make:

    make

This will generate a `tictactoe` executable.

## Running

The `tictactoe` program has two required options:

* `-X <strategy>`: Specifies the strategy of the X player. This is the player that makes the first move.
* `-O <strategy>` *strategy*: Specifies the strategy of the O player.

And an optional option:

* `-n <num_games>`: Specifies the number of games to run. Defaults to 1000 if not specified.

The program will run `num_games` games with the specified strategies, and will print a summary of the outcome of the games.

## Examples

When both players follow a `random` strategy, the player that makes the first move (X) usually has the advantage:

```
$ ./tictactoe -X random -O random -n 1000000
X wins: 44.27% (avg moves to win: 7.71)
O wins: 19.22% (avg moves to win: 7.41)
Ties: 36.50%
```

That advantage goes away if the O player uses a smarter strategy:

```
$ ./tictactoe -X random -O smart-block -n 1000000
X wins: 1.75% (avg moves to win: 8.63)
O wins: 40.51% (avg moves to win: 7.11)
Ties: 57.74%
```

The tables are turned if X follows a smarter strategy than O:

```
$ ./tictactoe -X smart-block -O random -n 1000000
X wins: 65.94% (avg moves to win: 7.51)
O wins: 1.57% (avg moves to win: 8.00)
Ties: 32.48%
```

And when both players follow a smart strategy, the game ends in a tie almost every time:

```
$ ./tictactoe -X smart-block -O smart-block -n 1000000
X wins: 0.38% (avg moves to win: 7.00)
O wins: 0.38% (avg moves to win: 8.00)
Ties: 99.25%
```

