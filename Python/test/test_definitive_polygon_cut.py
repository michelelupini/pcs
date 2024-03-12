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
        factory.CreatePolygon("definitive_polygon_input.txt")

        #check = {
         #   {0, 1, 24, 27},
          #  {2, 3, 4, 5, 22, 6, 8, 19, 20, 21, 27, 24},
           # {7, 8, 6},
        #    {10, 25, 26, 13, 14, 15, 16, 17, 23, 19, 8, 9},
         #   {12, 26, 25, 11},
          #  {18, 19, 23, 17}
        #}

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            self.assertEqual(22, factory.GetPolygon().number_vertices)

            self.assertEqual(-1, editingP.check_side(0))
            self.assertEqual(-1, editingP.check_side(1))
            self.assertEqual(1, editingP.check_side(2))
            self.assertEqual(0, editingP.check_side(3))
            self.assertEqual(1, editingP.check_side(4))
            self.assertEqual(0, editingP.check_side(5))
            self.assertEqual(0, editingP.check_side(6))
            self.assertEqual(-1, editingP.check_side(7))
            self.assertEqual(0, editingP.check_side(8))
            self.assertEqual(-1, editingP.check_side(9))
            self.assertEqual(-1, editingP.check_side(10))
            self.assertEqual(1, editingP.check_side(11))
            self.assertEqual(1, editingP.check_side(12))
            self.assertEqual(-1, editingP.check_side(13))
            self.assertEqual(0, editingP.check_side(14))
            self.assertEqual(-1, editingP.check_side(15))
            self.assertEqual(0, editingP.check_side(16))
            self.assertEqual(0, editingP.check_side(17))
            self.assertEqual(1, editingP.check_side(18))
            self.assertEqual(0, editingP.check_side(19))
            self.assertEqual(1, editingP.check_side(20))
            self.assertEqual(1, editingP.check_side(21))

        except Exception as ex:
            self.fail()

    def test_cut_polygon(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("definitive_polygon_input.txt")

        editingP = polygon_edit.PolygonEdit(factory)

        try:
            editingP.cut_polygon()

        except Exception as ex:
            self.fail()

        check_list = [[d.Point(-14.0, -3.0),  # 0
                       d.Point(-12.0, -3.0),  # 1
                       d.Point(-12.0, 0.0),  # 24
                       d.Point(-14.0, 0.0)],  # 27

                      [d.Point(-12.0, 3.0),  # 2
                       d.Point(-11.0, 0.0),  # 3
                       d.Point(-10.0, 2.0),  # 4
                       d.Point(-9.0, 0.0),  # 5
                       d.Point(-7.0, 0.0),  # 22
                       d.Point(-5.0, 0.0),  # 6
                       d.Point(-2.0, 0.0),  # 8
                       d.Point(2.0, 0.0),  # 19
                       d.Point(-1.0, 5.0),  # 20
                       d.Point(-14.0, 5.0),  # 21
                       d.Point(-14.0, 0.0),  # 27,
                       d.Point(-12.0, 0.0)  # 24
                       ],

                      [d.Point(-3.0, -2.0),  # 7
                       d.Point(-2.0, 0.0),  # 8
                       d.Point(-5.0, 0.0)  # 6
                       ],

                      [d.Point(14.0, -5.0),  # 10
                       d.Point(14.0, 0.0),  # 25
                       d.Point(12.0, 0.0),  # 26
                       d.Point(12.0, -3.0),  # 13
                       d.Point(11.0, 0.0),  # 14
                       d.Point(10.0, -2.0),  # 15
                       d.Point(9.0, 0.0),  # 16
                       d.Point(5.0, 0.0),  # 17
                       d.Point(3.0, 0.0),  # 23
                       d.Point(2.0, 0.0),  # 19
                       d.Point(-2.0, 0.0),  # 8
                       d.Point(1.0, -5.0)  # 9
                       ],

                      [d.Point(12.0, 3.0),  # 12
                       d.Point(12.0, 0.0),  # 26
                       d.Point(14.0, 0.0),  # 25
                       d.Point(14.0, 3.0)  # 11
                       ],

                      [d.Point(3.0, 2.0),  # 18
                       d.Point(2.0, 0.0),  # 19
                       d.Point(3.0, 0.0),  # 23
                       d.Point(5.0, 0.0)  # 17
                       ]
                      ]

        try:
            self.assertEqual(28, factory.GetPointsNumber())

            check_pol = []
            for i in range(0, len(check_list)):
                check_pol_vert = [factory.GetPointAddress(check_list[i][j]) for j in range(0, len(check_list[i]))]
                check_pol.append(d.Polygon(check_pol_vert))

        except Exception as ex:
            self.fail()

        try:
            self.assertEqual(factory.GetNumberSubPolygons(), 6)
            self.assertEqual(factory.GetPolygon().number_vertices, 22)

            for i in range(6):
                self.assertEqual(factory.GetSubPolygon(i), check_pol[i])

        except Exception as ex:
            self.fail()

    def test_viewer(self):
        factory = geometry_factory.GeometryFactory()
        factory.CreatePolygon("definitive_polygon_input.txt")

        viewer = geometry_viewer.PolygonViewer(factory)

        viewer.view_polygon("view_definitive_polygon.m")