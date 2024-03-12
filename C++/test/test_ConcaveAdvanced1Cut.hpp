#ifndef __TEST_CONCAVEADVANCED1CUT_H
#define __TEST_CONCAVEADVANCED1CUT_H


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include <vector>
#include "GeometryFactory.hpp"
#include "PolygonEdit.hpp"
#include "PolygonConcaveCutter.hpp"
#include "PolygonViewer.hpp"

using namespace GeometryLibrary;
using namespace GeometryFactoryLibrary;
using namespace PolygonViewerLibrary;
using namespace testing;
using namespace std;
using namespace PolygonEditLibrary;
using namespace PolygonCuttersLibrary;

namespace ConcaveAdvanced1CutTesting {

  TEST(TestConcaveCutter, TestConcaveAdvanced1)
  {
    GeometryFactory factory = GeometryFactory("concave_advanced_1_input.txt");
    factory.CreatePolygon();

    PolygonEdit editingP = PolygonEdit(factory);

    vector<int> checksideP = {-1, 0, -1, 0, -1, -1, 1, 1, 0, 1},
                sideP;

    try
    {
        EXPECT_EQ(factory.GetPoint(0), Point(2.0, -2.0));
        EXPECT_EQ(factory.GetPoint(1), Point(0.0, -1.0));
        EXPECT_EQ(factory.GetPoint(2), Point(3.0, 1.0));
        EXPECT_EQ(factory.GetPoint(3), Point(0.0, 2.0));
        EXPECT_EQ(factory.GetPoint(4), Point(3.0, 2.0));
        EXPECT_EQ(factory.GetPoint(5), Point(3.0, 3.0));
        EXPECT_EQ(factory.GetPoint(6), Point(-1.0, 3.0));
        EXPECT_EQ(factory.GetPoint(7), Point(-3.0, 1.0));
        EXPECT_EQ(factory.GetPoint(8), Point(0.0, 0.0));
        EXPECT_EQ(factory.GetPoint(9), Point(-3.0, -2.0));
    }
    catch(const exception& exception)
    {
      FAIL();
    }

    try
    {
      EXPECT_EQ(factory.GetPolygon().NumberVertices, 10);

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
        {
            Point(2.0, -2.0),
            Point(0.0, -1.0),
            Point(0.0, -2.0)
        },

        {   Point(3.0, 1.0),
            Point(0.0, 2.0),
            Point(0.0, 0.0),
            Point(0.0, -1.0)
        },

        {   Point(3.0, 3.0),
            Point(0.0, 3.0),
            Point(0.0, 2.0),
            Point(3.0, 2.0)
        },

        {   Point(-3.0, 1.0),
            Point(0.0, 0.0),
            Point(0.0, 2.0),
            Point(0.0, 3.0),
            Point(-1.0, 3.0)
        },

        {   Point(-3.0, -2.0),
            Point(0.0, -2.0),
            Point(0.0, -1.0),
            Point(0.0, 0.0)
        },

         };

    try {
        editingP.CutPolygon();

    }
    catch (const exception& exception) {
        FAIL();
    }

    vector<Polygon> checkPol;
    for(unsigned int j =0; j<checklist.size(); j++)
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

        EXPECT_EQ(n, 5);
        int num_vertices = factory.GetPolygon().NumberVertices;

        EXPECT_EQ(num_vertices, 10);

        for(int i = 0; i<n; i++)
        {
            Polygon testedSubP = factory.GetSubPolygon(i);
            EXPECT_EQ(testedSubP, checkPol[i]);
        }
    }  catch (const exception& exception) {
        FAIL();
    }
    PolygonViewer viewer = PolygonViewer(factory);
    viewer.ViewPolygon("viewConcaveAdvanced1.m");
    viewer.ViewFinishedProblem("viewFinishedConcaveAdvanced1.m");

  }
}

#endif // __TEST_CONCAVEADVANCED1CUT_H
