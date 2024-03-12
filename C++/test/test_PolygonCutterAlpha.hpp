#ifndef __TEST_CUTTERTEST_H
#define __TEST_CUTTERTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "PolygonConvexCutter.hpp"

using namespace PolygonCuttersLibrary;
using namespace testing;
using namespace std;

namespace CutterTesting {

  /*//IPolygonEditMock
  class MockGeometryFactory: public IGeometryFactory{
    public:
      MOCK_METHOD0(CreatePolygon, void();
      //MOCK_CONST_METHOD0(GetPolygon, const Polygon&(const Polygon _polygon));

	//qual è la differenza tra MOCK_CONST_METHOD0, 1, 2 ECC? che argomenti prende?

  };*/


  TEST(TestConcaveCutter, TestEmptyMethod)
  {
    //

    //ConcaveCutter Cutter = ConcaveCutter();

    try
    {
      EXPECT_EQ(true, true);
    }
    catch (const exception& exception)
    {
      FAIL();
    }

  }
}

#endif // __TEST_CUTTERTEST_H
