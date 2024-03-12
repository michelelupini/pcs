#ifndef __TEST_MESHPENTAGONPOLYGON_H
#define __TEST_MESHPENTAGONPOLYGON_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include <vector>
#include "GeometryFactory.hpp"
#include "PolygonEdit.hpp"
#include "PolygonCutterAlpha.hpp"
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

  TEST(TestMesh, TestPentagon)
  {
    vector<Point> polygonPoints = { Point(2.5, 1.0), Point(4.0, 2.1), Point(3.4, 4.2), Point(1.6, 4.2), Point(1.0, 2.1) };

    ReferenceElt cell = ReferenceElt(polygonPoints);

    cell.PrintDebug("cellprintPentagon.m");

    vector<Point> domainPoints = { Point(0.0, 0.0), Point(20.0, 0.0),
                                   Point(15.0, 15.0), Point(2.0, 20.0) };
    PolygonAdvanced domain = PolygonAdvanced(domainPoints);

    AdvancedMesh advancedMesh = AdvancedMesh(domain, cell);

    AdvancedPolygonViewer pino = AdvancedPolygonViewer(advancedMesh);
    //pino.ViewOutputMesh("meshprintbeforecut.m");


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


    pino.ViewDomain("ViewDomainPentagon.m");
    pino.ViewMesh("ViewMeshPentagon.m");
    pino.ViewOutputMesh("ViewOutputMeshPentagon.m");
  }
}

#endif // __TEST_MESHPENTAGONPOLYGON_H
