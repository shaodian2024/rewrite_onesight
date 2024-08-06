#include <string>
#include "RtreeRepo/sqlite3rtree.h"
#include "extension.h"
#include "sqlite3rtree.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  std::string db_name = "test.osdb";
  sqlite3* db;
  char* error_message;
  auto open_result = sqlite3_open(db_name.c_str(), &db);
  if (open_result != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  int *pRes = 0;
  Extension::CircleExtensionManager::RegisterCircleFunction(db, pRes);
  if (pRes != 0) {
    std::cout << "find pRes" << pRes << std::endl;
  }
  std::string command;


  std::vector<const char*> sql_commands = {
    "CREATE VIRTUAL TABLE myRTree USING rtree(id, minX, maxX, minY, maxY);",
    "INSERT INTO myRTree VALUES (1, 1, 2, 1, 2);",
    "INSERT INTO myRTree VALUES (9, 1.8, 3, 1.8, 3);",
    "INSERT INTO myRTree VALUES (13, -15, 15, 1.8, 2.2);"
  };

  for (const auto& sql : sql_commands) {
    auto rc = sqlite3_exec(db, sql, NULL, 0, &error_message);
    if (rc != SQLITE_OK) {
      fprintf(stderr, "error command: %s\n", sql);
      fprintf(stderr, "SQL error: %s\n", error_message);
      sqlite3_free(error_message);
      sqlite3_close(db);
      return 1;
    }
  }

  std::vector<int> matching_ids;
  const char* select_sql = "SELECT id FROM myRTree WHERE id MATCH circle(0.0, 0.0, 2.0);";
  auto rc = sqlite3_exec(db, select_sql, Extension::CircleExtensionManager::Callback, &matching_ids, &error_message);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", error_message);
    sqlite3_free(error_message);
    sqlite3_close(db);
    return 1;
  }
  std::cout << "Matching ids: ";
  for (int id : matching_ids) {
    std::cout << id << " ";
  }
  std::cout << std::endl;
  
  sqlite3_close(db);
  return 0;
}