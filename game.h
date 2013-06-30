#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QStringList>

class LevelCollection;
class QSettings;

class Game : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QStringList collections READ collectionNames NOTIFY collectionsChanged);
	Q_PROPERTY(int currentCollectionIndex READ currentCollectionIndex NOTIFY currentCollectionChanged);
	Q_PROPERTY(LevelCollection *currentCollection READ currentCollection NOTIFY currentCollectionChanged);
	public:
		explicit Game(QObject *parent = 0);
		virtual ~Game();
		QStringList collectionNames() const;
		LevelCollection *currentCollection() const;
		int currentCollectionIndex() const;

	public slots:
		void changeCollection(QString name);

	signals:
		void collectionsChanged();
		void currentCollectionChanged();

	private slots:
		void saveSettings();

	private:
		LevelCollection *loadCollection(QString name);
		QStringList collectionNames_;
		QList<LevelCollection*> collections_;
		LevelCollection *currentCollection_;
		QSettings *settings;
		static const QString curColKey;
		static const QString levelsDir;
};

#endif // GAME_H