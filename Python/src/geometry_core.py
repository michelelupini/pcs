import numpy as np
from src import dataset as d

# la documentazione dei metodi virtuali si trova nelle rispettive interfacce

### \interface implemented by GeometryFactory
class IGeometryFactory:

    def ResetFactory(self):
        pass

    def CreatePolygon(self, file_path: str):
        pass

    def GetPointsNumber(self) -> int:
        pass

    def GetPolygon(self) -> d.Polygon:
        pass

    def GetPolygonEdge(self, edge_position: int) -> d.Segment:
        pass

    def GetAllEdges(self) -> []:
        pass

    def GetLine(self) -> d.Segment:
        pass

    def GetPoint(self, id_point: int) -> d.Point:
        pass

    def GetPointAddress(self, point: d.Point) -> int:
        pass

    def GetNumberSubPolygons(self) -> int:
        pass

    def GetSubPolygon(self, id_polygon: int) -> d.Polygon:
        pass

    def GetAllSubPolygons(self) -> []:
        pass

    def SetPoints(self, point: d.Point) -> int:
        pass

    def SetSubPolygons(self, new_sub_polygon: d.Polygon):
        pass


### \interface implemented by PolygonEdit
class IPolygonEdit:

    def get_problem(self) -> IGeometryFactory:
        pass

    def get_point(self) -> d.Point:
        pass

    def check_side(self, point_id: int) -> int:
        pass

    def cut_polygon(self):
        pass
