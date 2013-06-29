#ifndef LEVEL_H
#define LEVEL_H

#include <QList>
#include <QPoint>
#include <QByteArray>
#include <stdexcept>
#include <string>

class BadLevelDescription : public std::exception
{
	public:
		explicit BadLevelDescription(const char bad_char) : bad_(bad_char) {}
		virtual const char* what() const throw();

	private:
		char bad_;
};

class Level
{
	public:
		enum Tile {
			WALL,
			FLOOR,
			BOX_DESTINATION,
			OUTSIDE, //Out of board
			NEW_ROW
		};
		Level(QByteArray data);
		QByteArray serialize() const;
		
	private:
		QList<Tile> board_;
		QPoint moverPos_;
		QList<QPoint> boxesPos_;
};

#endif // LEVEL_H
