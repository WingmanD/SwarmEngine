cmake_minimum_required(VERSION 3.0)
project(SwarmEngineOGL)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(SwarmEngineOGL/dependencies/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
option(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_OBJ_EXPORTER TRUE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
add_subdirectory(SwarmEngineOGL/dependencies/assimp)

file(COPY SwarmEngineOGL/dependencies/imgui/imgui.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/imconfig.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/imgui_internal.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/imstb_rectpack.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/imstb_textedit.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/imstb_truetype.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/imgui/)

file(COPY SwarmEngineOGL/dependencies/imgui/imgui.cpp DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/sources/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/imgui_draw.cpp DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/sources/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/imgui_tables.cpp DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/sources/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/imgui_widgets.cpp DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/sources/imgui/)


file(COPY SwarmEngineOGL/dependencies/imgui/backends/imgui_impl_glfw.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/backends/imgui_impl_opengl3.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/backends/imgui_impl_opengl3_loader.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/imgui/)

file(COPY SwarmEngineOGL/dependencies/imgui/backends/imgui_impl_glfw.cpp DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/sources/imgui/)
file(COPY SwarmEngineOGL/dependencies/imgui/backends/imgui_impl_opengl3.cpp DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/sources/imgui/)


file(COPY SwarmEngineOGL/dependencies/tinyxml/tinyxml2.h DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/headers/tinyxml/)
file(COPY SwarmEngineOGL/dependencies/tinyxml/tinyxml2.cpp DESTINATION ../SwarmEngineOGL/SwarmEngineOGL/sources/tinyxml/)

add_subdirectory(SwarmEngineOGL/SwarmEngineOGL)
