QT += widgets

HEADERS       = mainwindow.h \
    mytreeview.h \
    settingsdialog.h
SOURCES       = main.cpp \
                mainwindow.cpp \
    mytreeview.cpp \
    settingsdialog.cpp
#! [0]
RESOURCES     = application.qrc
#! [0]

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target
