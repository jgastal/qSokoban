#include "level.h"
#include <QDebug>
#include <QHash>
#include "box.h"

Level::Level() : width_(0), height_(0), steps_(0), pushes_(0)
{
}

Level::Level(QByteArray data) : width_(0), height_(0), steps_(0), pushes_(0)
{
	QList<TileType> tiles;
	for (int i = 0, x = 0; i < data.size(); ++i, ++x)
	{
		switch (data.at(i))
		{
			case '*':
				tiles.append(BOX_DESTINATION);
				boxesPos_.append(new Box(x, height_, this));
				break;
			case '#':
				tiles.append(WALL);
				break;
			case '$':
				tiles.append(FLOOR);
				boxesPos_.append(new Box(x, height_, this));
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
	board_.fill(line, width_);
	bool lineHasWall = false;
	for (int i = 0, x = 0, y = 0; i < tiles.size(); ++i, ++x)
	{
		switch (tiles.at(i))
		{
			case FLOOR:
				if (lineHasWall) //TODO doesn't account for OUTSIDE in middle of line
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
	emit sizeChanged(width_, height_);
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
					else if (boxAt(x, y))
						data.append('$');
					else
						data.append(' ');
					break;
				case BOX_DESTINATION:
					if (manPos_.x() == x && manPos_.y() == y)
						data.append('+');
					else if (boxAt(x, y))
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

QPoint Level::manPos() const
{
	return manPos_;
}

void Level::setManPos(QPoint p)
{
	Movement mv;
	Box *b;
	mv.box = NULL; //If it doesn't exist it must be NULL
	if (board_[p.x()][p.y()] == WALL)
		return;
	if ((b = boxAt(p.x(), p.y())))
	{
		int dx, dy;
		bool won = true;
		dx = p.x() - manPos_.x();
		dy = p.y() - manPos_.y();
		if (board_[p.x() + dx][p.y() + dy] == WALL) //Object can't go past wall
			return;
		if (boxAt(p.x() + dx, p.y() + dy)) //Objects can't go into each other
			return;
		b->addToX(dx);
		b->addToY(dy);
		mv.box = b;
		mv.boxdx = -1 * dx;
		mv.boxdy = -1 * dy;
		++pushes_;
		emit pushed();
		for (int i = 0; Box *b = qobject_cast<Box*>(boxesPos_.value(i)); i++)
			if (board_[b->x()][b->y()] != BOX_DESTINATION)
				won = false;
		if (won)
			emit levelCompleted();
	}
	mv.mandx = manPos_.x() - p.x();
	mv.mandy = manPos_.y() - p.y();
	undoStack_.push(mv);
	emit undoStackChanged();
	manPos_.rx() = p.x();
	manPos_.ry() = p.y();
	emit manMoved(manPos_);
	++steps_;
	emit steped();
}

QList<QObject*> Level::boxes() const
{
	return boxesPos_;
}

bool Level::canUndo() const
{
	return !undoStack_.isEmpty();
}

int Level::rowCount(__attribute__((unused)) const QModelIndex &parent) const
{
	// Fucking grid view expects a list not a 2d array
	return width_ * height_;
}

QVariant Level::data(const QModelIndex &index, int role) const
{
	// Fucking grid view expects a list not a 2d array
	int x, y;
	x = index.row() % width_;
	y = index.row() / width_;
	if (role != Qt::DisplayRole)
		return QVariant();
	return board_[x][y];
}

void Level::undo()
{
	Movement mv = undoStack_.pop();
	manPos_.rx() += mv.mandx;
	manPos_.ry() += mv.mandy;
	emit manMoved(manPos_);
	--steps_;
	emit steped();
	if (mv.box)
	{
		mv.box->addToX(mv.boxdx);
		mv.box->addToY(mv.boxdy);
		--pushes_;
		emit pushed();
	}
	emit undoStackChanged();
}

void Level::reset()
{
	while (!undoStack_.isEmpty())
		undo();
}

Box *Level::boxAt(int x, int y) const
{
	for (int i = 0; Box *b = qobject_cast<Box*>(boxesPos_.value(i)); i++)
		if (b->x() == x && b->y() == y)
			return b;
	return NULL;
}
