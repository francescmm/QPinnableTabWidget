QT       += core gui widgets

TARGET = QPinableTabWidget

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = lib

SOURCES += \
    $$PWD/FakeCloseButton.cpp \
    $$PWD/RealCloseButton.cpp \
    $$PWD/QPinableTabWidget.cpp

HEADERS += \
    $$PWD/FakeCloseButton.h \
    $$PWD/QPinableTabWidget.h \
    $$PWD/RealCloseButton.h \
    $$PWD/qpinabletabwidget_global.h

win32 {
    QMAKE_TARGET_PRODUCT = "$$TARGET"
    QMAKE_TARGET_COMPANY = "Cesc Software"
    QMAKE_TARGET_COPYRIGHT = "Francesc M."
} else:mac {
    QMAKE_TARGET_BUNDLE_PREFIX = "com.francescmm."
}
