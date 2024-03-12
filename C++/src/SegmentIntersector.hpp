#ifndef SEGMENTINTERSECTOR_H
#define SEGMENTINTERSECTOR_H

#include "GeometryCore.hpp"

namespace Intersector {

  class Intersector1D1D
  {
    private:
      static double toleranceParallelism;       /// \param tolerance in evaluating parallelism
      static double toleranceIntersection;      /// \param tolerance in evaluating intersections

      Type type;                                /// \result type of the intersection

      ///@note THE FIRST SEGMENT IS THE CUTTING SEGMENT,INSTEAD THE SECOND SEGMENT IS ONE OF THE POLYGON'S EDGES

      Position positionIntersectionFirstEdge;   /// \result position
      Position positionIntersectionSecondEdge;  /// \result position
                                                /// if the intersection is of type 1 (on line) or 2 (on segment),
                                                ///   is the position of the intersection with respect to each segment
                                                /// if the intersection is of type 3 (parallel on line) or 4 ( parallel on segment),
                                                ///   is the position of the extremes of the second segment with respect to the first segment

      Vector2d resultParametricCoordinates;     /// \result parametric coordinates of the intersection with respect to the origin of the segments in case of intersection of type 1 and 2;
                                                ///   while in case of intersection of type 3 and 4 it represents the coordinates of the origin and end of the second segment with respect to the curvilinear abscissa of the first segment

      Vector2d originFirstSegment;              /// \param origin point of the first segment
      Vector2d rightHandSide;                   /// \param vector between the origin of the first segment and the origin of the second segment
      Matrix2d matrixTangentVector;             /// \param matrix of the first and the second segments on columns

      bool parallelTwist;                       /// \param when the segments are parallel if they are at odds, the extremes of the second segment are inverted and parallelTwist is true

    public:
      Intersector1D1D();
      ~Intersector1D1D();

      /// Setters
      /// \brief SetFirstSegment(origin, end) set the defining points of the first segment
      /// \brief SetSecondSegment(origin, end) set the defining points of the second segment
      void SetFirstSegment(const Vector2d& origin, const Vector2d& end) { matrixTangentVector.col(0) = end - origin; originFirstSegment = origin; }
      void SetSecondSegment(const Vector2d& origin, const Vector2d& end) { matrixTangentVector.col(1) = origin - end; rightHandSide = origin - originFirstSegment; }

      ///Compute the intersections between the line of the first edge and the second edge and returns a bool:True if there isintersection,False if there is no intersection
      ///@note In case of parallelism both the parametric coordinates refers to the first edge
      bool ComputeIntersection();

      /// Tolerance Getters
      /// \brief ToleranceIntersection() returns the tolerance in evaluating intersections
      /// \brief ToleranceParallelism() returns the tolerance in evaluating parallelism
      const double& ToleranceIntersection() const { return toleranceIntersection; }
      const double& ToleranceParallelism() const { return toleranceParallelism; }

      /// Result Getters
      /// \brief ParametricCoordinates() returns a vector containing the parametric coordinates of the intersection with respect to the origin of the segments
      /// \brief FirstParametricCoordinate() returns the parametric coordinate of the intersection with respect to the origin of the first segment
      /// \brief SecondParametricCoordinate() returns the parametric coordinate of the intersection with respect to the origin of the second segment
      /// \brief PositionIntersectionInFirstEdge() returns the position of the intersection with respect to the first segment
      /// \brief PositionIntersectionInSecondEdge() returns the position of the intersection with respect to the second segment
      /// \brief TypeIntersection() returns the type of the intersection

      const Vector2d& ParametricCoordinates() { return resultParametricCoordinates; }
      const double& FirstParametricCoordinate() { return resultParametricCoordinates(0); }
      const double& SecondParametricCoordinate() { return resultParametricCoordinates(1); }

      const Position& PositionIntersectionInFirstEdge() { return positionIntersectionFirstEdge; }
      const Position& PositionIntersectionInSecondEdge() { return positionIntersectionSecondEdge; }
      const Type& TypeIntersection() { return type; }

      /// \brief IntersectionFirstParametricCoordinate(const Vector2d& origin,const Vector2d& end) takes in input the origin and the end of the first segment and it returns the intersection point (coordinates x,y)
      /// \brief IntersectionSecondParametricCoordinate(const Vector2d& origin,const Vector2d& end) in case of intersection of type 1 and 2 it takes in input the origin and the end of the second segment and it returns the intersection point (coordinates x,y)
      /// instead in case of intersection of type 3 (on line) and 4 (on segment) it takes in input the origin and end of the first segment and it returns the origin and end of the second segment with respect to the extremes of the first segment
      Vector2d IntersectionFirstParametricCoordinate(const Vector2d& origin,const Vector2d& end) const { return (1 - resultParametricCoordinates(0)) * origin + resultParametricCoordinates(0) * end; }
      Vector2d IntersectionSecondParametricCoordinate(const Vector2d& origin,const Vector2d& end) const { return (1 - resultParametricCoordinates(1)) * origin + resultParametricCoordinates(1) * end; }

      /// \brief ParallelTwist() returns true if vectors of first and second segement are opposite
      bool ParallelTwistHappened() { return parallelTwist; }
  };

};
#endif // SEGMENTINTERSECTOR_H
