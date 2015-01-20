from graph import *
from public_transport import *
from simulated_annealing import *
import unittest 

class TestSimulatedAnnealing(unittest.TestCase): 

	def setUp(self): 
		self.graph = Graph()
		self.graph.add_undirected_edge(Vertex("A"), Vertex("B"), 20)
		self.graph.add_undirected_edge(Vertex("A"), Vertex("C"), 42)
		self.graph.add_undirected_edge(Vertex("A"), Vertex("D"), 35)
		self.graph.add_undirected_edge(Vertex("B"), Vertex("C"), 30)
		self.graph.add_undirected_edge(Vertex("B"), Vertex("D"), 34)
		self.graph.add_undirected_edge(Vertex("C"), Vertex("D"), 12)

	def test_graph_correctness(self): 
		self.assertEqual(self.graph.number_of_vertices(), 4)
		self.assertEqual(self.graph.number_of_edges(), 6*2)

	def test_simulated_annealing_algorithm(self):
		algorithm = SimulatedAnnealing(self.graph)
		best_time, cycle = algorithm.optimal_hamiltonian_cycle()
		print("Otrzymany najkrótszy cykl: "  + cycle)
		self.assertEqual(best_time, 97)

	def test_simulated_annealing_algorithm_on_public_transport(self): 
		network = TransportNetwork()
		network.add_undirected_connection(10, Stop("A"), Stop("B"), 20)
		network.add_undirected_connection(10, Stop("A"), Stop("C"), 42)
		network.add_undirected_connection(15, Stop("A"), Stop("D"), 35)
		network.add_undirected_connection(15, Stop("B"), Stop("C"), 30)
		network.add_undirected_connection(20, Stop("B"), Stop("D"), 34)
		network.add_undirected_connection(20, Stop("C"), Stop("D"), 12)

		algorithm = SimulatedAnnealing(network)
		best_time, cycle = algorithm.optimal_hamiltonian_cycle()
		print("Otrzymany najkrótszy cykl: "  + cycle)
		self.assertEqual(best_time, 97)


	def tearDown(self): 
		return 

if __name__ == "__main__":
	unittest.main()
