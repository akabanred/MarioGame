cmake_minimum_required(VERSION 3.6)

# -------- Project --------
set(APP_NAME MarioGame)
project(${APP_NAME})

# -------- Cocos2d-x cmake modules (PHẢI đặt trước khi dùng các macro cocos_*) --------
# Trỏ tới thư mục engine bạn đã clone (đang để song song với CMakeLists.txt)
set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH   ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

# Helpers định nghĩa các macro: cocos_mark_multi_resources, setup_cocos_app_config,
# cocos_get_resource_path, cocos_copy_target_res, cocos_copy_target_dll, ...
include(CocosBuildHelpers)

# -------- Thêm engine (chỉ khi chưa có biến BUILD_ENGINE_DONE) --------
if(NOT DEFINED BUILD_ENGINE_DONE)
    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# -------- Nguồn & tài nguyên --------
set(GAME_SOURCE)
set(GAME_HEADER)

# Thư mục tài nguyên game
set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
)

# Tạo list tài nguyên (Windows / Apple)
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# Mã nguồn cross-platform
file(GLOB_RECURSE GAME_SOURCE "Classes/*.cpp")
file(GLOB_RECURSE GAME_HEADER "Classes/*.h")

# -------- Platform specifics --------
if(ANDROID)
    # Tên .so trên Android thường lấy từ AndroidManifest, để tạm
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE proj.android/app/jni/hellocpp/main.cpp)

elseif(LINUX)
    list(APPEND GAME_SOURCE proj.linux/main.cpp)

elseif(WINDOWS)
    list(APPEND GAME_HEADER proj.win32/main.h)
    list(APPEND GAME_SOURCE proj.win32/main.cpp ${common_res_files})

elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
            proj.ios_mac/ios/AppController.h
            proj.ios_mac/ios/RootViewController.h
        )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
        )
        list(APPEND GAME_SOURCE
            proj.ios_mac/ios/main.m
            proj.ios_mac/ios/AppController.mm
            proj.ios_mac/ios/RootViewController.mm
            proj.ios_mac/ios/Prefix.pch
            ${APP_UI_RES}
        )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
        )
        list(APPEND GAME_SOURCE
            proj.ios_mac/mac/main.cpp
            proj.ios_mac/mac/Prefix.pch
            ${APP_UI_RES}
        )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# -------- Tạo target app --------
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
)

if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

# Link với engine
target_link_libraries(${APP_NAME} cocos2d)

# Include paths
target_include_directories(${APP_NAME}
    PRIVATE
        Classes
        ${COCOS2DX_ROOT_PATH}/cocos
        ${COCOS2DX_ROOT_PATH}/cocos/2d
        ${COCOS2DX_ROOT_PATH}/cocos/3d
        ${COCOS2DX_ROOT_PATH}/cocos/base
        ${COCOS2DX_ROOT_PATH}/cocos/math
        ${COCOS2DX_ROOT_PATH}/cocos/platform
        ${COCOS2DX_ROOT_PATH}/cocos/platform/win32
        ${COCOS2DX_ROOT_PATH}/cocos/audio/include
        ${COCOS2DX_ROOT_PATH}/cocos/ui
        ${COCOS2DX_ROOT_PATH}/extensions
        ${COCOS2DX_ROOT_PATH}/external
        ${COCOS2DX_ROOT_PATH}/cocos/editor-support/cocostudio
)

# -------- Thiết lập tài nguyên & DLL theo nền tảng --------
setup_cocos_app_config(${APP_NAME})

if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    set_xcode_property(${APP_NAME} INSTALL_PATH "\$(LOCAL_APPS_DIR)")

    if(MACOSX)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
    elseif(IOS)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()

elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

# ---- Copy Resources sau khi build (Windows & Linux) ----
if(WINDOWS OR LINUX)
    # Đặt working dir khi chạy từ VS/CLI (tùy chọn nhưng hữu ích)
    set_target_properties(${APP_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${APP_NAME}>"
    )

    # Tạo thư mục Resources cạnh .exe và copy toàn bộ nội dung Resources vào đó
    add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${APP_NAME}>/Resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
                "$<TARGET_FILE_DIR:${APP_NAME}>/Resources"
        COMMENT "Copying game Resources to output folder"
    )
endif()


