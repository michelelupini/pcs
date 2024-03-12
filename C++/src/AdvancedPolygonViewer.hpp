#ifndef ADVANCEDPOLYGONVIEWER_H
#define ADVANCEDPOLYGONVIEWER_H

#include "GeometryCore.hpp"

using namespace GeometryLibrary;

namespace PolygonViewerLibrary {

  /// \class AdvancedPolygonViewer contains methods to visualize the mesh and the domain
  class AdvancedPolygonViewer {
    private:
      /// \brief reference to the interface of AdvancedMesh
      IAdvancedMesh& _problem;

    public:
      /// \brief PolygonViewer(mesh) construct the reference to the mesh
      AdvancedPolygonViewer(IAdvancedMesh& problem) : _problem(problem) { }

      /// \brief ViewDomain() creates a file .m in order to plot the domain of the mesh
      /// \brief ViewMesh() creates a file .m in order to plot the mesh before cutting
      /// \brief ViewOutputMesh() creates a file .m in order to plot the mesh after cutting
      void ViewDomain(const string& fileName) const;
      void ViewMesh(const string& fileName) const;
      void ViewOutputMesh(const string& fileName) const;

  };

}

#endif // ADVANCEDPOLYGONVIEWER_H
