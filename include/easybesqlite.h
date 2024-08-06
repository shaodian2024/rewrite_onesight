#pragma once
#include "RtreeRepo/sqlite3ext.h"
namespace Extension{
class __declspec(dllexport) ExtensionManager {
public:
  ExtensionManager() = default;
  ~ExtensionManager() = default;
  static int Callback(void* data, int argc, char** argv, char** azColName);
  static void circle_del(void *p);
  static int circle_geom(sqlite3_rtree_geometry *p, int nCoord, sqlite3_rtree_dbl *aCoord, int *pRes);
  static void RegisterCircleFunction(sqlite3 *db, void* pRes);
};
}
