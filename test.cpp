#include <string>
#include <iostream>
#include <format>
#include "export.h"
#include "iostream"
#include <chrono>
int main(int argc, char* argv[]) {
    auto file_name = argv[1];
    double minX = -15.78279f, minY = -0.1f, minZ = -30.30879f, maxX = 4.667215f, maxY = 9.0f, maxZ = 17.0f;

    // std::string fmt = std::format(
    //     "SELECT EleId FROM DB_SpatialIndex "
    //     "WHERE EleId MATCH iModel_spatial_overlap_aabb({0:G17}, {1:G17}, {2:G17}, {3:G17}, {4:G17}, {5:G17}) "
    //     "ORDER BY iModel_bbox_volume(iModel_bbox(MinX,MinY,MinZ,MaxX,MaxY,MaxZ)) DESC",
    //     std::to_string(minX), std::to_string(minY), std::to_string(minZ), std::to_string(maxX), std::to_string(maxY), std::to_string(maxZ)
    // );
    // std::string fmt = "SELECT EleId FROM DB_SpatialIndex WHERE EleId MATCH iModel_spatial_overlap_aabb({0:G17}, {1:G17}, {2:G17}, {3:G17}, {4:G17}, {5:G17}) ORDER BY iModel_bbox_volume(iModel_bbox(-15.78279f,-0.1f,-30.30879f,4.667215f,9.0f,17.0f)) DESC";
    // std::string fmt = "SELECT EleId FROM DB_SpatialIndex WHERE EleId MATCH iModel_spatial_overlap_aabb(-15.78279f,-0.1f,-30.30879f,4.667215f,9.0f,17.0f) ORDER BY iModel_bbox_volume(iModel_bbox(-15.78279f,-0.1f,-30.30879f,4.667215f,9.0f,17.0f)) DESC";
    // std::string fmt = "SELECT EleId FROM DB_SpatialIndex WHERE EleId MATCH iModel_spatial_overlap_aabb(-15.78279f,-0.1f,-30.30879f,4.667215f,9.0f,17.0f)";
    // std::string fmt = "SELECT EleId FROM DB_SpatialIndex WHERE EleId MATCH iModel_spatial_overlap_aabb(-16, -0 , -31, 5, 9, 17) ORDER BY iModel_bbox_volume(iModel_bbox(MinX,MinY,MinZ,MaxX,MaxY,MaxZ)) DESC";
    // no such function: iModel_bbox
    // std::string fmt = "SELECT EleId FROM DB_SpatialIndex WHERE EleId MATCH iModel_spatial_overlap_aabb(-23.774, -5.125, -39.024, 8.586, 3.524, -25.9)";
    // std::string fmt = "SELECT EleId FROM DB_SpatialIndex WHERE EleId MATCH iModel_spatial_overlap_aabb(-23.774, 8.586, -5.125, 3.524, -39.024, -25.9) ORDER BY screen height_ratio(camera_position_threshold(-15.0, -0.1, -30.3, 0.3, 0.05, 0.01)) DESC";
    // minX, maxX, minY, maxY, minZ, MaxZ
    // std::cout << fmt << std::endl;
    std::string fmt = "SELECT EleId FROM DB_SpatialIndex WHERE EleId MATCH onesight_spatial_overlap_aabb(-1,-1,-1,10000,-1,-1,-1,10000,-1,-1,-1,10000,-1,-1,-1,10000,-1,-1,-1,10000,-1,-1,-1,10000) ORDER BY screen height_ratio(camera_position_threshold(45.969471,36.1554375,42.2682533,0.3,0.05,0.01)) DESC";
    // std::string fmt = "SELECT EleId FROM DB_SpatialIndex WHERE EleId MATCH onesight_spatial_overlap_aabb(0,0,1,-2,0,0,1,10,-1,0,0,2,1,0,0,100,0,1,0,100,0,-1,0,100) ORDER BY screen height_ratio(camera_position_threshold(45.969471,36.1554375,42.2682533,0.3,0.05,0.01)) DESC";

    int* array = new int[1000];
    int** id_array = &array;
    int* lods = new int[1000];
    int** lod_array = &lods;
    int useless2 = 10;
    int* id_count = &useless2;
    for (auto i = 1; i <= 10; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        auto sucess = ApplyStringToGetId(fmt.c_str(), file_name, id_array, lod_array, id_count);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "ids are" << std::endl;
        for (auto i = 0; i < *id_count; i++) {
            std::cout << "id " << (*id_array)[i] << " lod " << (*lod_array)[i] << std::endl;
        }
        std::cout << "ids end" << std::endl;
        std::cout << "round" << i << "cost " << elapsed.count() << " ms." << std::endl;
    }
    return 0;
}
// int WinMain() {
//     BentleyM0200::BeSQLite::iModel_spatial_overlap_aabb a("");
//     BentleyM0200::BeSQLite::RTreeMatchFunction::QueryInfo info;
//     a._TestRange(info);
//     return 0;
// }