from unittest import TestCase

import sys
sys.path.insert(1, '../')

import src.polygon_viewer as polygon_viewer
import src.geometry_factory as geometry_factory


class TestViewer(TestCase):
    def test_view_polygon(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("pentagon_input.txt")

        viewer = polygon_viewer.PolygonViewer(factory)

        try:
            viewer.view_polygon("view_polygon.m")
        except Exception as ex:
            self.fail()

    def test_view_line(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("pentagon_input.txt")

        viewer = polygon_viewer.PolygonViewer(factory)

        try:
            viewer.view_line("view_line.m")
        except Exception as ex:
            self.fail()
