#include "box.h"

Box::Box(QObject *parent) : QObject(parent), x_(0), y_(0)
{
}

Box::Box(int x, int y, QObject *parent) : QObject(parent), x_(x), y_(y)
{
}

int Box::x() const
{
	return x_;
}

int Box::y() const
{
	return y_;
}

void Box::addToX(int dx)
{
	x_ += dx;
	emit moved();
}

void Box::addToY(int dy)
{
	y_ += dy;
	emit moved();
}
