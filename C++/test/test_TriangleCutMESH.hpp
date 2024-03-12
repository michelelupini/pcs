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
    GeometryFactory factory = GeometryFactory("triangle_input_2.txt");
    factory.CreatePolygon();
    PolygonEdit editingP = PolygonEdit(factory);

    /*
    vector<int> checksideP = { 0, -1, 0 },
                sideP;
    */
    try
    {
      /*
      for (int i=0; i<factory.GetPolygon().NumberVertices; i++ )
        sideP.push_back(editingP.CheckSide(i));
      EXPECT_EQ(sideP, checksideP);
      */
      EXPECT_EQ(factory.GetPolygon().NumberVertices, 3);
      EXPECT_EQ(editingP.CheckSide(0), 0);
      EXPECT_EQ(editingP.CheckSide(1), 0);
      EXPECT_EQ(editingP.CheckSide(2), 1);
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    /*
    vector<vector<Point>> checklist = {
        {   Point(0.0, 0.0),
            Point(4.0, 0.0),
            Point(2.0, 2.0),
            Point(1.0, 1.0) },

         };

    //questa è una pezza fatta di fretta lol
    vector<int> checkPolVert;

    for(int j = 0; j < checklist[i].size(); j++)
        checkPolVert.push_back(factory.GetPointAddress(checklist[0][j]));
    Polygon checkPol = Polygon(checkPolVert);
    */

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

        EXPECT_EQ(factory.GetPoint(0), Point(0.5, 0.0));
        EXPECT_EQ(factory.GetPoint(1), Point(4.6, 0.0));
        EXPECT_EQ(factory.GetPoint(2), Point(4.6, 0.5));
        EXPECT_EQ(factory.GetPoint(3), Point(0.0, 0.0));
        EXPECT_EQ(factory.GetPoint(4), Point(40.0, 0.0));
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    try {
        int numberSubPolygons = factory.GetNumberSubPolygons();
        EXPECT_EQ(numberSubPolygons, 1);

        /* //testato all'inizio
        int num_vertices = factory.GetPolygon().NumberVertices;
        EXPECT_EQ(num_vertices, 3);
        */

        EXPECT_EQ(factory.GetSubPolygon(0).NumberVertices, 3);

        EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[0], 2);
        EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[1], 0);
        EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[2], 1);

        /*
        for(int i = 0; i<n; i++)
        {
            Polygon testedSubP = factory.GetSubPolygon(i);
            for(int j = 0; j < testedSubP.NumberVertices ; i++)
            {
              EXPECT_EQ(factory.GetPoint(testedSubP.PolygonVertices[j]), checklist[i][j]);
            }
            EXPECT_EQ(testedSubP, checkPol);
        }*/
    }
    catch (const exception& exception)
    {
      FAIL();
    }
    /*PolygonViewer viewer = PolygonViewer(factory);
    viewer.ViewPolygon("viewTriangle.m");
    viewer.ViewLine("viewLineTriangle.m");
    */
  }
}

#endif // __TEST_TRIANGLECUT_H
