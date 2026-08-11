cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("DoubleDecker")

# Add source to this project's executable.
add_executable (CMakeTarget "src/DoubleDecker.cpp" "src/Pieces/Pawn.cpp" "src/Pieces/Piece.cpp" "src/Position.cpp" "src/Board.cpp"  "src/Pieces/Knight.cpp" "src/Pieces/King.cpp" "src/Pieces/Rook.cpp" "src/Pieces/Bishop.cpp" "src/Pieces/Queen.cpp")
target_include_directories(CMakeTarget PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
