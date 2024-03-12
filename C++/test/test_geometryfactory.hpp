#ifndef __TEST_GEOMETRYFACTORY_H
#define __TEST_GEOMETRYFACTORY_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "GeometryFactory.hpp"

using namespace GeometryLibrary;
using namespace GeometryFactoryLibrary;
using namespace testing;
using namespace std;

namespace GeometryFactoryTesting {

  TEST(TestGeometryFactory, TestFileInput)
  {
    ///Prova la corretta apertura del file
    try
    {
      GeometryFactory failingfactory = GeometryFactory("fakeinput.txt");
      failingfactory.CreatePolygon();
    }
    catch(const exception &exception)
    {
      EXPECT_THAT(string(exception.what()), Eq("Cannot open file. Something went wrong"));
    }

    try
    {
      GeometryFactory failingfactory = GeometryFactory("bad_input_1.txt");
      failingfactory.CreatePolygon();
    }
    catch(const exception &exception)
    {
      EXPECT_THAT(string(exception.what()), Eq("Syntax error: bad points input"));
    }

    try
    {
      GeometryFactory failingfactory = GeometryFactory("bad_input_2.txt");
      failingfactory.CreatePolygon();
    }
    catch(const exception &exception)
    {
      EXPECT_THAT(string(exception.what()), Eq("Syntax error: bad polygon vertices input"));
    }

    try
    {
      GeometryFactory failingfactory = GeometryFactory("bad_input_3.txt");
      failingfactory.CreatePolygon();
    }
    catch(const exception &exception)
    {
      EXPECT_THAT(string(exception.what()), Eq("Syntax error: bad segment input"));
    }

    GeometryFactory factory = GeometryFactory("input.txt");

    try
    {
      factory.CreatePolygon();
    }
    catch (const exception &exception)
    {
      FAIL();
    }

    try
    {
      EXPECT_EQ(factory.GetPointsNumber(), 8);
      EXPECT_EQ(factory.GetPolygon().NumberVertices, 6);

      EXPECT_EQ(factory.GetPoint(0), Point(1.5, 1.0));
      EXPECT_EQ(factory.GetPoint(1), Point(5.6, 1.5));
      EXPECT_EQ(factory.GetPoint(2), Point(5.5, 4.8));
      EXPECT_EQ(factory.GetPoint(3), Point(4.0, 6.2));
      EXPECT_EQ(factory.GetPoint(4), Point(3.2, 4.2));
      EXPECT_EQ(factory.GetPoint(5), Point(1.0, 4.0));
      EXPECT_EQ(factory.GetPoint(6), Point(2.0, 3.7));
      EXPECT_EQ(factory.GetPoint(7), Point(4.1, 5.9));

      Polygon testpolygon = factory.GetPolygon();
      vector<Point> pointlist = { Point(1.5, 1.0),
                                  Point(5.6, 1.5),
                                  Point(5.5, 4.8),
                                  Point(4.0, 6.2),
                                  Point(3.2, 4.2),
                                  Point(1.0, 4.0) };

      EXPECT_EQ(factory.GetPolygon().NumberVertices, 6);

      EXPECT_EQ(factory.GetPoint(factory.GetPolygon().PolygonVertices[0]), Point(1.5, 1.0));
      EXPECT_EQ(factory.GetPoint(factory.GetPolygon().PolygonVertices[1]), Point(5.6, 1.5));
      EXPECT_EQ(factory.GetPoint(factory.GetPolygon().PolygonVertices[2]), Point(5.5, 4.8));
      EXPECT_EQ(factory.GetPoint(factory.GetPolygon().PolygonVertices[3]), Point(4.0, 6.2));
      EXPECT_EQ(factory.GetPoint(factory.GetPolygon().PolygonVertices[4]), Point(3.2, 4.2));
      EXPECT_EQ(factory.GetPoint(factory.GetPolygon().PolygonVertices[5]), Point(1.0, 4.0));

      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(0).From), pointlist[0]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(0).To), pointlist[1]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(1).From), pointlist[1]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(1).To), pointlist[2]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(2).From), pointlist[2]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(2).To), pointlist[3]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(3).From), pointlist[3]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(3).To), pointlist[4]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(4).From), pointlist[4]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(4).To), pointlist[5]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(5).From), pointlist[5]);
      EXPECT_EQ(factory.GetPoint(factory.GetPolygonEdge(5).To), pointlist[0]);

      EXPECT_EQ(factory.GetPoint(factory.GetLine().From), Point(2.0, 3.7));
      EXPECT_EQ(factory.GetPoint(factory.GetLine().To), Point(4.1, 5.9));

    }
    catch (const exception &exception)
    {
      FAIL();
    }
  }

}

#endif // __TEST_GEOMETRYFACTORY_H
