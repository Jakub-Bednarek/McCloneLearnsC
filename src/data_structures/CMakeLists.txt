set(INCLUDE_DIR include/)

set(HEADER
    ${INCLUDE_DIR}/hash_map.h)

set(SOURCE
    src/hash_map.c)

add_library(data_structures STATIC ${SOURCE})
target_include_directories(data_structures PUBLIC ${INCLUDE_DIR})

add_ut_subdirectory(tests)