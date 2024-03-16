cmake_minimum_required (VERSION 3.6)

# 这段代码是用在 CMake 构建系统脚本中的，它用来设置 CMake 的安装前缀路径。让我解释一下这段代码的含义：
#set() 函数: set() 函数用于设置 CMake 变量的值。在这里，它被用来设置 CMAKE_INSTALL_PREFIX 这个 CMake 变量的值。
#**"{CMAKE_BINARY_DIR}"**: `{CMAKE_BINARY_DIR} 是一个 CMake 预定义变量，表示 CMake 构建时所生成的中间文件和目标文件的目录。在这里，"${CMAKE_BINARY_DIR}"` 表示将安装目录设置为构建过程中生成的目录，即将生成的文件安装到构建目录下。
#CACHE PATH "Installation directory" FORCE: 这部分是给 set() 函数提供了额外的参数。它指定了 CMAKE_INSTALL_PREFIX 这个变量的类型为 PATH，并且设置了默认的描述信息为 "Installation directory"。FORCE 标志表示无论之前是否已经设置过 CMAKE_INSTALL_PREFIX 这个变量，都强制重新设置它的值。
#因此，这段代码的作用是将 CMake 构建时生成的文件安装到构建目录下。这通常在开发和调试阶段很有用，但在最终部署时，可能需要将安装目录设置为一个更合适的位置。
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Installation directory" FORCE)
#这段代码是在 CMake 构建脚本中使用的一条消息输出语句，用于在 CMake 构建过程中打印出 CMAKE_INSTALL_PREFIX 变量的值。
#message() 函数: message() 函数在 CMake 中用于输出消息到终端或日志中，帮助开发者调试和了解构建过程中的一些信息。
# STATUS 参数: 在message()函数中，STATUS 参数用来指定消息的级别，通常用于输出一些重要信息或状态提示。
# "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}": 这是要打印的具体消息内容。在这里，它输出了一个包含变量值的字符串，其中 ${CMAKE_INSTALL_PREFIX} 是 CMake 变量 CMAKE_INSTALL_PREFIX 的值，通过这个消息，可以查看该变量的当前设置值。
# 通过这段代码，开发人员可以在 CMake 构建过程中实时查看 CMAKE_INSTALL_PREFIX 这个变量的值，以确保它被设置为他们期望的安装目录路径。
message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
# 函数是在 CMake 中用于向项目添加包含目录（include directories）的指令。包含目录是指编译器在编译源文件时要搜索头文件（header files）的目录
# 这个函数通常在 add_executable 或 add_library 之前被调用，以确保编译器能够找到项目中使用的头文件。
# 如果你的项目使用了某些库并且这些库的头文件在非标准的位置，你可以通过include_directories函数告诉 CMake 编译器应该到哪里查找这些头文件。
# 自 CMake 3.12 版本起，推荐使用 target_include_directories 函数来代替include_directories，因为target_include_directories允许更精细地控制包含目录，可以针对特定目标（target）进行设置。
include_directories(openssl/include)
# 将子目录添加到当前项目的命令，以便构建这些子目录中的代码。
# add_subdirectory(subdirectory_name [binary_dir])
# subdirectory_name: 要添加的子目录的路径，可以是相对路径或绝对路径。
# binary_dir (可选): 指定一个用于生成输出文件的目录，如果不指定，将使用当前构建目录。
# add_subdirectory 命令告诉 CMake 在当前项目中包含一个独立的子目录，并在构建时处理这个子目录中的 CMakeLists.txt 文件。add_subdirectory可以帮助将一个大型项目分解为多个较小的子项目，每个子项目可以有自己的构建过程和设置。
add_subdirectory(comm comm)
add_subdirectory(boot boot)
add_subdirectory(boost boost)
add_subdirectory(app app)
add_subdirectory(baseevent baseevent)
add_subdirectory(xlog xlog)
add_subdirectory(sdt sdt)
add_subdirectory(stn stn)

# for zstd
# 预编译
# option用于定义一个用户选项，允许用户在配置CMake时选择不同的选项，从而影响生成的构建系统
# option(<option_variable> "description" [initial_value])
option(ZSTD_BUILD_STATIC "BUILD STATIC LIBRARIES" ON)
option(ZSTD_BUILD_SHARED "BUILD SHARED LIBRARIES" OFF)

set(ZSTD_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zstd")
set(LIBRARY_DIR ${ZSTD_SOURCE_DIR}/lib)

# include命令用于在当前 CMakeLists 文件中包含另一个 CMake 脚本文件。
# include(filename [OPTIONAL] [RESULT_VARIABLE result])
# include命令会读取指定的 CMake 脚本文件并将其内容包含到当前的 CMakeLists 文件中。
include(GNUInstallDirs)
add_subdirectory(zstd/build/cmake/lib zstd)

project (mars)

include(comm/CMakeUtils.txt)

include_directories(.)
include_directories(..)

set(SELF_LIBS_OUT ${CMAKE_SYSTEM_NAME}.out)

if(ANDROID)

    if(NATIVE_CALLBACK)
        message("common native callback")
        # 向编译过程中添加预定义的编译器宏定义
        # add_definitions(-DMACRO1 -DMACRO2 ...)
        add_definitions(-DNATIVE_CALLBACK)
    endif()

    find_library(log-lib log)
    find_library(z-lib z)

    link_directories(app baseevent xlog sdt stn comm boost zstd)

    # marsxlog
    set(SELF_LIB_NAME marsxlog)
    # find_library(VAR name path1 path2 ...)
    # find_library命令在CMake中用于查找特定库文件的路径。这对于在CMake项目中引入外部库非常有用。
    # 在path中找到name库,将路径存储在VAR中
    file(GLOB SELF_SRC_FILES libraries/mars_android_sdk/jni/JNI_OnLoad.cc
            libraries/mars_xlog_sdk/jni/import.cc)
    # add_library(library_name [SHARED|STATIC|MODULE] source1 source2 ...)
    # 定义一个新的库
    # [SHARED|STATIC|MODULE]: 可选参数，指定库的类型，可以是共享库
    # source1 source2 ...: 库的源文件列表，这些源文件将会被编译为库文件。
    add_library(${SELF_LIB_NAME} SHARED ${SELF_SRC_FILES})
    #install(
        # TARGETS  target  要安装的目标，可以是可执行文件、库文件或其他CMake目标。
        # DESTINATION <dir> 指定安装目标文件的目录。
        # [COMPONENT <component>]指定安装组件。
        # [CONFIGURATIONS [Debug|Release|...]] 指定在特定配置（如Debug、Release等）下安装目标
        # [OPTIONAL] 指定安装为可选步骤
        # [PERMISSIONS permissions...] 指定安装后的文件权限
        # [RENAME name] 指定安装后的文件重命名
        # [EXPORT <export-name>] 指定要导出的目标
        # [INCLUDES DESTINATION include] 将头文件复制到指定的目录
    install(TARGETS ${SELF_LIB_NAME} LIBRARY DESTINATION ${SELF_LIBS_OUT} ARCHIVE DESTINATION ${SELF_LIBS_OUT})
    get_filename_component(EXPORT_XLOG_EXP_FILE libraries/mars_android_sdk/jni/export.exp ABSOLUTE)
    set(SELF_XLOG_LINKER_FLAG "-Wl,--gc-sections -Wl,--version-script='${EXPORT_XLOG_EXP_FILE}'") 
    if(ANDROID_ABI STREQUAL "x86_64" OR ANDROID_ABI STREQUAL "x86")
        set(SELF_XLOG_LINKER_FLAG "-Wl,--gc-sections -Wl,--version-script='${EXPORT_XLOG_EXP_FILE}' -Wl,--no-warn-shared-textrel") 
    endif()
    # target_link_libraries(target library1 library2 ...)
    target_link_libraries(${SELF_LIB_NAME} "${SELF_XLOG_LINKER_FLAG}"
                            xlog
                            mars-boost
                            comm
                            libzstd_static
                            ${log-lib}
                            ${z-lib}
                            )
 
    # marsstn
    set(SELF_LIB_NAME marsstn)
    file(GLOB SELF_SRC_FILES libraries/mars_android_sdk/jni/*.cc)
    add_library(${SELF_LIB_NAME} SHARED ${SELF_SRC_FILES})
    install(TARGETS ${SELF_LIB_NAME} LIBRARY DESTINATION ${SELF_LIBS_OUT} ARCHIVE DESTINATION ${SELF_LIBS_OUT})
    link_directories(${SELF_LIBS_OUT})
    find_library(CRYPT_LIB crypto PATHS openssl/openssl_lib_android/${ANDROID_ABI} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    find_library(SSL_LIB ssl PATHS openssl/openssl_lib_android/${ANDROID_ABI} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    target_link_libraries(${SELF_LIB_NAME} "-Wl,--gc-sections"
                        ${log-lib}
                        stn
                        sdt
                        app
                        baseevent
                        comm
                        boot
                        mars-boost
                        marsxlog
                        libzstd_static
                        ${SSL_LIB}
                        ${CRYPT_LIB})
endif()
