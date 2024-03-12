#ifndef POLYGONADVANCED_H
#define POLYGONADVANCED_H

#include "GeometryCore.hpp"

using namespace GeometryLibrary;

namespace PolygonAdvancedLibrary {

  /// \class PolygonAdvanced is a vector of Point
  class PolygonAdvanced : public IPolygonAdvanced {
    private:
      int _numberVertices;             /// \param number of vertices of the polygon
      vector<Point> _polygonVertices;  /// \param vertices of the polygon

      /// \brief reset all the variables
      void Reset();

    public:
      /// \brief PolygonAdvanced() reset the polygon
      PolygonAdvanced(const vector<Point> &points);
      PolygonAdvanced() {  }

      /// Getters
      /// \brief GetNumberPolygonVertices() returns the number of the vertices
      /// \brief GetPolygonVertex(i) returns the i-th vertex
      int& GetNumberPolygonVertices() { return _numberVertices; };
      Point& GetPolygonVertex(const int& index) { return _polygonVertices[index]; };
      vector<Point>& GetVertices() { return _polygonVertices; };

      /// Setters
      /// \brief SetPolygonVertex(point) add point to _polygonVertices
      void SetPolygonVertex(const Point& point);

      /// \brief ComputeArea() compute the area of the polygon
      double ComputeArea();

      /// Operators in order to compare polygons
      bool operator== (const PolygonAdvanced& rhs) const;

      /// \brief PrintDebug(filepath) prints an .m file to show the polygon on matlab
      void PrintDebug(const string& filepath);
  };
}

#endif // POLYGONADVANCED_H
