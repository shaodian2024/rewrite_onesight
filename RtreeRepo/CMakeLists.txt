if(MSVC)
  set (OTHER_FILES    
    ./sqlite3.h
    ./sqlite3ext.h
  )
else()
  set (OTHER_FILES )
endif()
file (GLOB_RECURSE BE_SOURCES 
  ./*.cpp
  ./*.c
  ./*.cc
  ./*/*.cpp
  ./*/*.c
  ./*/*.cc
  ${OTHER_FILES}
)
#for windows
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if (EMSCRIPTEN)
  # 添加库目标
  add_library(RtreeRepo SHARED
    ${BE_SOURCES}
  )

  # 设置输出文件后缀为 .wasm
  set_target_properties(RtreeRepo PROPERTIES OUTPUT_NAME "wasmRtreeRepo")
  
  # 设置编译和链接选项
  target_compile_options(RtreeRepo PRIVATE 
    -Os 
    --no-entry
  )

  # 设置链接选项，添加导出函数
  target_link_options(RtreeRepo PRIVATE 
      -s EXPORTED_FUNCTIONS='["_ApplyStringToGetId","_FreeIntArrayMemory"]'
      -s SIDE_MODULE=1
      -s WASM=1
      -s STANDALONE_WASM
      -s EXPORT_NAME=RtreeRepoWasm
  )

else()
  # 如果不是在 Emscripten 环境中，构建为普通静态库
  add_library(RtreeRepo SHARED
    ${BE_SOURCES}
  )
endif()
  # link_directories(${PROJECT_SOURCE_DIR}/third_party)
  # find_library(SHLWAPI_LIBRARY ShLwApi PATHS ${PROJECT_SOURCE_DIR}/third_party)
  # target_link_libraries(RtreeRepo ${SHLWAPI_LIBRARY})

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