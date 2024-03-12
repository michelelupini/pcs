#ifndef __TEST_MESHTRIANGLEPOLYGON_H
#define __TEST_MESHTRIANGLEPOLYGON_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include <vector>
#include "GeometryFactory.hpp"
#include "PolygonEdit.hpp"
#include "PolygonConcaveCutter.hpp"
#include "PolygonViewer.hpp"
#include "AdvancedMesh.hpp"
#include "PolygonAdvanced.hpp"
#include "ReferenceElt.hpp"
#include "AdvancedPolygonViewer.hpp"

using namespace GeometryLibrary;
using namespace GeometryFactoryLibrary;
using namespace PolygonViewerLibrary;
using namespace testing;
using namespace std;
using namespace PolygonEditLibrary;
using namespace PolygonCuttersLibrary;
using namespace PolygonAdvancedLibrary;
using namespace AdvancedMeshLibrary;

namespace MeshTriangleTesting {

  TEST(TestMesh, TestTriangle)
  {
    vector<Point> polygonPoints = { Point(0.0, 0.0), Point(4.0, 0.0), Point(2.0, 2.0) };

    ReferenceElt cell = ReferenceElt(polygonPoints);

    cell.PrintDebug("Mesh_Triangle_CellPrint.m");

    vector<Point> domainPoints = { Point(0.0, 0.0), Point(20.0, 0.0),
                                   Point(15.0, 15.0), Point(2.0, 20.0) };

    PolygonAdvanced domain = PolygonAdvanced(domainPoints);

    AdvancedMesh advancedMesh = AdvancedMesh(domain, cell);

    AdvancedPolygonViewer viewer = AdvancedPolygonViewer(advancedMesh);


    try
    {
      advancedMesh.ComputeMesh();
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    try
    {
      double areaDomain = domain.ComputeArea();
      double areaElements = 0.0;

      for(int i = 0; i < advancedMesh.GetNumberPolygonInMesh(); i++)
        areaElements += advancedMesh.GetPolygonInMesh(i).ComputeArea();

      EXPECT_NEAR(areaDomain, areaElements, TOL);

    }
    catch (const exception& exception)
    {
      FAIL();
    }


    viewer.ViewDomain("Mesh_Triangle_Domain.m");
    viewer.ViewMesh("Mesh_Triangle.m");
    viewer.ViewOutputMesh("Mesh_Triangle_OutputMesh.m");
  }
}

#endif // __TEST_MESHTRIANGLEPOLYGON_H
