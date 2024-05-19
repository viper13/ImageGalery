cmake_minimum_required(VERSION 3.16)

project(ImageGalery VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appImageGalery
    main.cpp
)

qt_add_qml_module(appImageGalery
    URI ImageGalery
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES imagesmodel.h imagesmodel.cpp
        QML_FILES CoruselList.qml
        SOURCES downloader.h downloader.cpp
        QML_FILES LoadingAnimation.qml
        SOURCES settings.h settings.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appImageGalery PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appImageGalery
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appImageGalery
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appImageGalery
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
