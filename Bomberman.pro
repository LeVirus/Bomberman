TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_DEBUG += -Wall -Wextra -Wpedantic -Og

INCLUDEPATH += IncludeLib/

LIBS += -L../Bomberman/Lib -lsfml-graphics -lsfml-window -lsfml-system -lECS  #-lsfml-network  -lsfml-audio

SOURCES += main.cpp \
    jeu.cpp \
    moteur.cpp \
    moteurgraphique.cpp \
    gestionnaireecs.cpp \
    niveau.cpp \
    tilemap.cpp \
    tableau2d.cpp \
    inputbombermansystem.cpp \
    explosionbombermansystem.cpp \
    collisionbombermansystem.cpp \
    tilemapsystem.cpp \
    bombbombermansystem.cpp

HEADERS += \
    jeu.hpp \
    moteur.hpp \
    moteurgraphique.hpp \
    gestionnaireecs.hpp \
    niveau.hpp \
    tilemap.hpp \
    tableau2d.hpp \
    constants.hpp \
    inputbombermansystem.hpp \
    inputbombermancomponent.hpp \
    explosionbombermansystem.cpp \
    moveablebombermancomponent.hpp \
    collisionbombermansystem.hpp \
    flagcomponent.hpp \
    bombbombermansystem.hpp \
    tilemapsystem.hpp \
    timerbombermancomponent.hpp
