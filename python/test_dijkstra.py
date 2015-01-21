import unittest
import dijkstra
from graph import Graph, Vertex, Edge


class TestDijkstraAlgorithm(unittest.TestCase): 

    def setUp(self): 
        self.graph = Graph()
        self.graph.add_vertex(Vertex("a"))
        self.graph.add_vertex(Vertex("b"))
        self.graph.add_vertex(Vertex("c"))
        self.graph.add_vertex(Vertex("d"))
        self.graph.add_vertex(Vertex("e"))
        self.graph.add_vertex(Vertex("f"))

        self.graph.add_undirected_edge(Vertex("a"), Vertex("b"), 2)
        self.graph.add_undirected_edge(Vertex("a"), Vertex("e"), 3)
        self.graph.add_undirected_edge(Vertex("b"), Vertex("e"), 2)
        self.graph.add_undirected_edge(Vertex("b"), Vertex("c"), 1)
        self.graph.add_undirected_edge(Vertex("c"), Vertex("d"), 2)
        self.graph.add_undirected_edge(Vertex("d"), Vertex("e"), 3)
        self.graph.add_undirected_edge(Vertex("d"), Vertex("f"), 4)

        self.dijkstra = dijkstra.Dijkstra(self.graph)

    def test_min_path(self): 
        """ Bezpośrednie testowanie metod wyszukiwania 
            minimalnej ścieżki na obiekcie reprezentującym 
            algorytm Dijkstry."""
            
        min_cost, min_path_edges = self.dijkstra.min_path(Vertex("a"), Vertex("f"))

        print( dijkstra.Dijkstra.printable(min_path_edges))
        self.assertEqual(min_cost, 9)
        self.assertEqual(dijkstra.Dijkstra.printable(min_path_edges), 
            "a-2-b-1-c-2-d-4-f")

        min_cost, min_path_edges = self.dijkstra.min_path(Vertex("e"), Vertex("c"))
        print( dijkstra.Dijkstra.printable(min_path_edges))
        self.assertEqual(min_cost, 3)
        self.assertEqual(dijkstra.Dijkstra.printable(min_path_edges), 
            "e-2-b-1-c")

    def test_find_min_path(self): 
        """ Metoda testująca znajdowanie minimalnej ścieżki za 
            pośrednictwem metody zdefiniowanej na obiekcie grafu."""

        min_cost, min_path = self.graph.find_min_path(Vertex("a"), Vertex("f"))
        self.assertEqual(min_cost, 9)
        self.assertEqual(min_path, ["a", "b", "c", "d", "f"])

    def tearDown(self): 
        return 

if __name__ == "__main__":
    unittest.main()