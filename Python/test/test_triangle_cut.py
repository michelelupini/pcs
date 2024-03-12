from unittest import TestCase

import sys
sys.path.insert(1, '../')

import src.dataset as d
import src.geometry_factory as geometry_factory
import src.polygon_viewer as geometry_viewer
import src.polygon_edit as polygon_edit

class TestTriangle(TestCase):
    def test_check_side(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("triangle_input.txt")

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            self.assertEqual(factory.GetPolygon().number_vertices, 3)
            self.assertEqual(editingP.check_side(0), 0)
            self.assertEqual(editingP.check_side(1), -1)
            self.assertEqual(editingP.check_side(2), 0)

        except Exception as ex:
            self.fail()

    def test_cut_polygon(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("triangle_input.txt")

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            editingP.cut_polygon()

        except Exception as ex:
            self.fail()

        try:
            self.assertEqual(factory.GetPointsNumber(), 5)

            self.assertEqual(factory.GetPoint(0), d.Point(0.0, 0.0))
            self.assertEqual(factory.GetPoint(1), d.Point(4.0, 0.0))
            self.assertEqual(factory.GetPoint(2), d.Point(2.0, 2.0))
            self.assertEqual(factory.GetPoint(3), d.Point(-1.0, -1.0))
            self.assertEqual(factory.GetPoint(4), d.Point(1.0, 1.0))

        except Exception as ex:
            self.fail()

        try:
            number_sub_polygons = factory.GetNumberSubPolygons()
            self.assertEqual(number_sub_polygons, 1)

            self.assertEqual(factory.GetSubPolygon(0).number_vertices, 4)

            self.assertEqual(factory.GetSubPolygon(0).polygon_vertices[0], 1)
            self.assertEqual(factory.GetSubPolygon(0).polygon_vertices[1], 2)
            self.assertEqual(factory.GetSubPolygon(0).polygon_vertices[2], 4)
            self.assertEqual(factory.GetSubPolygon(0).polygon_vertices[3], 0)

        except Exception as ex:
            self.fail()

    def test_viewer(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("triangle_input.txt")

        viewer = geometry_viewer.PolygonViewer(factory)

        viewer.view_polygon("view_triangle.m")
