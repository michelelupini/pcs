#ifndef __TEST_POLYGONCUTTER_H
#define __TEST_POLYGONCUTTER_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "PolygonConvexCutter.hpp"

using namespace PolygonCuttersLibrary;
using namespace testing;
using namespace std;

namespace EmptyTesting {

  class MockGeometryFactory : public IGeometryFactory {
    public:
      MOCK_METHOD0(CreatePolygon, void());
      MOCK_CONST_METHOD0(GetPointsNumber, int());
      MOCK_CONST_METHOD0(GetPolygonNumberVertices, int());
      MOCK_CONST_METHOD0(GetPolygon, const Polygon&());
      MOCK_CONST_METHOD1(GetPolygonEdge, const Segment&(const int& edgePosition));
      MOCK_CONST_METHOD0(GetLine, const Segment&());
      MOCK_CONST_METHOD1(GetPoint, const Point*(const int& idPoint));
      MOCK_CONST_METHOD1(GetSubPolygon, const Polygon&(const int& idPolygon));
      MOCK_CONST_METHOD1(SetPoints, void(const Point& point));
      MOCK_CONST_METHOD1(SetSubPolygons, void(const Polygon& newSubPolygon));
  };

  TEST(TestPolygonCutterAlpha, TestComputeNewPoints)
  {
    EmptyClass emptyObject;

    try
    {
      EXPECT_EQ(emptyObject.EmptyMethod(), true);
    }
    catch (const exception& exception)
    {
      FAIL();
    }

  }
}

#endif // __TEST_EMPTYCLASS_H
