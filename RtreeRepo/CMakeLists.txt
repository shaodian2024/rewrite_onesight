cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(RtreeRepoWeb3 VERSION 1.0
    LANGUAGES C CXX
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
OPTION(BUILD_FOR_WINDOWS "build dll for windows" OFF)
OPTION(BUILD_FOR_ANDROID "build dll for android" OFF)
OPTION(BUILD_FOR_APPLE "build dll for apple" OFF)
OPTION(BUILD_FOR_WEB3 "build for web3" ON)
IF(BUILD_FOR_WEB3)
  set(CMAKE_CXX_STANDARD 14)
  message(STATUS "cmake cxx standard is modified to 14")
  set(CMAKE_TOOLCHAIN_FILE "${PROJECT_SOURCE_DIR}/../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
  message(STATUS "WEB3_CMAKE_TOOL_CHAIN is ${CMAKE_TOOLCHAIN_FILE}")
  include_directories("${PROJECT_SOURCE_DIR}/../emsdk/upstream/emscripten/cache/sysroot/include")
  
  if(EMSCRIPTEN)
    add_compile_options(-Wno-deprecated-declarations)
    set(CMAKE_C_COMPILER emcc)
    set(CMAKE_CXX_COMPILER em++)
  endif()  
ENDIF()
file (GLOB_RECURSE CXX_SOURCES 
  ./*.cpp
  # ./*.c
  # ./*.cc
  ./*/*.cpp
  # ./*/*.c
  # ./*/*.cc
  # ${OTHER_FILES}
)
file (GLOB_RECURSE C_SOURCES 
  # ./*.cpp
  ./*.c
  ./*.cc
  # ./*/*.cpp
  ./*/*.c
  ./*/*.cc
  # ${OTHER_FILES}
)
# set(CMAKE_CXX_COMPILE_OPTIONS
#   -s EXPORTED_FUNCTIONS='["_ApplyStringToGetId","_FreeIntArrayMemory"]'
#   -s SIDE_MODULE=1
#   -s WASM=1
#   -s STANDALONE_WASM
#   -s EXPORT_NAME=RtreeRepo
# )
# add_custom_target(build_wasm ALL
#     COMMAND em++ -Os --no-entry -s EXPORTED_FUNCTIONS='["_ApplyStringToGetId","_FreeIntArrayMemory"]'  
#                  -s SIDE_MODULE=1 -s WASM=1 -s STANDALONE_WASM -s EXPORT_NAME=RtreeRepo  
#                  ${BE_SOURCES} -o RtreeRepo.wasm  
#     DEPENDS ${BE_SOURCES}
#     COMMENT "Building RtreeRepo as a WebAssembly module"  
#     message(STATUS "RtreeRepo as a WebAssembly module")
# )
# message(STATUS "RtreeRepo as a WebAssembly module2")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# 定义要生成的输出文件
set(CXX_WASM_FILE "${CMAKE_CURRENT_BINARY_DIR}/RtreeRepo_CXX.wasm")

# 使用 add_custom_command 定义一个命令来生成 WebAssembly 模块
add_custom_command(
    OUTPUT ${CXX_WASM_FILE}
    COMMAND em++ -Os --no-entry -s EXPORTED_FUNCTIONS='["_ApplyStringToGetId","_FreeIntArrayMemory"]'  
                    -s SIDE_MODULE=1 -s WASM=1 -s STANDALONE_WASM -s EXPORT_NAME=RtreeRepo 
                    -I${CMAKE_CURRENT_SOURCE_DIR}/include  # 添加头文件路径
                    -I${CMAKE_CURRENT_SOURCE_DIR}/../
                    -I${CMAKE_CURRENT_SOURCE_DIR}/
                    -I${CMAKE_CURRENT_SOURCE_DIR}/../include
                    # -Wno-redefinition
                    -Wno-everything
                    ${CXX_SOURCES} -o ${CXX_WASM_FILE} 
    DEPENDS ${CXX_SOURCES}
    COMMENT "Building RtreeRepo C code as WebAssembly module"
    VERBATIM
)
set(C_WASM_FILE "${CMAKE_CURRENT_BINARY_DIR}/RtreeRepo_C.wasm")
add_custom_command(
    OUTPUT ${C_WASM_FILE}
    COMMAND emcc -Os --no-entry -s EXPORTED_FUNCTIONS='["_ApplyStringToGetId","_FreeIntArrayMemory"]'  
                    -s SIDE_MODULE=1 -s WASM=1 -s STANDALONE_WASM -s EXPORT_NAME=RtreeRepo 
                    -I${CMAKE_CURRENT_SOURCE_DIR}/include  # 添加头文件路径
                    -I${CMAKE_CURRENT_SOURCE_DIR}/../
                    -I${CMAKE_CURRENT_SOURCE_DIR}/
                    -I${CMAKE_CURRENT_SOURCE_DIR}/../include
                    # -Wno-redefinition
                    -Wno-everything
                    ${C_SOURCES} -o ${C_WASM_FILE} 
    DEPENDS ${C_SOURCES}
    COMMENT "Building RtreeRepo C code as WebAssembly module"
    VERBATIM
)
####################分割线1 begin#######################


# # # 使用 add_custom_command 定义一个命令来生成 WebAssembly 模块
# # add_custom_command(
# #     OUTPUT ${OUTPUT_WASM_FILE}
# #     COMMAND em++ -Os --no-entry -s EXPORTED_FUNCTIONS='["_ApplyStringToGetId","_FreeIntArrayMemory"]'  
# #                     -s SIDE_MODULE=1 -s WASM=1 -s STANDALONE_WASM -s EXPORT_NAME=RtreeRepo 
# #                     -I${CMAKE_CURRENT_SOURCE_DIR}/include  # 添加头文件路径
# #                     -I${CMAKE_CURRENT_SOURCE_DIR}/../
# #                     -I${CMAKE_CURRENT_SOURCE_DIR}/
# #                     -I${CMAKE_CURRENT_SOURCE_DIR}/../include
# #                     # -Wno-redefinition
# #                     -Wno-everything
# #                     ${BE_SOURCES} -o ${OUTPUT_WASM_FILE} 
# #     DEPENDS ${BE_SOURCES}
# #     COMMENT "Building RtreeRepo as a WebAssembly module"
# #     VERBATIM
# # )
# # add_custom_target(build_wasm ALL
# #     DEPENDS ${OUTPUT_WASM_FILE}
# # )

# # 使用 add_custom_command 定义 C 代码的编译
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/c_code.o
#     COMMAND emcc -Os --no-entry -s 
#                         -s SIDE_MODULE=1 -s WASM=1 -s STANDALONE_WASM -s 
#                         -I${CMAKE_CURRENT_SOURCE_DIR}/include  # 添加头文件路径
#                         -I${CMAKE_CURRENT_SOURCE_DIR}/../
#                         -I${CMAKE_CURRENT_SOURCE_DIR}/
#                         -I${CMAKE_CURRENT_SOURCE_DIR}/../include
#                         ${C_SOURCES} -o ${CMAKE_CURRENT_BINARY_DIR}/c_code.o
#     DEPENDS ${C_SOURCES}
#     COMMENT "Compiling C code to object file"
# )

# # 使用 add_custom_command 定义 C++ 代码的编译
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cpp_code.o
#     COMMAND em++ -Os --no-entry -s  
#                           -s SIDE_MODULE=1 -s WASM=1 -s STANDALONE_WASM -s 
#                           -I${CMAKE_CURRENT_SOURCE_DIR}/include  # 添加头文件路径
#                           -I${CMAKE_CURRENT_SOURCE_DIR}/../
#                           -I${CMAKE_CURRENT_SOURCE_DIR}/
#                           -I${CMAKE_CURRENT_SOURCE_DIR}/../include
#                           ${CXX_SOURCES} -o ${CMAKE_CURRENT_BINARY_DIR}/cpp_code.o
#     DEPENDS ${CXX_SOURCES}
#     COMMENT "Compiling C++ code to object file"
# )

# # 使用 add_custom_command 将 C 和 C++ 对象文件链接为 WebAssembly 模块
# add_custom_command(
#     OUTPUT ${OUTPUT_WASM_FILE}
#     COMMAND em++ -Os -o ${OUTPUT_WASM_FILE} ${CMAKE_CURRENT_BINARY_DIR}/c_code.o ${CMAKE_CURRENT_BINARY_DIR}/cpp_code.o
#     DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/c_code.o ${CMAKE_CURRENT_BINARY_DIR}/cpp_code.o
#     COMMENT "Linking object files to create WebAssembly module"
# )


####################分割线1 end#######################
# 定义自定义目标
add_custom_target(build_wasm ALL
    DEPENDS ${C_WASM_FILE} ${CXX_WASM_FILE}
)
# target_include_directories(build_wasm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
# ${CMAKE_CURRENT_SOURCE_DIR}/../
# ${CMAKE_CURRENT_SOURCE_DIR}/
# ${CMAKE_CURRENT_SOURCE_DIR}/../include
# )

# 定义一个自定义目标，依赖于生成的 WebAssembly 文件


# 可选：在构建时输出一条消息
message(STATUS "RtreeRepo as a WebAssembly module will be built.")











if(MSVC)
else()
    add_compile_options(-fPIC)
endif()
add_definitions(-DSQLITE_DQS=0)
add_definitions(-DSQLITE_DEFAULT_MEMSTATUS=0)
add_definitions(-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1)
add_definitions(-DSQLITE_LIKE_DOESNT_MATCH_BLOBS)
add_definitions(-DSQLITE_MAX_EXPR_DEPTH=0)
add_definitions(-DSQLITE_OMIT_DECLTYPE)
add_definitions(-DSQLITE_OMIT_DEPRECATED)
add_definitions(-DSQLITE_OMIT_PROGRESS_CALLBACK)
add_definitions(-DSQLITE_OMIT_SHARED_CACHE)
add_definitions(-DSQLITE_USE_ALLOCA)
add_definitions(-DSQLITE_USE_URI)
add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-D__BENTLEYDLL_BUILD__=1)
# add_definitions(-DNDEBUG)
if(NOT uppercase_CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -s")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -s")
elseif(uppercase_CMAKE_BUILD_TYPE MATCHES "DEBUG")
  add_definitions(-DDEBUG)
endif()
message (STATUS "CMAKE_CURRENT_SOURCE_DIR is ${CMAKE_CURRENT_SOURCE_DIR}")
message ("PROJECT_SOURCE_DIR is ${PROJECT_SOURCE_DIR}")



if(MSVC)
  set (OTHER_FILES    
    ./sqlite3.h
    ./sqlite3ext.h
  )
else()
  set (OTHER_FILES )
endif()



if (EMSCRIPTEN)
  # add_executable(RtreeRepo OBJECT
  add_library(RtreeRepo
    ${CXX_SOURCES}
    ${C_SOURCES}
  )
  # 设置输出文件后缀为 .wasm
  set_target_properties(
    RtreeRepo PROPERTIES OUTPUT_NAME "RtreeRepo"
    # SUFFIX ".wasm"
  )
  
  target_compile_options(RtreeRepo PRIVATE 
    -Os 
    --no-entry
  )

  # target_link_options(RtreeRepo PRIVATE 
  #     -s EXPORTED_FUNCTIONS='["_ApplyStringToGetId","_FreeIntArrayMemory"]'
  #     -s SIDE_MODULE=1
  #     -s WASM=1
  #     -s STANDALONE_WASM
  #     -s EXPORT_NAME=RtreeRepo
  # )

else()
  # 如果不是在 Emscripten 环境中，构建为普通静态库
  add_library(RtreeRepo SHARED
    ${BE_SOURCES}
  )
endif()

add_definitions(-DMYLIBRARY_EXPORTS=1)
add_definitions(-DBENTLEY_CPP_MISSING_WCHAR_SUPPORT=1)
add_definitions(-DNDEBUG)
add_definitions(-DSQLITE_DQS=0)
add_definitions(-DSQLITE_DEFAULT_MEMSTATUS=0)
add_definitions(-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1)
add_definitions(-DSQLITE_LIKE_DOESNT_MATCH_BLOBS)
add_definitions(-DSQLITE_MAX_EXPR_DEPTH=0)
add_definitions(-DSQLITE_OMIT_DECLTYPE)
add_definitions(-DSQLITE_OMIT_DEPRECATED)
add_definitions(-DSQLITE_OMIT_PROGRESS_CALLBACK)
add_definitions(-DSQLITE_OMIT_SHARED_CACHE)
add_definitions(-DSQLITE_USE_ALLOCA)
add_definitions(-DSQLITE_USE_URI)
add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-DNDEBUG)
add_definitions(-D__BENTLEYDLL_BUILD__=1)
add_definitions(-D__BE_SQLITE_HOST_DLL__=1)

add_compile_definitions(BENTLEY_WIN32)
add_compile_definitions(BENTLEY_WINRT)
# set_source_files_properties(sqlite3.c PROPERTIES LANGUAGE C)