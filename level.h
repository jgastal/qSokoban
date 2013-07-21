#ifndef LEVEL_H
#define LEVEL_H

#include <QList>
#include <QPoint>
#include <QByteArray>
#include <QAbstractListModel>
#include <QStack>
#include "badleveldescription.h"
#include "movementstack.h"

class Box;
class Board;

class Level : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int steps READ steps NOTIFY steped);
	Q_PROPERTY(int pushes READ pushes NOTIFY pushed);
	Q_PROPERTY(Board* board READ board CONSTANT)
	Q_PROPERTY(MovementStack* undoStack READ undoStack CONSTANT)
	Q_ENUMS(Direction);

	public:
		enum Direction {
			Down,
			Up,
			Left,
			Right
		};
		explicit Level(QByteArray data, QObject *parent = 0);
		Board *board() const;
		int steps() const;
		int pushes() const;
		MovementStack *undoStack() const;

	public slots:
		void move(Direction d);

	signals:
		void manMoved(MovementStack::Movement mv);
		void levelCompleted();
		void sizeChanged(int width, int height);
		void steped();
		void pushed();

	private:
		Board *board_;
		int steps_, pushes_;
		MovementStack *undoStack_;
		friend void MovementStack::undo();
		friend void MovementStack::redo();
};

#endif // LEVEL_H
