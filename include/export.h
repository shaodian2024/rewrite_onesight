#pragma once
#include <string>
typedef char Utf8Char;
typedef Utf8Char const* Utf8CP;
extern "C" {
	int __declspec(dllexport) ApplyStringToGetId(const char* s, Utf8CP db_name, int** id_array, int** lods_after_sort, int* id_count);
	void __declspec(dllexport) FreeIntArrayMemory(int* id_array);
}