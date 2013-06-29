#ifndef LEVELCOLLECTION_H
#define LEVELCOLLECTION_H

#include "level.h"
#include <QList>
#include <QString>

class LevelCollection
{
	public:
		LevelCollection(QString name, QByteArray collectionData);
		virtual ~LevelCollection();
		QString name() const;
		const QList<Level*> levels() const;

	private:
		QList<Level*> levels_;
		QString name_;
};

#endif // LEVELCOLLECTION_H
