#ifndef __TEST_MESHPENTAGON_H
#define __TEST_MESHPENTAGON_H

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

namespace MeshPentagonTesting {

  TEST(TestMesh, TestMeshPentagon)
  {
    vector<Point> polygonPoints = { Point(2.5, 1.0), Point(4.0, 2.1), Point(3.4, 4.2),
                                    Point(1.6, 4.2), Point(1.0, 2.1)};
    ReferenceElt cell = ReferenceElt(polygonPoints);

    cell.PrintDebug("Mesh_Pentagon_CellPrint.m");

    vector<Point> domainPoints = { Point(0.0, 0.0), Point(30.0, 0.0),
                                   Point(30.0, 20.0), Point(0.0, 20.0) };
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


    viewer.ViewDomain("Mesh_Pentagon_Domain.m");
    viewer.ViewMesh("Mesh_Pentagon.m");
    viewer.ViewOutputMesh("Mesh_Pentagon_OutputMesh.m");
  }
}

#endif // __TEST_MESHPENTAGON_H
