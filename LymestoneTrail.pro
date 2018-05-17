QT += widgets
QT += multimedia

CONFIG += gui
CONFIG += c++11

SOURCES += \
    main.cpp \
    vec2d.cpp \
    graphics.cpp \
    playerdata.cpp \
    button.cpp \
    item.cpp \
    town.cpp

HEADERS  += \
    graphics.h \
    vec2d.h \
    graphics.h \
    window.h \
    playerdata.h \
    button.h \
    item.h \
    town.h

DISTFILES += \
    data/landOne.png \
    data/landTwo.png \
    data/walk1.png \
    data/walk2.png \
    data/walk3.png


ASSETS_DIR = data

CONFIG(debug, debug|release) {
    DATADIR = debug/$$ASSETS_DIR
} else {
    DATADIR = release/$$ASSETS_DIR
}

copydata.commands = $(COPY_DIR) \"$$shell_path($$absolute_path("$$ASSETS_DIR", $$_PRO_FILE_PWD_))\" \"$$shell_path($$absolute_path($$DATADIR, $$OUT_PWD))\"
first.depends = $(first) copydata

export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata
