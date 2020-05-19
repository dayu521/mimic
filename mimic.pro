######################################################################
# Automatically generated by qmake (3.1) Tue Dec 24 13:04:31 2019
######################################################################

TEMPLATE = app
CONFIG +=c++17
QT +=widgets
TARGET = mimic
INCLUDEPATH +=  common_ui/  \
                common_ui/ui    \
                core_interface  \
                core_interface/datasource   \
                component   \
                utils

#CONFIG += use_git_submodule_loki
#DEFINES += fuck

use_git_submodule_loki{
    DEFINES +=USE_GIT_SUBMODULE_LOKI
    INCLUDEPATH +=\
                "lib/loki-lib/include"
}

#可执行文件放在此目录,即当前工程目录

# DESTDIR =$$PWD

#msvc编译器识别正常utf-8.
#莫非我的vs版本太低了?
msvc {
	QMAKE_CFLAGS += /utf-8
	QMAKE_CXXFLAGS += /utf-8
}

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += \
           common_ui/ui/flutteringwings.h \
           component/demosim.h \
           component/freeboy.h \
           component/hanoisim.h \
           component/mazesim.h \
           component/quicksortsim.h \
           component/rbtree_sim.h \
#           component/rbtreesimulation.h \
           common_ui/ui/setting.h \
           core_interface/alarm.h \
           core_interface/animation/abstract_animation.h \
           core_interface/animation/rbtree_model.h \
           core_interface/datasource/far_away.h \
           core_interface/datasource/rbdata.h \
           core_interface/datasource/wrap_far_away.h \
           core_interface/simulator.h \
           common_ui/widget.h \
           utils/convenience.h \
           utils/loki_wrap.h \
           utils/register_type.h \
           utils/something.h
use_git_submodule_loki{
    HEADERS +=\
        loki/Typelist.h \
}
FORMS += common_ui/ui/form.ui common_ui/widget.ui

SOURCES += \
           common_ui/ui/flutteringwings.cpp \
           component/demosim.cpp \
           component/freeboy.cpp \
           component/hanoisim.cpp \
           component/mazesim.cpp \
           component/quicksortsim.cpp \
           component/rbtree_sim.cpp \
#           component/rbtreesimulation.cpp \
           core_interface/alarm.cpp \
           core_interface/animation/rbtree_model.cpp \
           core_interface/datasource/far_away.cpp \
           core_interface/datasource/rbdata.cpp \
           core_interface/datasource/wrap_far_away.cpp \
           core_interface/simulator.cpp \
           main.cpp \
           common_ui/ui/setting.cpp \
           common_ui/widget.cpp \
           utils/something.cpp

TRANSLATIONS +=abc_en_US.ts \
            abc_zh_CN.ts \

#message($$CONFIG)
#message($$QMAKESPEC)