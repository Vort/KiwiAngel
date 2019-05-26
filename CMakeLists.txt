project(kiwisdr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(kiwisdr_SOURCES
	kiwisdrgui.cpp
	kiwisdrinput.cpp
	kiwisdrplugin.cpp
	kiwisdrworker.cpp
	kiwisdrsettings.cpp
)

set(kiwisdr_HEADERS
	kiwisdrgui.h
	kiwisdrinput.h
	kiwisdrplugin.h
	kiwisdrworker.h
	kiwisdrsettings.h
)

set(kiwisdr_FORMS
	kiwisdrgui.ui
)

include_directories(
	.
	${CMAKE_CURRENT_BINARY_DIR}
	${Boost_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/swagger/sdrangel/code/qt5/client
)

#include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

qt5_wrap_ui(kiwisdr_FORMS_HEADERS ${kiwisdr_FORMS})

add_library(inputkiwisdr SHARED
	${kiwisdr_SOURCES}
	${kiwisdr_HEADERS_MOC}
	${kiwisdr_FORMS_HEADERS}
)

target_link_libraries(inputkiwisdr
	${QT_LIBRARIES}
	sdrbase
	sdrgui
	swagger
)

target_link_libraries(inputkiwisdr Qt5::Core Qt5::Widgets Qt5::WebSockets)

install(TARGETS inputkiwisdr DESTINATION lib/plugins/samplesource)
