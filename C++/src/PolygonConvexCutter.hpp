#ifndef POLYGONCONVEXCUTTER_H
#define POLYGONCONVEXCUTTER_H

#include "PolygonEdit.hpp"

namespace PolygonCuttersLibrary {

  /// \class cuts a convex polygon with "ad hoc" methods
  class ConvexCutter {

    private:
      /// \brief reference to the interface of PolygonEdit
      IPolygonEdit& _editPolygon;

      /// \param _borderPoints: holds the indices of all points which lies on the boundary of the polygon, both original vertices and intersections;
      ///        point indices are stored without duplicates (counterclockwise order is kept)
      /// \param _linePoints: holds the indices of all points which lies on the cutting line, both intersections and the line defining couple of points;
      ///        no duplicates, indices kept in order by their curvilinear coordinate on the line
      /// \param _sideList: map whose key is a point index and the mapped value is an integer (-1, 0 or 1) representing which side of the line the key-point lies on
      ///        (respectively right, on or left)
      /// \param _graph: map, representing a graph, which use point-ids as key and their respective adjacency informations
      vector<int> _borderPoints;
      vector<int> _linePoints;
      map<int, const int> _sideList;
      map<int, unordered_set<int>> _graph;


      /// \brief GetPoint(i) returns the i-th point in _points (N.B. not sorted) in _problem
      const Point GetPoint(const int& id){ return _editPolygon.GetProblem().GetPoint(id);};

      /// \brief IsInVector(elt, visited) returns true if searched element (Polygon or point index) is already in a vector, false elseway
      bool IsInVector(const int& elt, const vector<int>& visited);
      bool IsInVector(const Polygon& elt, const vector<Polygon>& visited);

      /// \brief InitializeGraph() fills _graph with all points involved in the problem and their respective just-computed adjacency informations
      /// \brief FillSideList() fills _sideList with all pairs made by point-id and its side-representing integer respect to the line
      /// \brief FindCycle(visited, newPoint) is a recoursive method for cycle research inside _graph, with few more conditions depicting the definition of "subpolygon"
      void InitializeGraph();
      void FillSideList();
      void FindCycle(vector<int> visited, int newPoint);

  public:
      /// \brief class constructor, create reference to IPolygonEdit and alloc memory for _borderPoints and _sidePoint
      ConvexCutter(IPolygonEdit& beCutted);

      /// \brief ComputeNewPoints() calculates the intersections of the polygon with the cutting line, adding them in _points (in GeometryFactory class)
      ///        it also finishes _borderPoints and fills _linePoints
      /// \brief FindSubpolygons() calls mostly private methods which had to be called all in once and looks in _borderPoints for starting points for FindCycle
      void ComputeNewPoints();
      void FindSubpolygons();

  };

};


#endif // POLYGONCONVEXCUTTER_H
