file(GLOB PRNG_SRC
    "*.h"
    "*.c"
)

add_library(prng ${PRNG_SRC})

target_include_directories(prng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(prng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)