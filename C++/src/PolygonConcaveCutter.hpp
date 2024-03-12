#ifndef POLYGONCUTTERALPHA_H
#define POLYGONCUTTERALPHA_H

#include "PolygonEdit.hpp"

namespace PolygonCuttersLibrary {

  /// \class cuts a convex polygon with "ad hoc" methods
  class ConcaveCutter {

    private:
      /// \brief reference to the interface of PolygonEdit
      IPolygonEdit& _editPolygon;

      /// \param _borderPoints: holds the indices of all points which lies on the boundary of the polygon, both original vertices and intersections;
      ///        point indices are stored without duplicates (counterclockwise order is kept)
      /// \param _linePoints: holds the indices of all points which lies on the cutting line, both intersections and the line defining couple of points;
      ///        no duplicates, indices kept in order by their curvilinear coordinate on the line
      /// \param _sideList: map whose key is a point index and the mapped value is an integer (1, 0 or -1) representing which side of the line the key-point lies on
      ///        (respectively left, on or right)
      /// \param _inOutList: map whose key is a point index and the mapped value is an integer (1, 0 or -1) representing what kind of points are the ones in _linePoints
      ///        (respectively an entering intersection, an exiting one or none of both [default assigned to the From and To points of the cutting line])
      /// \param _graph: map, representing a graph, which use point-ids as key and their respective adjacency informations
      vector<int>_borderPoints;
      vector<int> _linePoints;
      map<int, const int> _sideList; //<punti, lati>
      map<int, int> _inOutList; //intersezioni entranti (1) o uscenti (0), punti definenti retta (-1)
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

      // nel caso di intersezione interna al segmento si usi il prodotto vettoriale (funzione non necessaria)
      // in input prende vettori 3D, la cui terza componente è nulla
      /// \brief IsEntering: given a vector and two consecutive segments of a polygon, determines if the vector is entering in the polygon throught the vertex
      /// \param line: vector which can enter or not
      /// \param fistSegment: first segment counterclockwise
      /// \param secondSegment second segment counterclockwise
      /// \return 1 if the vector is entering in the polygon, 0 else
      int IsEntering(const Segment &segmentLine, const Segment &firstSegment, const Segment &secondSegment);
      int IsEntering(const Segment &line, const Segment &edge);
      void AddinInOutList(const int& tmpadd, const int& side);

  public:
      /// \brief class constructor, create reference to IPolygonEdit and alloc memory for _borderPoints and _sidePoint
      ConcaveCutter(IPolygonEdit& beCutted);
      /// \brief ComputeNewPoints() calculates the intersections of the polygon with the cutting line, adding them in _points (in GeometryFactory class)
      ///        it also finishes _borderPoints and fills _linePoints
      /// \brief FindSubpolygons() calls mostly private methods which had to be called all in once and looks in _borderPoints for starting points for FindCycle
      void ComputeNewPoints();
      void FindSubpolygons();

  };

};


#endif // POLYGONCUTTERALPHA_H
