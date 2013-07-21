#include "movementstack.h"

#include "level.h"
#include "board.h"
#include "box.h"

MovementStack::MovementStack(Level *parent)
	: QObject(parent), lvl_(parent)
{
	connect(lvl_, &Level::manMoved, this, &MovementStack::moved);
}

bool MovementStack::canUndo() const
{
	return done_.size() != 0;
}

bool MovementStack::canRedo() const
{
	return undone_.size() != 0;
}

void MovementStack::undo()
{
	Movement mv = done_.pop();
	QPoint delta;
	Board *board = lvl_->board();
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

	board->setManPos(board->manPos() + delta);
	--lvl_->steps_;
	emit lvl_->steped();
	if (mv >= PUSH_UP)
	{
		Box *b = board->boxAt(board->manPos() + delta * -2);
		b->move(delta);
		--lvl_->pushes_;
		emit lvl_->pushed();
	}

	undone_.push(mv);
	emit stackChanged();
}

void MovementStack::redo()
{
	Movement mv = undone_.pop();
	QPoint delta;
	Board *board = lvl_->board();
	switch (mv)
	{
		case STEP_UP:
		case PUSH_UP:
			delta.ry() = -1;
			break;
		case STEP_DOWN:
		case PUSH_DOWN:
			delta.ry() = 1;
			break;
		case STEP_LEFT:
		case PUSH_LEFT:
			delta.rx() = -1;
			break;
		case STEP_RIGHT:
		case PUSH_RIGHT:
			delta.rx() = 1;
			break;
	}

	board->setManPos(board->manPos() + delta);
	++lvl_->steps_;
	emit lvl_->steped();
	if (mv >= PUSH_UP)
	{
		Box *b = board->boxAt(board->manPos());
		b->move(delta);
		++lvl_->pushes_;
		emit lvl_->pushed();
	}

	done_.push(mv);
	emit stackChanged();
}

void MovementStack::moved(Movement mv)
{
	done_.push(mv);
	undone_.clear();
	emit stackChanged();
}

void MovementStack::reset()
{
	while (done_.size())
		undo();
}
