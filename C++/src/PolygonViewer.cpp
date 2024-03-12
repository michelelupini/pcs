#include "PolygonViewer.hpp"

namespace PolygonViewerLibrary {

  void PolygonViewer::ViewPolygon(const string& fileName) const
  {
    /// Open file
    ofstream ofile;
    ofile.open(fileName);
    if(ofile.fail())
      throw runtime_error("Cannot open file. Something went wrong");

    Polygon initialPolygon = _geometryFactory.GetPolygon();

    /// Data input
    // scrivo i punti che sono i vertici del poligono
    ofile << "points = [";
    for(int i = 0; i < _geometryFactory.GetPolygon().NumberVertices - 1; i++)
      ofile << _geometryFactory.GetPoint(initialPolygon.PolygonVertices[i]) << endl;
    ofile << _geometryFactory.GetPoint(initialPolygon.PolygonVertices[_geometryFactory.GetPolygon().NumberVertices - 1]) << "];" << endl;
    // scrivo i comandi che su matlab permettono di costruire un poligono
    ofile << "polygon = polyshape(points(:, 1),points(:, 2));" << endl;

    ofile << "ptlbls = \"";
    for(int i = 0; i < _geometryFactory.GetPolygon().NumberVertices-1; i++)
        ofile << i << ", ";
    ofile << _geometryFactory.GetPolygon().NumberVertices-1 << '"' << ";" << endl
          << "pointlabels = strsplit(ptlbls, [\",\", \" \"]);"<< endl << endl;


    // scrivo i punti che sono gli estremi del segmento tagliante
    ofile << "segment = [";
    ofile << _geometryFactory.GetPoint(_geometryFactory.GetLine().From) << endl
          << _geometryFactory.GetPoint(_geometryFactory.GetLine().To) << "];" << endl << endl;

    ofile << "seglabels = [ 'P1'; 'P2'];"<<endl;

    /// Functions
    // scrivo i comandi che su matlab permettono di plottare segmenti,poligoni e punti
    ofile << "figure;" << endl << "plot(polygon);" << endl << "hold on;" << endl
          << "plot(segment(:, 1), segment(:, 2), 'r--');" << endl << "hold on;" << endl
          << "plot(points(:, 1),points(:, 2), 'ko');" << endl << "hold on;" << endl
          << "plot(segment(:, 1), segment(:, 2), 'r*');" << endl
          << "text(points(:, 1),points(:, 2), pointlabels', "
          << "'VerticalAlignment','bottom','HorizontalAlignment','right', 'FontSize', 14);" << endl
          << "text(segment(:, 1), segment(:, 2), seglabels, "
          << "'VerticalAlignment','top','HorizontalAlignment','left', 'FontSize', 14, 'Color', 'r');" << endl;


    /// Close file
    ofile.close();
  }

  void PolygonViewer::ViewLine(const string& fileName) const
  {
    /// Create file
    ofstream ofile;
    ofile.open(fileName);
    if(ofile.fail())
      throw runtime_error("Cannot open file. Something went wrong");

    /// Data input
    // scrivo i punti che sono gli estremi del segmento tagliante
    ofile << "segment = [";
    ofile << _geometryFactory.GetPoint(_geometryFactory.GetLine().From) << endl
          << _geometryFactory.GetPoint(_geometryFactory.GetLine().To) << "];" << endl << endl;

    /// Functions
    // scrivo i comandi che su matlab permettono di plottare segmenti e punti
    ofile << "figure;" << endl << "plot(segment(:, 1), segment(:, 2), 'r--');" << endl
          << "hold on;" << endl << "plot(segment(:, 1), segment(:, 2), 'r*');" << endl;

    /// Close file
    ofile.close();
  }

  void PolygonViewer::ViewFinishedProblem(const string &fileName) const
  {
      /// Create file
      ofstream ofile;
      ofile.open(fileName);
      if(ofile.fail())
        throw runtime_error("Cannot open file. Something went wrong");

      ofile << "figure;" << endl;

      ofile << "ptlbls = \"";
      for(int i = 0; i < _geometryFactory.GetPointsNumber()-1; i++)
          ofile << i << ", ";
      ofile << _geometryFactory.GetPointsNumber()-1 << '"' << ";" << endl
            << "pointlabels = strsplit(ptlbls, [\",\", \" \"]);"<< endl
            << "seglabels = [ 'P1'; 'P2' ];"<< endl << endl;

      /// Data input

      ofile << "segment = [";
      ofile << _geometryFactory.GetPoint(_geometryFactory.GetLine().From) << endl
            << _geometryFactory.GetPoint(_geometryFactory.GetLine().To) << "];" << endl << endl;

      unsigned int n = _geometryFactory.GetNumberLeftSubPolygons();
      for(unsigned int i = 0; i < n; i++)
      {
          Polygon polygon = _geometryFactory.GetLeftSubPolygon(i);

          ofile << "points" << i << " = [";
          for(int j = 0; j < polygon.NumberVertices - 1; j++)
            ofile << _geometryFactory.GetPoint(polygon.PolygonVertices[j]) << endl;
          ofile << _geometryFactory.GetPoint(polygon.PolygonVertices[polygon.NumberVertices - 1]) << "];" << endl;

          ofile << "polygon" << i <<" = polyshape(points" << i << "(:, 1),points" << i << "(:, 2));" << endl;

          ofile << "plot(polygon" << i << ");"<< endl << "hold on" << endl << endl;
      }

      for(int i = 0; i < _geometryFactory.GetNumberRightSubPolygons(); i++)
      {
          Polygon polygon = _geometryFactory.GetRightSubPolygon(i);

          ofile << "points" << i + n << " = [";
          for(int j = 0; j < polygon.NumberVertices - 1; j++)
            ofile << _geometryFactory.GetPoint(polygon.PolygonVertices[j]) << endl;
          ofile << _geometryFactory.GetPoint(polygon.PolygonVertices[polygon.NumberVertices - 1]) << "];" << endl;

          ofile << "polygon" << i + n<<" = polyshape(points" << i + n<< "(:, 1),points" << i + n<< "(:, 2));" << endl;
          ofile << "plot(polygon" << i + n << ");"<< endl << "hold on" << endl << endl;
      }

      ofile << endl << "problempoints = [";
      for(int i = 0; i < _geometryFactory.GetPointsNumber(); i++)
        ofile << endl << _geometryFactory.GetPoint(i);
      ofile << "];" << endl << endl;

      ofile << endl << "plot(segment(:, 1), segment(:, 2), 'r--');" << endl
            << "hold on;" << endl << "plot(segment(:, 1), segment(:, 2), 'r*');" << endl<< endl
            << "text(problempoints(:, 1),problempoints(:, 2), pointlabels', "
            << "'VerticalAlignment','bottom','HorizontalAlignment','right', 'FontSize', 14);" << endl
            << "text(segment(:, 1), segment(:, 2), seglabels, "
            << "'VerticalAlignment','top','HorizontalAlignment','left', 'FontSize', 14, 'Color', 'r');" << endl;

      /// Close file
      ofile.close();
  }

}
