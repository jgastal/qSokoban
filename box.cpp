#include "box.h"

Box::Box(int x, int y, QObject *parent) : QObject(parent), pos_(x, y)
{
}

QPoint Box::pos() const
{
	return pos_;
}

void Box::addToX(int dx)
{
	pos_.rx() += dx;
	emit moved();
}

void Box::addToY(int dy)
{
	pos_.ry() += dy;
	emit moved();
}
