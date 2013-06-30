QMAKE_CXXFLAGS += -std=c++0x

QT += qml quick

HEADERS += \
    level.h \
    levelcollection.h

SOURCES += \
    level.cpp \
    main.cpp \
    levelcollection.cpp

RESOURCES += \
    data.qrc

OTHER_FILES += \
    qml/Board.qml \
    qml/main.qml
