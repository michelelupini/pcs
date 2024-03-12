from unittest import TestCase

import sys
sys.path.insert(1, '../')

import src.dataset as d
import src.geometry_factory as geometry_factory
import src.polygon_viewer as geometry_viewer
import src.polygon_edit as polygon_edit


class TestPentagon(TestCase):
    def test_check_side(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("pentagon_input.txt")

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            self.assertEqual(factory.GetPolygon().number_vertices, 5)
            self.assertEqual(editingP.check_side(0), -1)
            self.assertEqual(editingP.check_side(1), 0)
            self.assertEqual(editingP.check_side(2), 1)
            self.assertEqual(editingP.check_side(3), 1)
            self.assertEqual(editingP.check_side(4), -1)

        except Exception as ex:
            self.fail()

    def test_cut_polygon(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("pentagon_input.txt")

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            editingP.cut_polygon()

        except Exception as ex:
            self.fail()

        check_list = [
            [d.Point(2.5, 1.0),
             d.Point(4.0, 2.1),
             d.Point(3.6, 2.2),
             d.Point(1.4, 2.75),
             d.Point(1.2, 2.8),
             d.Point(1.0, 2.1)],

            [d.Point(1.6, 4.2),
             d.Point(1.2, 2.8),
             d.Point(1.4, 2.75),
             d.Point(3.6, 2.2),
             d.Point(4.0, 2.1),
             d.Point(3.4, 4.2)]
        ]

        try:
            self.assertEqual(factory.GetPointsNumber(), 8)

            check_pol = []
            for i in range(0, len(check_list)):
                check_pol_vert = [factory.GetPointAddress(check_list[i][j]) for j in range(0, len(check_list[i]))]
                check_pol.append(d.Polygon(check_pol_vert))

        except Exception as ex:
            self.fail()

        try:
            self.assertEqual(factory.GetNumberSubPolygons(), 2)
            self.assertEqual(factory.GetPolygon().number_vertices, 5)

            for i in range(2):
                self.assertEqual(factory.GetSubPolygon(i), check_pol[i])

        except Exception as ex:
            self.fail()

    def test_viewer(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("pentagon_input.txt")

        viewer = geometry_viewer.PolygonViewer(factory)

        viewer.view_polygon("view_pentagon.m")