#-*-coding:utf-8-*-
import random
import math

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
				new_solution, vi_idx, vj_idx = self.generate_new_solution(curr_solution)
				# obliczenie różnicy czasu pomiędzy starym i nowym rozwiązaniem 
				time_delta = self.travel_time_change(curr_travel_time, new_solution, vi_idx, vj_idx)

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
		""" Metoda pomocnicza zwracająca czas przejazdu dla danego cyklu. 
			Jeżeli dla przekazanej permutacji wierzchołków cykl nie istnieje 
			(brak którejś z krawędzi) to metoda zwraca INFINITY."""

		travel_time = 0 

		# pętla po kolejnych parach wierzchołków
		for i in range(len(permutation)): 
			vi = permutation[i]
			vj = permutation[(i+1)/len(best_solution)]

			# zapytanie o czas przejazdu między vi - vj
			edge_travel_time = self.graph.weight_between(vi,vj)
			# jeżeli INFINITY to brak krawędzi 
			if edge_travel_time == SimulatedAnnealing.INFINITY:
				return SimulatedAnnealing.INFINITY
			travel_time += edge_travel_time
		
		return travel_time 

	def generate_new_solution(old_solution):
		""" Metoda pomocnicza, które generuje nowe rozwiązanie (permutacje) 
			na podstawie starego. Algorytm generowania nowej permutacji: 
			1. Losowe wygenerowanie indeksów vi_idx, vj_idx z przedziału [0, len(permutation))
			2. Zamiana par wierzchołków (vi, vi+1) oraz (vj, vj+1) tak by otrzymać 
			   pary (vi, vj) oraz (vi+1, vj+1) 
			3. Odwrócenie kolejności elementów permutacji (vi+1, ..., vj) -> (vj, ..., vi+1) """

		new_solution = old_solution[:] # przekopiowanie permutacji
		if len(new_solution) < 4: return new_solution # za mało elementów by wylosować 2 pary

		# 1. losowanie indeksów vi_idx < vj_idx
		n = len(new_solution)
		vi_idx = random.randrange(0, n-2)
		while True: 
			vj_idx = (random.randrange(0, n-(vi_idx+1)) + (vi_idx+2) ) % n 
			if vi_idx != vj_idx and vi_idx != ((vj_idx+1)%n):
				break

		print("Wylosowano pary indeksów (vi, vi+1) = (%d, %d) oraz (vj, vj+1) = (%d,%d)", 
			vi_idx, (vi_idx+1)%n, vj_idx, (vj_idx+1)%n)

		# 2. zamiana par wierzchołków (vi, vi+1) i (vj, vj+1)
		#    otrzymując parę (vi, vj) oraz (vi+1, vj+1)
		#	 czyli swap(vi+1, vj)
		(new_solution[(vi_idx+1)%n], new_solution[vj_idx]) = (new_solution[vj_idx],new_solution[(vi_idx+1)%n])

		# 3. odwróceie kolejności elementów podlisty (vi+1,...vj) -> (vj, ..., vi+1)
		new_solution[((vi_idx+1)%n+1):vj_idx] = new_solution[((vi_idx+1)%n+1):vj_idx][::-1]

		print_permutation(old_solution)
		print_permutation(new_solution)

		return (new_solution, vi_idx, vj_idx)

	def travel_time_change(old_travel_time, new_solution, vi_idx, vj_idx): 
		""" Metoda pomocincza, która oblicza zmianę czasu przejazdu pomiędzy starym, a nowym rozwiązaniem. 
			1. stare rozwiązanie nie było poprawnym cyklem, nowa permutacja może nim być, ale nie musi
			   (potrzeba na nowo wyznaczyć długość cyklu dla permutacji wierzchołków)
			2. stare rozwiązanie było poprawnym cyklem, po zmianie nowa permutacja może być poprawnym 
			   cyklem, ale nie musi."""

		time_delta = 0 

		# 1. stara permutacja nie była cyklem 
		if old_travel_time == SimulatedAnnealing.INFINITY: 
			# obliczamy czas przejścia nowej permutacji 
			new_travel_time = self.hamiltonian_cycle_travel_time(new_solution)
			# obliczenie zmiany czasu 
			# a) INFINITY - INFINITY == 0, obie permutacje nie są cyklem
			# b) new_time - INFINITY < 0, nowa permutacja jest cyklem 
			time_delta = new_travel_time - old_travel_time
		else:
			# 2. stara permutacja była cyklem 
			#    wyznaczenie różnicy przyrostowo ze wzoru: 
			#    time_delta = time(vi, vi+1) + time(vj,vj+1) - time(vi,vj) - time(vi+1,vj+1)
			#	 jeżeli którakolwiek z nowych krawędzi nie istnieje to time_delta = INFINITY
			time_new_edge_i = self.graph.weight_between(new_solution[vi_idx], new_solution[(vi_idx+1)%n])
			time_new_edge_j = self.graph.weight_between(new_solution[vj_idx], new_solution[(vj_idx+1)%n])

			if time_new_edge_i == SimulatedAnnealing.INFINITY or time_new_edge_j == SimulatedAnnealing.INFINITY:
				time_delta = INFINITY - old_travel_time # należy odjąć czas starego cyklu 
			else: 
				# obydwa cykle poprawne - obliczmy różnicę czasów 
				time_old_edge_i = self.graph.weight_between(new_solution[vi_idx], new_solution[vj_idx])
				time_old_edge_j = self.graph.weight_between(new_solution[(vi_idx+1)%n], new_solution[(vj_idx+1)%n])

				time_delta = time_new_edge_i + time_new_edge_j - time_old_edge_i - time_old_edge_j

		# możliwe wyniki: 
		# time_delta == 0, obie prmutacje nie są cyklami, lub czas się nie zmienił 
		# time_delta << 0, nowa permutacja jest cyklem, a stara nie 
		# time_delta < 0, obie permutacje są cyklami, nowa ma krótszy czas 
		# time_delta > 0, obie pertmuacje są cyklami, nowa ma dłuższy czas 
		# time_delta >> 0, nowa permutacja nie jest cyklem, stara była cyklem
		return travel_time_delta
		

	def print_permutation(permutation): 
		""" Wypisanie permutacji."""
		print("-".join(permutation)) 

	def probability(time_delta, T): 
		""" Prawdopodobieństwo akceptacji nowej permutacji cyklu. Zależy od:
			1. różnica czasu przejazdu między nowym i starym cyklem 
			   im większe wydłużenie czasu to mniejsze prawdopodobieństwo 
			2. temperatura - im wyższa tym większe prawdopodobieństwo 
			Zwracana wartość jest w przedziale (0,1] bo exp(negative_x). """

		if time_delta < 0: return 1

		return math.exp(-time_delta/T)

