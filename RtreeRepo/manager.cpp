#include "extension.h"
#include <iostream>
namespace Extension {
int BasicExtensionManager::OpenDb(const char* db_name, sqlite3*& db) {
  sqlite3* open_db;
  char* error_message;
  auto open_result = sqlite3_open(db_name, &open_db);
  if (open_result != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(open_db));
    sqlite3_close(open_db);
    return FAILED;
  } else {
    db = open_db;
    std::cout << "open database success" << std::endl;
    return SUCCESS;
  }
}

int BasicExtensionManager::CloseDb(sqlite3* db) {
  return sqlite3_close(db);
}

static int rTreeMatch(sqlite3_rtree_query_info& info) {
  return ((BasicFunction*) info.pContext)->TestRange(info);
}

int BasicExtensionManager::RegisterFunction(sqlite3* db, BasicFunction* function) {
  return sqlite3_rtree_query_callback(db, function->GetFunctionName().c_str(), 
    (int(*)(sqlite3_rtree_query_info*)) rTreeMatch, function, nullptr);
}

}