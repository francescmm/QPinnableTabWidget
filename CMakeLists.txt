file(GLOB SRC_FILES
  *.cpp
  *.h
  *.ui
)


add_library(QPinnableTabWidget STATIC
    ${SRC_FILES}
)

target_link_libraries(QPinnableTabWidget
    PUBLIC
    Qt::Core
    Qt::Widgets
)
target_include_directories(QPinnableTabWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
