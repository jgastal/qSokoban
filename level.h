#ifndef LEVEL_H
#define LEVEL_H

#include <QList>
#include <QPoint>
#include <QByteArray>
#include <QAbstractItemModel>
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

class Level : public QAbstractItemModel
{
	Q_OBJECT
	Q_PROPERTY(QPoint manPos READ manPos WRITE setManPos NOTIFY manMoved)
	Q_PROPERTY(QVariantList boxes READ boxes NOTIFY boxMoved)
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
		int width() const;
		int height() const;
		QPoint manPos() const;
		void setManPos(QPoint p);
		QVariantList boxes() const;

		//Item model methods
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;
		QHash<int, QByteArray> roleNames() const;
		QModelIndex index(int row, int column, const QModelIndex &parent) const;
		QModelIndex parent(const QModelIndex &child) const;

	signals:
		void manMoved(QPoint newPos);
		void boxMoved(QList<QPoint> newBoxesPos);
		void levelCompleted();

	private:
		QVector<QVector<Tile>> board_;
		QPoint manPos_;
		QList<QPoint> boxesPos_;
		int width_, height_;
		static const int tileImageRole;
};

#endif // LEVEL_H
