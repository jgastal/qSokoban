#ifndef LEVEL_H
#define LEVEL_H

#include <QList>
#include <QPoint>
#include <QByteArray>
#include <QAbstractItemModel>
#include <QStack>
#include "badleveldescription.h"

struct Movement {
	int mandx, mandy;
	int boxdx, boxdy;
	QPoint box;
};

class Level : public QAbstractItemModel
{
	Q_OBJECT
	Q_PROPERTY(QPoint manPos READ manPos WRITE setManPos NOTIFY manMoved)
	Q_PROPERTY(QVariantList boxes READ boxes NOTIFY boxMoved)
	Q_PROPERTY(int width MEMBER width_ NOTIFY sizeChanged)
	Q_PROPERTY(int height MEMBER height_ NOTIFY sizeChanged)
	Q_PROPERTY(int steps MEMBER steps_ NOTIFY steped);
	Q_PROPERTY(int pushes MEMBER pushes_ NOTIFY pushed);
	Q_PROPERTY(bool canUndo READ canUndo NOTIFY undoStackChanged);

	public:
		enum TileType {
			WALL,
			FLOOR,
			BOX_DESTINATION,
			OUTSIDE, //Out of board
			NEW_ROW
		};
		Level();
		Level(QByteArray data);
		QByteArray serialize() const;
		QPoint manPos() const;
		void setManPos(QPoint p);
		QVariantList boxes() const;
		bool canUndo() const;

		//Item model methods
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;
		QHash<int, QByteArray> roleNames() const;
		QModelIndex index(int row, int column, const QModelIndex &parent) const;
		QModelIndex parent(const QModelIndex &child) const;

	public slots:
		void undo();
		void reset();

	signals:
		void manMoved(QPoint newPos);
		void boxMoved(QList<QPoint> newBoxesPos);
		void levelCompleted();
		void sizeChanged(int width, int height);
		void steped();
		void pushed();
		void undoStackChanged();

	private:
		QVector<QVector<TileType>> board_;
		QPoint manPos_;
		QList<QPoint> boxesPos_;
		int width_, height_;
		int steps_, pushes_;
		static const int tileImageRole;
		QStack<Movement> undoStack_;
};

#endif // LEVEL_H
