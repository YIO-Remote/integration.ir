TEMPLATE        = lib
CONFIG         += plugin
QT             += core quick

include(../remote-software/qmake-target-platform.pri)
include(../remote-software/qmake-destination-path.pri)

HEADERS         = ir.h \
                  ../remote-software/sources/integrations/integration.h \
                  ../remote-software/sources/integrations/integrationinterface.h
SOURCES         = ir.cpp
TARGET          = ir

# Configure destination path by "Operating System/Compiler/Processor Architecture/Build Configuration"
DESTDIR = $$PWD/../binaries/$$DESTINATION_PATH/plugins
OBJECTS_DIR = $$PWD/build/$$DESTINATION_PATH/obj
MOC_DIR = $$PWD/build/$$DESTINATION_PATH/moc
RCC_DIR = $$PWD/build/$$DESTINATION_PATH/qrc
UI_DIR = $$PWD/build/$$DESTINATION_PATH/ui

# install
unix {
    target.path = /usr/lib
    INSTALLS += target
}
