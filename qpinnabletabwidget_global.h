#pragma once

#include <QtCore/QtGlobal>

#if defined(QPinnableTabWidgetLIB_LIBRARY)
#   define QPinnableTabWidgetLIB_EXPORT Q_DECL_EXPORT
#else
#   define QPinnableTabWidgetLIB_EXPORT Q_DECL_IMPORT
#endif

