#ifndef EMPTYCLASS_H
#define EMPTYCLASS_H

/*#include "Eigen"
#include <iostream>
#include <vector>

using namespace Eigen;
using namespace std;*/

#include "GeometryCore.hpp"
#include <unordered_set>

namespace ToolFunctions {

  class Tools {
    public:

      bool tempCheckSide(){
          //se pt. a sx della linea vera, se a dx falsa
          return true;}
      bool EmptyMethod()
      {
          class Vertex{
          public:
            int value;
            vector<Vertex*> adj;
          };

          //assuming I receive 2 vectors
          vector<int> border;
          vector<int> line;

          unordered_map<int, vector<int*>> pino;

          //creation and initialization of graph





          //ATTEMPT 1: fallisce idea con disegno V, MA CON UN PUNTO IN MEZZO
          /*class Vertex{
          public:
              int value;

              bool status = false;
              //serve false di default perchè sì
              //per il secondo vettore sarebbe carina una enum con estremo/non estremo
          };


          vector<Vertex> borderPoints;
          vector<Vertex> linePoints;

          //importazione via cicli o con costruttore, poi vedo come mi fa comodo

          vector<vector<int>> foundSubPolygons;
          vector<int> tempPolygon;

          bool side = tempCheckSide(); //gli argomenti sarebbero borderPoints[0] e la linea

          tempPolygon.clear();*/


      };
  };


}



//le nostre memorie
//controlla sta funzione, che d'auria ha fatto un macello
#endif // EMPTYCLASS_H
