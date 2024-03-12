#ifndef __TEST_MESHCONCAVEPOLYGON_H
#define __TEST_MESHCONCAVEPOLYGON_H

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

  TEST(TestMesh, TestConcavePolygon)
  {
    vector<Point> polygonPoints = { Point(1.5, 1.0), Point(5.6, 1.5), Point(5.5, 4.8),
                                    Point(4.0, 6.2), Point(3.2, 4.2), Point(1.0, 4.0), };
    ReferenceElt cell = ReferenceElt(polygonPoints);

    cell.PrintDebug("Mesh_ConcavePolygon_CellPrint.m");

    vector<Point> domainPoints = { Point(0.0, 0.0), Point(30.0, 5.0),
                                   Point(38.0, 26.0), Point(2.0, 20.0) };

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


    viewer.ViewDomain("Mesh_ConcavePolygon_Domain.m");
    viewer.ViewMesh("Mesh_ConcavePolygon.m");
    viewer.ViewOutputMesh("Mesh_ConcavePolygon_OutputMesh.m");
  }
}

#endif // __TEST_MESHCONCAVEPOLYGON_H
