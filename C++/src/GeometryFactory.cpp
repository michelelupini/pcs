#include "GeometryFactory.hpp"

namespace GeometryFactoryLibrary {

  void GeometryFactory::ResetFactory()
  {
    _initialPolygon.NumberVertices = 0;
    _initialPolygon.PolygonVertices.clear();
    _line.From = -1;
    _line.To = -1;
    _points.clear();
    _initialEdges.clear();
    _subPolygons.clear();
  }


  void GeometryFactory::CreatePolygon()
  {
    ResetFactory();

    /// Open File
    ifstream file;
    file.open(_geometryFilePath.c_str());
    if(file.fail())
      throw runtime_error("Cannot open file. Something went wrong");

    /// Load input
    try
    {
      string line;
      string check;  // check if the identifier of the line is correct
      char c;  // skippable character

      /// Get points
      getline(file, line);
      check = line.substr(0,6);
      if(check != "points")  // check if the user want to set a list of points
        throw -1;
      line.erase(0,10);
      istringstream converterPoints;
      converterPoints.str(line);

      do {
        Point point;
        converterPoints >> c >> point.X >> c >> point.Y >> c >> c;
        _points.push_back(point);
      } while (c == ';');

      /// Get polygon vertices: construct initial polygon (vertices and segments)
      int numberVertices = 0;
      getline(file, line);
      check = line.substr(0,15);
      if(check != "polygonVertices")  // check if the user want to set a list of polygon vertices
        throw -2;
      line.erase(0,19);
      istringstream converterVertices;
      converterVertices.str(line);
      do {
        int i;
        converterVertices >> i >> c;
        _initialPolygon.PolygonVertices.push_back(i);  // construct polygon vertices
        if(numberVertices > 0)
        {
          Segment segment;
          segment.From = _initialPolygon.PolygonVertices[numberVertices - 1];
          segment.To = _initialPolygon.PolygonVertices[numberVertices];

          _initialEdges.push_back(segment);  // construct polygon segments
        }
        numberVertices++;
      } while (c == ',');
      // construct the last segment
      _initialEdges.push_back(Segment { _initialPolygon.PolygonVertices[numberVertices - 1], _initialPolygon.PolygonVertices[0] } );
      _initialPolygon.NumberVertices = numberVertices;

      /// Get segment without point duplicates
      getline(file, line);
      check = line.substr(0,7);
      if(check != "segment")  // check if the user want to set the line
        throw -3;
      line.erase(0,11);
      istringstream converterLine;
      converterLine.str(line);
      Point segmentPoint;
      int indexDuplicate;
      converterLine >> c >> segmentPoint.X >> c >> segmentPoint.Y >> c >> c;
      indexDuplicate = GetPointAddress(segmentPoint);  // if a duplicate is already in _points, then indexDuplicate = -1
      if(indexDuplicate == -1)  // if the point is not in _points yet, add the new point with a new index
      {
        _points.push_back(segmentPoint);
        _line.From = _points.size() - 1;
      }
      else  // if an identical point is already in _points, use its index
        _line.From = indexDuplicate;
      converterLine >> c >> segmentPoint.X >> c >> segmentPoint.Y >> c >> c;
      indexDuplicate = GetPointAddress(segmentPoint);  // if a duplicate is already in _points, then indexDuplicate = -1
      if(indexDuplicate == -1)  // if the point is not in _points yet, add the new point with a new index
      {
        _points.push_back(segmentPoint);
        _line.To = _points.size() - 1;
      }
      else  // if an identical point is already in _points, use its index
        _line.To = indexDuplicate;

      /// Close File
      file.close();
    }
    catch(int exception)
    {
      ResetFactory();
      if(exception == -1)
        throw runtime_error("Syntax error: bad points input");
      else if(exception == -2)
        throw runtime_error("Syntax error: bad polygon vertices input");
      else if(exception == -3)
        throw runtime_error("Syntax error: bad segment input");
      else throw runtime_error("Something went wrong");
    }
    catch(...)
    {
      ResetFactory();
      throw runtime_error("Something went wrong");
    }
  }

  // create the polygon through a vector of point, without using an input file
  void GeometryFactory::CreatePolygon(const vector<Point> &points, const vector<int> &polygonVertices, const vector<Point> &segment)
  {
    ResetFactory();

    /// Get points
    _points = points;

    /// Get polygon vertices: construct initial polygon (vertices and segments)
    _initialPolygon.PolygonVertices = polygonVertices;
    _initialPolygon.NumberVertices = polygonVertices.size();
    for(int i = 0; i < _initialPolygon.NumberVertices - 1; i++)
      _initialEdges.push_back(Segment { polygonVertices[i], polygonVertices[i + 1] });
    _initialEdges.push_back(Segment { polygonVertices[_initialPolygon.NumberVertices - 1], polygonVertices[0] });

    /// Get segment without point duplicates
    int indexDuplicate[2] = { -1, -1 };
    // find point duplicates and save their indices relative to _points in indexDuplicate
    // if no duplicate is found for the k-th segment point, then indexDuplicate[k] = -1
    for(int i = 0; i < _initialPolygon.NumberVertices && (indexDuplicate[0] == -1 || indexDuplicate[1] == -1); i++)
    {
      if(indexDuplicate[0] == -1 && _points[i] == segment[0])
        indexDuplicate[0] = i;
      if(indexDuplicate[1] == -1 && _points[i] == segment[1])
        indexDuplicate[1] = i;
    }

    if(indexDuplicate[0] == -1)  // if the point is not in _points yet, add the new point with a new index
    {
      _points.push_back(segment[0]);
      _line.From = _points.size() - 1;
    }
    else  // if an identical point is already in _points, use its index
      _line.From = indexDuplicate[0];

    if(indexDuplicate[1] == -1)  // if the point is not in _points yet, add the new point with a new index
    {
      _points.push_back(segment[1]);
      _line.To = _points.size() - 1;
    }
    else  // if an identical point is already in _points, use its index
      _line.To = indexDuplicate[1];
  }

  int GeometryFactory::GetPointAddress(const Point &point)
  {
      for(int i = 0; i < GetPointsNumber(); i++)
        if(point == _points[i])
          return i;
      return -1;
  }

  int GeometryFactory::SetPoints(const Point &point)
  {
      _points.push_back(point);
      return _points.size() - 1;
  }

}
