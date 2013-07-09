#include "board.h"
#include "box.h"
#include "badleveldescription.h"
#include <QDebug>

Board::Board(QObject *parent) : QAbstractListModel(parent), width_(0), height_(0)
{
}

Board::Board(QByteArray data, QObject *parent)
	: QAbstractListModel(parent), width_(0), height_(0)
{
	QVector<TileType> line;
	bool lineHasWall = false;

	height_ = data.count('\n');
	for (int idx = 0, last_idx = 0; (idx = data.indexOf("\n", idx + 1)) != -1; last_idx = idx)
		width_ = width_ > idx - last_idx ? width_ : idx - last_idx;


	line.fill(OUTSIDE, height_);
	tiles_.fill(line, width_);
	for (int i = 0, x = 0, y = 0; i < data.size(); ++i, ++x)
	{
		switch (data.at(i))
		{
			case '*':
				tiles_[x][y] = BOX_DESTINATION;
				boxes_.append(new Box(x, y, this));
				break;
			case '#':
				tiles_[x][y] = WALL;
				lineHasWall = true;
				break;
			case '$':
				tiles_[x][y] = FLOOR;
				boxes_.append(new Box(x, y, this));
				break;
			case ' ':
				if (lineHasWall) //TODO doesn't account for OUTSIDE in middle of line
					tiles_[x][y] = FLOOR;
				break;
			case '@':
				tiles_[x][y] = FLOOR;
				manPos_.rx() = x;
				manPos_.ry() = y;
				break;
			case '\n':
				++y;
				x = -1;
				lineHasWall = false;
				break;
			case '.':
				tiles_[x][y] = BOX_DESTINATION;
				break;
			case '+':
				tiles_[x][y] = BOX_DESTINATION;
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
	return tiles_[p.x()][p.y()];
}

Box *Board::boxAt(QPoint p)
{
	for (int i = 0; Box *b = qobject_cast<Box*>(boxes_.value(i)); i++)
		if (b->pos() == p)
			return b;
	return NULL;
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

int Board::rowCount(const QModelIndex &parent) const
{
	(void)parent;
	// Fucking grid view expects a list not a 2d array
	return width_ * height_;
}

QVariant Board::data(const QModelIndex &index, int role) const
{
	// Fucking grid view expects a list not a 2d array
	int x, y;
	x = index.row() % width_;
	y = index.row() / width_;
	if (role != Qt::DisplayRole)
		return QVariant();
	return tiles_[x][y];
}
