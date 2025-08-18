cmake_minimum_required(VERSION 3.10)
project(ambarc_llvm)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Configurações para Flex e Bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

flex_target(LEXER src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
bison_target(PARSER src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)

add_executable(minillvm
    src/main.cpp
    src/ast.hpp
    src/codegen.hpp
    ${BISON_PARSER_OUTPUTS}
    ${FLEX_LEXER_OUTPUTS}
)

target_link_libraries(minillvm
    LLVMCore
    LLVMSupport
    LLVMIRReader
    LLVMAsmParser
)