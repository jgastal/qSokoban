#include "board.h"
#include "box.h"
#include "badleveldescription.h"

Board::Board(QByteArray data, QObject *parent)
	: QObject(parent), width_(0), height_(0)
{
	bool lineHasWall = false;

	height_ = data.count('\n');
	for (int idx = 0, last_idx = 0; (idx = data.indexOf("\n", idx + 1)) != -1; last_idx = idx)
		width_ = width_ > idx - last_idx ? width_ : idx - last_idx;

	for (int i = 0, x = 0, y = 0; i < data.size(); ++i, ++x)
	{
		switch (data.at(i))
		{
			case '*':
				tiles_.append(BOX_DESTINATION);
				boxes_.append(new Box(x, y, this));
				break;
			case '#':
				tiles_.append(WALL);
				lineHasWall = true;
				break;
			case '$':
				tiles_.append(FLOOR);
				boxes_.append(new Box(x, y, this));
				break;
			case ' ':
				if (lineHasWall) //TODO doesn't account for OUTSIDE in middle of line
					tiles_.append(FLOOR);
				else
					tiles_.append(OUTSIDE);
				break;
			case '@':
				tiles_.append(FLOOR);
				manPos_.rx() = x;
				manPos_.ry() = y;
				break;
			case '\n':
				for (; x < width_; x++)
					tiles_.append(OUTSIDE);
				++y;
				x = -1;
				lineHasWall = false;
				break;
			case '.':
				tiles_.append(BOX_DESTINATION);
				break;
			case '+':
				tiles_.append(BOX_DESTINATION);
				manPos_.rx() = x;
				manPos_.ry() = y;
				break;
			default:
				throw BadLevelDescription(data.at(i));
		}
	}
}

QList<QObject *> Board::boxes()
{
	return boxes_;
}

int Board::width()
{
	return width_;
}

int Board::height()
{
	return height_;
}

Board::TileType Board::tileAt(QPoint p)
{
	return (TileType)tiles_.at(p.y() * width_ + p.x());
}

Box *Board::boxAt(QPoint p)
{
	for (int i = 0; Box *b = qobject_cast<Box*>(boxes_.value(i)); i++)
		if (b->pos() == p)
			return b;
	return NULL;
}

QList<int> Board::tiles() const
{
	return tiles_;
}

QPoint Board::manPos() const
{
	return manPos_;
}

void Board::setManPos(QPoint p)
{
	manPos_ = p;
	emit manMoved();
}
