cmake_minimum_required(VERSION 3.5...4.0)

project(Uno VERSION 1.0)

# ------------------ cpp ------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ------------------ packages ------------------
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(RmlUi REQUIRED CONFIG)
find_package(Protobuf REQUIRED CONFIG)
find_package(GameNetworkingSockets REQUIRED CONFIG)

# ----------------------------------------------
add_executable(uno)

# ----------------------------------------------
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# ----------------------------------------------
set(PROTO_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/proto/net_message.proto
)
protobuf_generate_cpp(PROTO_SOURCES PROTO_HEADERS ${PROTO_FILES})

# ----------------------------------------------
target_sources(uno PRIVATE 
    ${SOURCES} 
    ${HEADERS} 
    ${PROTO_SOURCES} 
    ${PROTO_HEADERS}
)

# ----------------------------------------------
target_link_libraries(uno PRIVATE 	
	SDL3::SDL3
	SDL3_image::SDL3_image
    RmlUi::RmlUi
	protobuf::libprotobuf
	GameNetworkingSockets::static
)

target_compile_definitions(uno PRIVATE 
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(uno PUBLIC
	${Protobuf_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ----------------------------------------------
add_custom_command(TARGET uno POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:uno>/assets"
    COMMENT "Copying assets to output directory..."
)