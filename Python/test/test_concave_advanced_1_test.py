from unittest import TestCase

import sys
sys.path.insert(1, '../')

import src.dataset as d
import src.geometry_factory as geometry_factory
import src.polygon_viewer as geometry_viewer
import src.polygon_edit as polygon_edit

class TestConcaveAdvanced(TestCase):
    def test_check_side(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("concave_advanced_1_input.txt")

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            self.assertEqual(10, factory.GetPolygon().number_vertices)
            self.assertEqual(-1, editingP.check_side(0))
            self.assertEqual(0, editingP.check_side(1))
            self.assertEqual(-1, editingP.check_side(2))
            self.assertEqual(0, editingP.check_side(3))
            self.assertEqual(-1, editingP.check_side(4))
            self.assertEqual(-1, editingP.check_side(5))
            self.assertEqual(1, editingP.check_side(6))
            self.assertEqual(1, editingP.check_side(7))
            self.assertEqual(0, editingP.check_side(8))
            self.assertEqual(1, editingP.check_side(9))

        except Exception as ex:
            self.fail()

    def test_cut_polygon(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("concave_advanced_1_input.txt")

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            editingP.cut_polygon()

        except Exception as ex:
            self.fail()

        try:
            self.assertEqual(14, factory.GetPointsNumber())

            self.assertEqual(d.Point(2.0, -2.0), factory.GetPoint(0))
            self.assertEqual(d.Point(0.0, -1.0), factory.GetPoint(1))
            self.assertEqual(d.Point(3.0, 1.0), factory.GetPoint(2))
            self.assertEqual(d.Point(0.0, 2.0), factory.GetPoint(3))
            self.assertEqual(d.Point(3.0, 2.0), factory.GetPoint(4))
            self.assertEqual(d.Point(3.0, 3.0), factory.GetPoint(5))
            self.assertEqual(d.Point(-1.0, 3.0), factory.GetPoint(6))
            self.assertEqual(d.Point(-3.0, 1.0), factory.GetPoint(7))
            self.assertEqual(d.Point(0.0, 0.0), factory.GetPoint(8))
            self.assertEqual(d.Point(-3.0, -2.0), factory.GetPoint(9))
            self.assertEqual(d.Point(0.0, -3.0), factory.GetPoint(10))
            self.assertEqual(d.Point(0.0, 4.0), factory.GetPoint(11))
            self.assertEqual(d.Point(0.0, 3.0), factory.GetPoint(12))
            self.assertEqual(d.Point(0.0, -2.0), factory.GetPoint(13))

        except Exception as ex:
            self.fail()

        try:
            self.assertEqual(5, factory.GetNumberSubPolygons())

            self.assertEqual(3, factory.GetSubPolygon(0).number_vertices)
            self.assertEqual(0, factory.GetSubPolygon(0).polygon_vertices[0])
            self.assertEqual(1, factory.GetSubPolygon(0).polygon_vertices[1])
            self.assertEqual(13, factory.GetSubPolygon(0).polygon_vertices[2])

            self.assertEqual(4, factory.GetSubPolygon(1).number_vertices)
            self.assertEqual(2, factory.GetSubPolygon(1).polygon_vertices[0])
            self.assertEqual(3, factory.GetSubPolygon(1).polygon_vertices[1])
            self.assertEqual(8, factory.GetSubPolygon(1).polygon_vertices[2])
            self.assertEqual(1, factory.GetSubPolygon(1).polygon_vertices[3])

            self.assertEqual(4, factory.GetSubPolygon(2).number_vertices)
            self.assertEqual(5, factory.GetSubPolygon(2).polygon_vertices[0])
            self.assertEqual(12, factory.GetSubPolygon(2).polygon_vertices[1])
            self.assertEqual(3, factory.GetSubPolygon(2).polygon_vertices[2])
            self.assertEqual(4, factory.GetSubPolygon(2).polygon_vertices[3])

            self.assertEqual(5, factory.GetSubPolygon(3).number_vertices)
            self.assertEqual(7, factory.GetSubPolygon(3).polygon_vertices[0])
            self.assertEqual(8, factory.GetSubPolygon(3).polygon_vertices[1])
            self.assertEqual(3, factory.GetSubPolygon(3).polygon_vertices[2])
            self.assertEqual(12, factory.GetSubPolygon(3).polygon_vertices[3])
            self.assertEqual(6, factory.GetSubPolygon(3).polygon_vertices[4])

            self.assertEqual(4, factory.GetSubPolygon(4).number_vertices)
            self.assertEqual(9, factory.GetSubPolygon(4).polygon_vertices[0])
            self.assertEqual(13, factory.GetSubPolygon(4).polygon_vertices[1])
            self.assertEqual(1, factory.GetSubPolygon(4).polygon_vertices[2])
            self.assertEqual(8, factory.GetSubPolygon(4).polygon_vertices[3])

        except Exception as ex:
            self.fail()

    def test_viewer(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("concave_advanced_1_input.txt")

        viewer = geometry_viewer.PolygonViewer(factory)

        viewer.view_polygon("view_concave_advanced_1.m")
