#include "PolygonAdvanced.hpp"

namespace PolygonAdvancedLibrary {

  void PolygonAdvanced::Reset()
  {
    _polygonVertices.clear();
    _numberVertices = 0;
  }

  PolygonAdvanced::PolygonAdvanced(const vector<Point> &points)
  {
    Reset();

    _polygonVertices = points;
    _numberVertices = points.size();
  }

  void PolygonAdvanced::SetPolygonVertex(const Point &point)
  {
    _polygonVertices.push_back(point);
    _numberVertices++;
  }

  /// \brief ComputeArea() calcola l'area di un poligono generico
  double PolygonAdvanced::ComputeArea()
  {
    double sum1 = 0;
    double sum2 = 0;

    // Shoelace formula o formula di Gauss
    // moltiplico in croce le coordinate corrispondenti seguendo uno schema simile a quello dei lacci della scarpa
    for(int i = 0; i < _numberVertices - 1; i++)
    {
      sum1 += _polygonVertices[i].X * _polygonVertices[i + 1].Y;
      sum2 += _polygonVertices[i + 1].X * _polygonVertices[i].Y;
    }
    sum1 += _polygonVertices[_numberVertices - 1].X * _polygonVertices[0].Y;
    sum2 += _polygonVertices[0].X * _polygonVertices[_numberVertices - 1].Y;

    return abs(sum1 - sum2)/2;
  }

  /// \brief confronta 2 poligoni, scorrendo sui vertici fino a che non trovo 2 sequenze identiche
  bool PolygonAdvanced::operator==(const PolygonAdvanced &rhs) const
  {
      bool exit = false;
      if(_numberVertices == rhs._numberVertices) //controllo che i numeri di vertici siano uguali
      {
          for(int i = 0; !exit && i < _numberVertices; i++) //con i 2 cicli for eseguo il controllo (fino a quando exit è falso)
          {
              exit = true;
              for(int j = 0; exit == true && j < _numberVertices; j++) //fino a quando exit è vero
                  if(!(_polygonVertices[j] == rhs._polygonVertices[(i + j) % _numberVertices]))
                      exit = false;
          }
      }
      return exit; // ritorno exit = 1 se ho trovato una sequenza corrispondente
  }

  void PolygonAdvanced::PrintDebug(const string &filepath)
  {
      /// Creates a file
      ofstream ofile;
      ofile.open(filepath);
      if(ofile.fail())
          throw runtime_error("Cannot open file. Something went wrong");

      // Prendo una referenza a se stessa (stessa cosa di utilizzare direttamente gli attributi)
      PolygonAdvanced& domain = *this;

      /// Data input
      // per ogni vertice del poligono scrivo i suoi punti in points
      ofile << "points = [";
      for(int i = 0; i < GetNumberPolygonVertices() - 1; i++)
          ofile << GetPolygonVertex(i) << endl;
      ofile << GetPolygonVertex(GetNumberPolygonVertices() - 1) << "];" << endl;
      // creo con il comando polyshape un poligono su matlab
      ofile << "polygon = polyshape(points(:, 1),points(:, 2));" << endl;

      ofile << "ptlbls = \"";
      for(int i = 0; i < GetNumberPolygonVertices()-1; i++)
          ofile << i << ", ";
      ofile << GetNumberPolygonVertices() -1 << '"' << ";" << endl
            << "pointlabels = strsplit(ptlbls, [\",\", \" \"]);"<< endl <<endl;

      /// Functions
      // Scrivo i comandi per plottare poligoni e punti su matlab ('FaceColor' colore del poligono,'FaceAlpha' gradazione del colore)
      //ofile << "figure;" << endl << "plot(polygon, 'FaceColor','red','FaceAlpha',0.1);" << endl << "hold on;" << endl
      ofile << "plot(polygon, 'FaceColor','red','EdgeColor', 'k', 'LineWidth', 1);" << endl << "hold on;" << endl
            << "plot(points(:, 1),points(:, 2), 'ko');" << endl << "hold on;" << endl << endl
            << "text(points(:, 1),points(:, 2), pointlabels, "
            << "'VerticalAlignment','bottom','HorizontalAlignment','right', 'FontSize', 14);" << endl<< endl;

      /// Close file
      ofile.close();
  }

}
