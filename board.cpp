#include "board.h"
#include "box.h"
#include "badleveldescription.h"
#include <QList>

Board::Board(QByteArray data, QObject *parent)
	: QObject(parent), width_(0), height_(0)
{
	height_ = data.count('\n') + 1;
	for (int idx = 0, last_idx = 0; (idx = data.indexOf("\n", idx + 1)) != -1; last_idx = idx)
		width_ = width_ > idx - last_idx - 1 ? width_ : idx - last_idx - 1;

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
				break;
			case '$':
				tiles_.append(OUTSIDE);
				boxes_.append(new Box(x, y, this));
				break;
			case ' ':
				tiles_.append(OUTSIDE);
				break;
			case '@':
				tiles_.append(OUTSIDE);
				manPos_.rx() = x;
				manPos_.ry() = y;
				break;
			case '\n':
				for (; x < width_; x++)
					tiles_.append(OUTSIDE);
				++y;
				x = -1;
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
	for (int i = tiles_.size(); i < width_ * height_; i++)
		tiles_.append(OUTSIDE);

	//flood fill from man with FLOOR
	QList<QPoint> nodes;
	nodes << manPos_;
	for (int i = 0; i < nodes.size(); ++i)
	{
		QPoint p = nodes.at(i);
		if (tileAt(p) == FLOOR)
			continue;
		for (int j = 0; tileAt(p + QPoint(j, 0)) != WALL; --j) //Check all tiles to the left that until a wall is hit
		{
			QPoint below = p + QPoint(j, 1);
			QPoint above = p + QPoint(j, -1);
			if (tileAt(p + QPoint(j, 0)) == OUTSIDE)
				tiles_[p.y() * width_ + (p.x() + j)] = FLOOR;
			if (!nodes.contains(below) && (tileAt(below) == OUTSIDE || tileAt(below) == BOX_DESTINATION))
				nodes.push_back(below);
			if (!nodes.contains(above) && (tileAt(above) == OUTSIDE || tileAt(above) == BOX_DESTINATION))
				nodes.push_back(above);
		}
		for (int j = 1; tileAt(p + QPoint(j, 0)) != WALL; ++j) //Check all tiles to the right that until a wall is hit
		{
			QPoint below = p + QPoint(j, 1);
			QPoint above = p + QPoint(j, -1);
			if (tileAt(p + QPoint(j, 0)) == OUTSIDE)
				tiles_[p.y() * width_ + (p.x() + j)] = FLOOR;
			if (!nodes.contains(below) && (tileAt(below) == OUTSIDE || tileAt(below) == BOX_DESTINATION))
				nodes.push_back(below);
			if (!nodes.contains(above) && (tileAt(above) == OUTSIDE || tileAt(above) == BOX_DESTINATION))
				nodes.push_back(above);
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
