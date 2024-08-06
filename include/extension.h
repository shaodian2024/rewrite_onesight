#pragma once
#include "RtreeRepo/sqlite3rtree.h"
namespace Extension{
class __declspec(dllexport) BasicExtensionManager {
public:
  BasicExtensionManager() = default;
  ~BasicExtensionManager() = default;
};

class __declspec(dllexport) CircleExtensionManager : public BasicExtensionManager{
public:
  CircleExtensionManager() = default;
  ~CircleExtensionManager() = default;
  // CircleExtensionManager* GetCircleExtensionManager() {
  //   static CircleExtensionManager circle_manager;
  //   return &circle_manager;
  // }
  static int Callback(void* data, int argc, char** argv, char** azColName);
  static void circle_del(void *p);
  static int circle_geom(sqlite3_rtree_geometry *p, int nCoord, sqlite3_rtree_dbl *aCoord, int *pRes);
  static void RegisterCircleFunction(sqlite3 *db, void* pRes);
};
/*
** Destructor function for Circle objects allocated by circle_geom().
*/
}
