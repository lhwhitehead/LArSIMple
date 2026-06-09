cmake_minimum_required(VERSION 3.14)

project(LArSIMple LANGUAGES CXX)

option(USE_PANDORA "Build with Pandora SDK support" ON)
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and visualization" ON)

# ------------------------------------------------------------------

# Geant4

# ------------------------------------------------------------------

if(WITH_GEANT4_UIVIS)
find_package(Geant4 REQUIRED ui_all vis_all)
else()
find_package(Geant4 REQUIRED)
endif()

# ------------------------------------------------------------------

# ROOT

# ------------------------------------------------------------------

find_package(ROOT REQUIRED)

# ------------------------------------------------------------------

# Source files

# ------------------------------------------------------------------

include(cmake/Sources.cmake)

# ------------------------------------------------------------------

# Pandora support

# ------------------------------------------------------------------

if(USE_PANDORA)

find_package(PandoraSDK REQUIRED)
find_package(larpandoracontent REQUIRED)

include(cmake/PandoraSources.cmake)

endif()

# ------------------------------------------------------------------

# Executable

# ------------------------------------------------------------------

add_executable(
LArSIMple
LArSIMple.cc
${LARSIMPLE_SOURCES}
${LARSIMPLE_HEADERS}
)

target_include_directories(LArSIMple PRIVATE
${PROJECT_SOURCE_DIR}/include
${ROOT_INCLUDE_DIRS}
)

target_link_libraries(LArSIMple PRIVATE
${Geant4_LIBRARIES}
${ROOT_LIBRARIES}
${ROOT_LIB_EG}
z
)

# ------------------------------------------------------------------
# Optional dependency for Pandora file writing
# ------------------------------------------------------------------

if(USE_PANDORA)

target_compile_definitions(LArSIMple PRIVATE USE_PANDORA)

target_include_directories(LArSIMple PRIVATE
    ${larpandoracontent_INCLUDE_DIRS}
    ${PandoraSDK_INCLUDE_DIRS}
)

target_link_libraries(LArSIMple PRIVATE
    larpandoracontent::LArPandoraContent
    ${PandoraSDK_LIBRARIES}
)

endif()

target_compile_options(
LArSIMple PRIVATE
-Wno-misleading-indentation
)

# ------------------------------------------------------------------
# Install
# ------------------------------------------------------------------

install(TARGETS LArSIMple DESTINATION bin)

