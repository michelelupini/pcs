import enum

tol = 1e-5


class Type(enum.Enum):
    no_intersection = 0                     ### \enum no intersection of the segments
    intersection_on_line = 1,               ### \enum intersection of the extension of the first segment and the second segment
    intersection_on_segment = 2,            ### \enum intersection of the segments
    intersection_parallel_on_line = 3,      ### \enum parallel intersection of the extension of the first segment and the second segment
    intersection_parallel_on_segment = 4    ### \enum parallel intersection of the segments


class Position(enum.Enum):
    begin = 0,      ### \enum intersection of the extension of the first segment and the second segment on the origin of the first segment
    inner = 1,      ### \enum intersection of the extension of the first segment and the second segment inside the first segment
    end = 2,        ### \enum intersection of the extension of the first segment and the second segment on the end of the first segment
    outer = 3,      ### \enum intersection of the extension of the first segment and the second segment out of the first segment
    default = 4,    ### \enum initial default position (no intersection)


### \class Point is a couple of doubles, which are the coordinates of a point in a cartesian reference frame
class Point:
    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y

    # \brief Operator (==) compares two points returning True if the 2 points are identical, False otherwise
    def __eq__(self, other) -> bool:
        return abs(self.x - other.x) < tol and abs(self.y - other.y) < tol


### \class Segment is  a couple of indexes referred to the vector _points of Point in GeometryFactory
class Segment:
    def __init__(self, new_from: int, new_to: int):
        self.From = new_from
        self.To = new_to


### \class Polygon is a vector of indexes referred to the vector _points of Point in GeometryFactory
class Polygon:
    def __init__(self, vertices: []):
        self.polygon_vertices = vertices.copy()
        self.number_vertices = len(vertices)

    ### \brief compare two polygons, scanning their vertices until it finds two identical sequences
    def __eq__(self, other):
        exit = False
        n = self.number_vertices
        if n is other.number_vertices: # controllo che i numeri di vertici siano uguali
            i = 0
            while i < n  and (not exit): # con i 2 cicli for eseguo il controllo (fino a quando exit è falso)
                exit = True
                j = 0
                while j < n and exit: # fino a quando exit è vero
                    if not (self.polygon_vertices[j] == other.polygon_vertices[(i+j)%n]):
                        exit = False
                    j += 1
                i += 1
        # ritorno exit = 1 se ho trovato una sequenza corrispondente
        return exit
