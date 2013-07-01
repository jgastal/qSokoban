qSokoban
========
sokoban like game

Build instructions
==================
Building requires Qt 5.1.0
Run:
qmake-qt5 && make

Goal
====
The goal of the game is to move all the red gems to the green marked spots.

TODO
====
 * Don't animate man movement when changing levels
 * Handle empty spaces in between walls as OUTSIDE
 * Handle reaching the end of the collection
 * Allow saving position of a given level
 * Implement Redo
 * Allow saving solutions.
 * Implement help(moves a box by one tile each time called) for levels for which the solution is known.
