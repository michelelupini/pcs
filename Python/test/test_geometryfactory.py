from unittest import TestCase

import sys
sys.path.insert(1, '../')

import src.geometry_factory as geometry_factory
import src.dataset as d


class TestGeometryFactory(TestCase):
    def test_CreatePolygon(self):
        try:
            geometryFactory = geometry_factory.GeometryFactory()
            geometryFactory.CreatePolygon("fakeinput.txt")
            self.fail()
        except Exception as ex:
            self.assertEqual(str(ex), "Cannot open file. Something went wrong")

        try:
            geometryFactory = geometry_factory.GeometryFactory()
            geometryFactory.CreatePolygon("bad_input_1.txt")
            self.fail()
        except Exception as ex:
            self.assertEqual(str(ex), "Syntax error: 'bad points input'")

        try:
            geometryFactory = geometry_factory.GeometryFactory()
            geometryFactory.CreatePolygon("bad_input_2.txt")
            self.fail()
        except Exception as ex:
            self.assertEqual(str(ex), "Syntax error: 'bad polygon vertices input'")

        try:
            geometryFactory = geometry_factory.GeometryFactory()
            geometryFactory.CreatePolygon("bad_input_3.txt")
            self.fail()
        except Exception as ex:
            self.assertEqual(str(ex), "Syntax error: 'bad segment input'")

        geometryFactory = geometry_factory.GeometryFactory()

        try:
            geometryFactory.CreatePolygon("input.txt")
        except Exception as ex:
            self.fail()

        try:
            self.assertEqual(geometryFactory.GetPointsNumber(), 8)
            self.assertEqual(geometryFactory.GetPolygon().number_vertices, 6)

            self.assertEqual(geometryFactory.GetPoint(0), d.Point(1.5, 1.0))
            self.assertEqual(geometryFactory.GetPoint(1), d.Point(5.6, 1.5))
            self.assertEqual(geometryFactory.GetPoint(2), d.Point(5.5, 4.8))
            self.assertEqual(geometryFactory.GetPoint(3), d.Point(4.0, 6.2))
            self.assertEqual(geometryFactory.GetPoint(4), d.Point(3.2, 4.2))
            self.assertEqual(geometryFactory.GetPoint(5), d.Point(1.0, 4.0))
            self.assertEqual(geometryFactory.GetPoint(6), d.Point(2.0, 3.7))
            self.assertEqual(geometryFactory.GetPoint(7), d.Point(4.1, 5.9))

            test_polygon: d.Polygon = geometryFactory.GetPolygon()
            pointList = [d.Point(1.5, 1.0),
                         d.Point(5.6, 1.5),
                         d.Point(5.5, 4.8),
                         d.Point(4.0, 6.2),
                         d.Point(3.2, 4.2),
                         d.Point(1.0, 4.0)]

            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygon().polygon_vertices[0]),
                             d.Point(1.5, 1.0))
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygon().polygon_vertices[1]),
                             d.Point(5.6, 1.5))
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygon().polygon_vertices[2]),
                             d.Point(5.5, 4.8))
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygon().polygon_vertices[3]),
                             d.Point(4.0, 6.2))
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygon().polygon_vertices[4]),
                             d.Point(3.2, 4.2))
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygon().polygon_vertices[5]),
                             d.Point(1.0, 4.0))

            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(0).From), pointList[0])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(0).To), pointList[1])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(1).From), pointList[1])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(1).To), pointList[2])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(2).From), pointList[2])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(2).To), pointList[3])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(3).From), pointList[3])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(3).To), pointList[4])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(4).From), pointList[4])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(4).To), pointList[5])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(5).From), pointList[5])
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetPolygonEdge(5).To), pointList[0])

            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetLine().From), d.Point(2.0, 3.7))
            self.assertEqual(geometryFactory.GetPoint(geometryFactory.GetLine().To), d.Point(4.1, 5.9))

        except Exception as ex:
            self.fail()
