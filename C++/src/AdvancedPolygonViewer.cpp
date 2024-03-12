#include "AdvancedPolygonViewer.hpp"

namespace PolygonViewerLibrary {

  void AdvancedPolygonViewer::ViewDomain(const string &fileName) const
  {
      /// Create file
      ofstream ofile;
      ofile.open(fileName);
      if(ofile.fail())
        throw runtime_error("Cannot open file. Something went wrong");

      // creo una referenza all'interfaccia IPolygonAdvanced in modo tale da prendere il dominio della mesh
      IPolygonAdvanced& domain = _problem.GetDomain();

      /// Data input
      // scrivo i punti che sono i vertici del dominio della mesh
      ofile << "points = [";
      for(int i = 0; i < domain.GetNumberPolygonVertices() - 1; i++)
        ofile << domain.GetPolygonVertex(i) << endl;
      ofile << domain.GetPolygonVertex(domain.GetNumberPolygonVertices() - 1) << "];" << endl;
      // scrivo i comandi che su matlab permettono di costruire un poligono
      ofile << "polygon = polyshape(points(:, 1),points(:, 2));" << endl;

      /// Functions
      // scrivo i comandi che su matlab permettono di plottare poligoni e punti
      ofile << "figure;" << endl << "plot(polygon, 'FaceColor','red','FaceAlpha',0.1);" << endl << "hold on;" << endl
            << "plot(points(:, 1),points(:, 2), 'ko');" << endl << "hold on;" << endl;

      /// Close file
      ofile.close();
  }

  void AdvancedPolygonViewer::ViewMesh(const string &fileName) const
  {
      /// Create file
      ofstream ofile;
      ofile.open(fileName);
      if(ofile.fail())
        throw runtime_error("Cannot open file. Something went wrong");

      // creo una referenza all'interfaccia IReferenceElt in modo tale da prendere la cella (reference element) associata al
      // poligono iniziale nella mesh
      IReferenceElt& cell = _problem.GetCell();


      ofile << "figure;" << endl;
      // ricostruisco la mesh traslando la cella di un'unità per ogni direzione
      for(int ii = 0; ii<_problem.GetIntSizeMesh(0); ii++)
      {

          for(int jj = 0; jj<_problem.GetIntSizeMesh(1); jj++)
          {
              ///import Polygon
              IPolygonAdvanced& polygon = cell.GetPolygon(0);
              // scrivo i punti per ogni poligono della cella
              ofile << "points" << ii << jj << " = [";
              for(int i = 0; i < polygon.GetNumberPolygonVertices() - 1; i++)
                  ofile << polygon.GetPolygonVertex(i) << endl;
              ofile << polygon.GetPolygonVertex(polygon.GetNumberPolygonVertices() - 1) << "];" << endl;
              // costruisco il poligono con il comando polyshape
              ofile << "polygon" << ii << jj << " = polyshape(points" <<ii<<jj<< "(:, 1),points" <<ii<<jj<< "(:, 2));" << endl << endl;

              ///import Box
              vector<Point> box = cell.GetBoundingBox();
              // scrivo i punti del bounding box
              ofile << "box" << ii << jj << " = [";
              for(unsigned int i = 0; i < box.size() - 1; i++)
                  ofile << box[i] << endl;
              ofile << box[box.size() - 1] << "];" << endl;

              //ofile << "box" << ii << jj << " = polyshape(points(:, 1),points(:, 2));" << endl << endl;

              /// Functions
              // scrivo i comandi per le plottare su matlab poligoni e punti
              ofile << "plot(polygon" << ii << jj <<", 'FaceColor','blue','FaceAlpha',0.7);" << endl << "hold on;" << endl
                    << "plot(points" <<ii<<jj<< "(:, 1),points" <<ii<<jj<< "(:, 2), 'ko');" << endl << "hold on;" << endl
                    << "plot(box" <<ii<<jj<< "(:, 1), box" <<ii<<jj<< "(:, 2), 'r--');" << endl << "hold on;" << endl;


              ofile << "plot(polygon, 'FaceColor','red','FaceAlpha',0.1);" << endl << "hold on;" << endl
                    << "plot(points" <<ii<<jj<< "(:, 1),points" <<ii<<jj<< "(:, 2), 'ko');" << endl << "hold on;" << endl;

              // traslo la cella lungo l'asse y di un'unità
              cell.IntTranslate(0, 1);
          }
          // ritorno indietro lungo l'asse y (riportando la y a 0) per poi ricominciare a traslare lungo l'asse y
          cell.IntTranslate(1, -_problem.GetIntSizeMesh(1));
      }
      // ritorno indietro lungo l'asse x (riportando la coordinata x a 0 e la y all'altezza a cui ero arrivata)
      // faccio questo perchè sto manipolando una referenza della cella iniziale
      cell.IntTranslate(-_problem.GetIntSizeMesh(0), 0);
      /// Close file
      ofile.close();
  }

  void AdvancedPolygonViewer::ViewOutputMesh(const string &fileName) const
  {
      /// Create file
      ofstream ofile;
      ofile.open(fileName);
      if(ofile.fail())
        throw runtime_error("Cannot open file. Something went wrong");

      ofile << "figure;" << endl;
      // per ogni poligono all'interno della mesh
      for(int i = 0; i < _problem.GetNumberPolygonInMesh(); i++)
      {
          // prendo una referenza al dominio della mesh
          IPolygonAdvanced& domain = _problem.GetPolygonInMesh(i);

          /// Data input
          // scrivo i punti di ogni poligono all'interno della mesh
          ofile << "points" << i << " = [";
          for(int j = 0; j < domain.GetNumberPolygonVertices() - 1; j++)
            ofile << domain.GetPolygonVertex(j) << endl;
          ofile << domain.GetPolygonVertex(domain.GetNumberPolygonVertices() - 1) << "];" << endl;
          // scrivo i comandi che su matlab permettono di costruire un poligono
          ofile << "polygon" << i << " = polyshape(points" << i << "(:, 1),points" << i << "(:, 2));" << endl;

          /// Functions
          // scrivo i comandi per le plottare su matlab poligoni e punti della mesh finale (dopo il taglio)
          ofile << "plot(polygon" << i << ", 'EdgeColor', 'k', 'LineWidth', 1);" << endl << "hold on;" << endl;
      }

      /// Close file
      ofile.close();
  }

}

