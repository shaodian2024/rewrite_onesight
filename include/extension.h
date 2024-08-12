#pragma once
#include <string>
#include "RtreeRepo/sqlite3rtree.h"
namespace Extension{
#define SUCCESS 0;
#define FAILED 1;
class BasicFunction;
class __declspec(dllexport) BasicExtensionManager {
public:
  BasicExtensionManager() = default;
  ~BasicExtensionManager() = default;
  static BasicExtensionManager* GetBasicExtensionManager() {
    static BasicExtensionManager manager;
    return &manager;
  }
  int OpenDb(const char* db_name, sqlite3*& open_db);
  int CloseDb(sqlite3 *db);
  int RegisterFunction(sqlite3* db, BasicFunction* function);
  // static int Callback();
  int RunFunction();
};

class BasicFunction {
public:
  BasicFunction() = default;
  ~BasicFunction() = default;
  enum class FunctionType : uint8_t{
    kUnknown = 0,
    kBoxQuery = 1, 
    kPlaneQuery = 2
  };
  std::string GetFunctionName() const { return function_name_; }
  void SetFunctionName(std::string function_name) { function_name_ = function_name; }
  virtual int TestRange(sqlite3_rtree_query_info& info) = 0;
  FunctionType GetType() { return type_; }

protected:
  FunctionType type_ = FunctionType::kUnknown;
  std::string function_name_;
};

class BoxQueryFunction : public BasicFunction {
public: 
  BoxQueryFunction() {
    type_ = FunctionType::kBoxQuery;
    function_name_ = "iModel_spatial_overlap_aabb";
  }
  ~BoxQueryFunction() = default;
  int TestRange(sqlite3_rtree_query_info& info);
};

class PlaneQueryFucntion : public BasicFunction {
public:
  PlaneQueryFucntion() {
    type_ = FunctionType::kPlaneQuery;
    function_name_ = "onesight_spatial_overlap_aabb";
  }
  ~PlaneQueryFucntion() = default;
  int TestRange(sqlite3_rtree_query_info& info);
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
