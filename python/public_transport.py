#-*-coding:utf-8-*-
import graph 


class Stop(graph.Vertex):
	""" Klasa reprezentuje przystanek. """
	pass 

class Connection(graph.Edge):
	""" Klasa reprezentująca połączenie komunikacyjne
	    pomiędzy przystankami."""
	def __init__(self, line_no, start, time, end):
		graph.Edge.__init__(self, start, end, time)
		self.line_no = line_no

	def __invert__(self): 
		return Stop(self, self.line_no, 
			self.end, self.time, self.start)

	def __repr__(self): 
		return "Connection(" + repr(self.line_no) + ", " + repr(self.start) + ", " + repr(self._weight) + "," + repr(self.end) + ", " + ")"
	
	def __eq__(self, other): 
		return repr(self) == repr(other)

	def time(self): 
		return self.weight 

	def line_no(self):
		return self.line_no

	def start_stop(self): 
		return graph.Edge.source(self)

	def end_stop(self): 
		return graph.Edge.target(self)

class TransportNetwork(graph.Graph): 
	""" Klasa reprezentuje sieć publicznego transportu."""

	@staticmethod
	def load_from_file(filename):
		""" Factory method, która tworzy sieć transportową z pliku."""
		network = TransportNetwork()
		print("Tworzenie sieci transportowej z pliku: %s..." % filename)
		return network 

	def __str__(self): 
		""" Reprezentacja tekstowa sieci transportowej."""
		output = "Sieć transportu publicznego: "
		for key in self.graph: 
			output += " %s | " % str(key)
			for conn in self.graph[key]: # pętla po połączeniach
				output += "%s (linia:%d, czas:%d), " % (
					conn.end_stop(), conn.line_no(), conn.time())
			output += "\n"
		output += "\n"
		return output

	def print_network(self): 
		print(self)

	def number_of_stops(self): 
		return graph.Graph.number_of_vertices(self)

	def number_of_connections(self): 
		return graph.Graph.number_of_edges(self)

	def add_stop(self, stop): 
		graph.Graph.add_vertex(self, stop)

	def add_connection(self, conn): 
		graph.Graph.add_edge(self, conn)

	def direct_connection(self, stop1, stop2): 
		return graph.Graph.direct_edge(self, stop1, stop2)

	def get_connections_from(self, stop):
		return graph.Graph.get_edges_from(self, stop)

	# get_neighbours is inherited 

	def travel_time_between(self, stop1, stop2): 
		""" Najkrótszy czas przejazdu pomiędzy przystankami lub INFINITE. """
		return graph.Graph.weight_between(self, stop1, stop2)

	def get_subnetwork(self, stops): 
		""" Metoda zwraca pod sieć publicznego transportu, 
			zawierająca wskazane przystanki."""
		return graph.Graph.get_subgraph(self, stops)





