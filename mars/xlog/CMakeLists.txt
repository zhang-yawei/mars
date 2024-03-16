# cmake最低版本
cmake_minimum_required (VERSION 3.6)
# 设置变量 CMAKE_INSTALL_PREFIX的值为 
#  CMAKE_BINARY_DIR 为cmake预定义变量, 中间构建输出的地址
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Installation directory" FORCE)
#  输出 CMAKE_BINARY_DIR
message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

project (xlog)


set(SELF_LIBS_OUT ${CMAKE_SYSTEM_NAME}.out)

# 包含其他组件的CmakeList
include(../comm/CMakeUtils.txt)
include(../comm/CMakeExtraFlags.txt)

include_directories(.)
include_directories(src)
include_directories(..)
include_directories(../..)
include_directories(../comm)
include_directories(../comm/xlogger)
include_directories(../crypt)
include_directories(../../..)
include_directories(../crypt/micro-ecc-master)

# file GLOB用于匹配规则在指定的目录内,匹配到所需要的文件
# file(GLOB variable_name [LIST_DIRECTORIES true|false] [RELATIVE path] [CONFIGURE_DEPENDS] glob1 [glob2 ...])
# variable_name:存储匹配到的文件列表
# LIST_DIRECTORIES: 可选参数, 指定是否包含目录
# RELATIVE 可选参数, 用于指定相对路径,文件匹配从这个相对路径开始
# CONFIGURE_DEPENDS 可选参数, 指定是否在构建的时候重新计算文件依赖关系
# 在路径 RELATIVE ${PROJECT_SOURCE_DIR}下查找符合条件 src/*.cc src/*.h 的文件,把地址列表存放在SELF_TEMP_SRC_FILES 中
file(GLOB SELF_TEMP_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} src/*.cc src/*.h)

# source_group 命令用于将源文件组织为逻辑组，并在 IDE 中以这些逻辑组的形式呈现，使得在大型项目中更容易管理和浏览源文件。这对于可视化 IDE （如 Visual Studio、Xcode 等）特别有用
source_group(src FILES ${SELF_TEMP_SRC_FILES})
list(APPEND SELF_SRC_FILES ${SELF_TEMP_SRC_FILES})


file(GLOB SELF_TEMP_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} crypt/*.cc crypt/*.h)
source_group(crypt FILES ${SELF_TEMP_SRC_FILES})
list(APPEND SELF_SRC_FILES ${SELF_TEMP_SRC_FILES})

file(GLOB SELF_TEMP_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} crypt/micro-ecc-master/*.c crypt/micro-ecc-master/*.h)
source_group(crypt\\micro-ecc-master FILES ${SELF_TEMP_SRC_FILES})
list(APPEND SELF_SRC_FILES ${SELF_TEMP_SRC_FILES})



if(MSVC)
    add_definitions(/FI"../../comm/projdef.h")

    include_directories(../comm/windows)
    include_directories(../comm/windows/zlib)

elseif(APPLE)
    file(GLOB SELF_TEMP_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} objc/*.mm)
    source_group(objc FILES ${SELF_TEMP_SRC_FILES})
    list(APPEND SELF_SRC_FILES ${SELF_TEMP_SRC_FILES})
elseif(ANDROID)
    file(GLOB SELF_ANDROID_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR}
            ../comm/xlogger/xloggerbase.c
            ../comm/xlogger/xlogger.cc
            jni/*.cc
            ../mk_template/JNI_OnLoad.cpp)
        
    list(APPEND SELF_SRC_FILES ${SELF_ANDROID_SRC_FILES})

    get_filename_component(EXPORT_EXP_FILE jni/export.exp ABSOLUTE)
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${EXPORT_EXP_FILE}")

endif()

# add_library(${PROJECT_NAME} STATIC ${SELF_SRC_FILES})
# install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION ${SELF_LIBS_OUT})

BuildWithUnitTest("${PROJECT_NAME}" "${SELF_SRC_FILES}")


    
    
