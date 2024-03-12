#ifndef DATASET_H
#define DATASET_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <exception>
#include "Eigen"

#define TOL 1.0E-7

using namespace std;
using namespace Eigen;

namespace DataSet {

  enum Type
  {
    NoIntersection = 0,                 /// \enum no intersection of the segments
    IntersectionOnLine = 1,             /// \enum intersection of the extension of the first segment and the second segment
    IntersectionOnSegment = 2,          /// \enum intersection of the segments
    IntersectionParallelOnLine = 3,     /// \enum parallel intersection of the extension of the first segment and the second segment
    IntersectionParallelOnSegment = 4,  /// \enum parallel intersection of the segments
  };

  enum Position
  {
    Begin = 0,  /// \enum intersection of the extension of the first segment and the second segment on the origin of the first segment
    Inner = 1,  /// \enum intersection of the extension of the first segment and the second segment inside the first segment
    End = 2,    /// \enum intersection of the extension of the first segment and the second segment on the end of the first segment
    Outer = 3,  /// \enum intersection of the extension of the first segment and the second segment out of the first segment
    Default = 4 /// \enum initial default position (no intersection)
  };


  /// \class Point is a couple of doubles, which are the coordinates of a point in a cartesian reference frame
  class Point {
    public:
      double X;
      double Y;

      Point() { };
      Point(const double x, const double y) { X = x, Y = y; };

      /// \brief Operator (==) compares two points returning True if the 2 points are identical, False otherwise
      inline bool operator== (const Point& rhs) const { return ((abs(this->X - rhs.X) < TOL) && (abs(this->Y - rhs.Y) < TOL)) ? true : false ; }

      /// \brief Operator << which is used to print the X and Y coordinates of a Point
      friend ostream& operator<<(ostream& os, const Point& point)
      {
          os << point.X << ", " << point.Y << ";";
          return os;
      };
  };

  /// \class Segment is  a couple of indexes referred to the vector _points of Point in GeometryFactory
  class Segment {
    public:
       int From;
       int To;

      Segment() { };
      Segment(const int& from, const int& to) : From(from), To(to) { };
  };

  /// \class Polygon is a vector of indexes referred to the vector _points of Point in GeometryFactory
  class Polygon {
    public:
      int NumberVertices;
      vector<int> PolygonVertices;

      Polygon() { };
      Polygon(vector <int>& polygonVertices) : PolygonVertices(polygonVertices) { NumberVertices = polygonVertices.size(); };

      /// \brief PrintDebug(fileName) creates a file .m which prints a Polygon
      void PrintDebug(const string& fileName) const
      {
          ofstream ofile;
          ofile.open(fileName);
          if(ofile.fail())
            throw runtime_error("Cannot open file. Something went wrong");

          ofile << "figure;" << endl;


          ofile << "points = [";
          for(int i = 0; i < NumberVertices - 1; i++)
              ofile << PolygonVertices[i] << endl;
          ofile << PolygonVertices[NumberVertices - 1] << "];" << endl;
          ofile << "polygon = polyshape(points(:, 1),points(:, 2));" << endl << endl;

          ofile << "plot(polygon, 'FaceColor','red','EdgeColor', 'k', 'LineWidth', 1);" << endl << "hold on;" << endl
                << "plot(points(:, 1),points(:, 2), 'ko');" << endl << "hold on;" << endl << endl;

          /// Close file
          ofile.close();
      }


      /// \brief compare two polygons, scanning their vertices until it finds two identical sequences
      bool operator== (const Polygon& rhs) const
      {
        bool exit = false;
        if(NumberVertices == rhs.NumberVertices)
        {
          for(int i = 0; !exit && i < NumberVertices; i++)
          {
            exit = true;
            for(int j = 0; exit == true && j < NumberVertices; j++)
              if(!(PolygonVertices[j] == rhs.PolygonVertices[(i + j) % NumberVertices]))
                exit = false;
          }
        }
        return exit;
      }

      /// \brief Operator << which is used to print the vertices of a Polygon
      friend ostream& operator<<(ostream& os, const Polygon& polygon)
      {
          os << "[ ";
          for(int i = 0; i < polygon.NumberVertices; i++)
              os << endl << polygon.PolygonVertices[i];
          os << "];" << endl;
          return os;
      };

  };

}

#endif // DATASET_H
