#define _USE_MATH_DEFINES
#include "extension.h"
#include <stdio.h>
#include <vector>
#include "sqlite3.h"
#include "export.h"
#include <cmath>
#include <algorithm>
#include <regex>
#define _TRY_WASM_ 1
#if !_TRY_WASM_
namespace Extension {
struct DPoint3d
{
//! x coordinate
double x;
//! y coordinate
double y;
//! z coordinate
double z;
};

struct DRange3d  {DPoint3d low, high;};
using DRange3dCP = const DRange3d*;
using DRange3dR = DRange3d&;


enum class Within : int
{
    Outside = 0,
    Partly = 1,
    Inside = 2,
};
class FrustumChecker {
public:
    static Within CheckBoxWithFrustum(
        double p0nx, double p0ny, double p0nz, double p0d,
        double p1nx, double p1ny, double p1nz, double p1d,
        double p2nx, double p2ny, double p2nz, double p2d,
        double p3nx, double p3ny, double p3nz, double p3d,
        double p4nx, double p4ny, double p4nz, double p4d,
        double p5nx, double p5ny, double p5nz, double p5d,
        double minx, double maxx,
        double miny, double maxy,
        double minz, double maxz)
    {
        if (IsBoxOutsideFrustum(
            p0nx, p0ny, p0nz, p0d,
            p1nx, p1ny, p1nz, p1d,
            p2nx, p2ny, p2nz, p2d,
            p3nx, p3ny, p3nz, p3d,
            p4nx, p4ny, p4nz, p4d,
            p5nx, p5ny, p5nz, p5d,
            minx, maxx,
            miny, maxy,
            minz, maxz
        ))
        {
            return Within::Outside;
        }
        else if (IsBoxInsideFrustum(
            p0nx, p0ny, p0nz, p0d,
            p1nx, p1ny, p1nz, p1d,
            p2nx, p2ny, p2nz, p2d,
            p3nx, p3ny, p3nz, p3d,
            p4nx, p4ny, p4nz, p4d,
            p5nx, p5ny, p5nz, p5d,
            minx, maxx,
            miny, maxy,
            minz, maxz
        ))
        {
            return Within::Inside;
        }
        else return Within::Partly;
    }

private:
    static bool IsBoxOutsideFrustum(
        double p0nx, double p0ny, double p0nz, double p0d,
        double p1nx, double p1ny, double p1nz, double p1d,
        double p2nx, double p2ny, double p2nz, double p2d,
        double p3nx, double p3ny, double p3nz, double p3d,
        double p4nx, double p4ny, double p4nz, double p4d,
        double p5nx, double p5ny, double p5nz, double p5d,
        double minx, double maxx,
        double miny, double maxy,
        double minz, double maxz)
    {
        if (IsBehindPlane(p0nx, p0ny, p0nz, p0d, minx, maxx, miny, maxy, minz, maxz)) return true;
        if (IsBehindPlane(p1nx, p1ny, p1nz, p1d, minx, maxx, miny, maxy, minz, maxz)) return true;
        if (IsBehindPlane(p2nx, p2ny, p2nz, p2d, minx, maxx, miny, maxy, minz, maxz)) return true;
        if (IsBehindPlane(p3nx, p3ny, p3nz, p3d, minx, maxx, miny, maxy, minz, maxz)) return true;
        if (IsBehindPlane(p4nx, p4ny, p4nz, p4d, minx, maxx, miny, maxy, minz, maxz)) return true;
        if (IsBehindPlane(p5nx, p5ny, p5nz, p5d, minx, maxx, miny, maxy, minz, maxz)) return true;

        return false;
    }

    static bool IsBoxInsideFrustum(
        double p0nx, double p0ny, double p0nz, double p0d,
        double p1nx, double p1ny, double p1nz, double p1d,
        double p2nx, double p2ny, double p2nz, double p2d,
        double p3nx, double p3ny, double p3nz, double p3d,
        double p4nx, double p4ny, double p4nz, double p4d,
        double p5nx, double p5ny, double p5nz, double p5d,
        double minx, double maxx,
        double miny, double maxy,
        double minz, double maxz)
    {
        return IsFrontPlane(p0nx, p0ny, p0nz, p0d, minx, maxx, miny, maxy, minz, maxz) &&
            IsFrontPlane(p1nx, p1ny, p1nz, p1d, minx, maxx, miny, maxy, minz, maxz) &&
            IsFrontPlane(p2nx, p2ny, p2nz, p2d, minx, maxx, miny, maxy, minz, maxz) &&
            IsFrontPlane(p3nx, p3ny, p3nz, p3d, minx, maxx, miny, maxy, minz, maxz) &&
            IsFrontPlane(p4nx, p4ny, p4nz, p4d, minx, maxx, miny, maxy, minz, maxz) &&
            IsFrontPlane(p5nx, p5ny, p5nz, p5d, minx, maxx, miny, maxy, minz, maxz);
    }

    static bool IsBehindPlane(
        double pnx, double pny, double pnz, double pd,
        double minx, double maxx,
        double miny, double maxy,
        double minz, double maxz)
    {
        double pVertexX = pnx >= 0 ? maxx : minx;
        double pVertexY = pny >= 0 ? maxy : miny;
        double pVertexZ = pnz >= 0 ? maxz : minz;

        double d = pnx * pVertexX + pny * pVertexY + pnz * pVertexZ + pd;

        return d < 0;
    }

    static bool IsFrontPlane(
        double pnx, double pny, double pnz, double pd,
        double minx, double maxx,
        double miny, double maxy,
        double minz, double maxz)
    {
        double nVertexX = pnx >= 0 ? minx : maxx;
        double nVertexY = pny >= 0 ? miny : maxy;
        double nVertexZ = pnz >= 0 ? minz : maxz;

        double d = pnx * nVertexX + pny * nVertexY + pnz * nVertexZ + pd;

        return d >= 0;
    }
};

struct frustum_plane {
    double normal_x = 0;
    double normal_y = 0;
    double normal_z = 0;
    double d        = 0;
};


using DRange3dCR = const DRange3d&;
struct RTree3dVal
{
    double m_minx, m_maxx, m_miny, m_maxy, m_minz, m_maxz;

    RTree3dVal() {Invalidate();}
    RTree3dVal(DRange3dCR range) {FromRange(range);}
    void Invalidate() {m_minx=m_miny=m_minz=1; m_maxx=m_maxy=m_maxz=-1;}
    double Margin() const {return (m_maxx-m_minx) + (m_maxy-m_miny) + (m_maxz-m_minz);}
    double Margin2d() const {return (m_maxx-m_minx) + (m_maxy-m_miny);}
    void ToRangeR(DRange3dR range) const {range.low.x=m_minx; range.low.y=m_miny; range.low.z=m_minz; range.high.x=m_maxx; range.high.y=m_maxy; range.high.z=m_maxz;}
    DRange3d ToRange() const {DRange3d range; ToRangeR(range); return range;}
    void FromRange(DRange3dCR range) {m_minx=range.low.x; m_maxx=range.high.x; m_miny=range.low.y; m_maxy=range.high.y; m_minz=range.low.z; m_maxz=range.high.z;}
    bool IsValid() const {return m_maxx>=m_minx && m_maxy>=m_miny && m_maxz>=m_minz;}
    void Union(RTree3dVal const& other) {m_minx=std::min(m_minx,other.m_minx); m_miny=std::min(m_miny,other.m_miny); m_minz=std::min(m_minz,other.m_minz);
                                         m_maxx=std::max(m_maxx,other.m_maxx); m_maxy=std::max(m_maxy,other.m_maxy); m_maxz=std::max(m_maxz,other.m_maxz);}
    bool Contains(RTree3dVal const& other) const   {return m_minx<=other.m_minx && m_miny<=other.m_miny && m_minz<=other.m_minz &&
                                                           m_maxx>=other.m_maxx && m_maxy>=other.m_maxy && m_maxz>=other.m_maxz;}
    bool Intersects(RTree3dVal const& other) const {return m_minx<=other.m_maxx && m_maxx>=other.m_minx &&
                                                           m_miny<=other.m_maxy && m_maxy>=other.m_miny &&
                                                           m_minz<=other.m_maxz && m_maxz>=other.m_minz;}
    bool Intersection(RTree3dVal const& left, RTree3dVal const& right) 
                                                            {
                                                            m_minx = left.m_minx > right.m_minx ? left.m_minx : right.m_minx;
                                                            m_miny = left.m_miny > right.m_miny ? left.m_miny : right.m_miny;
                                                            m_minz = left.m_minz > right.m_minz ? left.m_minz : right.m_minz;
                                                            m_maxx = left.m_maxx < right.m_maxx ? left.m_maxx : right.m_maxx;
                                                            m_maxy = left.m_maxy < right.m_maxy ? left.m_maxy : right.m_maxy;
                                                            m_maxz = left.m_maxz < right.m_maxz ? left.m_maxz : right.m_maxz;
                                                            return IsValid();
                                                            }
    bool Intersects2d(RTree3dVal const& other) const {return m_minx<= other.m_maxx && m_maxx>= other.m_minx &&
                                                            m_miny<= other.m_maxy && m_maxy>= other.m_miny ;}
};
typedef RTree3dVal const* RTree3dValCP;

int PlaneQueryFucntion::TestRange(sqlite3_rtree_query_info &info) {
    info.eWithin = static_cast<int>(Within::Outside);
    frustum_plane plane1, plane2, plane3, plane4, plane5, plane6;

    plane1.normal_x = sqlite3_value_double(info.apSqlParam[0]);
    plane1.normal_y = sqlite3_value_double(info.apSqlParam[1]);
    plane1.normal_z = sqlite3_value_double(info.apSqlParam[2]);
    plane1.d        = sqlite3_value_double(info.apSqlParam[3]);
    plane2.normal_x = sqlite3_value_double(info.apSqlParam[4]);
    plane2.normal_y = sqlite3_value_double(info.apSqlParam[5]);
    plane2.normal_z = sqlite3_value_double(info.apSqlParam[6]);
    plane2.d        = sqlite3_value_double(info.apSqlParam[7]);
    plane3.normal_x = sqlite3_value_double(info.apSqlParam[8]);
    plane3.normal_y = sqlite3_value_double(info.apSqlParam[9]);
    plane3.normal_z = sqlite3_value_double(info.apSqlParam[10]);
    plane3.d        = sqlite3_value_double(info.apSqlParam[11]);
    plane4.normal_x = sqlite3_value_double(info.apSqlParam[12]);
    plane4.normal_y = sqlite3_value_double(info.apSqlParam[13]);
    plane4.normal_z = sqlite3_value_double(info.apSqlParam[14]);
    plane4.d        = sqlite3_value_double(info.apSqlParam[15]);
    plane5.normal_x = sqlite3_value_double(info.apSqlParam[16]);
    plane5.normal_y = sqlite3_value_double(info.apSqlParam[17]);
    plane5.normal_z = sqlite3_value_double(info.apSqlParam[18]);
    plane5.d        = sqlite3_value_double(info.apSqlParam[19]);
    plane6.normal_x = sqlite3_value_double(info.apSqlParam[20]);
    plane6.normal_y = sqlite3_value_double(info.apSqlParam[21]);
    plane6.normal_z = sqlite3_value_double(info.apSqlParam[22]);
    plane6.d        = sqlite3_value_double(info.apSqlParam[23]);
    RTree3dValCP pt = (RTree3dValCP)info.aCoord;
 
    if (info.eParentWithin == static_cast<int>(Within::Outside)) {
        return SQLITE_OK;
    }

    // bool passedTest = (info.eParentWithin == Within::Inside) ? true : bounds.Intersects(*pt);
    // if (!passedTest)
       // return BE_SQLITE_OK;

    if (info.iLevel > 0)
    {
        // For nodes, return 'level-score'.
        info.rScore = info.iLevel;
        if (info.eParentWithin == static_cast<int>(Within::Inside)) {
            info.eWithin = static_cast<int>(Within::Inside);
        } else {
            info.eWithin = static_cast<int>(FrustumChecker::CheckBoxWithFrustum(
                plane1.normal_x, plane1.normal_y, plane1.normal_z, plane1.d,
                plane2.normal_x, plane2.normal_y, plane2.normal_z, plane2.d,
                plane3.normal_x, plane3.normal_y, plane3.normal_z, plane3.d,
                plane4.normal_x, plane4.normal_y, plane4.normal_z, plane4.d,
                plane5.normal_x, plane5.normal_y, plane5.normal_z, plane5.d,
                plane6.normal_x, plane6.normal_y, plane6.normal_z, plane6.d,
                pt->m_minx, pt->m_maxx, pt->m_miny, pt->m_maxy, pt->m_minz, pt->m_maxz
            ));
        }
    }
    else
    {
        // For entries (ilevel==0), we return 0 so they are processed immediately (lowest score has highest priority).
        info.rScore = 0;
        info.eWithin = static_cast<int>(Within::Partly);
    }
    return SQLITE_OK;
}


int BoxQueryFunction::TestRange(sqlite3_rtree_query_info& info)
    {
// #if !shenghang_aabb
//     // shenghang hide the code below since the info.m_nParam should be 6
//     if (info.m_nParam != 1 || info.m_args[0].GetValueBytes() != sizeof(DRange3d))
//          return SQLITE_ERROR;
// #endif
    info.eWithin = static_cast<int>(Within::Outside);

    RTree3dVal bounds;
    bounds.m_minx = sqlite3_value_double(info.apSqlParam[0]);
    bounds.m_maxx = sqlite3_value_double(info.apSqlParam[1]);
    bounds.m_miny = sqlite3_value_double(info.apSqlParam[2]);
    bounds.m_maxy = sqlite3_value_double(info.apSqlParam[3]);
    bounds.m_minz = sqlite3_value_double(info.apSqlParam[4]);
    bounds.m_maxz = sqlite3_value_double(info.apSqlParam[5]);

    // RTree3dVal bounds(*(DRange3dCP) info.m_args[0].GetValueBlob());
    RTree3dValCP pt = (RTree3dValCP)info.aCoord;    
#if shenghang_aabb
    bounds.m_minx = info.m_args[0].GetValueDouble();
    bounds.m_maxx = info.m_args[1].GetValueDouble();
    bounds.m_miny = info.m_args[2].GetValueDouble();
    bounds.m_maxy = info.m_args[3].GetValueDouble();
    bounds.m_minz = info.m_args[4].GetValueDouble();
    bounds.m_maxz = info.m_args[5].GetValueDouble();
#endif
    bool passedTest = (info.eParentWithin == static_cast<int>(Within::Inside)) ? true : bounds.Intersects(*pt);
    if (!passedTest)
        return SQLITE_OK;

    if (info.iLevel>0)
        {
        // For nodes, return 'level-score'.
        info.rScore = info.iLevel;
        info.eWithin = info.eParentWithin == static_cast<int>(Within::Inside) ? static_cast<int>(Within::Inside) : bounds.Contains(*pt) ? static_cast<int>(Within::Inside) : static_cast<int>(Within::Partly);
        }
    else
        {
        // For entries (ilevel==0), we return 0 so they are processed immediately (lowest score has highest priority).
        info.rScore = 0;
        info.eWithin = static_cast<int>(Within::Partly);
        }
    // if (info.eWithin == Within::Inside) {
    //     mutex_.lock();
    //     info_.emplace()
    // }
    return SQLITE_OK;
    }

// here apply EleId as hard code to test.
// int Callback(void* data, int argc, char** argv, char** azColName) {
int Callback(void* data, int argc, char** argv, char** azColName) {
  std::vector<int>* ids = static_cast<std::vector<int>*>(data);
  for (int i = 0; i < argc; i++) {
    if (std::string(azColName[i]) == "EleId") {
      ids->push_back(std::stoi(argv[i]));
    }
  }
  return 0;
}

struct IdData {
    int Id;
    int Lod;
    float Es_Divid_H;
};

float Square(float x) {
    return x * x;
}

int GetLod(double es_divid_H, float th1, float th2, float th3) {
    if (es_divid_H >= th1) {
        return 0;
    } else if (es_divid_H >= th2) {
        return 1;
    } else if (es_divid_H >= th3) {
        return 2;
    } else {
        return -1;
    }
}

void SortElems(
    sqlite3* db, 
    const std::vector<int>& idArray,
    float cx, 
    float cy, 
    float cz, 
    float th1, 
    float th2, 
    float th3,
    std::vector<int>& sortedIdArray,
    std::vector<int>& lodArray) {

    std::vector<IdData> idDatas;

    for (int id : idArray) {
        sqlite3_stmt* stmt;
        std::string sql = "SELECT * FROM DB_SpatialIndex WHERE EleId = ?";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            // std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            continue;
        }
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            float minX = sqlite3_column_double(stmt, 1);
            float maxX = sqlite3_column_double(stmt, 2);
            float minY = sqlite3_column_double(stmt, 3);
            float maxY = sqlite3_column_double(stmt, 4);
            float minZ = sqlite3_column_double(stmt, 5);
            float maxZ = sqlite3_column_double(stmt, 6);

            float centerX = (minX + maxX) / 2.0f;
            float centerY = (minY + maxY) / 2.0f;
            float centerZ = (minZ + maxZ) / 2.0f;

            double diameter = std::sqrt(
                Square(maxX - minX) +
                Square(maxY - minY) +
                Square(maxZ - minZ)
            );

            double distance = std::sqrt(
                Square(cx - centerX) +
                Square(cy - centerY) +
                Square(cz - centerZ)
            );

            double fov = 60.0;
            double es_divid_H = diameter / (distance * 2 * std::tan(fov / 2.0 * M_PI / 180.0));

            int lodLevel = GetLod(es_divid_H, th1, th2, th3);

            if (lodLevel != -1) {
                idDatas.push_back({id, lodLevel, static_cast<float>(es_divid_H)});
            }
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    std::sort(idDatas.begin(), idDatas.end(), [](const IdData& a, const IdData& b) {
        return a.Es_Divid_H > b.Es_Divid_H;
    });

    sortedIdArray.resize(idDatas.size());
    lodArray.resize(idDatas.size());

    for (size_t i = 0; i < idDatas.size(); ++i) {
        sortedIdArray[i] = idDatas[i].Id;
        lodArray[i] = idDatas[i].Lod;
    }
}


void SortElems(
    const std::string& databasePath,
    const std::vector<int>& idArray,
    float cx, 
    float cy, 
    float cz, 
    float th1, 
    float th2, 
    float th3,
    std::vector<int>& sortedIdArray,
    std::vector<int>& lodArray)
{
    sqlite3* db;
    if (sqlite3_open(databasePath.c_str(), &db)) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        // std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    SortElems(db, idArray, cx, cy, cz, th1, th2, th3, sortedIdArray, lodArray);
}

std::vector<double> ExtractFloatingPoints(const std::string& str) {
    std::vector<double> floats;
    std::regex floatRegex(R"([-+]?\d*\.?\d+([eE][-+]?\d+)?)");
    std::sregex_iterator it(str.begin(), str.end(), floatRegex);
    std::sregex_iterator end;
    while (it != end) {
        floats.push_back(std::stod(it->str()));
        ++it;
    }
    return floats;
}

std::vector<std::string> ExtractWords(const std::string& str) {
    std::vector<std::string> words;
    std::regex wordRegex(R"(\b[a-zA-Z_]+\b)");
    std::sregex_iterator it(str.begin(), str.end(), wordRegex);
    std::sregex_iterator end;
    while (it != end) {
        words.push_back(it->str());
        ++it;
    }
    return words;
}

void SplitOrderBy(const std::string& query, std::string& beforeOrderBy, std::string& afterOrderBy) {
    std::string orderByKeyword = "ORDER BY";
    size_t pos = query.find(orderByKeyword);
    if (pos != std::string::npos) {
        beforeOrderBy = query.substr(0, pos);
        afterOrderBy = query.substr(pos + orderByKeyword.length());
    }
    else {
        beforeOrderBy = query;
        afterOrderBy = "";
    }
}
} // namespace Extension

extern "C" int StringForWeb3Db(const char* function_name, unsigned char* serialized_data, sqlite3_int64 db_size, int** ids_after_sort, int**lods_after_sort, int* id_count) {
    sqlite3* db = nullptr;
    const char* temp_db_name = "temp_database_serialized_for_web3"; // 数据库名称

    auto manager = Extension::BasicExtensionManager::GetBasicExtensionManager(); 
    manager->OpenSerializedDb(temp_db_name, serialized_data, db_size, db);

    Extension::BoxQueryFunction box_query_function;
    Extension::PlaneQueryFucntion plane_query_function;
    int add_function_result = -1;
    if (std::string(function_name).find("iModel_spatial_overlap_aabb") != std::string::npos) {
        add_function_result = manager->RegisterFunction(db, &box_query_function);
    } else if (std::string(function_name).find("onesight_spatial_overlap_aabb") != std::string::npos)  {
        add_function_result = manager->RegisterFunction(db, &plane_query_function);
    }

    // BentleyM0200::BeSQLite::RTreeMatchFunction::QueryInfo info; // QueryInfo here is sqlite3_rtree_query_info actually.
    if (add_function_result == SQLITE_OK) {
        printf("db AddRTreeMatchFunction success \n");
        // std::cout << "db AddRTreeMatchFunction success" << std::endl;
    } else {
        printf("db AddRTreeMatchFunction failed \n");
        // std::cout << "db AddRTreeMatchFunction failed" << std::endl;
        manager->CloseDb(db);
        return FAILED;
    }
    std::vector<int> matching_ids;
    char* error_message;

    std::string before_order_by_string;
    std::string after_order_by_string;
    Extension::SplitOrderBy(std::string(function_name), before_order_by_string, after_order_by_string);

    auto rc = sqlite3_exec(db, before_order_by_string.c_str(), Extension::Callback, &matching_ids, &error_message);

    // std::cout << "Matching ids: ";
    // for (int id : matching_ids) {
    //   std::cout << id << " ";
    // }
    // std::cout << "Matching ids end" << std::endl;
    std::vector<int> sorted_ids;
    std::vector<int> lod_after_sorting_id;
    std::vector<double> order_detail_double_vec;
    std::vector<std::string> order_detail_word_vec;
    if (after_order_by_string.length() > 0) {
        order_detail_double_vec = Extension::ExtractFloatingPoints(after_order_by_string);
        order_detail_word_vec = Extension::ExtractWords(after_order_by_string);
        if (order_detail_word_vec.size() == 4) {
            if (!order_detail_word_vec[0].compare("screen") && !order_detail_word_vec[1].compare("height_ratio") && 
                !order_detail_word_vec[2].compare("camera_position_threshold") && !order_detail_word_vec[3].compare("DESC") &&
                order_detail_double_vec.size() == 6
                ) {
                printf("computing info \n");
                // std::cout << "computing info" << std::endl;
                Extension::SortElems(db, matching_ids, order_detail_double_vec[0], order_detail_double_vec[1], order_detail_double_vec[2],
                            order_detail_double_vec[3], order_detail_double_vec[4], order_detail_double_vec[5], sorted_ids, lod_after_sorting_id);
            }
        }
    }
    *ids_after_sort = (int*)malloc(matching_ids.size() * sizeof(int));
    *lods_after_sort = (int*)malloc(matching_ids.size() * sizeof(int));
    for (auto j = 0; j < sorted_ids.size(); j++) {
        (*ids_after_sort)[j] = sorted_ids[j];
        (*lods_after_sort)[j] = lod_after_sorting_id[j];
    }
    *id_count = static_cast<int>(sorted_ids.size());
    manager->CloseDb(db);
    // delete db;
    // db = nullptr;
    return 0;
}

extern "C" int ApplyStringToGetId(const char* function_name, Utf8CP db_name, int** ids_after_sort, int**lods_after_sort, int* id_count) {
    std::locale::global(std::locale(""));

    auto manager = Extension::BasicExtensionManager::GetBasicExtensionManager(); 
    sqlite3* db = nullptr;
    manager->OpenDb(db_name, db);

    Extension::BoxQueryFunction box_query_function;
    Extension::PlaneQueryFucntion plane_query_function;
    int add_function_result = -1;
    if (std::string(function_name).find("iModel_spatial_overlap_aabb") != std::string::npos) {
        add_function_result = manager->RegisterFunction(db, &box_query_function);
    } else if (std::string(function_name).find("onesight_spatial_overlap_aabb") != std::string::npos)  {
        add_function_result = manager->RegisterFunction(db, &plane_query_function);
    }

    // BentleyM0200::BeSQLite::RTreeMatchFunction::QueryInfo info; // QueryInfo here is sqlite3_rtree_query_info actually.
    if (add_function_result == SQLITE_OK) {
        printf("db AddRTreeMatchFunction success \n");
        // std::cout << "db AddRTreeMatchFunction success" << std::endl;
    } else {
        printf("db AddRTreeMatchFunction failed \n");
        // std::cout << "db AddRTreeMatchFunction failed" << std::endl;
        manager->CloseDb(db);
        return FAILED;
    }
    std::vector<int> matching_ids;
    char* error_message;

    std::string before_order_by_string;
    std::string after_order_by_string;
    Extension::SplitOrderBy(std::string(function_name), before_order_by_string, after_order_by_string);

    auto rc = sqlite3_exec(db, before_order_by_string.c_str(), Extension::Callback, &matching_ids, &error_message);

    // std::cout << "Matching ids: ";
    // for (int id : matching_ids) {
    //   std::cout << id << " ";
    // }
    // std::cout << "Matching ids end" << std::endl;
    std::vector<int> sorted_ids;
    std::vector<int> lod_after_sorting_id;
    std::vector<double> order_detail_double_vec;
    std::vector<std::string> order_detail_word_vec;
    if (after_order_by_string.length() > 0) {
        order_detail_double_vec = Extension::ExtractFloatingPoints(after_order_by_string);
        order_detail_word_vec = Extension::ExtractWords(after_order_by_string);
        if (order_detail_word_vec.size() == 4) {
            if (!order_detail_word_vec[0].compare("screen") && !order_detail_word_vec[1].compare("height_ratio") && 
                !order_detail_word_vec[2].compare("camera_position_threshold") && !order_detail_word_vec[3].compare("DESC") &&
                order_detail_double_vec.size() == 6
                ) {
                printf("computing info \n");
                // std::cout << "computing info" << std::endl;
                Extension::SortElems(db, matching_ids, order_detail_double_vec[0], order_detail_double_vec[1], order_detail_double_vec[2],
                            order_detail_double_vec[3], order_detail_double_vec[4], order_detail_double_vec[5], sorted_ids, lod_after_sorting_id);
            }
        }
    }
    *ids_after_sort = (int*)malloc(matching_ids.size() * sizeof(int));
    *lods_after_sort = (int*)malloc(matching_ids.size() * sizeof(int));
    for (auto j = 0; j < sorted_ids.size(); j++) {
        (*ids_after_sort)[j] = sorted_ids[j];
        (*lods_after_sort)[j] = lod_after_sorting_id[j];
    }
    *id_count = static_cast<int>(sorted_ids.size());
    manager->CloseDb(db);
    // delete db;
    // db = nullptr;
    return 0;
}



extern "C" void FreeIntArrayMemory(int* id_array) {
    free(id_array);
}
#else
extern "C" void FreeIntArrayMemory(int* id_array) {
    ;
}
extern "C" int StringForWeb3Db(const char* function_name, unsigned char* serialized_data, sqlite3_int64 db_size, int** ids_after_sort, int**lods_after_sort, int* id_count) {
    return 1;
}
extern "C" int ApplyStringToGetId(const char* function_name, Utf8CP db_name, int** ids_after_sort, int**lods_after_sort, int* id_count) {
    return 0;
}
#endif