# Kalah
CS32 Project 3 Winter 2022 with Prof. David Smallberg

## Description
Using a minimax algorithm to create a computer player capable of defeating humans in Kalah.

## Rules of Kalah (qtd. from Smallberg)
<p align="center">
  <img width="682" alt="image" src="https://github.com/SPerrott22/Intelligent-Mankalah/assets/56176775/e625ee93-d457-429d-bc18-df5b29916790">
</p>

Each of the two players, North and South, has six holes on their side of the board and their own pot (their "kalah"). Each hole start off with four beans in it. Players take turns making moves. A move begins with a player picking up all of the beans in one of their six holes. Then, proceeding counterclockwise, the player puts one bean in each hole and their own pot (skipping over their opponent's pot if the "sowing" gets that far), until all of the beans that were picked up have been sown. For example, if South had 4 beans in her #5 hole, she might pick them up and deposit one each in her #6 hole, her pot, and North's #6 and #5 hole. Depending on where the last bean ends up, one of three things happens:
If the last bean was placed in the player's pot, the player must take another turn.

If the last bean was placed in one of the player's own holes that was empty just a moment before, and if the opponent's hole directly opposite from that hole is not empty, then that last bean and all beans in the opponent's hole directly opposite from that hole are put into the player's pot, and the turn ends. (This is a capture.) Notice that it is not a capture if the player's last bean ends up in what was an empty hole on the opponent's side.

In all other cases, the turn ends.

Whenever the player whose turn it is can not make a move (because their side is empty), any beans remaining in their opponent's holes are put into that opponent's pot and the game is over. The winner is the player whose pot has the most beans.

## How to Run
Edit the Main.cpp's main function to select whether each of the two players will be an intelligent bot using the minimax algorithm, a non-intelligent bot that makes moves randomly, or a human player (requires user console input). The bots will take no longer than 5 seconds to choose their moves.

_On the SEASnet Linux server:_

Compile/link the files into an executable using:
`g32 -o kalah *.cpp`

_If not on the server:_

Compile/link the files using your favorite compiler e.g.:
`g++ -o kalah *.cpp`

Run the program:
`./kalah`
