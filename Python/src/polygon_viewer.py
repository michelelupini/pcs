from src import geometry_core as g

### \class PolygonViewer contains methods to visualize the polygon and the line
class PolygonViewer():
    ### \brief construct the reference to GeometryFactory
    def __init__(self, geometryFactory: g.IGeometryFactory):
        self.__geometryFactory = geometryFactory

    ### \brief ViewPolygon() creates a file .m in order to plot the initial polygon and the cutting segment
    def view_polygon(self, file_name: str):
        ### Open file
        try:
           ofile = open(file_name, 'w')
        except IOError:
            raise RuntimeError("Cannot open file. Something went wrong")

        initialPolygon = self.__geometryFactory.GetPolygon()

        ### Data input
        # scrivo i punti che sono i vertici del poligono
        ofile.write("points = [")
        for i in range(0, self.__geometryFactory.GetPolygon().number_vertices - 1):
            ofile.write(str(self.__geometryFactory.GetPoint(initialPolygon.polygon_vertices[i]).x) + ", " +\
                        str(self.__geometryFactory.GetPoint(initialPolygon.polygon_vertices[i]).y) + ";\n")
        ofile.write(str(self.__geometryFactory.GetPoint(initialPolygon.polygon_vertices[self.__geometryFactory.GetPolygon().number_vertices - 1]).x) + ", " +\
                        str(self.__geometryFactory.GetPoint(initialPolygon.polygon_vertices[self.__geometryFactory.GetPolygon().number_vertices - 1]).y) + "];\n")
        # scrivo i comandi che su matlab permettono di costruire un poligono
        ofile.write("polygon = polyshape(points(:, 1),points(:, 2));\n")
        # scrivo i punti che sono gli estremi del segmento tagliante
        ofile.write("segment = [")
        ofile.write(str(self.__geometryFactory.GetPoint(self.__geometryFactory.GetLine().From).x) + ", " +\
                    str(self.__geometryFactory.GetPoint(self.__geometryFactory.GetLine().From).y) + ";\n" + \
                    str(self.__geometryFactory.GetPoint(self.__geometryFactory.GetLine().To).x) + ", " + \
                    str(self.__geometryFactory.GetPoint(self.__geometryFactory.GetLine().To).y) + "];\n\n")

        ### Functions
        # scrivo i comandi che su matlab permettono di plottare segmenti,poligoni e punti
        ofile.write("figure;" + "\n" + "plot(polygon);" + "\n" + "hold on;" + "\n" +\
                    "plot(segment(:, 1), segment(:, 2), 'r--');" + "\n" + "hold on;" + "\n" +\
                    "plot(points(:, 1),points(:, 2), 'ko');" + "\n" + "hold on;" + "\n" +\
                    "plot(segment(:, 1), segment(:, 2), 'r*');" + "\n")

        ### Close file
        ofile.close()

    ### \brief ViewLine() creates a file .m in order to plot the cutting segment
    def view_line(self, file_name: str):
        ### Open file
        try:
            ofile = open(file_name, 'w')
        except IOError:
            raise RuntimeError("Cannot open file. Something went wrong")

        ### Data input
        # scrivo i punti che sono gli estremi del segmento tagliante
        ofile.write("segment = [")
        ofile.write(str(self.__geometryFactory.GetPoint(self.__geometryFactory.GetLine().From).x) + ", " +\
                    str(self.__geometryFactory.GetPoint(self.__geometryFactory.GetLine().From).y) + ";\n" + \
                    str(self.__geometryFactory.GetPoint(self.__geometryFactory.GetLine().To).x) + ", " + \
                    str(self.__geometryFactory.GetPoint(self.__geometryFactory.GetLine().To).y) + "];\n\n")

        ### Functions
        # scrivo i comandi che su matlab permettono di plottare segmenti e punti
        ofile.write("figure;" + "\n" + "plot(segment(:, 1), segment(:, 2), 'r--');" + "\n" +\
                    "hold on;" + "\n" + "plot(segment(:, 1), segment(:, 2), 'r*');" + "\n")

        ### Close file
        ofile.close()
