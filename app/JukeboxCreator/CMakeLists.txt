cmake_minimum_required(VERSION 3.5)

project(JukeboxCreator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")

include_directories(${RAPIDJSON_INCLUDE_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES JukeboxCreator_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        gtproject.h gtproject.cpp
        gtscene.h gtscene.cpp
        gtsprite.h gtsprite.cpp
        gtentityslot.h gtentityslot.cpp
        gtentity.h gtentity.cpp
        gtbehavior.h gtbehavior.cpp
        jsonbase.h jsonbase.cpp
        jsonmacros.h
        editorsession.h editorsession.cpp
        splashdialog.h splashdialog.cpp splashdialog.ui
        whereami.h whereami.cpp
        romserializer.h romserializer.cpp
        sceneeditorview.h sceneeditorview.cpp
        behaviorcompiler.h behaviorcompiler.cpp
        behaviormanagerdialog.h behaviormanagerdialog.cpp behaviormanagerdialog.ui
        behaviorslistmodel.h behaviorslistmodel.cpp
        behaviorlistview.h behaviorlistview.cpp
        assemblyunit.h assemblyunit.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(JukeboxCreator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET JukeboxCreator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(JukeboxCreator SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(JukeboxCreator
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

add_dependencies(JukeboxCreator rapidjson)

add_custom_command(
    TARGET JukeboxCreator POST_BUILD
    COMMAND ${QT_WINDEPLOYQT_EXECUTABLE}
)

add_custom_command(
    TARGET JukeboxCreator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/../../kit $<TARGET_FILE_DIR:JukeboxCreator>/kit
)

add_custom_command(
    TARGET JukeboxCreator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/../../lib/cc65/bin $<TARGET_FILE_DIR:JukeboxCreator>/lib
)

add_custom_command(
    TARGET JukeboxCreator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/../../engine/binclude $<TARGET_FILE_DIR:JukeboxCreator>/lib
)

add_custom_command(
    TARGET JukeboxCreator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resource $<TARGET_FILE_DIR:JukeboxCreator>/resource
)

target_include_directories(JukeboxCreator PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(JukeboxCreator PRIVATE ${PROJECT_SOURCE_DIR}/../../engine/binclude)

target_link_libraries(JukeboxCreator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(JukeboxCreator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS JukeboxCreator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(JukeboxCreator)
endif()
