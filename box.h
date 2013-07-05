#ifndef BOX_H
#define BOX_H

#include <QObject>

class Box : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int x READ x NOTIFY moved)
	Q_PROPERTY(int y READ y NOTIFY moved)
	public:
		explicit Box(QObject *parent = 0);
		explicit Box(int x, int y, QObject *parent = 0);
		int x() const;
		int y() const;
		void addToX(int dx);
		void addToY(int dy);

	signals:
		void moved();

	private:
		int x_, y_;
};

Q_DECLARE_METATYPE(QList<Box*>);

#endif // BOX_H
