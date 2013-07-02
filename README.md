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
 * Put proper icons in buttons
 * Don't animate man movement when changing levels
 * Handle empty spaces in between walls as OUTSIDE
 * Handle reaching the end of the collection
 * Allow saving position of a given level
 * Figure out way to animate movement of boxes
 * Consider using Repeater instead of GridView to simplify Level's code
 * Save window position and size
 * Implement Redo
