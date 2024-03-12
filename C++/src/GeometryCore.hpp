#ifndef GEOMETRYCORE_H
#define GEOMETRYCORE_H

#include "DataSet.hpp"

using namespace DataSet;

namespace GeometryLibrary {

  /// la documentazione dei metodi virtuali si trova nelle rispettive interfacce

  // Project 1
  /// \interface implemented by GeometryFactory
  class IGeometryFactory {
    public:
      virtual void CreatePolygon() = 0;
      virtual void CreatePolygon(const vector<Point>& points, const vector<int>& polygonVertices, const vector<Point>& segment) = 0;

      virtual int GetPointsNumber() = 0;
      virtual const Polygon& GetPolygon() = 0;
      virtual const Segment& GetPolygonEdge(const int& edgePosition) = 0;
      virtual const vector<Segment>& GetAllEdges() = 0;
      virtual const Segment& GetLine() = 0;
      virtual const Point& GetPoint(const int& idPoint) = 0;
      virtual int GetPointAddress(const Point& point) = 0;
      virtual int GetNumberSubPolygons() = 0;
      virtual const Polygon& GetSubPolygon(const int& idPolygon) = 0;
      virtual vector<Polygon>& GetAllSubPolygons() = 0;   
      virtual int GetNumberLeftSubPolygons() = 0;
      virtual int GetNumberRightSubPolygons() = 0;
      virtual const Polygon& GetLeftSubPolygon(const int& idPolygon) = 0;
      virtual const Polygon& GetRightSubPolygon(const int& idPolygon) = 0;

      virtual int SetPoints(const Point& point) = 0;
      virtual void SetSubPolygons(const Polygon& newSubPolygon) = 0;
      virtual void SetLeftSubPolygons(const Polygon& newSubPolygon) = 0;
      virtual void SetRightSubPolygons(const Polygon& newSubPolygon) = 0;
  };

  /// \interface implemented by PolygonEdit
  class IPolygonEdit {
    public:
      virtual const Point GetPoint(const int& id) = 0;
      virtual IGeometryFactory& GetProblem() = 0;
      virtual int CheckSide(const int& pointId, const Segment& line) = 0;
      virtual int CheckSide(const int& pointId) = 0;
      virtual void CutPolygon() = 0;
  };


  // Project 2-3
  /// \interface implmented by PolygonAdvanced
  class IPolygonAdvanced {
    public:
      virtual int& GetNumberPolygonVertices() = 0;
      virtual Point& GetPolygonVertex(const int& idPoint) = 0;

      virtual double ComputeArea() = 0;

      virtual void PrintDebug(const string& filepath) = 0;
  };

  /// \interface implmented by ReferenceElt
  class IReferenceElt {
    public:
      virtual IPolygonAdvanced& GetPolygon(const int& polygonPosition) = 0;
      virtual vector<Point> GetBoundingBox() = 0;
      virtual void Translate(const double& x, const double& y) = 0;
      virtual void IntTranslate(const int& x, const int& y) = 0;
      virtual void PrintDebug(const string& filepath) = 0;
  };

  /// \interface implemented by AdvancedMesh
  class IAdvancedMesh {
    public:
      virtual const int& GetIntSizeMesh(const int& axis) = 0;
      virtual IPolygonAdvanced& GetDomain() = 0;
      virtual IReferenceElt& GetCell() = 0;
      virtual int GetNumberPolygonInMesh() = 0;
      virtual IPolygonAdvanced& GetPolygonInMesh(const unsigned int& id) = 0;

      virtual void ComputeMesh() = 0;
  };


}

#endif // GEOMETRYCORE_H
