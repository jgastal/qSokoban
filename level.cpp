#include "level.h"
#include <QDebug>

Level::Level(QByteArray data)
{
	data.remove(0, 1); //Remove starting \n
	for (int i = 0, x = 0, y = 0; i < data.size(); ++i, ++x)
	{
		switch (data.at(i))
		{
			case '*':
				board_.append(BOX_DESTINATION);
				boxesPos_.append(QPoint(x, y));
			case '#':
				board_.append(WALL);
				break;
			case '$':
				board_.append(FLOOR);
				boxesPos_.append(QPoint(x, y));
				break;
			case ' ':
				board_.append(OUTSIDE);
				break;
			case '@':
				board_.append(FLOOR);
				moverPos_.rx() = x;
				moverPos_.ry() = y;
				break;
			case '\n':
				++y;
				x = 0;
				board_.append(NEW_ROW);
				break;
			case '.':
				board_.append(BOX_DESTINATION);
				break;
			case '+':
				board_.append(FLOOR);
				moverPos_.rx() = x;
				moverPos_.ry() = y;
				break;
			default:
				throw BadLevelDescription(data.at(i));
		}
	}
}

QByteArray Level::serialize() const
{
	QByteArray data;
	for (int i = 0, x = 0, y = 0; i < board_.size(); i++, x++)
	{
		switch (board_.at(i))
		{
			case OUTSIDE:
				data.append(' ');
				break;
			case WALL:
				data.append('#');
				break;
			case FLOOR:
				if (moverPos_.x() == x && moverPos_.y() == y)
					data.append('@');
				else if (boxesPos_.contains(QPoint(x, y)))
					data.append('$');
				else
					data.append(' ');
				break;
			case BOX_DESTINATION:
				if (moverPos_.x() == x && moverPos_.y() == y)
					data.append('+');
				else if (boxesPos_.contains(QPoint(x, y)))
					data.append('*');
				else
					data.append('.');
				break;
			case NEW_ROW:
				data.append('\n');
				++y;
				x = 0;
				break;
		}
	}
	return data;
}


const char *BadLevelDescription::what() const throw()
{
	char *what = (char*)malloc(sizeof(char) * 34);
	snprintf(what, 34, "Found unexepected character: '%c'", bad_);
	return what;
}
