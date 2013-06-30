#include "level.h"
#include <QDebug>
#include <QHash>

#define UNUSED __attribute__((unused))

const char *BadLevelDescription::what() const throw()
{
	char *what = (char*)malloc(sizeof(char) * 34);
	snprintf(what, 34, "Found unexepected character: '%c'", bad_);
	return what;
}

const int Level::tileImageRole = Qt::UserRole + 1;
const int Level::tileObjectImageRole = Qt::UserRole + 2;

Level::Level(QByteArray data) : width_(0), height_(0)
{
	QList<Tile> tiles;
	for (int i = 0, x = 0; i < data.size(); ++i, ++x)
	{
		switch (data.at(i))
		{
			case '*':
				tiles.append(BOX_DESTINATION);
				boxesPos_.append(QPoint(x, height_));
				break;
			case '#':
				tiles.append(WALL);
				break;
			case '$':
				tiles.append(FLOOR);
				boxesPos_.append(QPoint(x, height_));
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
				tiles.append(FLOOR);
				manPos_.rx() = x;
				manPos_.ry() = height_;
				break;
			default:
				throw BadLevelDescription(data.at(i));
		}
	}

	QVector<Tile> line;
	line.fill(OUTSIDE, height_);
	board_.fill(line, width_);
	bool lineHasWall = false;
	for (int i = 0, x = 0, y = 0; i < tiles.size(); ++i, ++x)
	{
		switch (tiles.at(i))
		{
			case FLOOR:
				if (lineHasWall)
					board_[x][y] = FLOOR;
				break;
			case WALL:
				board_[x][y] = WALL;
				lineHasWall = true;
				break;
			case BOX_DESTINATION:
				board_[x][y] = BOX_DESTINATION;
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

QByteArray Level::serialize() const
{
	QByteArray data;
	for (int y = 0; y < height_; ++y)
	{
		for (int x = 0; x < width_; ++x)
		{
			switch (board_[x][y])
			{
				case OUTSIDE:
					data.append(' ');
					break;
				case WALL:
					data.append('#');
					break;
				case FLOOR:
					if (manPos_.x() == x && manPos_.y() == y)
						data.append('@');
					else if (boxesPos_.contains(QPoint(x, y)))
						data.append('$');
					else
						data.append(' ');
					break;
				case BOX_DESTINATION:
					if (manPos_.x() == x && manPos_.y() == y)
						data.append('+');
					else if (boxesPos_.contains(QPoint(x, y)))
						data.append('*');
					else
						data.append('.');
					break;
				case NEW_ROW:
					qDebug() << "Will never happen";
					break;
			}
		}
		data.append("\n", 1);
	}
	return data;
}

int Level::width() const
{
	return width_;
}

int Level::height() const
{
	return height_;
}

QPoint Level::manPos() const
{
	return manPos_;
}

void Level::setManPos(QPoint p)
{
	if (board_[p.x()][p.y()] == WALL)
		return;
	manPos_.rx() = p.x();
	manPos_.ry() = p.y();
	//TODO move objects
	emit manMoved(manPos_);
}

int Level::rowCount(UNUSED const QModelIndex &parent) const
{
	// Fucking grid view expects a list not a 2d array
	return width_ * height_;
}

int Level::columnCount(UNUSED const QModelIndex &parent) const
{
	// Fucking grid view expects a list not a 2d array
	return 1;
}

QVariant Level::data(const QModelIndex &index, int role) const
{
	// Fucking grid view expects a list not a 2d array
	int x, y;
	x = index.row() % width_;
	y = index.row() / width_;
	switch(role)
	{
		case tileImageRole:
			switch(board_[x][y])
			{
				case WALL:
					return QVariant("qrc:/images/wall.png");
				case FLOOR:
					return QVariant("qrc:/images/floor.png");
				case BOX_DESTINATION:
					return QVariant("qrc:/images/box_destination.png");
				case OUTSIDE:
					return QVariant("qrc:/images/outside.png");
				case NEW_ROW:
					return QVariant();
			}
		case tileObjectImageRole:
			if (boxesPos_.contains(QPoint(x, y)))
				return QVariant("qrc:/images/object.png");
		default:
			return QVariant();
	}
}

QHash<int, QByteArray> Level::roleNames() const
{
	QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
	roles.insert(tileImageRole, QByteArray("tileImage"));
	roles.insert(tileObjectImageRole, QByteArray("tileObjectImage"));
	return roles;
}

QModelIndex Level::index(int row, int column, UNUSED const QModelIndex &parent) const
{
	return createIndex(row, column);
}

QModelIndex Level::parent(UNUSED const QModelIndex &child) const
{
	return QModelIndex();
}
