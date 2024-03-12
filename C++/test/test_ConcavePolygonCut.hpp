#ifndef __TEST_CONCAVEPOLYGONCUT_H
#define __TEST_CONCAVEPOLYGONCUT_H


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

namespace ConcavePolygonCutTesting {

  TEST(TestConcaveCutter, TestConcavePolygon)
  {
    GeometryFactory factory = GeometryFactory("concave_polygon_input.txt");
    factory.CreatePolygon();

    PolygonEdit editingP = PolygonEdit(factory);

    vector<int> checksideP = {-1, -1, -1, 1, -1, 1},
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
        {
            Point(5.5, 4.8), //punto 2
            Point(4.2043269230769, 6.0092948717949), //punto 6
            Point(4.1, 5.9), //punto 7
            Point(3.7213114754098, 5.5032786885246), //punto 8
            Point(3.2, 4.2), //punto 4
            Point(2.4085972850679, 4.1280542986425), //punto 9
            Point(2.0, 3.7), //punto 10
            Point(1.1912162162162, 2.8527027027027), //punto 11
            Point(1.5, 1.0), //punto 0
            Point(5.6, 1.5) //punto 1
        },

        {   Point(4.0, 6.2), //punto 3
            Point(3.7213114754098, 5.5032786885246), //punto 8
            Point(4.1, 5.9), //punto 7
            Point(4.2043269230769, 6.0092948717949) //punto 6
        },

        {   Point(1.0, 4.0), //punto 5
            Point(1.1912162162162, 2.8527027027027), //punto 11
            Point(2.0, 3.7), //punto 10
            Point(2.4085972850679, 4.1280542986425) //punto 9
        },

        };

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

        EXPECT_EQ(n, 3);
        int num_vertices = factory.GetPolygon().NumberVertices;

        EXPECT_EQ(num_vertices, 6);

        for(int i = 0; i<n; i++)
        {
            Polygon testedSubP = factory.GetSubPolygon(i);
            EXPECT_EQ(testedSubP, checkPol[i]);
        }
    }  catch (const exception& exception) {
        FAIL();
    }
    PolygonViewer viewer = PolygonViewer(factory);
    viewer.ViewPolygon("viewConcave.m");
    viewer.ViewFinishedProblem("viewFinishedConcave.m");

  }
}

#endif // __TEST_CONCAVEPOLYGONCUT_H
