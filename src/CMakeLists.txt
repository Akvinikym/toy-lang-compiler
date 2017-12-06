cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
project(ilang VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wextra -Wall -Wundef -Wpointer-arith -Wno-sign-compare -fno-rtti")

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
message(STATUS "include dirs: ${LLVM_INCLUDE_DIRS}")
add_definitions(${LLVM_DEFINITIONS})

find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIRS} )

find_package (Threads)

add_executable(voc
    codegen/codegen_visitor.cpp
    parsing/Lexer.cpp
    parsing/Parser.cpp
    parsing/Errors.cpp
    semantical_analysis/printname_visitor.cpp
    semantical_analysis/typechecking_visitor.cpp
    semantical_analysis/typededuce_visitor.cpp
    semantical_analysis/alias_unwrap_visitor.cpp
    semantical_analysis/const_eval_visitor.cpp
    main.cpp
    AST/integer.cpp
    AST/real.cpp
    AST/unary.cpp
    AST/binary.cpp
    AST/var.cpp
    AST/routinecall.cpp
    AST/prototype.cpp
    AST/routine.cpp
    AST/for.cpp
    AST/while.cpp
    AST/arraydecl.cpp
    AST/boolean.cpp
    AST/typedecl.cpp
    AST/assignment.cpp
    AST/return.cpp
    AST/cast.cpp
    semantical_analysis/error.cpp
)

llvm_map_components_to_libnames(llvm_libs all)

target_link_libraries(voc ${llvm_libs} ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
