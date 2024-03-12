#ifndef __TEST_CONCAVEADVANCED2CUT_H
#define __TEST_CONCAVEADVANCED2CUT_H


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
using namespace testing;
using namespace std;
using namespace PolygonEditLibrary;
using namespace PolygonCuttersLibrary;
using namespace PolygonViewerLibrary;

namespace ConcaveAdvanced1CutTesting {

  TEST(TestConcaveCutter, TestConcaveAdvanced2)
  {
    GeometryFactory factory = GeometryFactory("concave_advanced_2_input.txt");
    factory.CreatePolygon();

    PolygonEdit editingP = PolygonEdit(factory);

    vector<int> checksideP = {-1, -1, -1, 1, -1, 0, 1, 1, 0, 1},
                sideP;
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

    vector<vector<Point>> checklist = {
        {
            Point(3.0, 1.0),
            Point(1.5, 1.5),
            Point(0.0, 0.0),
            Point(-2.0, -2.0),
            Point(2.0, -2.0),
            Point(0.0, -1.0)
        },

        {   Point(0.0, 2.0),
            Point(2.0, 2.0),
            Point(3.0, 3.0),
            Point(-1.0, 3.0),
            Point(-3.0, 1.0),
            Point(0.0, 0.0),
            Point(1.5, 1.5)
        },

        {   Point(3.0, 2.0),
            Point(3.0, 3.0),
            Point(2.0, 2.0)
        },

        {   Point(-3.0, -2.0),
            Point(-2.0, -2.0),
            Point(0.0, 0.0)
        },

         };

    try
    {
      editingP.CutPolygon();
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    try
    {
      EXPECT_EQ(factory.GetPointsNumber(), 15);

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
      EXPECT_EQ(factory.GetPoint(10), Point(-4.0, -4.0));
      EXPECT_EQ(factory.GetPoint(11), Point(4.0, 4.0));
      EXPECT_EQ(factory.GetPoint(12), Point(1.5, 1.5));
      EXPECT_EQ(factory.GetPoint(13), Point(2.0, 2.0));
      EXPECT_EQ(factory.GetPoint(14), Point(-2.0, -2.0));
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    try
    {
      EXPECT_EQ(factory.GetNumberSubPolygons(), 4);

      EXPECT_EQ(factory.GetSubPolygon(0).NumberVertices, 6);
      EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[0], 2);
      EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[1], 12);
      EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[2], 8);
      EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[3], 14);
      EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[4], 0);
      EXPECT_EQ(factory.GetSubPolygon(0).PolygonVertices[5], 1);

      EXPECT_EQ(factory.GetSubPolygon(1).NumberVertices, 7);
      EXPECT_EQ(factory.GetSubPolygon(1).PolygonVertices[0], 3);
      EXPECT_EQ(factory.GetSubPolygon(1).PolygonVertices[1], 13);
      EXPECT_EQ(factory.GetSubPolygon(1).PolygonVertices[2], 5);
      EXPECT_EQ(factory.GetSubPolygon(1).PolygonVertices[3], 6);
      EXPECT_EQ(factory.GetSubPolygon(1).PolygonVertices[4], 7);
      EXPECT_EQ(factory.GetSubPolygon(1).PolygonVertices[5], 8);
      EXPECT_EQ(factory.GetSubPolygon(1).PolygonVertices[6], 12);

      EXPECT_EQ(factory.GetSubPolygon(2).NumberVertices, 3);
      EXPECT_EQ(factory.GetSubPolygon(2).PolygonVertices[0], 4);
      EXPECT_EQ(factory.GetSubPolygon(2).PolygonVertices[1], 5);
      EXPECT_EQ(factory.GetSubPolygon(2).PolygonVertices[2], 13);

      EXPECT_EQ(factory.GetSubPolygon(3).NumberVertices, 3);
      EXPECT_EQ(factory.GetSubPolygon(3).PolygonVertices[0], 9);
      EXPECT_EQ(factory.GetSubPolygon(3).PolygonVertices[1], 14);
      EXPECT_EQ(factory.GetSubPolygon(3).PolygonVertices[2], 8);
    }
    catch (const exception& exception)
    {
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

    try
    {
        int n = factory.GetNumberSubPolygons();
        EXPECT_EQ(n, 4);

        for(int i = 0; i<n; i++)
        {
            Polygon testedSubP = factory.GetSubPolygon(i);
            EXPECT_EQ(testedSubP, checkPol[i]);
        }

    }  catch (const exception& exception) {
        FAIL();
    }

    PolygonViewer viewer = PolygonViewer(factory);
    viewer.ViewPolygon("viewConcaveAdvanced2.m");
    viewer.ViewFinishedProblem("viewFinishedConcaveAdvanced2.m");

  }
}

#endif // __TEST_CONCAVEADVANCED2CUT_H
