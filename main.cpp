#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "game.h"

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	Game game;
	QQmlApplicationEngine engine;

	engine.rootContext()->setContextProperty("game", &game);
	engine.load(QUrl("qrc:/qml/main.qml"));

	return app.exec();
}
