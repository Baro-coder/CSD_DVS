file(GLOB APP_SRC
    "*.hpp"
    "*.cpp"
)

add_executable(app ${APP_SRC})

target_link_libraries(app LINK_PUBLIC rand)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib/rand)
