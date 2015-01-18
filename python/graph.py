class Edge: 
	""" Krawędzie będą elementami list sąsiedztwa przyporządkowanym 
	    poszczególnym wierzchołkom grafu. Dzięki temu mamy możliwość 
	    przechowywania sąsiadującego wierzchołka wraz z wagą krawędzi 
	    prowadzącej do tego wierzchołka. """

	def __init__(self, start, end, weight):
		""" Konstruktor krawędzi grafu w grafie skierowanym, z wagami. """
		self.start = start 
		self.end = end
		self.weight = weight

	def target(self): # wierzchołek docelowy
		return self.end

	def source(self): # wierzchołek źródłowy
		return self.start

	def weight(self): # waga krawędzi 
		return self.weight

	def __invert__(self): 
		return Edge(self.end, self.start, self.weight)

class Vertex(str): 
	""" Klasa reprezentująca wierzchołki grafu. """
	
	def __init__(self, label): 
		self.label = label 

	def __str__(self):
		return str(self.label)

	def __repr__(self): 
		return "Vertex(" + str(self) + ")"

	def __hash__(self): 
		return hash(repr(self))

class Graph: 
	""" Klasa reprezentująca graf. """

	def __init__(self):
		""" Kontruktor tworzący pusty graf.  
		    Graf będzie reprezentowany w postaci słownika, 
		    którego kluczami są wierzchołki, a elementami lista
		    krawędzi prowadzących do sąsiadującego wierzchołka."""

		self.graph = { } # pusty słownik (implementacja listy sąsiedztwa)
		self.vertices_number = 0 # liczba wierzchołków grafu  

	def __str__(self): 
		""" Reprezentacja tekstowa grafu. """
		print("Graf: ")
		for key in self.graph: 
			print(" %s | " % str(self.graph[key]), end="")
			for edge in self.graph[key]:
				print("%s (%d), " % (str(edge.target()), edge.weight()), end="")
			print()
		print()

	def number_of_vertices(self): 
		return self.vertices_number 

	def number_of_edges(self): 
		return sum(len(self.graph[node]) for node in self.graph )

	def add_vertex(self, node): 
		""" Metoda dodaje nowy wierzchołek do grafu."""
		if node not in self.graph: 
			graph[node] = []   # inicjalizacja pustą listą krawędzi 

	def add_edge(self, edge): 
		""" Metoda dodaje nową krawędź do grafu."""
		self.add_vertex(edge.source())
		self.add_vertex(edge.target()) # dodanie wierzchołków jeżeli nie istnieja
		self.graph[edge.source()].append(edge)

	def direct_edge(self, v1, v2): 
		""" Metoda wyszukuje bezpośredniej krawędzi z v1 do v2. """
		if v1 in self.graph: 
			for edge in self.graph[v1]:
				if edge.target() == v2:
					return edge 
		return None

	def vertices(self):
		for node in self.graph.keys():
			yield node

	def get_edges_from(self, v): 
		return self.graph.get(v)

	def get_neighbours(self, v): 
		if v in self.graph: 
			return [edge.target() for edge in self.graph[v]]
		return None




