#-*-coding:utf-8-*-
import random

class SimulatedAnnealing: 
	""" Klasa implementująca algorytm heurystyczny symulowanego wyrzażania."""

	INITIAL_TEMPERATURE = 150000
	INITIAL_TEMPTERATURE_LENGTH = 100
	COOLING_RATIO = 0.99

	INFINITY = float("inf")

	def __init__(self, graph): 
		""" Konstruktor obiektu. """
		self.graph = graph
		# mapowanie indeks -> wierzchołek grafu 
		self.index_to_node = self.graph.vertices(); 

	def optimal_hamiltonian_cycle(self):
		""" Metoda znajdująca optymalny (minimalny) cykl Hamiltona w grafie. """

		best_travel_time, edge_cycle = self.search_min_hamiltionian_cycle()

		if best_travel_time < 0: 
			return None 

		printable_cycle = ""
		# rekonstrukcja optymalnego cyklu 
		for edge in edge_cycle:
			printable_cycle += str(edge.source()) 
			if type(edge) is Connection: 
				printable_cycle += "(" + edge.line_number() + ")"
			printable_cycle += "->"

		return (best_travel_time, printable_cycle)

	def search_min_hamiltionian_cycle(self): 
		""" Metoda pomocnicza wykonująca algorytm poszukiwania cyklu.
			Implementacja algorytmu symulowanego wyrzażania. """
		
		# 1. PERMUTACJA POCZĄTKOWA
		permutation = [node for node in self.graph.vertices()] # wypełniamy wierzchołkami
		random.shuffle(permutation) # przetasowanie permutacji wierzchołków 

		# 2. INICJALIZACJA TEMPERATURY I POCZĄTKOWEGO ROZWIĄZANIA 
		# parametry sterujące algorytmem
		T = INITIAL_TEMPERATURE
		temperature_length = INITIAL_TEMPTERATURE_LENGTH
		# bieżące rozwiązanie 
		curr_solution = permutation
		curr_travel_time = hamiltonian_cycle_travel_time(curr_solution)

		# najlepsze rozwiązanie 
		best_solution = curr_solution
		best_travel_time = curr_travel_time

		# 3. PĘTLA OBNIŻANIA TEMPERATURY (SCHŁADZANIA)
		while True: 

			if len(curr_solution) < 4: break 

			# 4. PĘTLA PRÓB WYGENEROWANIA NOWEGO ROZWIĄZANIA DLA DANEJ TEMPERATURY
			for i in range(temperature_length): 

				# generowanie nowego rozwiązanie 
				new_solution, vi, vj = self.generate_new_solution(curr_solution)
				# obliczenie różnicy czasu pomiędzy starym i nowym rozwiązaniem 
				time_delta = self.travel_time_change(curr_travel_time, new_solution, vi, vj)

				if time_delta <= 0: 
					# <0 nowe rozwiązanie ma lepszy czas przejazdu 
					# =0 obydwie permutacje nie są cyklem lub taki sam czas przejazdu

					# 5.1 AKCEPTUJĘ NOWĄ PERMUTACJĘ 
					curr_solution = new_solution
					curr_travel_time += time_delta
					# bieżąca permutacja jest najlepsza
					best_solution = curr_solution
					best_travel_time = curr_travel_time

				else: 
					# >0 nowe rozwiązanie ma gorszy czas przejazdu 
					#    lub nie jest cyklem 

					# 5.2 AKCEPTUJĘ NOWĄ PERMUTACJĘ Z PEWNYM PRAWDOPODOBIEŃSTWEM
					# im gorszy czas przejazdu tym mniejsze prawdopodobieństwo
					# im wyższa temperatura tym większe prawdopodobieństwo 

					# losowanie wartości [0,1)
					rand_num = random.random()

					# sprawdzenie czy wylosowana wartość mniejsza od wyznaczonego 
					# prawdopodobieństwa akceptacji rozwiązania gorszego 
					if rand_num < self.probability(time_delta, T):
						# AKCEPTUJĘ GORSZE ROZWIĄZANIE 
						curr_solution = new_solution
						curr_travel_time += time_delta

			# 6. SCHŁADZANIE - OBNIŻANIE TEMPERATURY
			T *= COOLING_RATIO

			# zwiększenie liczby prób dla niższych temperatur 
			length_ratio =  5.0 if INITIAL_TEMPERATURE/T > 5.0 else INITIAL_TEMPERATURE/T
			temperature_length = INITIAL_TEMPTERATURE_LENGTH*length_ratio

			if T <= 1: # warunek końca pętli 
				break

		# 7. KONIEC - najlepsze rozwiązanie to best_travel_time, best_solution
		if best_travel_time == SimulatedAnnealing.INFINITY: 
			return (None, None)

		edge_cycle = []

		# 8. REKONSTRUKCJA CYKLU HAMILTONA NA PODSTAWIE PERMUTACJI WIERZCHOŁKÓW
		for i in range(len(best_solution)):
			vi = best_solution[i]
			vj = best_solution[(i+1)/len(best_solution)]

			best_edge = None 
			for edge in self.graph[vi]:
				if best_edge == None || best_edge.weight() > edge.weight:
					best_edge = edge

			if not best_edge: # brak połączenia pomiedzy wierzchołkiem vi - vj 
				return (None, None) 

			edge_cycle.append(best_edge)

		return (best_travel_time, edge_cycle)

	def hamiltonian_cycle_travel_time(self, permutation): 
		""" Metoda pomocnicza zwracająca czas przejazdu dla danego cyklu."""
		pass
		# return travel_time 

	def generate_new_solution(old_solution):
		""" Metoda pomocnicza, które generuje nowe rozwiązanie (permutacje) na podstawie starego."""
		pass
		# return (new_solution, vi, vj)

	def travel_time_change(old_travel_time, new_solution, vi, vj): 
		""" Metoda pomocincza, która oblicza zmianę czasu przejazdu pomiędzy starym, a nowym rozwiązaniem. """
		pass
		# return travel_time_delta

	def print_permutation(permutation): 
		""" Wypisanie permutacji."""
		pass

	def probability(time_delta, T): 
		pass

