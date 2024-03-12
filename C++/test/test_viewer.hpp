#ifndef __TEST_VIEWER_H
#define __TEST_VIEWER_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "GeometryFactory.hpp"
#include "PolygonViewer.hpp"

using namespace GeometryLibrary;
using namespace GeometryFactoryLibrary;
using namespace PolygonViewerLibrary;
using namespace testing;
using namespace std;

namespace EmptyTesting {

  TEST(TestPolygonViewer, TestFileOutput)
  {
    GeometryFactory factory = GeometryFactory("pentagon_input.txt");
    factory.CreatePolygon();

    PolygonViewer viewer = PolygonViewer(factory);

    try
    {
      viewer.ViewPolygon("viewPolygon.m");
    }
    catch (const exception& exception)
    {
      FAIL();
    }

    try
    {
      viewer.ViewLine("viewLine.m");
    }
    catch (const exception& exception)
    {
      FAIL();
    }
  }
}

#endif // __TEST_VIEWER_H
