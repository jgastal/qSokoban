#include "game.h"
#include <QDir>
#include <QFileInfoList>
#include <QFile>
#include <QDebug>
#include <QtQml>
#include <QSettings>
#include "levelcollection.h"
#include "level.h"

const QString Game::curColKey = QString("currentCollection");
const QString Game::levelsDir = QString(":/levels/");

Game::Game(QObject *parent) : QObject(parent), currentCollection_(NULL)
{
	QDir dir(levelsDir);
	settings = new QSettings("Parabola", "qSokoban");

	qmlRegisterType<Level>("Level", 1,0, "Level");
	qmlRegisterType<LevelCollection>("LevelCollection", 1,0, "LevelCollection");

	collectionNames_.append(dir.entryList());

	changeCollection(settings->value("currentCollection", collectionNames_.first()).toString());
}

Game::~Game()
{
	delete settings;
	while (!collections_.isEmpty())
		delete collections_.takeFirst();
}

QStringList Game::collectionNames() const
{
	return collectionNames_;
}


LevelCollection *Game::loadCollection(QString name)
{
	QFile f(levelsDir + name);
	LevelCollection *col = NULL;
	if (!f.open(QIODevice::ReadOnly))
		qWarning() << "Failed to open collection: " << f.fileName();
	QByteArray colData = f.readAll();
	if (!colData.isEmpty())
	{
		col = new LevelCollection(name, colData);
		collections_.append(col);
	}
	else
		qWarning() << "Empty collection found: " << f.fileName();
	return col;

}

LevelCollection *Game::currentCollection() const
{
	return currentCollection_;
}

int Game::currentCollectionIndex() const
{
	return collectionNames_.indexOf(currentCollection_->objectName());
}

void Game::changeCollection(QString name)
{
	if (name.isEmpty() || (currentCollection_ && currentCollection_->objectName() == name))
		return;
	currentCollection_ = loadCollection(name);
	settings->setValue("currentCollection", name);
	currentCollection_->setMaxUnlockedLevel(settings->value(name, 0).toInt());
	currentCollection_->setCurrentLevel(settings->value(name, 0).toInt());
	disconnect(currentCollection_);
	connect(currentCollection_, &LevelCollection::unlockedLevelChanged, this, &Game::saveSettings);

	connect(currentCollection_->currentLevel(), &Level::levelCompleted, currentCollection_, &LevelCollection::unlockNextLevel);
	connect(currentCollection_->currentLevel(), &Level::levelCompleted, currentCollection_, &LevelCollection::nextLevel);
	emit currentCollectionChanged();
}

void Game::saveSettings()
{
	settings->setValue(currentCollection_->objectName(), currentCollection_->maxUnlockedLevel());
}