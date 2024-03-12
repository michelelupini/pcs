import src.geometry_core as g
import src.segment_intersector as si
import src.dataset as d
import numpy as np

### \class cuts a convex polygon with "ad hoc" methods
class ConvexCutter():
    def __init__(self, edit_polygon: g.IPolygonEdit):
        # \brief reference to the interface of PolygonEdit
        self.__edit_polygon = edit_polygon

        # \param _borderPoints: holds the indices of all points which lies on the boundary of the polygon, both original
        #        vertices and intersections; point indices are stored without duplicates (counterclockwise order is kept)
        # \param _linePoints: holds the indices of all points which lies on the cutting line, both intersections and the line defining couple of points;
        #      no duplicates, indices kept in order by their curvilinear coordinate on the line
        # \param _sideList: map whose key is a point index and the mapped value is an integer (-1, 0 or 1) representing which side of the line the key-point lies on
        #        (respectively right, on or left)
        # \param _graph: map, representing a graph, which use point-ids as key and their respective adjacency informations

        self.__borderpoints = self.__edit_polygon.get_problem().GetPolygon().polygon_vertices
        self.__linepoints = []
        self.__sidelist = {}  # //<punti, lati>
        self.__graph = {}

    ### \brief GetPoint(i) returns the i-th point in _points (N.B. not sorted) in _problem
    def get_point(self, id: int) -> d.Point:
        return self.__edit_polygon.get_problem().GetPoint(id)

    ### \brief InitializeGraph() fills _graph with all points involved in the problem and their respective just-computed adjacency informations
    def initialize_graph(self):
        # _borderPoints sono tutti i punti sul bordo del poligono
        #   _linePoints sono tutti i punti sulla retta e le intersezioni
        n1 = len(self.__borderpoints)
        n2 = len(self.__linepoints)

        # inserisco nel grafo ogni punto di _borderpoints, nella cui lista di adiacenza
        #    viene inserito il successivo in _borderPoints
        for i in range(0, n1):
            self.__graph[self.__borderpoints[i]] = [self.__borderpoints[(i+1) % n1]]

        # inserisco (se non sono già presenti) i punti di _linePoints, nelle quali liste di adiacenza
        #   vengono inseriti (se esistono) i punti precedente e successivo in _linePoints
        for i in range(0, n2):
            curr = self.__linepoints[i]

            # se non lo trovo all'interno del grafo (== end), lo metto
            if curr not in self.__graph:
                self.__graph[curr] = []
            # se esiste un precedente (sulla retta) e quest'ultimo rappresenta l'intersezione entrante, lo aggiungo
            if i != 0:
                if self.__linepoints[i-1] not in self.__graph[curr]:
                    self.__graph[curr].append(self.__linepoints[i-1])

            # se esiste un successivo (sulla retta) e il punto corrente (curr) rappresenta l'intersezione entrante, lo aggiungo
            if i != n2-1:
                if self.__linepoints[i+1] not in self.__graph[curr]:
                    self.__graph[curr].append(self.__linepoints[i+1])

        return

    ### \brief FillSideList() fills _sideList with all pairs made by point-id and its side-representing integer respect to the line
    def fill_sidelist(self):
        self.__sidelist = {i: 0 for i in self.__linepoints}

        # FillSideList() riempie _sideList con tutte le coppie composte da id del punto e l'intero
        #   che rappresenta la posizione rispetto alla linea
        for i in range(0, len(self.__borderpoints)):
            self.__sidelist[self.__borderpoints[i]] = self.__edit_polygon.check_side(self.__borderpoints[i])

        return

    ### \brief FindCycle(visited, newPoint) is a recoursive method for cycle research inside _graph, with few more conditions depicting the definition of "subpolygon"
    def find_cycle(self, visited: [], new_point: int):
        if new_point not in visited:
            # controlla che il punto passato in input non sia stato già visitato
            visited.append(new_point)
            # abort line:
            #   se il nuovo punto non si trova dal lato opposto della retta rispetto al primo punto visitato allora
            #   rilancio il metodo per ogni punto presente nella lista di adiacenza del punto corrente nel grafo
            #   altrimenti mi fermo e chiudo a vuoto
            if not ((self.__sidelist[new_point] == 1 and self.__sidelist[visited[0]] == -1) or
                    (self.__sidelist[new_point] == -1 and self.__sidelist[visited[0]] == 1)):
                for next_point in self.__graph[new_point]:
                    self.find_cycle(visited, next_point)
            else:
                visited.pop()
        else:
            # se il punto è presente nei visitati,per verificare che ho trovato un subpoligono,
            #   controllo di aver visitato almeno 3 punti e che il punto corrente coincida con il primo mai visitato
            if len(visited) >= 3 and new_point == visited[0]:
                # costruisco il poligono e controllo di non averne già trovato uno uguale
                new_polygon = d.Polygon(visited)
                if new_polygon not in self.__edit_polygon.get_problem().GetAllSubPolygons():
                    # lo aggiungo al vettore generale di subpoligoni
                    #   e al vettore specifico a seconda del lato della retta in cui si trova
                    self.__edit_polygon.get_problem().SetSubPolygons(new_polygon)

        return

    ### \brief ComputeNewPoints() calculates the intersections of the polygon with the cutting line, adding them in _points (in GeometryFactory class)
    ###       it also finishes _borderPoints and fills _linePoints
    def compute_new_points(self):
        # costruisco vettore di segmenti del poligono e retta tagliante utilizzando i Getters
        segments = self.__edit_polygon.get_problem().GetAllEdges()
        line = self.__edit_polygon.get_problem().GetLine()

        n = len(segments)
        # l'iteratore it tiene memoria della posizione corrente all'interno del vettore di _borderPoints
        it = 0

        # creo una mappa che mappa i punti sulla retta tenendo come chiave (double) la loro ascissa curvilinea su questa
        # lo 0 dell'ascissa curvilinea corrisponde all'origine del segmento tagliante(From),
        #     mentre l'1 all'estremo finale di questo (To) e vengono aggiunti di default alla mappa
        intersezioni = {0.0: line.From, 1.0: line.To}

        # convertitore da Point a Vector2d
        From = self.get_point(line.From)
        To = self.get_point(line.To)

        origin_line = np.array([From.x, From.y])
        end_line = np.array([To.x, To.y])

        # chiamo il costruttore dell'Intersector
        intersector = si.SegmentIntersector()
        # setto la retta come FirstSegment
        intersector.set_first_segment(origin_line, end_line)

        # ciclo su tutti i segmenti del poligono
        for i in range(0, n):
            # incremento l'iteratore
            it += 1

            # converto il segmento considerato in vettore 2d
            tmp_from = self.get_point(segments[i].From)
            tmp_to = self.get_point(segments[i].To)

            tmp_origin = np.array([tmp_from.x, tmp_from.y])
            tmp_end = np.array([tmp_to.x, tmp_to.y])

            # setto il segmento considerato come SecondSegment
            intersector.set_second_segment(tmp_origin, tmp_end)

            # tramite ComputeIntersection() del SegmentIntersector se ho trovato un'intersezione tra segmento e lato
            if intersector.compute_intersection():
                # allora riconverto l'intersezione a Point
                solution = intersector.intersection_first_parametric_coordinate(origin_line, end_line)
                # se ho intersezione propria sul segmento o sul suo prolungamento
                tmp = d.Point(solution[0], solution[1])

                if intersector.type_intersection() == d.Type.intersection_on_line or\
                    intersector.type_intersection() == d.Type.intersection_on_segment:
                    # se l'intersezione è interna al lato considerato
                    if intersector.position_intersection_in_second_edge() == d.Position.inner:
                        # allora aggiungo il nuovo punto in Geometry Factory
                        self.__edit_polygon.get_problem().SetPoints(tmp)
                        # e aggiungo l'indice del punto corrispondente al vettore dei punti sul bordo e incremento l'iteratore
                        self.__borderpoints.insert(it, self.__edit_polygon.get_problem().GetPointAddress(tmp))
                        # aggiungo alla mappa il nuovo punto con la corripondente ascissa curvilinea come chiave
                        it += 1
                        found = False
                        for k in sorted(intersezioni.keys()):
                            if intersezioni[k] == self.__edit_polygon.get_problem().GetPointAddress(tmp):
                                found = True
                        if found == False:
                            intersezioni[intersector.first_parametric_coordinate()] = self.__edit_polygon.get_problem().GetPointAddress(tmp)
                    # altrimenti se l'intersezione coincide con un estremo del lato del poligono
                    elif intersector.position_intersection_in_second_edge() == d.Position.end:
                        # allora aggiungo alla mappa il nuovo punto con la corripondente ascissa curvilinea come chiave
                        #  ma non lo aggiungo a _borderPoints in quanto esiste già li dentro
                        found = False
                        for k in sorted(intersezioni.keys()):
                            if intersezioni[k] == self.__edit_polygon.get_problem().GetPointAddress(tmp):
                                found = True
                        if found == False:
                            intersezioni[intersector.first_parametric_coordinate()] = self.__edit_polygon.get_problem().GetPointAddress(tmp)

                else:
                    # altrimenti se sono nel caso di intersezione parallela
                    #   e i due vettori sono di verso opposto allora
                    if intersector.parallel_twist_happened():
                        # li scambio
                        temp = segments[i].From
                        segments[i].From = segments[i].To
                        segments[i].To = temp

                    # aggiungo alla mappa entrambi gli estremi del segmento con la corripondente ascissa curvilinea come chiave
                    found = False
                    for k in sorted(intersezioni.keys()):
                        if intersezioni[k] == segments[i].From:
                            found = True
                    if found == False:
                        intersezioni[intersector.first_parametric_coordinate()] = segments[i].From

                    found = False
                    for k in sorted(intersezioni.keys()):
                        if intersezioni[k] == segments[i].To:
                            found = True
                    if found == False:
                        intersezioni[intersector.second_parametric_coordinate()] = segments[i].To

                    # PositionIntersectionInFirstEdge o SecondEdge è la posizione degli estremi del secondo segmento (lato)
                    #   rispetto al primo segmento (retta)
                    # se l'intersezione cade tra l'estremo finale del lato e l'estremo iniziale della retta
                    #   (scambiati precedentemente a causa del Parallel twist)
                    if intersector.position_intersection_in_first_edge() == d.Position.outer and intersector.position_intersection_in_second_edge() == d.Position.inner:
                        # allora aggiungo al vettore di punti sul bordo del poligono l'origine della retta
                        #   e incremento l'iteratore
                        self.__borderpoints.insert(it, line.From)
                        it += 1
                    # se l'intersezione cade tra l'estremo iniziale del lato e l'estremo finale della retta
                    #   (scambiati precedentemente a causa del Parallel twist)
                    if intersector.position_intersection_in_first_edge() == d.Position.inner and intersector.position_intersection_in_second_edge() == d.Position.outer:
                        # allora aggiungo al vettore di punti sul bordo del poligono l'estremo finale della retta
                        #   e incremento l'iteratore
                        self.__borderpoints.insert(it, line.To)
                        it += 1
                    # se il segmento tagliante cade tutto all'interno del lato allora
                    if intersector.position_intersection_in_first_edge() == d.Position.outer and intersector.position_intersection_in_second_edge() == d.Position.outer:
                        # se gli estremi del segmento tagliante NON sono entrambi a destra o a sinistra rispetto agli estremi del lato
                        if not ((intersector.first_parametric_coordinate() <= 0 and intersector.second_parametric_coordinate() <=0 ) or \
                                (intersector.first_parametric_coordinate() >= 1 and intersector.second_parametric_coordinate() >= 1)):
                            # allora aggiungo entrambi gli estremi del segmento tagliante al vettore di punti
                            #   sul bordo del poligono e incremento per ogni estremo aggiunto l'iteratore
                            self.__borderpoints.insert(it, line.From)
                            it += 1
                            self.__borderpoints.insert(it, line.To)
                            it += 1

        # ricopio tutti i punti di intersezioni (mappa(punto,ascissa curvilinea)) in _linePoints,
        #   seguendo l'ordine dell'ascissa curvilinea
        for i in sorted(intersezioni.keys()):
            self.__linepoints.append(intersezioni[i])

        return

    ### \brief FindSubpolygons() calls mostly private methods which had to be called all in once and looks in _borderPoints for starting points for FindCycle
    def find_sub_polygons(self):
        # chiama 2 metodi void per il riempimento di _graph e _sideList
        n = len(self.__borderpoints)

        self.initialize_graph()
        self.fill_sidelist()

        # cicla sui punti del bordo del poligono alla ricerca dei punti opportuni per far partire l'algoritmo ricorsivo
        for i in range(0, n):
            if self.__sidelist[self.__borderpoints[i]] and not self.__sidelist[self.__borderpoints[(i+1)%n]]:
                # parto con il metodo ricorsivo
                visited = []
                self.find_cycle(visited, self.__borderpoints[i])
        
        return
