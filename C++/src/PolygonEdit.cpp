#include "PolygonEdit.hpp"
#include "PolygonConvexCutter.hpp"
#include "PolygonConcaveCutter.hpp"

using namespace PolygonCuttersLibrary;

namespace PolygonEditLibrary {

  void PolygonEdit::CutPolygon()
  {
    if(CheckExternal())  // if the line is external
    {
      _problem.SetSubPolygons(_problem.GetPolygon());  // set one subpolygon

      /// Set right or left subpolygon
      int side = 0;
      for(int i = 0; i < _problem.GetPolygon().NumberVertices && side == 0; i++)
        side = CheckSide(_problem.GetPolygon().PolygonVertices[i]);
      if(side == -1)
        _problem.SetRightSubPolygons(_problem.GetPolygon());
      else if(side == 1)
        _problem.SetLeftSubPolygons(_problem.GetPolygon());

      return;
    }

    if(CheckConvexity())  // if the polygon is convex
    {
      ConvexCutter Cutter = ConvexCutter(*this);

      Cutter.ComputeNewPoints();
      Cutter.FindSubpolygons();
    }
    else  // if the polygon is concave
    {
      ConcaveCutter Cutter = ConcaveCutter(*this);

      Cutter.ComputeNewPoints();
      Cutter.FindSubpolygons();
    }
  }

  int PolygonEdit::CheckSide(const int& pointId)
  {
    Point point = GetPoint(pointId);  // point to check
    int exit;  // side: -1 right, 0 on line, 1 left
    Segment line = _problem.GetLine();  // reference line
    Point To = GetPoint(line.To);
    Point From = GetPoint(line.From);

    // vline è il vettore 3d che corrisponde al segmento tagliante
    Vector3d vline = Vector3d(To.X - From.X, To.Y - From.Y, 0);
    // vpoint è il vettore 3d che corrisponde al segmento che va dal punto considerato all'origine del segmento tagliante
    Vector3d vpoint = Vector3d(point.X - From.X, point.Y - From.Y, 0);
    double cross = vline.cross(vpoint).z();

    if(cross > TOL)
        exit = 1;  // left
    else if(cross < -TOL)
        exit = -1;  // right
    else exit = 0;  // on line

    return exit;
  }

  int PolygonEdit::CheckSide(const int& pointId, const Segment& line)
  {
    Point point = GetPoint(pointId);  // point to check
    int exit;  // side: -1 right, 0 on line, 1 left
    // reference line (in input)
    Point To = GetPoint(line.To);
    Point From = GetPoint(line.From);

    // vline è il vettore 3d che corrisponde al segmento tagliante
    Vector3d vline = Vector3d(To.X - From.X, To.Y - From.Y, 0);
    // vpoint è il vettore 3d che corrisponde al segmento che va dal punto considerato all'origine del segmento tagliante
    Vector3d vpoint = Vector3d(point.X - From.X, point.Y - From.Y, 0);
    double cross = vline.cross(vpoint).z();

    if(cross > TOL)
        exit = 1;  // left
    else if(cross < -TOL)
        exit = -1;  // right
    else exit = 0;  // on line

    return exit;
  }

  bool PolygonEdit::CheckConvexity()
  {
    bool exit = true;  // is convex?
    int numberPolygonVertices = _problem.GetPolygon().NumberVertices;
    vector<int> vertices = _problem.GetPolygon().PolygonVertices;

    // per ogni lato del poligono controllo che tutti i punti del poligono che non apprtengono al lato siano a sinistra del lato
    for (int i = 0; i < numberPolygonVertices && exit; i++)
      if(CheckSide(vertices[(i + 2) % numberPolygonVertices], _problem.GetPolygonEdge(i)) == -1)
        exit = false;  // if a point is at the right of a segment, the polygon is not convex

    return exit;
  }

  bool PolygonEdit::CheckExternal()
  {
    bool exit = true;  // is the line external?
    vector<int> vertices = _problem.GetPolygon().PolygonVertices;
    int numberVertices = vertices.size();
    int sample = CheckSide(vertices[0]);

    for(int i = 1; i < numberVertices && sample && exit; i++)
      if(CheckSide(vertices[i]) != sample)  // if a point is on a different side with respect to the first vertex
        exit = false;

    return (sample && exit);  // if the first vertex is not on the line and there is no point in a different side
                              //   with respect to the first vertex, then the polygon is external
  }

}
