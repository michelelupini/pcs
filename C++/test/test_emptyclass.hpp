#ifndef __TEST_EMPTYCLASS_H
#define __TEST_EMPTYCLASS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "EmptyClass.hpp"

using namespace EmptyNamespace;
using namespace testing;
using namespace std;

namespace EmptyTesting {

  TEST(TestPolygonCutter, TestEmptyMethod)
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
