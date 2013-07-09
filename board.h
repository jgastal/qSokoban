#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QPoint>
#include <QByteArray>

class Box;

class Board : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QPoint manPos MEMBER manPos_ NOTIFY manMoved)
	Q_PROPERTY(QList<QObject*> boxes READ boxes CONSTANT)
	Q_PROPERTY(int width READ width CONSTANT)
	Q_PROPERTY(int height READ height CONSTANT)
	Q_ENUMS(TileType)

	public:
		enum TileType {
			WALL,
			FLOOR,
			BOX_DESTINATION,
			OUTSIDE //Out of board
		};

		explicit Board(QObject *parent = 0);
		explicit Board(QByteArray data, QObject *parent = 0);
		QList<QObject*> boxes();
		int width();
		int height();
		TileType tileAt(QPoint p);
		Box *boxAt(QPoint p);
		//Helpers to avoid calling property and converting to from qvariant
		QPoint manPos() const;
		void setManPos(QPoint p);

		Q_INVOKABLE QList<int> tiles() const;

	signals:
		void sizeChanged();
		void manMoved();

	private:
		int width_, height_;
		QPoint manPos_;
		QList<QObject*> boxes_;
		QList<int> tiles_;
};

Q_DECLARE_METATYPE(Board*);

#endif // BOARD_H
