#pragma once

#include <QtCore/QtGlobal>

#if defined(QPINABLETABWIDGETLIB_LIBRARY)
#   define QPINABLETABWIDGETLIB_EXPORT Q_DECL_EXPORT
#else
#   define QPINABLETABWIDGETLIB_EXPORT Q_DECL_IMPORT
#endif
