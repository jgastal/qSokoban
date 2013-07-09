#ifndef LEVELCOLLECTION_H
#define LEVELCOLLECTION_H

#include "level.h"
#include <QList>
#include <QString>
#include <QObject>

class LevelCollection : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int levelNumber MEMBER currentLevel_ NOTIFY currentLevelChanged);
	Q_PROPERTY(int numberOfLevels READ numberOfLevels CONSTANT);
	Q_PROPERTY(Level *currentLevel READ currentLevel NOTIFY currentLevelChanged);
	Q_PROPERTY(bool nextLevelUnlocked READ nextLevelUnlocked NOTIFY unlockedLevelChanged);
	Q_PROPERTY(bool hasPreviousLevel READ hasPreviousLevel NOTIFY currentLevelChanged);

	public:
		LevelCollection(QString name, QByteArray collectionData, QObject *parent = 0);
		const QList<Level*> levels() const;
		Level *currentLevel() const;
		bool nextLevelUnlocked() const;
		int maxUnlockedLevel() const;
		void setMaxUnlockedLevel(int level);
		void setCurrentLevel(int level);
		int numberOfLevels() const;

	public slots:
		bool hasPreviousLevel();
		void unlockNextLevel();
		void nextLevel();
		void previousLevel();

	signals:
		void currentLevelChanged();
		void unlockedLevelChanged();

	private:
		QList<Level*> levels_;
		int currentLevel_;
		int maxUnlockedLevel_;
};

#endif // LEVELCOLLECTION_H
