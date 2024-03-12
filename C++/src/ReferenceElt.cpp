#include "ReferenceElt.hpp"

namespace PolygonAdvancedLibrary {

  void ReferenceElt::FindMeshPolygons(const vector<Point> &polygonPoints, const vector<Point> &borderPoints, const map<int, int> linker)
  {
    int numberBorderPoints = borderPoints.size();
    int numberPolygonPoints = polygonPoints.size();

    // set the central polygon as _polygons[0]
    _polygons.push_back(PolygonAdvanced { polygonPoints });

    for(int i = 0; i < numberBorderPoints; i++)  // for each point laying on the bounding box
    {
      if(linker.at(i) != -1)  // if it coincides with a point of the central polygon
      {
        // if the following point of the central polygon does NOT coincide with the following point in borderPoints
        //   i.e. the following point in borderPoints does NOT coincide with a point of the central polygon
        if(!(polygonPoints[(linker.at(i) + 1) % numberPolygonPoints] == borderPoints[(i + 1) % numberBorderPoints]))
        {
          int j = i;  // index used to scan the defining subPolygon points
          vector<Point> subPolygonPoints;  // auxiliary vector of point, used to construct the subPolygon

          // scan counterclockwise borderPoints constructing subPolygonPoints, until an intersection point is found
          do
          {
            subPolygonPoints.push_back(borderPoints[j % numberBorderPoints]);
            j++;
          } while(linker.at(j % numberBorderPoints) == -1);

          // pass to the points of the central polygon and scan them clockwise constructing subPolygonPoints,
          //   until the initial point is found
          j = linker.at(j % numberBorderPoints);
          do
          {
            subPolygonPoints.push_back(polygonPoints[j]);

            j = (j+numberPolygonPoints - 1)%numberPolygonPoints;

          } while(!(polygonPoints[j] == subPolygonPoints[0]));

          _polygons.push_back(PolygonAdvanced { subPolygonPoints });
        }
      }
    }
  }

  void ReferenceElt::ComputeBoundingBox(const vector<Point> &points)
  {
    // components of the extremal points
    double minX = points[0].X;
    double maxX = points[0].X;
    double minY = points[0].Y;
    double maxY = points[0].Y;

    for(unsigned int i = 0; i < points.size(); i++)
    {
      // evaluation of maximum and minimum components
      if(points[i].X < minX)
        minX = points[i].X;
      if (points[i].X > maxX)
        maxX = points[i].X;
      if(points[i].Y < minY)
        minY = points[i].Y;
      if (points[i].Y > maxY)
        maxY = points[i].Y;
    }

    Point point;  // auxiliary point, in order to define the boundingBox

    point.X = minX;
    point.Y = minY;
    _boundingBox.push_back(point);

    point.X = maxX;
    point.Y = minY;
    _boundingBox.push_back(point);

    point.X = maxX;
    point.Y = maxY;
    _boundingBox.push_back(point);

    point.X = minX;
    point.Y = maxY;
    _boundingBox.push_back(point);
  }

  void ReferenceElt::ComputeIntersections(const vector<Point> &polygonPoints, vector<Point> &borderPoints)
  {
    // vectors contituting the 4 segments of the bounding box
    vector<Point> south, east, north, west;

    // set the initial point for each segment
    south.push_back(_boundingBox[0]);
    east.push_back(_boundingBox[1]);
    north.push_back(_boundingBox[2]);
    west.push_back(_boundingBox[3]);

    /// Find intersections (unordered)
    // a point of the central polygon is an intersection, if one its coordinates coincides with a the coordinate of a bounding box point
    // in order to ignore duplicates, it does not consider interections that coincide with a vertex of the boundig box
    for(unsigned int i = 0; i < polygonPoints.size(); i++)
    {
      if(polygonPoints[i].Y == _boundingBox[0].Y && polygonPoints[i].X != _boundingBox[0].X && polygonPoints[i].X != _boundingBox[1].X)
        south.push_back(polygonPoints[i]);
      if(polygonPoints[i].X == _boundingBox[1].X && polygonPoints[i].Y != _boundingBox[1].Y && polygonPoints[i].Y != _boundingBox[2].Y)
        east.push_back(polygonPoints[i]);
      if(polygonPoints[i].Y == _boundingBox[3].Y && polygonPoints[i].X != _boundingBox[2].X && polygonPoints[i].X != _boundingBox[3].X)
        north.push_back(polygonPoints[i]);
      if(polygonPoints[i].X == _boundingBox[0].X && polygonPoints[i].Y != _boundingBox[3].Y && polygonPoints[i].Y != _boundingBox[0].Y)
        west.push_back(polygonPoints[i]);
    }

    int ns = south.size();
    int ne = east.size();
    int nw = west.size();
    int nn = north.size();

    /// Sort points with respect to the coordinates (SelectionSort)

    for(int i = 0; i < ns - 1; i++)
    {
      int indexMin = i;

      for(int j = i + 1; j < ns; j++)
      {
        if(south[j].X < south[indexMin].X)
          indexMin = j;
      }
      if(indexMin != i)
      {
        Point temp;
        temp = south[i];
        south[i] = south[indexMin];
        south[indexMin] = temp;
      }
    }

    for(int i = 0; i < ne - 1; i++)
    {
      int indexMin = i;

      for(int j = i + 1; j < ne; j++)
      {
        if(east[j].Y < east[indexMin].Y)
          indexMin = j;
      }
      if(indexMin != i)
      {
        Point temp;
        temp = east[i];
        east[i] = east[indexMin];
        east[indexMin] = temp;
      }
    }

    for(int i = 0; i < nn - 1; i++)
    {
      int indexMax = i;

      for(int j = i + 1; j < nn; j++)
      {
        if(north[j].X > north[indexMax].X)
          indexMax = j;
      }
      if(indexMax != i)
      {
        Point temp;
        temp = north[i];
        north[i] = north[indexMax];
        north[indexMax] = temp;
      }
    }

    for(int i = 0; i < nw - 1; i++)
    {
      int indexMax = i;

      for(int j = i + 1; j < nw; j++)
      {
        if(west[j].Y > west[indexMax].Y)
          indexMax = j;
      }
      if(indexMax != i)
      {
        Point temp;
        temp = west[i];
        west[i] = west[indexMax];
        west[indexMax] = temp;
      }
    }

    /// Merge segments and construct borderPoints
    for(int i = 0; i < ns; i++)
      borderPoints.push_back(south[i]);
    for(int i = 0; i < ne; i++)
      borderPoints.push_back(east[i]);
    for(int i = 0; i < nn; i++)
      borderPoints.push_back(north[i]);
    for(int i = 0; i < nw; i++)
      borderPoints.push_back(west[i]);
  }

  ReferenceElt::ReferenceElt(const vector<Point> &points)
  {
    /// Compute bounging box
    // fill _boundingBox (4 points)
    ComputeBoundingBox(points);

    /// Compute polygons
    vector<Point> borderPoints;  // vector of the points laying on the bounding box (points of the bounding box and intersections)
    map<int, int> linker;  // links all indices of the points in borderPoints to their corresponding indices of the central polygon
                           //   if there is no correspondance for the key point in borderPoints, then set -1 as second

    // fill borderPoints
    ComputeIntersections(points, borderPoints);

    // fill linker
    int numberBorderPoints = borderPoints.size();
    for(int i = 0; i < numberBorderPoints; i++)
    {
      bool found = false;
      unsigned int j;

      for(j = 0; j < points.size() && !found; j++)
          if(points[j] == borderPoints[i])
          {
              found = true;
              linker.insert(make_pair(i, j));
          }
      linker.insert(make_pair(i, -1));
    }

    // find defining polygons and fill _polygons
    FindMeshPolygons(points, borderPoints, linker);
  }


  void ReferenceElt::Translate(const double &x, const double &y)
  {
    for(unsigned int poly = 0; poly < _polygons.size(); poly++) // per ogni poligono della cella e per ogni vertice del poligono
                                            // traslo ciascun vertice delle coordinate passate in input
    {
      for(int i = 0; i < _polygons[poly].GetNumberPolygonVertices(); i++)
      {
        _polygons[poly].GetPolygonVertex(i).X += x;
        _polygons[poly].GetPolygonVertex(i).Y += y;
      }
    }

    for(unsigned int i = 0; i < _boundingBox.size(); i++) //faccio lo stesso per i punti del bounding box
    {
      _boundingBox[i].X += x;
      _boundingBox[i].Y += y;
    }
  }

  void ReferenceElt::IntTranslate(const int &x, const int &y) //traslo ogni punto del bounding box e del poligono di un vettore
  {    //  (n*x, m*y) dove x ed y sono le dimensioni del bounding box
      double boxedge_x = abs(_boundingBox[1].X - _boundingBox[0].X),
             boxedge_y = abs(_boundingBox[2].Y - _boundingBox[1].Y);
      Translate(x*boxedge_x, y*boxedge_y);
  }

  void ReferenceElt::PrintDebug(const string &fileName)
  {
      ofstream ofile;
      ofile.open(fileName);
      if(ofile.fail())
        throw runtime_error("Cannot open file. Something went wrong");

      ofile << "figure;" << endl;

      for(unsigned int j = 0 ;j <_polygons.size(); j++)
      {
          IPolygonAdvanced& polygon = _polygons[j];
          ofile << "points" << j << " = [";
          for(int i = 0; i < polygon.GetNumberPolygonVertices() - 1; i++)
              ofile << polygon.GetPolygonVertex(i) << endl;
          ofile << polygon.GetPolygonVertex(polygon.GetNumberPolygonVertices() - 1) << "];" << endl;
          ofile << "polygon" << j << " = polyshape(points" << j << "(:, 1),points" << j << "(:, 2));" << endl << endl;

          ofile << "plot(polygon" << j << ",'EdgeColor', 'k', 'LineWidth', 1);" << endl << "hold on;" << endl
                << "plot(points" << j << "(:, 1),points" << j << "(:, 2), 'ko');" << endl << "hold on;" << endl << endl;

      }

      ofile << "ptlbls = \"";
      for(char i = 'A'; i < 'A' + _polygons[0].GetNumberPolygonVertices() -1; i++)
          ofile << i << ", ";
      ofile << (char)('A' + _polygons[0].GetNumberPolygonVertices() - 1) << '"' << ";" << endl
            << "pointlabels = strsplit(ptlbls, [\",\", \" \"]);"<< endl <<endl
            << "text(points0(:, 1),points0(:, 2), pointlabels', "
            << "'VerticalAlignment','bottom','HorizontalAlignment','right', 'FontSize', 14);" << endl<< endl;

      ofile << "boxpoints = [";
      for(int i = 0; i < 3; i++)
          ofile << _boundingBox[i] << endl;
      ofile << _boundingBox[3] << "];" << endl << endl
            << "text(boxpoints(:, 1),boxpoints(:, 2), ['0', '1', '2', '3']', "
            << "'VerticalAlignment','top','HorizontalAlignment','left', 'FontSize', 14, 'Color', 'r');" << endl<< endl;;


      /// Close file
      ofile.close();




  }

}
