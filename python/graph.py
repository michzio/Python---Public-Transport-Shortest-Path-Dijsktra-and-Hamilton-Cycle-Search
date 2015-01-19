#-*-coding:utf-8-*-
import dijkstra

class Edge: 
	""" Krawędzie będą elementami list sąsiedztwa przyporządkowanym 
	    poszczególnym wierzchołkom grafu. Dzięki temu mamy możliwość 
	    przechowywania sąsiadującego wierzchołka wraz z wagą krawędzi 
	    prowadzącej do tego wierzchołka. """

	def __init__(self, start, end, weight):
		""" Konstruktor krawędzi grafu w grafie skierowanym, z wagami. """
		self.start = start 
		self.end = end
		self._weight = weight

	def target(self): # wierzchołek docelowy
		return self.end

	def source(self): # wierzchołek źródłowy
		return self.start

	def weight(self): # waga krawędzi 
		return self._weight

	def __invert__(self): 
		return Edge(self.end, self.start, self.weight)

	def __repr__(self): 
		return "Edge(" + repr(self.start) + ", " + repr(self.end) + ", " + repr(self._weight) + ")"

	def __eq__(self, other): 
		return repr(self) == repr(other)

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

	INFINITY = float("inf")

	def __init__(self):
		""" Kontruktor tworzący pusty graf.  
		    Graf będzie reprezentowany w postaci słownika, 
		    którego kluczami są wierzchołki, a elementami lista
		    krawędzi prowadzących do sąsiadującego wierzchołka."""

		self.graph = { } # pusty słownik (implementacja listy sąsiedztwa)

	def __str__(self): 
		""" Reprezentacja tekstowa grafu. """
		output = "Graf: "
		for key in self.graph: 
			output += " %s | " % str(key)
			for edge in self.graph[key]:
				output += "%s (%s), " % (edge.target(), edge.weight())
			output += "\n"
		output += "\n"
		return output

	def print_graph(self):
		print(self)

	def number_of_vertices(self): 
		return len(self.graph)

	def number_of_edges(self): 
		return sum(len(self.graph[node]) for node in self.graph )

	def add_vertex(self, node): 
		""" Metoda dodaje nowy wierzchołek do grafu."""
		if node not in self.graph: 
			self.graph[node] = []   # inicjalizacja pustą listą krawędzi 

	def add_edge(self, edge): 
		""" Metoda dodaje nową krawędź do grafu."""
		self.add_vertex(edge.source())
		self.add_vertex(edge.target()) # dodanie wierzchołków jeżeli nie istnieja
		self.graph[edge.source()].append(edge)

	def add_undirected_edge(self, v1, v2, weight): 
		""" Metoda dodaje nieskierowaną krwaędź pomiędzy 
			wierzchołkami v1, v2 z wagą weight."""
		self.add_edge(Edge(v1,v2,weight))
		self.add_edge(Edge(v2, v1, weight))

	def direct_edge(self, v1, v2): 
		""" Metoda wyszukuje bezpośredniej krawędzi z v1 do v2. """
		if v1 in self.graph: 
			for edge in self.graph[v1]:
				if edge.target() == v2:
					return edge 
		return None

	def vertices_iterator(self):
		for node in self.graph.keys():
			yield node

	def vertices(self): 
		return self.graph.keys()

	def get_edges_from(self, v): 
		return self.graph.get(v)

	def get_neighbours(self, v): 
		if v in self.graph: 
			return [edge.target() for edge in self.graph[v]]
		return None

	def weight_between(self, v1, v2): 
		""" Metoda zwraca minimalną wagę połączenia 
		    bezpośredniego (pojedynczej krawędzi) pomiędzy 
			dwoma wierzchołkami grafu lub INFINITY. """ 
		min_edge_weight = float("inf")

		# przeglądamy lis†ę sąsiedztwa dla wierzchołka v1 
		for edge in self.get_edges_from(v1):
			if edge.target() == v2: 
				if min_edge_weight > edge.weight():
					min_edge_weight = edge.weight()

		return min_edge_weight


	def get_subgraph(self, nodes):
		""" Metoda zwraca podgraf z wybranymi węzłami. """
		pass






