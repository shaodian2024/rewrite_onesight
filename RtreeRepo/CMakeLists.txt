cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# project(RtreeRepoWeb3 VERSION 1.0
#     LANGUAGES C CXX
# )

set(CMAKE_C_COMPILER "D:/Unity_Hub/unity/2022.3.44f1c1/Editor/Data/PlaybackEngines/WebGLSupport/BuildTools/Emscripten/emscripten/emcc")
set(CMAKE_CXX_COMPILER "D:/Unity_Hub/unity/2022.3.44f1c1/Editor/Data/PlaybackEngines/WebGLSupport/BuildTools/Emscripten/emscripten/em++")
message("test2------------------------------------")
project(RtreeRepoWeb3 VERSION 1.0
    LANGUAGES C CXX
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sWHOLE_ARCHIVE=1")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sWHOLE_ARCHIVE=1")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sWHOLE_ARCHIVE=1")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--whole-archive")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--whole-archive")
message("asdasdfasdfasdfadsfasdf-------------------")
add_definitions(-DSQLITE_ENABLE_RTREE=1)
add_definitions(-DSQLITE_ENABLE_RTREE)
message("test------------------------------------")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s LLD_REPORT_UNDEFINED")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s LLD_REPORT_UNDEFINED")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    # 使用MSVC编译器的优化选项
    # add_compile_options(/O2)
    # shenghang modify /O2, need be unhidden.
    # include(mdl.mki)
else()
    # 使用GCC或Clang的优化选项
    add_compile_options(-O3)
    add_compile_options(-fPIC)
endif()
set(SQLITE_COMPILE_DEFINITIONS 
  -DSQLITE_ENABLE_RTREE=1 
  -DSQLITE_DQS=0 
  -DSQLITE_DEFAULT_MEMSTATUS=0
  -DSQLITE_LIKE_DOESNT_MATCH_BLOBS
  -DSQLITE_MAX_EXPR_DEPTH=0
  -DSQLITE_OMIT_DECLTYPE
  -DSQLITE_OMIT_DEPRECATED
  -DSQLITE_OMIT_PROGRESS_CALLBACK
  -DSQLITE_OMIT_SHARED_CACHE
  -DSQLITE_USE_ALLOCA
  -DSQLITE_USE_URI
  -DSQLITE_ENABLE_RTREE
  -D__BENTLEYDLL_BUILD__=1
  -O3
  -fPIC
)

message("SQLITE_COMPILE_DEFINITIONS is ${SQLITE_COMPILE_DEFINITIONS}")
add_definitions(${SQLITE_COMPILE_DEFINITIONS})

OPTION(BUILD_FOR_WINDOWS "build dll for windows" OFF)
OPTION(BUILD_FOR_ANDROID "build dll for android" OFF)
OPTION(BUILD_FOR_APPLE "build dll for apple" OFF)
OPTION(BUILD_FOR_WASM "build wasm for web3" OFF)
OPTION(BUILD_FOR_BC "build bc for web3" ON)
IF(BUILD_FOR_WASM)
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
file (GLOB_RECURSE C_CXX_SOURCES
  ./*.cpp
  ./*.c
  ./*.cc
  ./*/*.cpp
  ./*/*.c
  ./*/*.cc
  # ${OTHER_FILES}
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

IF(BUILD_FOR_WASM)

  # 定义要生成的输出文件
  set(CXX_WASM_FILE "${CMAKE_CURRENT_BINARY_DIR}/RtreeRepo_CXX.wasm")

  # 使用 add_custom_command 定义一个命令来生成 WebAssembly 模块
  add_custom_command(
      OUTPUT ${CXX_WASM_FILE}
      COMMAND em++ -Os --no-entry -s
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
      COMMAND emcc -Os --no-entry -s
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
ENDIF()

IF(BUILD_FOR_BC)

# 为每个源文件生成.bc文件  
foreach(SRC ${C_SOURCES})  
    get_filename_component(SRC_NAME ${SRC} NAME_WE)  
    get_filename_component(SRC_EXT ${SRC} EXT)  
    string(REPLACE "." "_" BC_FILE_NAME "${SRC_NAME}${SRC_EXT}_bc")  
    set(BC_FILES ${BC_FILES} ${BC_FILE_NAME}.bc)  
  
    # 添加自定义命令来生成.bc文件  
    add_custom_command(  
        OUTPUT ${BC_FILE_NAME}.bc  
        COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} ${SQLITE_COMPILE_DEFINITIONS} -emit-llvm -c ${SRC} -o ${BC_FILE_NAME}.bc                       
        -I${CMAKE_CURRENT_SOURCE_DIR}/include  # 添加头文件路径
        -I${CMAKE_CURRENT_SOURCE_DIR}/../
        -I${CMAKE_CURRENT_SOURCE_DIR}/
        -I${CMAKE_CURRENT_SOURCE_DIR}/../include
        -DSQLITE_ENABLE_RTREE=1
        # 如果源文件是C++，需要调整编译器和可能的其他选项  
        COMMAND_EXPAND_LISTS  
        DEPENDS ${SRC}  
        COMMENT "Compiling ${SRC} to LLVM Bitcode"  
    )  
endforeach()  
foreach(SRC ${CXX_SOURCES})  
    get_filename_component(SRC_NAME ${SRC} NAME_WE)  
    get_filename_component(SRC_EXT ${SRC} EXT)  
    string(REPLACE "." "_" BC_FILE_NAME "${SRC_NAME}${SRC_EXT}_bc")  
    set(BC_FILES ${BC_FILES} ${BC_FILE_NAME}.bc)  
  
    # 添加自定义命令来生成.bc文件  
    add_custom_command(  
        OUTPUT ${BC_FILE_NAME}.bc  
        COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} ${SQLITE_COMPILE_DEFINITIONS} -emit-llvm -c ${SRC} -o ${BC_FILE_NAME}.bc                       
        -I${CMAKE_CURRENT_SOURCE_DIR}/include  # 添加头文件路径
        -I${CMAKE_CURRENT_SOURCE_DIR}/../
        -I${CMAKE_CURRENT_SOURCE_DIR}/
        -I${CMAKE_CURRENT_SOURCE_DIR}/../include
        -DSQLITE_ENABLE_RTREE=1
        # 如果源文件是C++，需要调整编译器和可能的其他选项  
        COMMAND_EXPAND_LISTS  
        DEPENDS ${SRC}  
        COMMENT "Compiling ${SRC} to LLVM Bitcode"  
    )  
endforeach()  
  
# 创建一个自定义目标来链接所有的.bc文件到一个单一的.bc文件中（可选）  
add_custom_target(link_bitcode ALL  
    COMMAND llvm-link ${BC_FILES} -o final.bc  
    DEPENDS ${BC_FILES}  
    COMMENT "Linking LLVM Bitcode files"  
)  
  
# # 让'make'默认构建link_bitcode目标  
# add_dependencies(all link_bitcode)



  # add_custom_command(
  #     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/axi.bc
  #     COMMAND ${CMAKE_C_COMPILER} -c -emit-llvm -o ${CMAKE_CURRENT_BINARY_DIR}/axi.bc ${CXX_SOURCES} ${C_SOURCES}
  #     DEPENDS ${CXX_SOURCES} ${C_SOURCES}
  #     COMMENT "Generating LLVM bitcode for SQLite"
  # )

  # # 添加自定义目标以便于构建
  # add_custom_target(GenerateBC ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/axi.bc)

  
ENDIF()
add_custom_target(build_wasm ALL
    DEPENDS ${C_WASM_FILE} ${CXX_WASM_FILE}
)

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
# set_target_properties(final.bc PROPERTIES LINK_FLAGS "-s EXPORTED_FUNCTIONS='[\"sqlite3_rtree_query_callback\", \"sqlite3_rtree_geometry_callback\"]'")
# set_source_files_properties(sqlite3.c PROPERTIES LANGUAGE C)