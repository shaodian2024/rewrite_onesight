#pragma once
#include "RtreeRepo/sqlite3ext.h"
namespace Extension{
#ifndef ONESIGHT_EXPORT
    #ifdef BUILD_FOR_WINDOWS
    # define ONESIGHT_EXPORT __declspec(dllexport)
  #else
    # define ONESIGHT_EXPORT
  #endif
#endif
class ONESIGHT_EXPORT ExtensionManager {
public:
  ExtensionManager() = default;
  ~ExtensionManager() = default;
  static int Callback(void* data, int argc, char** argv, char** azColName);
  static void circle_del(void *p);
  static int circle_geom(sqlite3_rtree_geometry *p, int nCoord, sqlite3_rtree_dbl *aCoord, int *pRes);
  static void RegisterCircleFunction(sqlite3 *db, void* pRes);
};
}
