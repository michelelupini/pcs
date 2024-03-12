#include "PolygonConcaveCutter.hpp"
#include "SegmentIntersector.hpp"

using namespace Intersector;

namespace PolygonCuttersLibrary {

bool ConcaveCutter::IsInVector(const int& elt, const vector<int>& visited)
{
    bool found = false;

    for(unsigned int i = 0; i<visited.size() && !found; i++) //controllo che l'elemento passato in input non sia nel vettore
        if(visited[i] == elt) //se ho trovato l'elemento found = true esco dal ciclo e ritorno true
            found = true;

    return found;
}

bool ConcaveCutter::IsInVector(const Polygon& elt, const vector<Polygon>& visited)
{
    bool found = false;

    for(unsigned int i = 0; i<visited.size() && !found; i++) //controllo che l'elemento passato in input non sia nel vettore
        if(visited[i] == elt) //se ho trovato l'elemento found = true esco dal ciclo e ritorno true
            found = true;

    return found;
}

void ConcaveCutter::InitializeGraph()
{
    // _borderPoints sono tutti i punti sul bordo del poligono
    // _linePoints sono tutti i punti sulla retta e le intersezioni
    int n1=(int)_borderPoints.size(),
        n2=(int)_linePoints.size(),
        i;

    //inserisco nel grafo ogni punto di _borderpoints, nella cui lista di adiacenza
    //viene inserito il successivo in _borderPoints
    for(i=0; i<n1; i++)
    {
        _graph.insert(make_pair(_borderPoints[i], (unordered_set<int>){_borderPoints[(i+1)%n1]}));
    }

    //inserisco (se non sono già presenti) i punti di _linePoints, nelle quali liste di adiacenza
    //vengono inseriti (se esistono) i punti precedente e successivo in _linePoints
    for(i=0; i<n2; i++)
    {
        const int curr = _linePoints[i];

        //se non lo trovo all'interno del grafo (== end), lo metto
        if(_graph.find(curr) == _graph.end())
            _graph.insert(make_pair(curr, (unordered_set< int>){} ));

        //se esiste un precedente (sulla retta) e quest'ultimo rappresenta l'intersezione entrante, lo aggiungo
        if(i!=0)
            if (_graph[curr].find(_linePoints[i-1]) == _graph[curr].end() && _inOutList[_linePoints[i-1]])
                _graph[curr].insert(_linePoints[i-1]);

        //se esiste un successivo (sulla retta) e il punto corrente (curr) rappresenta l'intersezione entrante, lo aggiungo
        if(i!=n2-1)
            if(_graph[curr].find(_linePoints[i+1]) == _graph[curr].end() && _inOutList[curr])
                _graph[curr].insert(_linePoints[i+1]);
    }
}

void ConcaveCutter::FillSideList()
{
   //  FillSideList() riempie _sideList con tutte le coppie composte da id del punto e l'intero
    // che rappresenta la posizione rispetto alla linea
    for(auto it = _borderPoints.begin(); it!=_borderPoints.end(); it++)
    {
        int side = _editPolygon.CheckSide(*it);
        _sideList.insert(make_pair(*it, side));
    }
}

/// N.B. il vettore visited NON viene passato per referenza, ma per copia, quindi non viene aggiornato a ogni chiamata del metodo
void ConcaveCutter::FindCycle(vector<int> visited, int newPoint)
{
    // controlla che il punto passato in input non sia stato già visitato
    if(!IsInVector(newPoint, visited))
    {
        //se non è stato visitato allora lo aggiungo ai visitati
        visited.push_back(newPoint);
        //abort line:
        // se il nuovo punto non si trova dal lato opposto della retta rispetto al primo punto visitato allora
        // rilancio il metodo per ogni punto presente nella lista di adiacenza del punto corrente nel grafo
        // altrimenti mi fermo e chiudo a vuoto
        if(!((_sideList[newPoint]==1 &&_sideList[visited[0]]==-1)||
             (_sideList[newPoint]==-1 &&_sideList[visited[0]]==1)))
            for(auto it = _graph[newPoint].begin(); it!=_graph[newPoint].end(); ++it)
                FindCycle(visited, *it);
    }
    else
    {
        // se il punto è presente nei visitati,per verificare che ho trovato un subpoligono,
        // controllo di aver visitato almeno 3 punti e che il punto corrente coincida con il primo mai visitato
        if((visited.size()>=3) && (newPoint == *visited.begin()))
        {
            // costruisco il poligono e controllo di non averne già trovato uno uguale
            Polygon newPolygon = Polygon(visited);
            if(!IsInVector(newPolygon, _editPolygon.GetProblem().GetAllSubPolygons()))
            {
                // lo aggiungo al vettore generale di subpoligoni
                //e al vettore specifico a seconda del lato della retta in cui si trova
              _editPolygon.GetProblem().SetSubPolygons(newPolygon);
              if(_sideList[visited[0]] == -1)
                _editPolygon.GetProblem().SetRightSubPolygons(newPolygon);
              else if(_sideList[visited[0]] == 1)
                _editPolygon.GetProblem().SetLeftSubPolygons(newPolygon);
            }
        }
    }
    return;
}

int ConcaveCutter::IsEntering(const Segment &segmentLine, const Segment &firstSegment, const Segment &secondSegment)
{
  // Assumo che due segmenti consecutivi non possano essere paralleli

  // auxiliary converter
  Vector2d originLine;
  Vector2d endLine;

  // conversion Segment >> Vector2d
  originLine = Vector2d(GetPoint(segmentLine.From).X, GetPoint(segmentLine.From).Y);
  endLine = Vector2d(GetPoint(segmentLine.To).X, GetPoint(segmentLine.To).Y);
  // line è il vettore 2d che rappresenta il segmento tagliante
  Vector2d line = endLine - originLine;
  // conversion Segment >> Vector2d
  originLine = Vector2d(GetPoint(firstSegment.From).X, GetPoint(firstSegment.From).Y);
  endLine = Vector2d(GetPoint(firstSegment.To).X, GetPoint(firstSegment.To).Y);
  // firstVector è il vettore 2d che rappresenta un lato del poligono
  Vector2d firstVector = originLine - endLine;
  // conversion Segment >> Vector2d
  originLine = Vector2d(GetPoint(secondSegment.From).X, GetPoint(secondSegment.From).Y);
  endLine = Vector2d(GetPoint(secondSegment.To).X, GetPoint(secondSegment.To).Y);
  // secondVector è il vettore 2d che rappresenta il lato consecutivo al precedente nel poligono
  Vector2d secondVector = endLine - originLine;

  // matrice di cambio base che prende come base (firstVector, secondVector)
  Matrix2d baseChanger;
  baseChanger.col(0) = firstVector;
  baseChanger.col(1) = secondVector;
  baseChanger = baseChanger.inverse().eval();

  // trasformo i vettori firstVector e secondVector in vettori 3d
  Vector3d first = Vector3d(firstVector.x(), firstVector.y(), 0);
  Vector3d second = Vector3d(secondVector.x(), secondVector.y(), 0);
  //calcolo il prodotto vettoriale tra i 2 vettori che rappresentano i lati consecutivi del poligono
  double crossProduct = first.cross(second).z();

  if(crossProduct < 0)  // if the angle at the polygon vertex is < pi
  {
      //se il prodotto vettoriale è < 0 e se una delle due coordinate del vettore rispetto alla base è negativa
     if((baseChanger*line).x() < - TOL || (baseChanger*line).y() < - TOL)
       return 0; // allora il vettore è uscente
     else return 1;// altrimenti entrante
  }
  else  // if the angle at the polygon vertex is > pi
  {
    //se il prodotto vettoriale è > 0 e se una delle due coordinate del vettore rispetto alla base è negativa
    if((baseChanger*line).x() < - TOL || (baseChanger*line).y() < - TOL)
      return 1; // allora il vettore è entrante
    else return 0; // altrimenti uscente
  }

}

int ConcaveCutter::IsEntering(const Segment &line, const Segment &edge)
{
    // conversion Segment >> Vector2d
    Point lineFrom = GetPoint(line.From),
          lineTo = GetPoint(line.To),
          edgeFrom = GetPoint(edge.From),
          edgeTo = GetPoint(edge.To);

    // vline è il vettore 3d che rapprensenta il segmento tagliante
    // vedge è il vettore 3d che rappresenta un lato del poligono
    Vector3d vline = Vector3d(lineTo.X-lineFrom.X, lineTo.Y-lineFrom.Y, 0),
             vedge = Vector3d(edgeTo.X-edgeFrom.X, edgeTo.Y-edgeFrom.Y, 0);

    // normalizzo i due vettori
    vline/=vline.norm();
    vedge/=vedge.norm();

    //calcolo il prodotto vettoriale tra lato e retta
    double cross = vedge.cross(vline).z();

    // se il prodotto vettoriale è positivo allora la retta è entrante, altrimenti è uscente
    if(cross > TOL)
        return 1;
    else return 0;
}

void ConcaveCutter::AddinInOutList(const int &tmpadd, const int &side)
{
    // _inOutList è una mappa che associa ad ogni punto (indice punto e la chiave) un intero che rappresenta
    // se la retta è entrante o uscente in quel punto
    // se non ho trovato il punto passato in input in _inOutList allora lo aggiungo,
    // altrimenti ne aggiorno il valore
    if(_inOutList.find(tmpadd) == _inOutList.end())
        _inOutList.insert(make_pair(tmpadd, side));
    else _inOutList[tmpadd] = side;
}

ConcaveCutter::ConcaveCutter(IPolygonEdit &beCutted): _editPolygon(beCutted)
{
    // per _borderPoints riservo memoria pari a due volte il numero dei vertici del poligono,perchè posso avere tante
    // intersezioni quanti sono il numero di vertici
    _borderPoints.reserve(_editPolygon.GetProblem().GetPolygon().PolygonVertices.size()*2+2);
    _borderPoints = _editPolygon.GetProblem().GetPolygon().PolygonVertices;

    // per _linePoints riservo memoria pari al numero di vertici del poligono +2,perchè posso avere tante intersezioni
    // quanti sono il numero di vertici del poligono più i due estremi della retta
    _linePoints.reserve(_editPolygon.GetProblem().GetPolygon().PolygonVertices.size() + 2);

}

void ConcaveCutter::ComputeNewPoints()
{
    // costruisco vettore di segmenti del poligono e retta tagliante utilizzando i Getters
    vector <Segment> _segments = _editPolygon.GetProblem().GetAllEdges();
    Segment line = _editPolygon.GetProblem().GetLine();

    int side = 0;

    unsigned int n = _segments.size();
    // l'iteratore it tiene memoria della posizione corrente all'interno del vettore di _borderPoints
    auto it = _borderPoints.begin();

    // creo una mappa che mappa i punti sulla retta tenendo come chiave (double) la loro ascissa curvilinea su questa
    map <double, int> intersezioni;
    // lo 0 dell'ascissa curvilinea corrisponde all'origine del segmento tagliante(From),
    //mentre l'1 all'estremo finale di questo (To) e vengono aggiunti di default alla mappa
    intersezioni.insert(make_pair(0, line.From));
    intersezioni.insert(make_pair(1, line.To));

    // di default vengono settati anche i valori di _inOutList a -1 (entrante/uscente)
    _inOutList.insert(make_pair(line.From, -1));
    _inOutList.insert(make_pair(line.To, -1));

    //convertitore da Point a Vector2d
    Point From = GetPoint(line.From),
          To = GetPoint(line.To);

    Vector2d originline = Vector2d(From.X, From.Y),
             endline = Vector2d(To.X, To.Y);

    // chiamo il costruttore dell'Intersector
    Intersector1D1D intersezione;
    // setto la retta come FirstSegment
    intersezione.SetFirstSegment(originline, endline);

    // ciclo su tutti i segmenti del poligono
    for(unsigned int i = 0; i < n; i++)
    {
      //incremento l'iteratore
      ++it;

      // converto il segmento considerato in vettore 2d
      Point tmpFrom = GetPoint(_segments[i].From),
            tmpTo = GetPoint(_segments[i].To);


      Vector2d tmporigin = Vector2d(tmpFrom.X, tmpFrom.Y),
               tmpend = Vector2d(tmpTo.X, tmpTo.Y);

      // setto il segmento considerato come SecondSegment
      intersezione.SetSecondSegment(tmporigin, tmpend);

      // tramite ComputeIntersection() del SegmentIntersector se ho trovato un'intersezione tra segmento e lato
      if(intersezione.ComputeIntersection()){

          // allora riconverto l'intersezione a Point
          Vector2d solution = intersezione.IntersectionFirstParametricCoordinate(originline, endline);
          Point tmp = Point(solution[0], solution[1]);
          // se ho intersezione propria sul segmento o sul suo prolungamento
          if(intersezione.TypeIntersection()==IntersectionOnLine || intersezione.TypeIntersection()== IntersectionOnSegment)
          {
              // se l'intersezione è interna al lato considerato
              if(intersezione.PositionIntersectionInSecondEdge()== Inner)
              {
                  // allora aggiungo il nuovo punto in Geometry Factory
                  _editPolygon.GetProblem().SetPoints(tmp);
                  int tmpaddr = _editPolygon.GetProblem().GetPointAddress(tmp);

                  // e aggiungo l'indice del punto corrispondente al vettore dei punti sul bordo e incremento l'iteratore
                  _borderPoints.insert(it, tmpaddr);
                  ++it;
                  // aggiungo alla mappa il nuovo punto con la corripondente ascissa curvilinea come chiave
                  intersezioni.insert(make_pair(intersezione.FirstParametricCoordinate(), _editPolygon.GetProblem().GetPointAddress(tmp)));

                  // controllo se la retta è entrante o uscente rispetto al nuovo punto e aggiungo l'informazione
                  // al vettore _inOutList tramite AddinInOutList()
                  side = IsEntering(line, _segments[i]);
                  AddinInOutList(tmpaddr, side);
              }
              // altrimenti se l'intersezione coincide con un estremo del lato del poligono
              else if(intersezione.PositionIntersectionInSecondEdge()== End)
              {
                  // allora aggiungo alla mappa il nuovo punto con la corripondente ascissa curvilinea come chiave
                  // ma non lo aggiungo a _borderPoints in quanto esiste già li dentro
                  int tmpaddr = _editPolygon.GetProblem().GetPointAddress(tmp);
                  intersezioni.insert(make_pair(intersezione.FirstParametricCoordinate(), tmpaddr));

                  // controllo se la retta è entrante o uscente rispetto al nuovo punto e aggiungo l'informazione
                  // al vettore _inOutList tramite AddinInOutList()
                  side = IsEntering(line, _segments[i], _segments[(i+1)%n]);
                  AddinInOutList(tmpaddr, side);
              }
          }
          else{
              // altrimenti se sono nel caso di intersezione parallela
              // e i due vettori sono di verso opposto allora
              if(intersezione.ParallelTwistHappened())
              {
                  // li scambio
                  int tmp = _segments[i].From;
                  _segments[i].From = _segments[i].To;
                  _segments[i].To = tmp;
              }

              // aggiungo alla mappa entrambi gli estremi del segmento con la corripondente ascissa curvilinea come chiave
              intersezioni.insert(make_pair(intersezione.FirstParametricCoordinate(), _segments[i].From));
              intersezioni.insert(make_pair(intersezione.SecondParametricCoordinate(), _segments[i].To));

              // side mi dice se la retta è entrante o uscente rispetto al nuovo punto
              side = IsEntering(line, _segments[i], _segments[(i+1)%n]);
              // se i due vettori sono di verso opposto
              if(intersezione.ParallelTwistHappened()){
                  // allora aggiungo ad _inOutList l'origine del segmento tagliante con il rispettivo side
                  //che indica se la retta è entrante o uscente nell'origine del segmento
                  AddinInOutList(_segments[i].From, side);
              }
              else{
                  // altrimenti aggiungo ad _inOutList la fine del segmento tagliante con il rispettivo side
                  //che indica se la retta è entrante o uscente nell'estremo finale del segmento
                  AddinInOutList(_segments[i].To, side);
              }

              /// PositionIntersectionInFirstEdge o SecondEdge è la posizione degli estremi del secondo segmento (lato)
              /// rispetto al primo segmento (retta)
              // se l'intersezione cade tra l'estremo finale del lato e l'estremo iniziale della retta
              //(scambiati precedentemente a causa del Parallel twist)
              if(intersezione.PositionIntersectionInFirstEdge()== Outer && intersezione.PositionIntersectionInSecondEdge() == Inner)
              {
                  // allora aggiungo al vettore di punti sul bordo del poligono l'origine della retta
                  // e incremento l'iteratore
                  _borderPoints.insert(it, line.From);
                  ++it;
              }
              // se l'intersezione cade tra l'estremo iniziale del lato e l'estremo finale della retta
              //(scambiati precedentemente a causa del Parallel twist)
              if(intersezione.PositionIntersectionInFirstEdge()== Inner && intersezione.PositionIntersectionInSecondEdge() == Outer)
              {
                  // allora aggiungo al vettore di punti sul bordo del poligono l'estremo finale della retta
                  // e incremento l'iteratore
                  _borderPoints.insert(it, line.To);
                  ++it;
              }
              // se il segmento tagliante cade tutto all'interno del lato allora
              if(intersezione.PositionIntersectionInFirstEdge()== Outer && intersezione.PositionIntersectionInSecondEdge() == Outer)
              {
                  // se gli estremi del segmento tagliante NON sono entrambi a destra o a sinistra rispetto agli estremi del lato
                  if(!((intersezione.FirstParametricCoordinate() <= 0 && intersezione.SecondParametricCoordinate() <= 0)||
                       (intersezione.FirstParametricCoordinate() >= 1 && intersezione.SecondParametricCoordinate() >= 1)))
                  {
                      // allora aggiungo entrambi gli estremi del segmento tagliante al vettore di punti
                      // sul bordo del poligono e incremento per ogni estremo aggiunto l'iteratore
                      _borderPoints.insert(it, line.From);
                      ++it;
                      _borderPoints.insert(it, line.To);
                      ++it;
                  }
               }
            }
        }
    }

    // ricopio tutti i punti di intersezioni (mappa(punto,ascissa curvilinea)) in _linePoints,
    //seguendo l'ordine dell'ascissa curvilinea
    for(auto it2 = intersezioni.begin(); it2 != intersezioni.end(); it2++)
          _linePoints.push_back(it2->second);
  }

  void ConcaveCutter::FindSubpolygons()
  {
      // chiama 2 metodi void per il riempimento di _graph e _sideList
      unsigned int n = _borderPoints.size();

      InitializeGraph();
      FillSideList();

      // cicla sui punti del bordo del poligono alla ricerca dei punti opportuni per far partire l'algoritmo ricorsivo
      for(unsigned int i = 0; i<n; i++ )
          // cioè se il punto non appartiene alla retta e il successivo si
          if(_sideList[_borderPoints[i]] && !_sideList[_borderPoints[(i+1)%n]])
          {
              //parto con il metodo ricorsivo
              vector<int> visited;
              FindCycle(visited, _borderPoints[i]);
          }
  }

};

