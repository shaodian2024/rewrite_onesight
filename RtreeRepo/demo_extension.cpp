#include <string>
#include "RtreeRepo/sqlite3rtree.h"
#include "include/extension.h"
#include "sqlite3rtree.h"
#include <iostream>
#include <vector>
namespace Extension{

typedef struct Circle Circle;
struct Circle {
  struct Box {
    double xmin;
    double xmax;
    double ymin;
    double ymax;
  } aBox[2];
  double centerx;
  double centery;
  double radius;
  double mxArea;
  int eScoreType;
};

/*
** Implementation of "circle" r-tree geometry callback.
*/
void CircleExtensionManager::circle_del(void *p) {
  sqlite3_free(p);
}

int CircleExtensionManager::circle_geom (
  sqlite3_rtree_geometry *p,
  int nCoord, 
  sqlite3_rtree_dbl *aCoord,
  int *pRes
){
  int i;                          /* Iterator variable */
  Circle *pCircle;                /* Structure defining circular region */
  double xmin, xmax;              /* X dimensions of box being tested */
  double ymin, ymax;              /* X dimensions of box being tested */

  xmin = aCoord[0];
  xmax = aCoord[1];
  ymin = aCoord[2];
  ymax = aCoord[3];
  pCircle = (Circle *)p->pUser;
  if( pCircle==0 ){
    /* If pUser is still 0, then the parameter values have not been tested
    ** for correctness or stored into a Circle structure yet. Do this now. */

    /* This geometry callback is for use with a 2-dimensional r-tree table.
    ** Return an error if the table does not have exactly 2 dimensions. */
    if( nCoord!=4 ) return SQLITE_ERROR;

    /* Test that the correct number of parameters (3) have been supplied,
    ** and that the parameters are in range (that the radius of the circle 
    ** radius is greater than zero). */
    if( p->nParam!=3 || p->aParam[2]<0.0 ) return SQLITE_ERROR;

    /* Allocate a structure to cache parameter data in. Return SQLITE_NOMEM
    ** if the allocation fails. */
    pCircle = (Circle *)(p->pUser = sqlite3_malloc(sizeof(Circle)));
    if( !pCircle ) return SQLITE_NOMEM;
    p->xDelUser = circle_del;

    /* Record the center and radius of the circular region. One way that
    ** tested bounding boxes that intersect the circular region are detected
    ** is by testing if each corner of the bounding box lies within radius
    ** units of the center of the circle. */
    pCircle->centerx = p->aParam[0];
    pCircle->centery = p->aParam[1];
    pCircle->radius = p->aParam[2];

    /* Define two bounding box regions. The first, aBox[0], extends to
    ** infinity in the X dimension. It covers the same range of the Y dimension
    ** as the circular region. The second, aBox[1], extends to infinity in
    ** the Y dimension and is constrained to the range of the circle in the
    ** X dimension.
    **
    ** Then imagine each box is split in half along its short axis by a line
    ** that intersects the center of the circular region. A bounding box
    ** being tested can be said to intersect the circular region if it contains
    ** points from each half of either of the two infinite bounding boxes.
    */
    pCircle->aBox[0].xmin = pCircle->centerx;
    pCircle->aBox[0].xmax = pCircle->centerx;
    pCircle->aBox[0].ymin = pCircle->centery + pCircle->radius;
    pCircle->aBox[0].ymax = pCircle->centery - pCircle->radius;
    pCircle->aBox[1].xmin = pCircle->centerx + pCircle->radius;
    pCircle->aBox[1].xmax = pCircle->centerx - pCircle->radius;
    pCircle->aBox[1].ymin = pCircle->centery;
    pCircle->aBox[1].ymax = pCircle->centery;
    pCircle->mxArea = (xmax - xmin)*(ymax - ymin) + 1.0;
  }

  /* Check if any of the 4 corners of the bounding-box being tested lie 
  ** inside the circular region. If they do, then the bounding-box does
  ** intersect the region of interest. Set the output variable to true and
  ** return SQLITE_OK in this case. */
  for(i=0; i<4; i++){
    double x = (i&0x01) ? xmax : xmin;
    double y = (i&0x02) ? ymax : ymin;
    double d2;
    
    d2  = (x-pCircle->centerx)*(x-pCircle->centerx);
    d2 += (y-pCircle->centery)*(y-pCircle->centery);
    if( d2<(pCircle->radius*pCircle->radius) ){
      *pRes = 1;
      return SQLITE_OK;
    }
  }

  /* Check if the bounding box covers any other part of the circular region.
  ** See comments above for a description of how this test works. If it does
  ** cover part of the circular region, set the output variable to true
  ** and return SQLITE_OK. */
  for(i=0; i<2; i++){
    if( xmin<=pCircle->aBox[i].xmin 
     && xmax>=pCircle->aBox[i].xmax 
     && ymin<=pCircle->aBox[i].ymin 
     && ymax>=pCircle->aBox[i].ymax 
    ){
      *pRes = 1;
      return SQLITE_OK;
    }
  }

  /* The specified bounding box does not intersect the circular region. Set
  ** the output variable to zero and return SQLITE_OK. */
  *pRes = 0;
  return SQLITE_OK;
}

int CircleExtensionManager::Callback(void* data, int argc, char** argv, char** azColName) {
  std::vector<int>* ids = static_cast<std::vector<int>*>(data);
  for (int i = 0; i < argc; i++) {
    if (std::string(azColName[i]) == "id") {
      ids->push_back(std::stoi(argv[i]));
    }
  }
  return 0;
}

void CircleExtensionManager::RegisterCircleFunction(sqlite3 *db, void* pRes) {
  sqlite3_rtree_geometry_callback(db, "circle", circle_geom, pRes);
}
}
