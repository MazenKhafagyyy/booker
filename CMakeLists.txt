# # Generated Cmake Pico project file

# cmake_minimum_required(VERSION 3.13)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# # Initialise pico_sdk from installed location
# # (note this can come from environment, CMake cache etc)

# # == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
# if(WIN32)
#     set(USERHOME $ENV{USERPROFILE})
# else()
#     set(USERHOME $ENV{HOME})
# endif()
# set(sdkVersion 2.2.0)
# set(toolchainVersion 14_2_Rel1)
# set(picotoolVersion 2.2.0-a4)
# set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# if (EXISTS ${picoVscode})
#     include(${picoVscode})
# endif()
# # ====================================================================================
# set(PICO_BOARD pico2 CACHE STRING "Board type")

# # Pull in Raspberry Pi Pico SDK (must be before project)
# include(pico_sdk_import.cmake)

# project(booker C CXX ASM)

# # Initialise the Raspberry Pi Pico SDK
# pico_sdk_init()

# add_subdirectory(lib/Config)
# add_subdirectory(lib/e-Paper)
# add_subdirectory(lib/Fonts)
# add_subdirectory(lib/GUI)

# include_directories(./lib/Config)
# include_directories(./lib/GUI)

# # Add executable. Default name is the project name, version 0.1
# add_executable(booker
#     booker.c 
# )

# # Add the libraries to the build
# target_link_libraries(booker
#     Config
#     ePaper
#     Fonts
#     GUI
#     pico_stdlib
#     hardware_spi
# )

# pico_set_program_name(booker "booker")
# pico_set_program_version(booker "0.1")

# # Add the libraries to the build
# target_link_libraries(booker
#     Config
#     ePaper
#     Fonts
#     GUI
#     pico_stdlib
#     hardware_spi
# )

# # Add the standard include files to the build
# target_include_directories(booker PRIVATE
#     ${CMAKE_CURRENT_LIST_DIR}
#     ${CMAKE_CURRENT_LIST_DIR}/lib/Config
#     ${CMAKE_CURRENT_LIST_DIR}/lib/e-Paper
#     ${CMAKE_CURRENT_LIST_DIR}/lib/Fonts
#     ${CMAKE_CURRENT_LIST_DIR}/lib/GUI
# )

# # Modify the below lines to enable/disable output over UART/USB
# pico_enable_stdio_uart(booker 0)
# pico_enable_stdio_usb(booker 1)

# pico_add_extra_outputs(booker)


cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(booker C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# add a compilation subdirectory
add_subdirectory(lib/Config)
add_subdirectory(lib/e-Paper)
add_subdirectory(lib/Fonts)
add_subdirectory(lib/GUI)
# add_subdirectory(examples)

# add a header directory
# include_directories(examples)
include_directories(./lib/Config)
include_directories(./lib/E-Paper)
include_directories(./lib/Fonts)
include_directories(./lib/GUI)

# generate an executable file
add_executable(booker 
    booker.c
)

target_link_libraries(booker
    ePaper
    GUI
    Fonts
    Config
    pico_stdlib
    hardware_spi
)

# Add the standard include files to the build
target_include_directories(booker PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib/Config
    ${CMAKE_CURRENT_LIST_DIR}/lib/e-Paper
    ${CMAKE_CURRENT_LIST_DIR}/lib/Fonts
    ${CMAKE_CURRENT_LIST_DIR}/lib/GUI
)

# enable usb output, disable uart output
pico_enable_stdio_usb(booker 1)
pico_enable_stdio_uart(booker 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(booker)
