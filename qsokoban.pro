QMAKE_CXXFLAGS += -std=c++11

QT += qml quick

HEADERS += \
    level.h \
    levelcollection.h \
    game.h \
    badleveldescription.h \
    box.h \
    board.h

SOURCES += \
    level.cpp \
    main.cpp \
    levelcollection.cpp \
    game.cpp \
    badleveldescription.cpp \
    box.cpp \
    board.cpp

RESOURCES += \
    data.qrc

OTHER_FILES += \
    qml/Board.qml \
    qml/main.qml
