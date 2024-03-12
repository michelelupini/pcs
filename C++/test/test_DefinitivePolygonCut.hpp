#ifndef __TEST_DEFINITIVEPOLYGONCUT_H
#define __TEST_DEFINITIVEPOLYGONCUT_H


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

namespace ConcavePolygonCutTesting {

  TEST(TestConcaveCutter, TestDefinitivePolygon)
  {
    GeometryFactory factory = GeometryFactory("definitive_Polygon_input.txt");
    factory.CreatePolygon();

    PolygonEdit editingP = PolygonEdit(factory);

    try {
        editingP.CutPolygon();

    }
    catch (const exception& exception) {
        FAIL();
    }

    vector<vector<int>> checklist = {
        {0, 1, 24, 27},
        {2, 3, 4, 5, 22, 6, 8, 19, 20, 21, 27, 24},
        {7, 8, 6},
        {10, 25, 26, 13, 14, 15, 16, 17, 23, 19, 8, 9},
        {12, 26, 25, 11},
        {18, 19, 23, 17}
    };

    vector<Polygon> checkPol;

    for(unsigned int j =0; j<checklist.size(); j++)
    {
        checkPol.push_back(Polygon(checklist[j]));
    }

    try {
        int n = factory.GetNumberSubPolygons();

        EXPECT_EQ(n, 6);
        int num_vertices = factory.GetPolygon().NumberVertices;

        EXPECT_EQ(num_vertices, 22);

        for(int i = 0; i<n; i++)
        {
            Polygon testedSubP = factory.GetSubPolygon(i);
            EXPECT_EQ(testedSubP, checkPol[i]);      
        }
    }  catch (const exception& exception) {
        FAIL();
    }

    PolygonViewer viewer = PolygonViewer(factory);
    viewer.ViewFinishedProblem("viewDefinitive.m");
    viewer.ViewFinishedProblem("viewFinishedDefinitive.m");

  }
}

#endif // __TEST_DEFINITIVEPOLYGONCUT_H
