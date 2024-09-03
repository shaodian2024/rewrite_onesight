#pragma once
#include <string>
#ifndef ONESIGHT_EXPORT
  #ifdef BUILD_FOR_WINDOWS
    #define ONESIGHT_EXPORT __declspec(dllexport)
  #else
    #ifdef BUILD_FOR_ANDROID
      #define ONESIGHT_EXPORT JNICALL
    #else
      #define ONESIGHT_EXPORT 
    #endif
  #endif
#endif
#ifdef BUILD_FOR_ANDROID
  # define JAVA_EXPORT JNIEXPORT
  #include <jni.h>
#else
  # define JAVA_EXPORT
#endif

typedef char Utf8Char;
typedef Utf8Char const* Utf8CP;
extern "C" {
	JAVA_EXPORT int ONESIGHT_EXPORT ApplyStringToGetId(const char* s, Utf8CP db_name, int** id_array, int** lods_after_sort, int* id_count);
	JAVA_EXPORT void ONESIGHT_EXPORT FreeIntArrayMemory(int* id_array);
}
