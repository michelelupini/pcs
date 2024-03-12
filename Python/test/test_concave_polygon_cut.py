from unittest import TestCase

import sys
sys.path.insert(1, '../')

import src.dataset as d
import src.geometry_factory as geometry_factory
import src.polygon_viewer as geometry_viewer
import src.polygon_edit as polygon_edit

class TestConcavePolygon(TestCase):
    def test_check_side(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("concave_polygon_input.txt")

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            self.assertEqual(6, factory.GetPolygon().number_vertices)
            self.assertEqual(-1, editingP.check_side(0))
            self.assertEqual(-1, editingP.check_side(1))
            self.assertEqual(-1, editingP.check_side(2))
            self.assertEqual(1, editingP.check_side(3))
            self.assertEqual(-1, editingP.check_side(4))
            self.assertEqual(1, editingP.check_side(5))

        except Exception as ex:
            self.fail()

    def test_cut_polygon(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("concave_polygon_input.txt")

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            editingP.cut_polygon()

        except Exception as ex:
            self.fail()

        try:
            self.assertEqual(12, factory.GetPointsNumber())

            self.assertEqual(d.Point(1.5, 1.0), factory.GetPoint(0))
            self.assertEqual(d.Point(5.6, 1.5), factory.GetPoint(1))
            self.assertEqual(d.Point(5.5, 4.8), factory.GetPoint(2))
            self.assertEqual(d.Point(4.0, 6.2), factory.GetPoint(3))
            self.assertEqual(d.Point(3.2, 4.2), factory.GetPoint(4))
            self.assertEqual(d.Point(1.0, 4.0), factory.GetPoint(5))
            self.assertEqual(d.Point(2.0, 3.7), factory.GetPoint(6))
            self.assertEqual(d.Point(4.1, 5.9), factory.GetPoint(7))
            self.assertEqual(d.Point(4.2043269230769, 6.0092948717949), factory.GetPoint(8))
            self.assertEqual(d.Point(3.7213114754098, 5.5032786885246), factory.GetPoint(9))
            self.assertEqual(d.Point(2.4085972850679, 4.1280542986425), factory.GetPoint(10))
            self.assertEqual(d.Point(1.1912162162162, 2.8527027027027), factory.GetPoint(11))

        except Exception as ex:
            self.fail()

        try:
            self.assertEqual(3, factory.GetNumberSubPolygons())

            self.assertEqual(10, factory.GetSubPolygon(0).number_vertices)
            self.assertEqual(2, factory.GetSubPolygon(0).polygon_vertices[0])
            self.assertEqual(8, factory.GetSubPolygon(0).polygon_vertices[1])
            self.assertEqual(7, factory.GetSubPolygon(0).polygon_vertices[2])
            self.assertEqual(9, factory.GetSubPolygon(0).polygon_vertices[3])
            self.assertEqual(4, factory.GetSubPolygon(0).polygon_vertices[4])
            self.assertEqual(10, factory.GetSubPolygon(0).polygon_vertices[5])
            self.assertEqual(6, factory.GetSubPolygon(0).polygon_vertices[6])
            self.assertEqual(11, factory.GetSubPolygon(0).polygon_vertices[7])
            self.assertEqual(0, factory.GetSubPolygon(0).polygon_vertices[8])
            self.assertEqual(1, factory.GetSubPolygon(0).polygon_vertices[9])

            self.assertEqual(4, factory.GetSubPolygon(1).number_vertices)
            self.assertEqual(3, factory.GetSubPolygon(1).polygon_vertices[0])
            self.assertEqual(9, factory.GetSubPolygon(1).polygon_vertices[1])
            self.assertEqual(7, factory.GetSubPolygon(1).polygon_vertices[2])
            self.assertEqual(8, factory.GetSubPolygon(1).polygon_vertices[3])

            self.assertEqual(4, factory.GetSubPolygon(2).number_vertices)
            self.assertEqual(5, factory.GetSubPolygon(2).polygon_vertices[0])
            self.assertEqual(11, factory.GetSubPolygon(2).polygon_vertices[1])
            self.assertEqual(6, factory.GetSubPolygon(2).polygon_vertices[2])
            self.assertEqual(10, factory.GetSubPolygon(2).polygon_vertices[3])

        except Exception as ex:
            self.fail()

    def test_viewer(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("concave_polygon_input.txt")

        viewer = geometry_viewer.PolygonViewer(factory)

        viewer.view_polygon("view_concave.m")
