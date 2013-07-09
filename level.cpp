#include "level.h"
#include <QDebug>
#include <QHash>
#include "box.h"
#include "board.h"

Level::Level() : steps_(0), pushes_(0)
{
}

Level::Level(QByteArray data) : steps_(0), pushes_(0)
{
	board_ = new Board(data, this);
}

Board *Level::board() const
{
	return board_;
}

bool Level::canUndo() const
{
	return !undoStack_.isEmpty();
}

int Level::steps() const
{
	return steps_;
}

int Level::pushes() const
{
	return pushes_;
}

void Level::move(Level::Direction d)
{
	Movement mv;
	bool push = false;
	Box *b;
	QPoint destination;
	QPoint delta;
	switch (d)
	{
		case Down:
			delta.ry() = 1;
			break;
		case Up:
			delta.ry() = -1;
			break;
		case Left:
			delta.rx() = -1;
			break;
		case Right:
			delta.rx() = 1;
			break;
	}
	destination = board_->manPos() + delta;

	if (board_->tileAt(destination) == Board::WALL)
		return;
	b = board_->boxAt(destination);
	if (b)
	{
		bool won = true;
		QPoint boxDestination = destination + delta;
		if (board_->tileAt(boxDestination) == Board::WALL) //Object can't go past wall
			return;
		if (board_->boxAt(boxDestination)) //Objects can't go into each other
			return;
		b->move(delta);
		push = true;
		++pushes_;
		emit pushed();
		for (int i = 0; Box *b = qobject_cast<Box*>(board_->boxes().value(i)); i++)
			if (board_->tileAt(b->pos()) != Board::BOX_DESTINATION)
				won = false;
		if (won)
			emit levelCompleted();
	}
	if (delta.x() < 0)
		mv = push ? PUSH_LEFT : STEP_LEFT;
	else if (delta.x() > 0)
		mv = push ? PUSH_RIGHT : STEP_RIGHT;
	else if (delta.y() > 0)
		mv = push ? PUSH_DOWN : STEP_DOWN;
	else if (delta.y() < 0)
		mv = push ? PUSH_UP : STEP_UP;
	undoStack_.push(mv);
	emit undoStackChanged();
	board_->setManPos(destination);
	++steps_;
	emit steped();
}

void Level::undo()
{
	Movement mv = undoStack_.pop();
	QPoint delta;
	switch (mv)
	{
		case STEP_UP:
		case PUSH_UP:
			delta.ry() = 1;
			break;
		case STEP_DOWN:
		case PUSH_DOWN:
			delta.ry() = -1;
			break;
		case STEP_LEFT:
		case PUSH_LEFT:
			delta.rx() = 1;
			break;
		case STEP_RIGHT:
		case PUSH_RIGHT:
			delta.rx() = -1;
			break;
	}

	board_->setManPos(board_->manPos() + delta);
	--steps_;
	emit steped();
	if (mv < PUSH_UP)
	{
		emit undoStackChanged();
		return;
	}

	Box *b = board_->boxAt(board_->manPos() + delta * -2);
	b->move(delta);
	--pushes_;
	emit pushed();
	emit undoStackChanged();
}

void Level::reset()
{
	while (!undoStack_.isEmpty())
		undo();
}
