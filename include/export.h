#pragma once
// #ifdef __EMSCRIPTEN__
// #define DEPRECATED(msg) __attribute__((deprecated(msg)))
// #else
// #define DEPRECATED(msg) __attribute__((deprecated))
// #endif
#include <emscripten.h>
#include <string>
// #ifndef ONESIGHT_EXPORT_WEB3
//   #define ONESIGHT_EXPORT_WEB3 EMSCRIPTEN_KEEPALIVE
// #endif
typedef char Utf8Char;
typedef Utf8Char const* Utf8CP;
extern "C" {
  EMSCRIPTEN_KEEPALIVE int StringForWeb3Db(const char* function_name, unsigned char* serialized_data, int64_t db_size, int** ids_after_sort, 
    int**lods_after_sort, int* id_count);
  EMSCRIPTEN_KEEPALIVE int ApplyStringToGetId(const char* function, Utf8CP db_name, int** id_array, int** lods_after_sort, int* id_count);
  EMSCRIPTEN_KEEPALIVE void FreeIntArrayMemory(int* id_array);
  EMSCRIPTEN_KEEPALIVE void TestFunction() {;}
}