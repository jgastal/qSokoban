#include "levelcollection.h"
#include <QDebug>

LevelCollection::LevelCollection(QString name, QByteArray collectionData, QObject *parent)
	: QObject(parent), currentLevel_(0), maxUnlockedLevel_(0)
{
	setObjectName(name);

	while (!collectionData.isEmpty())
	{
		int separator = collectionData.indexOf('\0');
		if (separator == -1)
			break; //No more levels
		Level *l = new Level(collectionData.left(separator), this);
		levels_.append(l);
		collectionData.remove(0, separator + 1);
	}
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

int LevelCollection::maxUnlockedLevel() const
{
	return maxUnlockedLevel_;
}

void LevelCollection::setMaxUnlockedLevel(int level)
{
	maxUnlockedLevel_ = level;
}

void LevelCollection::setCurrentLevel(int level)
{
	disconnect(levels_.at(currentLevel_));
	currentLevel_ = level;
	connect(levels_.at(currentLevel_), &Level::levelCompleted, this, &LevelCollection::unlockNextLevel);
	connect(levels_.at(currentLevel_), &Level::levelCompleted, this, &LevelCollection::nextLevel);
	emit currentLevelChanged();
	levels_.at(currentLevel_)->reset();
	//Making sure next button's state is recalculated
	emit unlockedLevelChanged();
}

int LevelCollection::numberOfLevels() const
{
	return levels_.size();
}

void LevelCollection::unlockNextLevel()
{
	++maxUnlockedLevel_;
	unlockedLevelChanged();
}

void LevelCollection::nextLevel()
{
	setCurrentLevel(++currentLevel_);
}

void LevelCollection::previousLevel()
{
	setCurrentLevel(--currentLevel_);
}
