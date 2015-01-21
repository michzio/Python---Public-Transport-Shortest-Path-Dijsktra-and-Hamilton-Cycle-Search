#-*-coding:utf-8-*-
import unittest
from graph import *

class TestGraph(unittest.TestCase): 

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

    def test_add_vertex(self): 
        G = Graph() 
        G.add_vertex(Vertex("5"))
        self.assertTrue(Vertex("5") in G.vertices())
        self.assertEqual(self.graph.number_of_vertices(), 6)

    def test_add_edges(self): 
        G = Graph()
        G.add_edge(Edge(Vertex("1"), Vertex("2"), 5))
        self.assertTrue(Edge(Vertex("1"), Vertex("2"), 5) in G.get_edges_from(Vertex("1")) )
        self.assertTrue(Vertex("2") in G.get_neighbours(Vertex("1")))
        self.assertEqual(self.graph.number_of_edges(), 7*2) # undirected = 2*directed

    def test_direct_edge(self): 
        self.assertEqual(self.graph.direct_edge(Vertex("b"), Vertex("e")), 
            Edge(Vertex("b"), Vertex("e"), 2))

    def test_print_graph(self): 
        self.assertTrue(len(str(self.graph)) > 0)
        self.graph.print_graph()

    def test_get_subgraph(self): 
        subgraph = self.graph.get_subgraph(["e", "b", "d", "c"])
        subgraph.print_graph()
        self.assertEqual(subgraph.number_of_vertices(), 4)
        self.assertEqual(subgraph.number_of_edges(), 8)
        pass

    def tearDown(self): 
        return 

if __name__ == '__main__':
    unittest.main() # uruchomienie wszystkich testów 