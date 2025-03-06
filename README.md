# Algorithm minimax and alpha-beta pruning in a two-player game halma

Implementation of decision making algorithms minimax and alpha-beta pruning in halma game. Halma is a zero-sum two-player game where each player tries to move all their pawns from their starting base to their opponent's base. Each pawn can make two types of moves in any direction - steps, where a pawn moves to an adjacent field (as long as it is not occupied by another pawn), and jumps, where a pawn can 'jump' over a pawn located on an adjacent field, provided the target field of the jump is free. Jumps can only be made over one pawn at a time, no matter whether over its own or an opponent's pawn. A single move can include several jumps in succession, and they can change direction. Structures defining the state of the game, functions calculating the value of four types of heuristics and the algorithms themselves - minimax and alphabet - have been implemented. It was made possible for both players to play using one of eight strategies, making it possible to compare their effectiveness. 

## Game state structures. Algorithm implementation

Game state was implemented as a structure containing the current board state (pawn placement), lists with coordinates of pawns of each player (it's information redundancy, but allows to shorten the time of computations) and a list of children-states which are the next game states after each permitted move from the current state.

The most important method is the 

```
void generateChildren();
```

from `GameStructures.hpp`. It generates all children states by discovering all possible moves. The problem of finding all possible moves-jumps as a non-trivial one. The recursive solution was implemented first to test the algorithm on a smaller set of pawns, but due to extended computing time for full-size board it was changed to an iterative solution which utilized queues.

The minimax and alpha-beta were implemented to generate a decision tree until a win condition is encountered in a branch, or to a certain depth. As a result of the operation, the algorithms returned the next suggested state, that is, they indicated the move the player should make in the starting state. This made possible for two players to use different strategies for move evaluation - for each of them, the decision tree was generated to a `depth = 2` (one move for the maximizing and one for the minimizing player). The depth was chosen as a result of compromise between choosing the best move and computation time.

The performance of the algorithms was tested by comparing the transition from one state to another at three stages of the game - the initial stage, where the pawns are close to the starting bases, the middle stage, where the pawns are scattered all over the board, and the final stage, where the pawns are mostly at or near the final bases. In each test, alpha-beta suggested exactly the same move as minimax, but did so after visiting far fewer nodes and with shorter computation time. 

## Heuristics

Four different heuristics were used to evaluate the game state: the distance heuristic, the jump heuristic, the base heuristic and the opponent distance heuristic. Since the values they assigned to game states were of a similar order of magnitude, it was unnecessary to scale when combining the heuristics into a strategy. 

1. distance heuristic - the value of a given state was calculated as the sum of Manhattan distances from the initial base for all pawns - maximization resulted in favoring the pursuit of pawns towards the final base. When used as a major part of a strategy, this heuristic caused much scattering of pawns across the board. It was used as a base part of all strategies, as it rewarded the movement of pawns in the desired direction at all stages of the game.

1. jump heuristic - the value of a given state was calculated as the sum of Manhattan distances for all jumps possible to generate from the evaluated state in the diagonal direction opposite to the starting base.It rewarded the possibility of jumps on the grounds that they are a more favorable type of movement. In order to increase the speed of calculation, it was limited to single jumps, and eliminated other directions also roughly bringing the pawns closer to the final base. When it was present as part of the strategy, heuristics caused pawns to move in clustered groups, especially in the early stages of the game. Since the heuristics favored locking some pawns out of the final base at the final stage of the game (when they were all in the base it didn't leave many jumping opportunities), they were replaced by distance heuristics at this stage. 

1. base heuristic - this heuristic penalized pawns that were still in the starting base and rewarded those that were in the final base. The penalty and reward values were -30 and 30, respectively (30 being the largest Manhattan distance value possible for the 16x16 board used in the game). Interestingly, contrary to expectations, blocked gameplay (blocking of one or more pawns in the starting base by opponent's pawns that pursued it as the final base) occured only when this heuristic was used as part of the strategy.

1. opponent's distance heuristic - a heuristic identical in implementation to the distance heuristic allowed to evaluate the state not by calculating the distance of the player's playing pawns from the starting base, but their opponent's from the final base. These heuristics allowed the creation of “mean” strategies that focused not only on maximizing the benefits of one's moves, but also on making the game more difficult for the opponent by blocking their favorable moves.

## Strategies

A strategy was a set of four multipliers for the result of each of the heuristics, which summed together would make the final grade of the game state. That is, for strategy of `(m1,m2,m3,m4)` and heuristics `h1, h2, h3, h4` the final value of the grading function was:
```
h = m1 * h1 + m2 * h2 + m3 * h3 + m4 * h4
```
for the initial and middle game stage and
```
h = 2 * m1 * h1 + m3 * h3 + m4 * h4
```
for the ending stage.

Eight strategies were proposed and tested:

1. Purely distance strategy `(1,0,0,0)` - a strategy that only uses the distance heuristic, which forms the basis of each of the eight strategies. The simplest of the proposed strategies.
1. Jump strategy `(1,1.1,0,0)` - a strategy that, in addition to favoring long distance from the starting base, also favors jump capabilites. The value of 1.1 multiplier for the value of the jumping heuristics was chosen through experiments playing this strategy with different multiplier values against a pure distance strategy. For proposed multiplier value, the algorithm achieved victory for the smallest number of nodes visited. 
1. Base strategy `(1,0,1,0)` - a strategy that, in addition to distance heuristics, takes into account penalizing pawns remaining at the starting base and rewarding pawns that have reached the the final base. The value of the multiplier `m3` was determined similarly to strategy 2. 
1. Base + jump strategy `(1,1.1,1,0)` - a strategy combining strategies 2 and 3.
1. "Mean" strategy `(1,0,0,1)` - the first of the strategies, which was also based on minimizing the opponent's gain. In this form, hindering the opponent's moves had the the same importance as striving for the most advantageous of one's own moves. 
1. "Very mean" strategy `(1,0,0,2)` - a strategy analogous to the previous one, except that in this case, obstructing the opponent's moves had twice the weight of choosing the most advantageous of one's moves. 
1. "Mean" strategy with jumps `(1,1.1,0,1)` - combining strategies 2 and 5.
1. "Everything bagel" `(1,1.1,1,1)` - combining all strategies in one.

The effectiveness of each strategy was evaluated through a tournament, where opponents used different strategies so that each was pitted against each. For a given pair of strategies, two games were held, where players swapped places using a given strategy. The following rules were established for scoring - strategy A won against B when both games (where the player using a given strategy played first as the first, starting player, and then as the second player) were won by the player who used strategy A. Strategy A tied with strategy B when only one of the games was won by the player with strategy A. Strategy A lost to strategy B when both games were won by the player using strategy B.

In addition to the number of wins, draws and losses, the analysis also included the average number of nodes visited during the game, as well as the advantage the winning player had over the losing player at the end of the game. This advantage was calculated as the sum of the Manhattan distances of all opponent pawns remaining outside the final base from the edge of the board where the base was located. 

Results:

| strategy | wins | draws | losses | avg. number of visited nodes | avg. advantage if won | avg. advantage of opponent if lost |
| -------- | ---- | ----- | ------ | ---------------------------- | --------------------- | ---------------------------------- |
| s1       | 0    | 2     | 5      |    285853.36                 |     51.50             |     47.75                          |
| s2       | 1    | 3     | 3      |    251625.21                 |     14.00             |     46.44                          |
| s3       | 1    | 3     | 3      |    269826.00                 |     16.60             |     52.00                          |
| s4       | 0    | 4     | 3      |    231687.14                 |     12.25             |     58.20                          |
| s5       | 5    | 1     | 1      |    616972.62                 |     60.80             |     36.00                          |
| s6       | 4    | 3     | 0      |    883292.79                 |     60.09             |     27.00                          |
| s7       | 5    | 2     | 0      |    336569.07                 |     65.92             |     28.00                          |
| s8       | 1    | 4     | 2      |    325948.54                 |     36.00             |     42.14                          |



It can be seen that the “pure” “mean” strategies, despite winning a lot and achieving a significant advantage over the opponent (and the presence of a small advantage of the opponent in the case of losing), also resulted in visiting a much larger number of nodes during the algorithm, and thus longer computation time. This was probably caused by the fact that, as a result of blocking the opponent's move-jumps, the possibility of his jumps increased. 

As a result of the tournament, the following hierarchy of analyzed strategies emerged: 
1. “Mean” + jumps 
2. “Mean” 
3. “Very mean”
4. “Everything bagel” 
5. Ex aequo jumps and base 
6. Base + jumps 
7. Pure distance 