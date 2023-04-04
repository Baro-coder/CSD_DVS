file(GLOB LOGS_SRC
    "*.h"
    "*.c"
)

add_library(logs ${LOGS_SRC})

set_property(TARGET logs PROPERTY C_STANDARD 99)

target_include_directories(logs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
