#include "AdvancedMesh.hpp"

namespace AdvancedMeshLibrary {

  void AdvancedMesh::ComputeFinalDomain()
  {
    /// Compute final rectangular domain
    // set defalut min and max coordiantes of the domain
    double minX = _domain.GetPolygonVertex(0).X;
    double maxX = _domain.GetPolygonVertex(0).X;
    double minY = _domain.GetPolygonVertex(0).Y;
    double maxY = _domain.GetPolygonVertex(0).Y;

    for(int i = 0; i < _domain.GetNumberPolygonVertices(); i++)
    {
      // evaluation of maximum and minimum components
      if(_domain.GetPolygonVertex(i).X < minX)
        minX = _domain.GetPolygonVertex(i).X;
      if (_domain.GetPolygonVertex(i).X > maxX)
        maxX = _domain.GetPolygonVertex(i).X;
      if(_domain.GetPolygonVertex(i).Y < minY)
        minY = _domain.GetPolygonVertex(i).Y;
      if (_domain.GetPolygonVertex(i).Y > maxY)
        maxY = _domain.GetPolygonVertex(i).Y;
    }

    /// Compute dimensions of the domain
    double xLenDomain = maxX - minX;
    double yLenDomain = maxY - minY;

    /// Compute dimensions of the cell
    double xLenCell = _cell.GetPointBoundingBox(1).X - _cell.GetPointBoundingBox(0).X;
    double yLenCell = _cell.GetPointBoundingBox(3).Y - _cell.GetPointBoundingBox(0).Y;

    xReps = ceil(xLenDomain / xLenCell);
    yReps = ceil(yLenDomain / yLenCell);

    /// Translation of the cell to the lower-left vertex of the final domain
    _cell.Translate(minX - _cell.GetPointBoundingBox(0).X,
                    minY - _cell.GetPointBoundingBox(0).Y);
  }

  bool AdvancedMesh::CheckPointInDomain(const Point &point)
  {
    bool pointInDomain = true;
    int numberVertices = _domain.GetNumberPolygonVertices();

    // assuming a convex domain, check if the point is on the left side for each segment counterclockwise
    for(int i = 0; i < numberVertices && pointInDomain; i++)
    {
      Point To = _domain.GetPolygonVertex((i + 1) % numberVertices);
      Point From = _domain.GetPolygonVertex(i);

      // vline è il vettore 3d che corrisponde al segmento tagliante
      Vector3d vline = Vector3d(To.X - From.X, To.Y - From.Y, 0);
      // vpoint è il vettore 3d che corrisponde al segmento che va dal punto considerato all'origine del segmento tagliante
      Vector3d vpoint = Vector3d(point.X - From.X, point.Y - From.Y, 0);

      // se il prodotto vettoriale tra vline e vpoint è > 0 allora il punto si trova fuori dal dominio
      double cross = vline.cross(vpoint).z();

      if(cross > TOL)
        pointInDomain = false;
    }

    return pointInDomain;
  }

  void AdvancedMesh::CutPolygonsKeepLeft(vector<PolygonAdvanced> &polygons, const vector<Point> &segment)
  {
    vector<PolygonAdvanced> leftPolygons;  // auxiliary vector of left subpolygons

    int numberPolygons = polygons.size();
    for(int k = 0; k < numberPolygons; k++)  // for each polygon to cut
    {
      GeometryFactory factory;
      vector<Point> points;         // points of the current polygon
      vector<int> polygonVertices;  // vertices of the current polygon

      /// Definition of the factory
      for(int h = 0; h < polygons[k].GetNumberPolygonVertices(); h++)
      {
          // per ogni poligono costruisco il vettore di punti e il vettore di vertici
        points.push_back(polygons[k].GetPolygonVertex(h));
        polygonVertices.push_back(h);
      }

      // creo il poligono
      factory.CreatePolygon(points, polygonVertices, segment);
      PolygonEdit editingP(factory);

      /// Cut the current polygon
      editingP.CutPolygon();

      /// Keep left subpolygons
      int numberLeftPolygons = factory.GetNumberLeftSubPolygons();
      for(int h = 0; h < numberLeftPolygons; h++)  // for each left subpolygon
      {
        PolygonAdvanced leftPolygon;  // auxiliary polygon
        vector<Point> leftPoints;     // points of the auxiliary polygon

        // conversion Polygon -> PolygonAdvanced
        for(int l = 0; l < factory.GetLeftSubPolygon(h).NumberVertices; l++)
          leftPoints.push_back(factory.GetPoint(factory.GetLeftSubPolygon(h).PolygonVertices[l]));
        leftPolygon = PolygonAdvanced(leftPoints);

        leftPolygons.push_back(leftPolygon);
      }
    }
    polygons.clear();
    polygons = leftPolygons;  // keep left subpolygons in polygons

  }

  void AdvancedMesh::ComputeMesh()
  {
    /// Compute dimensions of the domain and translate the cell
    ComputeFinalDomain();

    /// Compute mesh
    // for each cell in the final domain
    for(int i = 0; i < xReps; i++)
    {
      for(int j = 0; j < yReps; j++)
      {
        // create a temporaty cell, translating the original reference element to the current position
        ReferenceElt tempCell = _cell;
        tempCell.IntTranslate(i, j);

        bool cellInDomain = true;  // is the cell totally contained in the domain?
        bool cellOutDomain = false;
        for(int bB = 0; bB < 4 && (cellInDomain || !cellOutDomain); bB++)  // for each point of the cell bounding box
        {
          if(cellInDomain)
            cellInDomain = CheckPointInDomain(tempCell.GetPointBoundingBox(bB));  // check if the point is in the domain
          if(!cellOutDomain)
            cellOutDomain = CheckPointInDomain(tempCell.GetPointBoundingBox(bB));  // check if the point is in the domain
        }
        if(cellInDomain)  // then all polygons in the cell, are part of the mesh
        {
          for(int k = 0; k < tempCell.GetPolygonsNumber(); k++)
            _mesh.push_back(tempCell.GetPolygon(k));
        }
        else if(!cellOutDomain)  // if the cell is not totally out of the domain (intersection case)
        {
          vector<PolygonAdvanced> polygons = tempCell.GetPolygons();  // vector of polygons to cut (modified after each cut)

          for(int k = 0; k < _domain.GetNumberPolygonVertices(); k++)  // for each side of the domain
          {
            Point From = _domain.GetPolygonVertex(k);
            Point To = _domain.GetPolygonVertex((k + 1) % _domain.GetNumberPolygonVertices());
            vector<Point> segment = { From, To };  // current side of the domain

            // cut each polygon in polygons by means of segment, keeping in polygons the left subpolygons
            CutPolygonsKeepLeft(polygons, segment);
          }

          // add cutted polygons of the cell to the mesh
          _mesh.insert(_mesh.end(), polygons.begin(), polygons.end());
        }
      }
    }

    _numberPolygonInMesh = _mesh.size();
  }

  const int &AdvancedMesh::GetIntSizeMesh(const int &axis)
  {
      // se l'input è 0 voglio che mi ritorni la dimensione x del dominio (il numero di volte che la cella entra nel dominio)
      if(!axis)
          return xReps;
      // se l'input è 1 voglio che mi ritorni la dimensione y  del dominio (il numero di volte che la cella entra nel dominio)
      else if (axis == 1)
          return yReps;
      throw runtime_error("Invalid axis input, 0 for x, 1 for y;");
  }

}
