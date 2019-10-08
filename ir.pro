TEMPLATE        = lib
CONFIG         += plugin
QT             += core quick
HEADERS         = ir.h \
                  ../remote-software/sources/integrations/integration.h \
                  ../remote-software/sources/integrations/integrationinterface.h
SOURCES         = ir.cpp
TARGET          = ir
DESTDIR         = ../remote-software/plugins

# install
unix {
    target.path = /usr/lib
    INSTALLS += target
}
