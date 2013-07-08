#ifndef BOX_H
#define BOX_H

#include <QObject>
#include <QPoint>

class Box : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QPoint pos READ pos NOTIFY moved)
	public:
		explicit Box(int x, int y, QObject *parent = 0);
		QPoint pos() const;
		void addToX(int dx);
		void addToY(int dy);

	signals:
		void moved();

	private:
		QPoint pos_;
};

Q_DECLARE_METATYPE(QList<Box*>);

#endif // BOX_H
