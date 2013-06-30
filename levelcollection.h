#ifndef LEVELCOLLECTION_H
#define LEVELCOLLECTION_H

#include "level.h"
#include <QList>
#include <QString>
#include <QObject>
#include <QtQml>

QML_DECLARE_TYPE(Level)
//Q_DECLARE_METATYPE(Level)
//Q_DECLARE_METATYPE(QQmlListProperty<Level>)

class LevelCollection : public QObject
{
	Q_OBJECT
		Q_PROPERTY(int currentLevel MEMBER currentLevel_ NOTIFY currentLevelChanged);

	public:
		LevelCollection(QString name, QByteArray collectionData);
		virtual ~LevelCollection();
		const QList<Level*> levels() const;
		Level *currentLevel() const;

	signals:
		void currentLevelChanged();

	private:
		QList<Level*> levels_;
		int currentLevel_;
};

#endif // LEVELCOLLECTION_H
