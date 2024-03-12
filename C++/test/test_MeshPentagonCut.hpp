#ifndef __TEST_PENTAGONCUT_H
#define __TEST_PENTAGONCUT_H


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

namespace PentagonCutTesting {

  TEST(TestConvexCutter, TestPentagon)
  {
    GeometryFactory factory = GeometryFactory("pentagon_input.txt");
    factory.CreatePolygon();

    PolygonEdit editingP = PolygonEdit(factory);
    //editingP.CutPolygon();

    vector<int> checksideP = {-1, 0, 1, 1, -1},
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
        {   Point(2.5, 1.0),
            Point(4.0, 2.1),
            Point(3.6, 2.2),
            Point(1.4, 2.75),
            Point(1.2, 2.8),
            Point(1.0, 2.1)},

        {   Point(1.6, 4.2),
            Point(1.2, 2.8),
            Point(1.4, 2.75),
            Point(3.6, 2.2),
            Point(4.0, 2.1),
            Point(3.4, 4.2)}};

    vector<Polygon> checkPol;
    for(int j =0; j<2; j++)
    {
        vector<int> checkPolVert;
        for(int i = 0; i < checklist[j].size(); i++)
        {
            checkPolVert.push_back(factory.GetPointAddress(checklist[j][i]));
        }
        checkPol.push_back(Polygon(checkPolVert));
    }

    try {
        int n = factory.GetNumberSubPolygons();

        EXPECT_EQ(n, 2);

        int num_vertices = factory.GetPolygon().NumberVertices;

        EXPECT_EQ(num_vertices, 5);

        for(int i = 0; i<n; i++)
        {
            Polygon testedSubP = factory.GetSubPolygon(i);
            EXPECT_EQ(testedSubP, checkPol[i]);
        }

    }  catch (const exception& exception) {
        FAIL();
    }
    PolygonViewer viewer = PolygonViewer(factory);
    viewer.ViewPolygon("viewPentagon.m");
    viewer.ViewLine("viewLinePentagon.m");

  }
}

#endif // __TEST_PENTAGONCUT_H
