#include "box.h"

Box::Box(int x, int y, QObject *parent) : QObject(parent), pos_(x, y)
{
}

QPoint Box::pos() const
{
	return pos_;
}

void Box::move(QPoint delta)
{
	pos_ += delta;
	emit moved();
}
