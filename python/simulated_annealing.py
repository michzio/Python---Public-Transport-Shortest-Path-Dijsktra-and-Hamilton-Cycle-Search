#-*-coding:utf-8-*-

class SimulatedAnnealing: 
	""" Klasa implementująca algorytm heurystyczny symulowanego wyrzażania."""

	INITIAL_TEMPERATURE = 150000
	INITIAL_TEMPTERATURE_LENGTH = 100
	COOLING_RATIO = 0.99

	def __init__(self, graph): 
		""" Konstruktor obiektu. """
		self.graph = graph
		# mapowanie indeks -> wierzchołek grafu 
		self.index_to_node = self.graph.vertices(); 

	def optimal_hamiltonian_cycle(self):
		""" Metoda znajdująca optymalny (minimalny) cykl Hamiltona w grafie. """
		pass
		# return min_cycle

	def search_min_hamiltionian_cycle(self): 
		""" Metoda pomocnicza wykonująca algorytm poszukiwania cyklu."""
		pass
		# return edge_cycle

	def hamiltonian_cycle_travel_time(self, permutation): 
		""" Metoda pomocnicza zwracająca czas przejazdu dla danego cyklu."""
		pass
		# return travel_time 

	def generate_new_solution(old_solution, vi, vj):
		""" Metoda pomocnicza, które generuje nowe rozwiązanie (permutacje) na podstawie starego."""
		pass
		# return new_solution

	def travel_time_change(old_travel_time, new_solution, vi, vj): 
		""" Metoda pomocincza, która oblicza zmianę czasu przejazdu pomiędzy starym, a nowym rozwiązaniem. """
		pass
		# return new_travel_time

	def print_permutation(permutation): 
		""" Wypisanie permutacji."""
		pass

	def probability(delta_time, T): 
		pass

