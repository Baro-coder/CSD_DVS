file(GLOB APP_SRC
    "*.h"
    "*.c"
    "src/*.c"
    "src/*.h"
)

add_executable(app ${APP_SRC})

set_property(TARGET app PROPERTY C_STANDARD 11)

target_link_libraries(app LINK_PUBLIC logs)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
