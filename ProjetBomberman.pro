TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_DEBUG += -Wall -Wextra -Wpedantic -Og

INCLUDEPATH += IncludeLib/

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -L Lib/ -lECS#-lsfml-network  -lsfml-audio

SOURCES += main.cpp \
    jeu.cpp \
    moteur.cpp \
    moteurgraphique.cpp \
    gestionnaireecs.cpp

HEADERS += \
    jeu.hpp \
    moteur.hpp \
    moteurgraphique.hpp \
    gestionnaireecs.hpp
