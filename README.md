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
 * Set level to initial state when changing levels
 * Implement functionality of restart button
 * Put proper icons in buttons
 * Implement choosing level collection
 * Don't animate man movement when changing levels
 * Implement undo
 * Handle empty spaces in between walls as OUTSIDE
 * Notify user he has won when level is completed
 * Handle reaching the end of the collection
 * Save last level solved in each collection
 * Allow saving position of a given level
 * Figure out way to animate movement of boxes
 * Consider using Repeater instead of GridView to simplify Level's code
