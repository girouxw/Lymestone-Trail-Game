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
    data/land1.png \
    data/land2.png \
    data/land3.png \
    data/land4.png \
    data/land5.png \
    data/land6.png \
    data/walk1.png \
    data/walk2.png \
    data/walk3.png \
    data/banbury.png \
    data/birmingham.png \
    data/bristol.png \
    data/cambridge.png \
    data/canterbury.png \
    data/chester.png \
    data/derby.png \
    data/gloucester \
    data/hereford.png \
    data/leicester.png \
    data/london.png \
    data/lymestone.png \
    data/manchester.png \
    data/northampton.png \
    data/nottingham.png \
    data/oxford.png \
    data/oxford2.png \
    data/penkhull.png \
    data/peterborough.png \
    data/swindon.png \
    data/sheffield.png \
    data/titleScreen.png \




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
