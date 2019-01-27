TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_DEBUG += -Wall -Wextra -Wpedantic -Og

INCLUDEPATH += IncludeLib/
INCLUDEPATH += Systems/
INCLUDEPATH += Components/
INCLUDEPATH += Utils/

LIBS += -L../Bomberman/Lib -lsfml-graphics -lsfml-window -lsfml-system -lECS  -lsfml-network -lpthread #-lsfml-audio

SOURCES += main.cpp \
    jeu.cpp \
    moteur.cpp \
    moteurgraphique.cpp \
    gestionnaireecs.cpp \
    Utils/niveau.cpp \
    Utils/tilemap.cpp \
    Utils/tableau2d.cpp \
    Utils/basesocket.cpp \
    Utils/NetworkDataUtils.cpp \
    Systems/inputbombermansystem.cpp \
    Systems/explosionbombermansystem.cpp \
    Systems/collisionbombermansystem.cpp \
    Systems/tilemapsystem.cpp \
    Systems/bombbombermansystem.cpp \
    Systems/playerbombersystem.cpp \
    Systems/socketsystem.cpp

HEADERS += \
    jeu.hpp \
    moteur.hpp \
    moteurgraphique.hpp \
    gestionnaireecs.hpp \
    Utils/niveau.hpp \
    Utils/tilemap.hpp \
    Utils/tableau2d.hpp \
    Utils/constants.hpp \
    Utils/basesocket.hpp \
    Utils/NetworkDataUtils.hpp \
    Components/inputbombermancomponent.hpp \
    Components/playerconfigbombermancomponent.hpp \
    Components/tilemapbombermancomponent.hpp \
    Components/bombconfigbombermancomponent.hpp \
    Components/moveablebombermancomponent.hpp \
    Components/flagcomponent.hpp \
    Components/timerbombermancomponent.hpp \
    Systems/bombbombermansystem.hpp \
    Systems/inputbombermansystem.hpp \
    Systems/bombbombermansystem.hpp \
    Systems/tilemapsystem.hpp \
    Systems/explosionbombermansystem.cpp \
    Systems/collisionbombermansystem.hpp \
    Systems/playerbombersystem.hpp \
    Systems/socketsystem.hpp \
    Components/networkcomponent.hpp
