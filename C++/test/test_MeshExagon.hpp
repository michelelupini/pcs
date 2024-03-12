#ifndef __TEST_MESHEXAGON_H
#define __TEST_MESHEXAGON_H

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

namespace MeshConcavePolygonTesting {

  TEST(TestMesh, TestExagon)
  {
    vector<Point> polygonPoints = { Point(0.0, 0.0), Point(3.0, 1.0), Point(6.0, 0.0),
                                    Point(4.0, 3.0), Point(3.0, 5.5), Point(2.0, 3.0), };

    PolygonAdvanced polipoli = PolygonAdvanced(polygonPoints);
    polipoli.PrintDebug("mimmopinguino.m");


    ReferenceElt cell = ReferenceElt(polygonPoints);
    cell.PrintDebug("Mesh_Exagon_CellPrint.m");

    vector<Point> domainPoints = { Point(15.0, 0.0), Point(30.0, 30.0), Point(30.0, 45.0),
                                  Point(15.0, 75.0), Point(0.0, 45.0), Point(0.0, 30.0) };

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

    viewer.ViewDomain("Mesh_Exagon_Domain.m");
    viewer.ViewMesh("Mesh_Exagon.m");
    viewer.ViewOutputMesh("Mesh_Exagon_OutputMesh.m");
  }
}

#endif // __TEST_MESHEXAGON_H
