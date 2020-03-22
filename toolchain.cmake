include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME ChibiOS)

set(TOOLCHAIN_DIR $ENV{HOME}/Softwares/gcc-arm-none-eabi-8-2019-q3-update)
set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_DIR}/bin)
set(TOOLCHAIN_COMMON_PREFIX arm-none-eabi)
cmake_force_c_compiler(${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN_COMMON_PREFIX}-gcc GNU)
cmake_force_cxx_compiler(${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN_COMMON_PREFIX}-g++ GNU)
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# macro to find packages on the host OS
macro(find_host_package)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    find_package(${ARGN})
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endmacro(find_host_package)
