#ifndef __TEST_MESHDEFINITIVE_H
#define __TEST_MESHDEFINITIVE_H

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

namespace MeshDefinitiveTesting {

  TEST(TestMesh, TestDefinitive)
  {
    vector<Point> polygonPoints = {  Point(-14.0, -3.0), Point(-12.0, -3.0), Point(-12.0, 3.0), Point(-11.0, 0.0),
                                     Point(-10.0, 2.0), Point(-9.0, 0.0), Point(-5.0, 0.0), Point(-3.0, -2.0),
                                     Point(-2.0, 0.0), Point(1.0, -5.0), Point(14.0, -5.0), Point(14.0, 3.0),
                                     Point(12.0, 3.0), Point(12.0, -3.0), Point(11.0, 0.0), Point(10.0, -2.0),
                                     Point(9.0, 0.0), Point(5.0, 0.0),  Point(3.0, 2.0), Point(2.0, 0.0),
                                     Point(-1.0, 5.0), Point(-14.0, 5.0) };

    ReferenceElt cell = ReferenceElt(polygonPoints);
    cell.PrintDebug("Mesh_Definitive_CellPrint.m");

    vector<Point> domainPoints = { Point(40.0, 0.0),Point(80.0, 40.0),Point(40.0, 80.0),Point(0.0, 40.0)};

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

    viewer.ViewDomain("Mesh_Definitive_Domain.m");
    viewer.ViewMesh("Mesh_Definitive.m");
    viewer.ViewOutputMesh("Mesh_Definitive_OutputMesh.m");
  }
}

#endif // __TEST_MESHDEFINITIVE_H
