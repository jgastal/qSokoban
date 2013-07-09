#ifndef LEVEL_H
#define LEVEL_H

#include <QList>
#include <QPoint>
#include <QByteArray>
#include <QAbstractListModel>
#include <QStack>
#include "badleveldescription.h"

class Box;
class Board;

class Level : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int steps READ steps NOTIFY steped);
	Q_PROPERTY(int pushes READ pushes NOTIFY pushed);
	Q_PROPERTY(Board* board READ board CONSTANT)
	Q_PROPERTY(bool canUndo READ canUndo NOTIFY undoStackChanged);
	Q_ENUMS(Direction);

	enum Movement {
		STEP_UP,
		STEP_DOWN,
		STEP_LEFT,
		STEP_RIGHT,
		PUSH_UP,
		PUSH_DOWN,
		PUSH_LEFT,
		PUSH_RIGHT
	};

	public:
		enum Direction {
			Down,
			Up,
			Left,
			Right
		};
		explicit Level(QByteArray data, QObject *parent = 0);
		Board *board() const;
		bool canUndo() const;
		int steps() const;
		int pushes() const;

	public slots:
		void move(Direction d);
		void undo();
		void reset();

	signals:
		void manMoved(QPoint newPos);
		void levelCompleted();
		void sizeChanged(int width, int height);
		void steped();
		void pushed();
		void undoStackChanged();

	private:
		Board *board_;
		int steps_, pushes_;
		QStack<Movement> undoStack_;
};

#endif // LEVEL_H
