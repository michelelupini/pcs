import numpy as np
from src import dataset as d
from src import geometry_core as g


class SegmentIntersector():
    def __init__(self):
        self.__tolerance_parallelism = 1e-7     ### \param tolerance in evaluating parallelism
        self.__tolerance_intersection = 1e-7    ### \param tolerance in evaluating intersections
        self.__type = d.Type.no_intersection    ### \result type of the intersection

        ### ///@note THE FIRST SEGMENT IS THE CUTTING SEGMENT,INSTEAD THE SECOND SEGMENT IS ONE OF THE POLYGON'S EDGES

        self.__position_intersection_first_edge: d.Position = d.Position.default    ### \result position
        self.__position_intersection_second_edge: d.Position = d.Position.default   ### \result position
            ### if the intersection is of type 1 (on line) or 2 (on segment),
            ###   is the position of the intersection with respect to each segment
            ### if the intersection is of type 3 (parallel on line) or 4 ( parallel on segment),
            ###   is the position of the extremes of the second segment with respect to the first segment
        self.__result_parametric_coordinates = np.zeros((2, 1))
            ### \result parametric coordinates of the intersection with respect to the origin of the segments in case of intersection of type 1 and 2;
            ###   while in case of intersection of type 3 and 4 it represents the coordinates of the origin and end of the second segment with respect to the curvilinear abscissa of the first segment
        self.__origin_first_segment: np.array([])           ### \param origin point of the first segment
        self.__right_hand_side: np.array([])                ### \param vector between the origin of the first segment and the origin of the second segment
        self.__matrix_tangent_vector = np.zeros((2, 2))     ### \param matrix of the first and the second segments on columns
        self.__parallel_twist = False                       ### \param when the segments are parallel if they are at odds, the extremes of the second segment are inverted and parallelTwist is true


    ### Setters
    ### \brief SetFirstSegment(origin, end) set the defining points of the first segment
    ### \brief SetSecondSegment(origin, end) set the defining points of the second segment

    def set_first_segment(self, origin: np.array([]), end: np.array([])):
        self.__matrix_tangent_vector[0, 0] = -(origin[0] - end[0])
        self.__matrix_tangent_vector[1, 0] = -(origin[1] - end[1])
        self.__origin_first_segment = origin

    def set_second_segment(self, origin: np.array([]), end: np.array([])):
        self.__matrix_tangent_vector[0, 1] = origin[0] - end[0]
        self.__matrix_tangent_vector[1, 1] = origin[1] - end[1]
        self.__right_hand_side = origin - self.__origin_first_segment


    ### Compute the intersections between the line of the first edge and the second edge and returns the parametric coordinates and the intersection type
    ### The first parametric coordinate refers to the first tangentVector and ...
    ### @note In case of parallelism both the parametric coordinates refers to the first edge

    def compute_intersection(self):
        parallelism = np.linalg.det(self.__matrix_tangent_vector)
        self.__type = d.Type.no_intersection
        intersection: bool = False

        check = self.__tolerance_parallelism * self.__tolerance_parallelism *\
                np.dot(self.__matrix_tangent_vector[:,0], self.__matrix_tangent_vector[:,0]) *\
                np.dot(self.__matrix_tangent_vector[:,1], self.__matrix_tangent_vector[:,1])
        if parallelism * parallelism > check:
            solverMatrix = np.array([[self.__matrix_tangent_vector[1, 1], -self.__matrix_tangent_vector[0, 1]],
                                     [-self.__matrix_tangent_vector[1, 0], self.__matrix_tangent_vector[0, 0]]])
            # matrice inversa
            # se i due segmenti non sono paralleli cerca l'intersezione tramite coordinate paramentiche
            # <li> If the edge and the trace are not parallel look for the intersection with parametric coordinates
            self.__result_parametric_coordinates = np.dot(solverMatrix, self.__right_hand_side)
            self.__result_parametric_coordinates /= parallelism
            
            if self.__result_parametric_coordinates[1] > -self.__tolerance_intersection and self.__result_parametric_coordinates[1] - 1.0 < self.__tolerance_intersection:
                # primo if: controlla che ci sia intersezione tra secondo segmento e prolungamento del primo
                self.__type = d.Type.intersection_on_line
                intersection = True
                if self.__result_parametric_coordinates[0] > -self.__tolerance_intersection and self.__result_parametric_coordinates[0] - 1.0 < self.__tolerance_intersection:
                    self.__type = d.Type.intersection_on_segment
                    # secondo if: controlla che l'intersezione sia ANCHE dentro al primo
                    # se l'intersezione è fuori dal secondo segmento, allora non c'è
        else:
            parallelism2 = abs(self.__matrix_tangent_vector[0, 0] * self.__right_hand_side[1] - self.__right_hand_side[0] * self.__matrix_tangent_vector[1, 0])
            # <li> In case of parallelism check if the segment is the same with parametric coordinates

            squaredNormFirstEdge = np.dot(self.__matrix_tangent_vector[:,0], self.__matrix_tangent_vector[:,0])
            check2 = self.__tolerance_parallelism * self.__tolerance_parallelism * squaredNormFirstEdge * np.dot(self.__right_hand_side, self.__right_hand_side)
            if parallelism2 * parallelism2 <= check2:
                tempNorm = 1.0 / squaredNormFirstEdge
                # parametric coordinates on the trace of the starting point and end point
                self.__result_parametric_coordinates[0] = np.dot(self.__matrix_tangent_vector[:,0], self.__right_hand_side) * tempNorm
                # -1 perchè nella matrice c'è il segno negativo nella seconda colonna
                self.__result_parametric_coordinates[1] = self.__result_parametric_coordinates[0] - np.dot(self.__matrix_tangent_vector[:,0], self.__matrix_tangent_vector[:,1]) * tempNorm
                # DA' LE COORDINATE PARAMETRICHE SUL PRIMO SEGMENTO DEGLI ESTREMI DEL SECONDO

                intersection = True
                self.__type = d.Type.intersection_parallel_on_line

                if self.__result_parametric_coordinates[1] < self.__result_parametric_coordinates[0]:
                    self.__parallel_twist = True
                    tmp = self.__result_parametric_coordinates[0]
                    self.__result_parametric_coordinates[0] = self.__result_parametric_coordinates[1]
                    self.__result_parametric_coordinates[1] = tmp

                # if one vertex is in the edge there is the intersection
                if (self.__result_parametric_coordinates[0] > -self.__tolerance_intersection and self.__result_parametric_coordinates[0] - 1.0 < self.__tolerance_intersection) or\
                        (self.__result_parametric_coordinates[1] > -self.__tolerance_intersection and self.__result_parametric_coordinates[1] - 1.0 < self.__tolerance_intersection):
                    self.__type = d.Type.intersection_parallel_on_segment
                else:
                    # IL PRIMO SEGMENTO DATO IN INPUT E' CONTENUTO NEL SECONDO
                    if (self.__result_parametric_coordinates[0] < self.__tolerance_intersection and self.__result_parametric_coordinates[1] - 1.0 > -self.__tolerance_intersection):
                        self.__type = d.Type.intersection_parallel_on_segment
        # </ul>
        # in case of intersection (cases 1,2,3,4) returns the position of the intersection with respect to the first segment
        if self.__result_parametric_coordinates[0] < -self.__tolerance_intersection or \
                self.__result_parametric_coordinates[0] > 1.0 + self.__tolerance_intersection:
            self.__position_intersection_first_edge = d.Position.outer
        elif self.__result_parametric_coordinates[0] > -self.__tolerance_intersection and \
                self.__result_parametric_coordinates[0] < self.__tolerance_intersection:
            self.__result_parametric_coordinates[0] = 0.0
            self.__position_intersection_first_edge = d.Position.begin
        elif self.__result_parametric_coordinates[0] > 1.0 - self.__tolerance_intersection and \
                self.__result_parametric_coordinates[0] < 1.0 + self.__tolerance_intersection:
            self.__result_parametric_coordinates[0] = 1.0
            self.__position_intersection_first_edge = d.Position.end
        else:
            self.__position_intersection_first_edge = d.Position.inner

        # in case of intersection (cases 1,2,3,4) returns the position of the intersection with respect to the second segment
        if self.__result_parametric_coordinates[1] < -self.__tolerance_intersection or \
                self.__result_parametric_coordinates[1] > 1.0 + self.__tolerance_intersection:
            self.__position_intersection_second_edge = d.Position.outer
        elif self.__result_parametric_coordinates[1] > -self.__tolerance_intersection and \
                self.__result_parametric_coordinates[1] < self.__tolerance_intersection:
            self.__result_parametric_coordinates[1] = 0.0
            self.__position_intersection_second_edge = d.Position.begin
        elif self.__result_parametric_coordinates[1] > 1.0 - self.__tolerance_intersection and \
                self.__result_parametric_coordinates[1] < 1.0 + self.__tolerance_intersection:
            self.__result_parametric_coordinates[1] = 1.0
            self.__position_intersection_second_edge = d.Position.end
        else:
            self.__position_intersection_second_edge = d.Position.inner

        return intersection


    ### Tolerance Getters
    ### \brief ToleranceIntersection() returns the tolerance in evaluating intersections
    ### \brief ToleranceParallelism() returns the tolerance in evaluating parallelism

    def tolerance_intersection(self) -> float:
        return self.__tolerance_intersection

    def tolerance_parallelism(self) -> float:
        return self.__tolerance_parallelism


    ### Result Getters
    ### \brief ParametricCoordinates() returns a vector containing the parametric coordinates of the intersection with respect to the origin of the segments
    ### \brief FirstParametricCoordinate() returns the parametric coordinate of the intersection with respect to the origin of the first segment
    ### \brief SecondParametricCoordinate() returns the parametric coordinate of the intersection with respect to the origin of the second segment
    ### \brief PositionIntersectionInFirstEdge() returns the position of the intersection with respect to the first segment
    ### \brief PositionIntersectionInSecondEdge() returns the position of the intersection with respect to the second segment
    ### \brief TypeIntersection() returns the type of the intersection
    ### \brief IntersectionFirstParametricCoordinate(const Vector2d& origin,const Vector2d& end) takes in input the origin and the end of the first segment and it returns the
    ### \brief IntersectionSecondParametricCoordinate(const Vector2d& origin,const Vector2d& end) in case of intersection of type 1 and 2 it takes in input the origin and the end of the second segment and it returns the intersection point(coordinates x,y)
    ###   instead in case of intersection of type 3 and 4 it takes in input the origin and end of the first segment and it returns the origin and end of the second segment with respect to the extremes of the first segment the intersection point(coordinates x,y)

    def parametric_coordinates(self) -> np.array([]):
        return self.__result_parametric_coordinates

    def first_parametric_coordinate(self) -> np.double:
        return self.__result_parametric_coordinates[0]

    def second_parametric_coordinate(self) -> np.double:
        return self.__result_parametric_coordinates[1]

    def position_intersection_in_first_edge(self) -> d.Position:
        return self.__position_intersection_first_edge

    def position_intersection_in_second_edge(self) -> d.Position:
        return self.__position_intersection_second_edge

    def type_intersection(self) -> d.Type:
        return self.__type

    def intersection_first_parametric_coordinate(self, origin: np.array([]), end: np.array([])) -> np.array([]):
        return (1 - self.__result_parametric_coordinates[0]) * origin + self.__result_parametric_coordinates[0] * end

    def intersection_second_parametric_coordinate(self, origin: np.array([]), end: np.array([])) -> np.array([]):
        return (1 - self.__result_parametric_coordinates[1]) * origin + self.__result_parametric_coordinates[1] * end

    # caso 1 intersection (on line) e 2 intersection (on segment) : input punti del segmento 1 e 2 rispettivamente per intersection first e second
    # caso 3 intersection (parallel on line) e 4 intersection (parallel on segment) : input punti del primo segmento, in output gli estremi del secondo segmento

    def parallel_twist_happened(self) -> bool:
        return self.__parallel_twist
