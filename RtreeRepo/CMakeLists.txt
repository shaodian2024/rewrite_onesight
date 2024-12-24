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
add_library(RtreeRepo SHARED
# add_library(RtreeRepo
  ${BE_SOURCES}
)

if(BUILD_FOR_macOS)
  message(STATUS "subdir build for macOS")
  set_target_properties(RtreeRepo PROPERTIES
    MACOSX_BUNDLE TRUE
    CMAKE_OSX_ARCHITECTURES "arm64;x86_64"  # 支持Universal Binary
    CMAKE_OSX_DEPLOYMENT_TARGET "11.0"      # macOS最低支持版本
    MACOSX_BUNDLE_BUNDLE_NAME "RtreeRepo"
    MACOSX_BUNDLE_BUNDLE_IDENTIFIER "com.onesight.RtreeRepo"
  )
elseif(BUILD_FOR_visionOS)
  message(STATUS "subdir build for visionOS")
  set_target_properties(RtreeRepo PROPERTIES
    ARCHITECTURES "arm64"
    MACOSX_BUNDLE TRUE
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.onesight.RtreeRepo"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.onesight.RtreeRepo"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
  )
elseif(BUILD_FOR_IOS)
  message(STATUS "subdir build for IOS")
  set_target_properties(RtreeRepo PROPERTIES
    ARCHITECTURES "arm64"
    XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "12.0"
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
