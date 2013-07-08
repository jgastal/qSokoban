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
	QList<TileType> tiles;
	for (int i = 0, x = 0; i < data.size(); ++i, ++x)
	{
		switch (data.at(i))
		{
			case '*':
				tiles.append(BOX_DESTINATION);
				boxes_.append(new Box(x, height_, this));
				break;
			case '#':
				tiles.append(WALL);
				break;
			case '$':
				tiles.append(FLOOR);
				boxes_.append(new Box(x, height_, this));
				break;
			case ' ':
				tiles.append(FLOOR);
				break;
			case '@':
				tiles.append(FLOOR);
				manPos_.rx() = x;
				manPos_.ry() = height_;
				break;
			case '\n':
				++height_;
				width_ = width_ >= x ? width_ : x;
				x = -1;
				tiles.append(NEW_ROW);
				break;
			case '.':
				tiles.append(BOX_DESTINATION);
				break;
			case '+':
				tiles.append(BOX_DESTINATION);
				manPos_.rx() = x;
				manPos_.ry() = height_;
				break;
			default:
				throw BadLevelDescription(data.at(i));
		}
	}

	QVector<TileType> line;
	line.fill(OUTSIDE, height_);
	tiles_.fill(line, width_);
	bool lineHasWall = false;
	for (int i = 0, x = 0, y = 0; i < tiles.size(); ++i, ++x)
	{
		switch (tiles.at(i))
		{
			case FLOOR:
				if (lineHasWall) //TODO doesn't account for OUTSIDE in middle of line
					tiles_[x][y] = FLOOR;
				break;
			case WALL:
				tiles_[x][y] = WALL;
				lineHasWall = true;
				break;
			case BOX_DESTINATION:
				tiles_[x][y] = BOX_DESTINATION;
				break;
			case NEW_ROW:
				x = -1;
				++y;
				lineHasWall = false;
				break;
			case OUTSIDE:
				qDebug() << "Will never happen";
				break;
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
		if (b->x() == p.x() && b->y() == p.y())
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
