cmake_minimum_required (VERSION 3.10)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ChessEngine")

find_package(SDL3 CONFIG REQUIRED)

add_executable (ChessEngine "src/main.cpp" "src/Pieces/Pawn.cpp" "src/Pieces/Piece.cpp" "src/Position.cpp" "src/Board.cpp"  "src/Pieces/Knight.cpp" "src/Pieces/King.cpp" "src/Pieces/Rook.cpp" "src/Pieces/Bishop.cpp" "src/Pieces/Queen.cpp" "src/GUI/Renderer.cpp" "src/Game.cpp")

target_include_directories(ChessEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ChessEngine PRIVATE SDL3::SDL3)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChessEngine PROPERTY CXX_STANDARD 20)
endif()
