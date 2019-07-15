TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

CONFIG += link_pkgconfig
PKGCONFIG += opencv

#malo drugacno testiranje

HEADERS += \
    prepoznavanjeobraza.h \
    odprikamero.h \
    odprisliko.h \
    prepoznavanjeroke.h \
    prepoznavanjerokemodel.h \
    prepoznavanjerokesubstrakcija.h \
    prepoznavanjerokehaar.h
