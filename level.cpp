#include "level.h"
#include <QDebug>
#include <QHash>
#include "box.h"
#include "board.h"

Level::Level(QByteArray data, QObject *parent)
	: QObject(parent), steps_(0), pushes_(0), undoStack_(new MovementStack(this))
{
	board_ = new Board(data, this);
}

Board *Level::board() const
{
	return board_;
}

int Level::steps() const
{
	return steps_;
}

int Level::pushes() const
{
	return pushes_;
}

MovementStack *Level::undoStack() const
{
	return undoStack_;
}

void Level::move(Level::Direction d)
{
	MovementStack::Movement mv;
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
		mv = push ? MovementStack::PUSH_LEFT : MovementStack::STEP_LEFT;
	else if (delta.x() > 0)
		mv = push ? MovementStack::PUSH_RIGHT : MovementStack::STEP_RIGHT;
	else if (delta.y() > 0)
		mv = push ? MovementStack::PUSH_DOWN : MovementStack::STEP_DOWN;
	else// if (delta.y() < 0)
		mv = push ? MovementStack::PUSH_UP : MovementStack::STEP_UP;
	emit manMoved(mv);
	board_->setManPos(destination);
	++steps_;
	emit steped();
}
