cmake_minimum_required(VERSION 3.5...4.0)

project(Uno VERSION 1.0)

# Common
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

# Debug
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/lib)

# Release
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/lib)

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
function(auto_source_group_by_folder FILES IS_HEADER)
    foreach(file ${${FILES}})
        get_filename_component(dir "${file}" PATH)
        file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}" "${dir}")

        string(REPLACE "/" ";" parts "${rel_path}")
        set(group_parts "")
		
        foreach(part ${parts})
            list(APPEND group_parts "${part}")
        endforeach()

        if(IS_HEADER)
            list(APPEND group_parts "headers")
        endif()

        string(REPLACE ";" "\\" group "${group_parts}")

        source_group("${group}" FILES "${file}")
    endforeach()
endfunction()

# ----------------------------------------------
add_executable(uno)

# ----------------------------------------------
file(GLOB_RECURSE headers ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

target_sources(uno PRIVATE ${sources} ${headers})

auto_source_group_by_folder(headers TRUE)
auto_source_group_by_folder(sources FALSE)

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

# ----------------------------------------------
set_target_properties(uno PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "uno")