file(GLOB RAND_SRC
    "*.hpp"
    "*.cpp"
)

add_library(rand ${RAND_SRC})

target_include_directories(rand PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(rand PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)