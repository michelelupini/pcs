#ifndef POLYGONVIEWER_H
#define POLYGONVIEWER_H

#include "GeometryCore.hpp"

using namespace GeometryLibrary;

namespace PolygonViewerLibrary {

  /// \class PolygonViewer contains methods to visualize the polygon and the line
  class PolygonViewer {
    private:
      /// \brief reference to the interface of GeometryFactory
      IGeometryFactory& _geometryFactory;
    public:
      /// \brief PolygonViewer(geometryFactory) construct the reference to GeometryFactory
      PolygonViewer(IGeometryFactory& geometryFactory) : _geometryFactory(geometryFactory) { }

      /// \brief ViewPolygon() creates a file .m in order to plot the initial polygon and the cutting segment
      /// \brief ViewLine() creates a file .m in order to plot the cutting segment
      /// \brief ViewFinishedProblem() creates a file .m in order to plot the result of the cutting process
      void ViewPolygon(const string& fileName) const;
      void ViewLine(const string& fileName) const;
      void ViewFinishedProblem(const string& fileName) const;
  };
}

#endif // POLYGONVIEWER_H
