#ifndef __TEST_RECTANGLECUT_H
#define __TEST_RECTANGLECUT_H


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

namespace RectangleCutTesting {

  TEST(TestConvexCutter, TestRectangle)
  {
    GeometryFactory factory = GeometryFactory("rectangle_input.txt");
    factory.CreatePolygon();

    PolygonEdit editingP = PolygonEdit(factory);

    vector<int> checksideP = {1, -1, -1, 1},
                sideP;
    try
    {
      for (int i=0; i<factory.GetPolygon().NumberVertices; i++ )
           sideP.push_back(editingP.CheckSide(i));
      EXPECT_EQ(sideP, checksideP);
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    try {
        editingP.CutPolygon();

    }
    catch (const exception& exception) {
        FAIL();
    }


    vector<vector<Point>> checklist = {
        {   Point(1.0, 1.0),
            Point(1.777777777, 1.0),
            Point(2.0, 1.2),
            Point(4.0, 3.0),
            Point(4.111111111, 3.1),
            Point(1.0, 3.1)},

        {   Point(5.0, 3.1),
            Point(4.111111111, 3.1),
            Point(4.0, 3.0),
            Point(2.0, 1.2),
            Point(1.777777777, 1.0),
            Point(5.0, 1.0)}};

    vector<Polygon> checkPol;
    for(unsigned int j =0; j < checklist.size(); j++)
    {
        vector<int> checkPolVert;
        for(unsigned int i = 0; i < checklist[j].size(); i++)
        {
            checkPolVert.push_back(factory.GetPointAddress(checklist[j][i]));
        }
        checkPol.push_back(Polygon(checkPolVert));
    }


    try {
        int n = factory.GetNumberSubPolygons();

        EXPECT_EQ(n, 2);

        int num_vertices = factory.GetPolygon().NumberVertices;

        EXPECT_EQ(num_vertices, 4);

        for(int i = 0; i<n; i++)
        {
            Polygon testedSubP = factory.GetSubPolygon(i);
            EXPECT_EQ(testedSubP, checkPol[i]);

        }
    }  catch (const exception& exception) {
        FAIL();
    }
    PolygonViewer viewer = PolygonViewer(factory);
    viewer.ViewPolygon("viewRectangle.m");
    viewer.ViewFinishedProblem("viewFinishedRectangle.m");

  }
}

#endif // __TEST_RECTANGLECUT_H
