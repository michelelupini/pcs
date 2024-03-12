#ifndef __TEST_TRIANGLECUT_H
#define __TEST_TRIANGLECUT_H


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include <vector>
#include "GeometryFactory.hpp"
#include "PolygonEdit.hpp"
#include "PolygonConvexCutter.hpp"
#include "PolygonViewer.hpp"

using namespace GeometryLibrary;
using namespace GeometryFactoryLibrary;
using namespace PolygonViewerLibrary;
using namespace testing;
using namespace std;
using namespace PolygonEditLibrary;
using namespace PolygonCuttersLibrary;

namespace TriangleCutTesting {

  TEST(TestConvexCutter, TestTriangle)
  {
    GeometryFactory factory = GeometryFactory("triangle_input.txt");
    factory.CreatePolygon();
    PolygonEdit editingP = PolygonEdit(factory);

    try
    {
      EXPECT_EQ(factory.GetPolygon().NumberVertices, 3);
      EXPECT_EQ(editingP.CheckSide(0), 0);
      EXPECT_EQ(editingP.CheckSide(1), -1);
      EXPECT_EQ(editingP.CheckSide(2), 0);
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    try
    {
      editingP.CutPolygon();
    }
    catch (const exception& exception) {
        FAIL();
    }

    try
    {
        EXPECT_EQ(factory.GetPointsNumber(), 5);

        EXPECT_EQ(factory.GetPoint(0), Point(0.0, 0.0));
        EXPECT_EQ(factory.GetPoint(1), Point(4.0, 0.0));
        EXPECT_EQ(factory.GetPoint(2), Point(2.0, 2.0));
        EXPECT_EQ(factory.GetPoint(3), Point(-1.0, -1.0));
        EXPECT_EQ(factory.GetPoint(4), Point(1.0, 1.0));
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    try {
        int numberSubPolygons = factory.GetNumberSubPolygons();
        EXPECT_EQ(numberSubPolygons, 1);


        EXPECT_EQ(factory.GetSubPolygon(0).NumberVertices, 4);

        EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[0], 1);
        EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[1], 2);
        EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[2], 4);
        EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[3], 0);

    }
    catch (const exception& exception)
    {
      FAIL();
    }
    PolygonViewer viewer = PolygonViewer(factory);
    viewer.ViewPolygon("viewTriangle.m");
    viewer.ViewFinishedProblem("viewFinishedTriangle.m");

  }
}

#endif // __TEST_TRIANGLECUT_H
