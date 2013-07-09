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
		void move(QPoint delta);

	signals:
		void moved();

	private:
		QPoint pos_;
};

#endif // BOX_H
