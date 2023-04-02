file(GLOB APP_SRC
    "*.h"
    "*.c"
    "src/*.c"
    "src/*.h"
)

add_executable(app ${APP_SRC})

target_link_libraries(app LINK_PUBLIC prng)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
