#include <QGuiApplication>
#include <QDir>
#include <QFileInfoList>
#include <QFile>
#include <QDebug>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlContext>

#include "levelcollection.h"

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	QDir levelsDir(":/levels/");
	QFileInfoList collectionFiles = levelsDir.entryInfoList();
	QList<LevelCollection*> collections;

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

	Level *l = collections.first()->levels().first();
	QQuickView viewer;
	viewer.rootContext()->setContextProperty("level", l);
	viewer.setSource(QStringLiteral("qrc:/qml/main.qml"));
	viewer.rootObject()->setWidth(64 * l->width());
	viewer.rootObject()->setHeight(64 * l->height());
	viewer.resize(64 * l->width(), 64 * l->height());
	viewer.show();

	return app.exec();
}
