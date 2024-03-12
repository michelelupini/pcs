#include "test_geometryfactory.hpp"
#include "test_viewer.hpp"

#include "test_TriangleCut.hpp"
#include "test_RectangleCut.hpp"
#include "test_PentagonCut.hpp"
#include "test_ConcavePolygonCut.hpp"
#include "test_ConcaveAdvanced1Cut.hpp"
#include "test_ConcaveAdvanced2Cut.hpp"
#include "test_DefinitivePolygonCut.hpp"

#include "test_MeshTrianglePolygon.hpp"
#include "test_MeshPentagon.hpp"
#include "test_MeshExagon.hpp"
#include "test_MeshConcavePolygon.hpp"
#include "test_MeshConcaveAdvanced.hpp"
#include "test_MeshDefinitive.hpp"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
