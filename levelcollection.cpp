#include "levelcollection.h"
#include <QDebug>

LevelCollection::LevelCollection(QString name, QByteArray collectionData) : currentLevel_(0), maxUnlockedLevel_(0)
{
	setObjectName(name);

	while (!collectionData.isEmpty())
	{
		int separator = collectionData.indexOf('\0');
		if (separator == -1)
			break; //No more levels
		Level *l = new Level(collectionData.left(separator));
		levels_.append(l);
		collectionData.remove(0, separator + 1);
	}
}

LevelCollection::~LevelCollection()
{
	while (!levels_.isEmpty())
		delete levels_.takeFirst();
}

const QList<Level *> LevelCollection::levels() const
{
	return levels_;
}

Level *LevelCollection::currentLevel() const
{
	return levels_.at(currentLevel_);
}

bool LevelCollection::hasPreviousLevel()
{
	return currentLevel_ != 0;
}

bool LevelCollection::nextLevelUnlocked() const
{
	return currentLevel_ + 1 <= maxUnlockedLevel_;
}

void LevelCollection::unlockNextLevel()
{
	++maxUnlockedLevel_;
	unlockedLevelChanged();
}

void LevelCollection::nextLevel()
{
	++currentLevel_;
	emit currentLevelChanged();
	//Making sure next button's state is recalculated
	emit unlockedLevelChanged();
}

void LevelCollection::previousLevel()
{
	--currentLevel_;
	emit currentLevelChanged();
	//Making sure next button's state is recalculated
	emit unlockedLevelChanged();
}
