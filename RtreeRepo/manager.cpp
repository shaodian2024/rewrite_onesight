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

int BasicExtensionManager::OpenSerializedDb(const char* db_name, unsigned char* serialized_data, sqlite3_int64 db_size, sqlite3*& db) {
  const char* dbName = "my_database"; // 数据库名称

  // 打开或创建数据库连接
  if (sqlite3_open(dbName, &db) != SQLITE_OK) {
    std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    return -1;
  }
  // 使用 sqlite3_deserialize 恢复数据库
  int result = sqlite3_deserialize(db, "main", serialized_data, db_size, db_size, 0);
  
  if (result != SQLITE_OK) {
    std::cerr << "Failed to deserialize database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return -1;
  }

  std::cout << "Database deserialized successfully!" << std::endl;

  return 0;
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