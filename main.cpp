#include <QGuiApplication>
#include <QDir>
#include <QFileInfoList>
#include <QFile>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QtQml>

#include "levelcollection.h"

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	QDir levelsDir(":/levels/");
	QFileInfoList collectionFiles = levelsDir.entryInfoList();
	QList<LevelCollection*> collections;

	qmlRegisterType<Level>("Level", 1,0, "Level");

	for (int i = 0; i < collectionFiles.size(); ++i)
	{
		QFile f(collectionFiles.at(i).filePath());
		if (!f.open(QIODevice::ReadOnly))
		    qWarning() << "Failed to open collection: " << f.fileName();
		QByteArray colData = f.readAll();
		if (!colData.isEmpty())
			collections.append(new LevelCollection(collectionFiles.at(i).fileName(), colData));
		else
			qWarning() << "Empty collection found: " << f.fileName();
	}

	LevelCollection *col = collections.first();
	engine.rootContext()->setContextProperty("collection", col);
	col->connect(col->currentLevel(), &Level::levelCompleted, col, &LevelCollection::unlockNextLevel);
	col->connect(col->currentLevel(), &Level::levelCompleted, col, &LevelCollection::nextLevel);
	engine.load(QUrl("qrc:/qml/main.qml"));

	return app.exec();
}
