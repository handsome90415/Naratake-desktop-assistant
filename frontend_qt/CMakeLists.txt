cmake_minimum_required(VERSION 3.16)

project(DesktopAICompanionFrontend VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

qt_standard_project_setup()

qt_add_executable(DesktopAICompanion
    main.cpp

    src/ChatWindow.h
    src/ChatWindow.cpp

    src/ApiClient.h
    src/ApiClient.cpp

    src/PetWindow.h
    src/PetWindow.cpp

    src/BackendManager.h
    src/BackendManager.cpp

    src/AppConfig.h
    src/AppConfig.cpp
)

target_link_libraries(DesktopAICompanion
    PRIVATE
    Qt6::Widgets
    Qt6::Network
)
