#ifndef ADVANCEDMESH_H
#define ADVANCEDMESH_H

#include "GeometryFactory.hpp"
#include "PolygonEdit.hpp"
#include "ReferenceElt.hpp"
#include "PolygonAdvanced.hpp"

using namespace GeometryFactoryLibrary;
using namespace PolygonEditLibrary;
using namespace PolygonAdvancedLibrary;

namespace AdvancedMeshLibrary {

  /// \class  given a convex domain and a refernce element, compute the mesh
  class AdvancedMesh : public IAdvancedMesh {

    private:
      /// \param _domain: stores the shape of the region which is gonna be covered by mesh
      /// \param _cell: elementar piece of the mesh, stores a primal shape and its bounding box
      /// \param _mesh: stores every polygon obtained by cropping the original ones in _cell in order to entirely
      ///        cover the domain
      PolygonAdvanced _domain;
      ReferenceElt _cell;
      vector<PolygonAdvanced> _mesh;

      /// \param _numberPolygonInMesh: size of _mesh
      /// \param xReps, yReps: dimensions of the bounding box of the domain expressed in units of cells
      ///         (or just how many cells you need to cover the domain along the x and y axes)
      int _numberPolygonInMesh;
      int xReps, yReps;


      /// \brief ComputeFinalDomain(): computes the dimensions of a box surrounding the domain, storing them in xReps and yReps
      ///         and tranlsate the reference element to the first vertex of the domain
      void ComputeFinalDomain();

      /// \brief CheckPointInDomain(point): check if a point is inside the domain or not
      bool CheckPointInDomain(const Point &point);

      /// \brief CutPolygonKeepLeft(polygons, segment): construct a GeometryFactory and PolygonEdit problem from a cell polygon and
      ///         a domain side, keeps only the freshly computed subpolygons on the left side of it and stores them in the mesh
      void CutPolygonsKeepLeft(vector<PolygonAdvanced> &polygons, const vector<Point> &segment);

    public:
      /// \brief AdvancedMesh(domain, cell): class constructor, create references to IPolygonAdvanced and IReferenceElt
      AdvancedMesh(const PolygonAdvanced &domain, const ReferenceElt &cell): _domain(domain), _cell(cell) {  };

      /// Getters
      /// \brief GetDomain(): returns a reference to the domain polygon
      /// \brief GetCell(): returns a reference to the cell
      /// \brief GetNumberPolygonInMesh(): returns the number of polygons in the mesh
      /// \brief GetWholeMesh(): returns a reference to the entire mesh
      /// \brief GetPolygonInMesh(id): return the id-th mesh element
      /// \brief GetIntSizeMesh(axis): returns the i-th dimensions of the domain box (0 for xReps, 1 for yReps, else an error occurs)
      PolygonAdvanced& GetDomain() { return _domain; };
      ReferenceElt& GetCell() { return _cell; };
      int GetNumberPolygonInMesh() { return _numberPolygonInMesh; };
      vector<PolygonAdvanced>& GetWholeMesh() { return _mesh; }
      PolygonAdvanced& GetPolygonInMesh(const unsigned int& id) { return _mesh[id]; } ;
      const int& GetIntSizeMesh(const int& axis);

      /// \brief ComputeMesh(): void method which collects and calls all the private methods involved in covering the domain with mesh
      void ComputeMesh();

  };

}

#endif // ADVANCEDMESH_H
