cmake_minimum_required (VERSION 3.26)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SoftwareRenderer")

add_executable (SoftwareRenderer "src/Main.cpp"  "src/Utils.h" "src/Common.h"  "src/Graphics.h"  "src/Graphics.cpp"  "src/Input.h" "src/Input.cpp"   )

set_property(TARGET SoftwareRenderer PROPERTY CXX_STANDARD 23)

if (MSVC)
    target_compile_options(SoftwareRenderer PRIVATE /W3)
else()
    target_compile_options(SoftwareRenderer PRIVATE -Wall)
endif()

add_subdirectory("SDL")

target_link_libraries(SoftwareRenderer SDL3::SDL3)

target_compile_definitions(SoftwareRenderer PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)
