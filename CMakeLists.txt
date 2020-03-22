cmake_minimum_required(VERSION 3.0)
project(Glitter)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Glitter/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Glitter/Vendor/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(Glitter/Vendor/bullet)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Glitter/Headers/
                    Glitter/Vendor/assimp/include/
                    Glitter/Vendor/bullet/src/
                    Glitter/Vendor/glad/include/
                    Glitter/Vendor/glfw/include/
                    Glitter/Vendor/glm/
                    Glitter/Vendor/stb/)

file(GLOB VENDORS_SOURCES Glitter/Vendor/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Glitter/Headers/*.hpp)
file(GLOB PROJECT_SOURCES Glitter/Sources/*.cpp)
file(GLOB PROJECT_SHADERS Glitter/Shaders/*.comp
                          Glitter/Shaders/*.frag
                          Glitter/Shaders/*.geom
                          Glitter/Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)


add_executable(HelloTriangle Samples/HelloTriangle/main.cpp ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_link_libraries(HelloTriangle assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)

add_executable(HelloRectangle Samples/HelloRectangle/main.cpp ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_link_libraries(HelloRectangle assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)

add_executable(HelloShader Samples/HelloShader/main.cpp ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_link_libraries(HelloShader assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)

add_executable(HelloShaderClass Samples/HelloShaderClass/main.cpp ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_link_libraries(HelloShaderClass assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)

add_custom_command(
    TARGET HelloShaderClass POST_BUILD
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/shaders
    COMMAND cp -rf  ${CMAKE_SOURCE_DIR}/Samples/HelloShaderClass/HSC* ${CMAKE_BINARY_DIR}/shaders )

add_executable(HelloTexture Samples/HelloTexture/main.cpp ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_link_libraries(HelloTexture assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)

add_custom_command(
    TARGET HelloTexture POST_BUILD
    COMMAND cp -rf  ${CMAKE_SOURCE_DIR}/Samples/HelloTexture/HT* ${CMAKE_BINARY_DIR}/shaders 
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/images
    COMMAND cp -rf  ${CMAKE_SOURCE_DIR}/Samples/images/* ${CMAKE_BINARY_DIR}/images/.)

add_executable(HelloTextureBlend Samples/HelloTextureBlend/main.cpp ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_link_libraries(HelloTextureBlend assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)

add_custom_command(
    TARGET HelloTextureBlend POST_BUILD
    COMMAND cp -rf  ${CMAKE_SOURCE_DIR}/Samples/HelloTextureBlend/HTB* ${CMAKE_BINARY_DIR}/shaders)

add_executable(LoadModeltest Samples/LoadModel/main.cpp Samples/LoadModel/stb_image.cpp ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_link_libraries(LoadModeltest assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)

add_custom_command(
    TARGET LoadModeltest POST_BUILD
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/LoadModel
    COMMAND mv -f ${CMAKE_BINARY_DIR}/LoadModeltest ${CMAKE_BINARY_DIR}/LoadModel
    COMMAND cp -rf  ${CMAKE_SOURCE_DIR}/Samples/LoadModel/vert.vert ${CMAKE_BINARY_DIR}/LoadModel 
    COMMAND cp -rf  ${CMAKE_SOURCE_DIR}/Samples/LoadModel/frag.frag ${CMAKE_BINARY_DIR}/LoadModel 
    COMMAND cp -rf  ${CMAKE_SOURCE_DIR}/Samples/LoadModel/nanosuit ${CMAKE_BINARY_DIR}/LoadModel/)


set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Glitter/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})
