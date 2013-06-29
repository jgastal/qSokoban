#include <QGuiApplication>
#include <QDir>
#include <QFileInfoList>
#include <QFile>
#include <QDebug>

#include "levelcollection.h"

int main(int argc, char **argv)
{
//	QGuiApplication app(argc, argv);
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

//	return app.exec();
	return 0;
}
