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

	def time(self): 
		return self.weight 

	def line_no(self):
		return self.line_no

	def start_stop(self): 
		return Edge.source(self)

	def end_stop(self): 
		return Edge.target(self)

class TransportNetwork(graph.Graph): 
	""" Klasa reprezentuje sieć publicznego transportu."""

	def __str__(self): 
		""" Reprezentacja tekstowa sieci transportowej."""
		print("Sieć transportu publicznego: ")
		for key in self.graph: 
			print(" %s | " % str(self.graph[key]), end="")
			for conn in self.graph[key]: # pętla po połączeniach
				print("%s (linia:%d, czas:%d), " % (
					str(conn.end_stop()), conn.line_no(), conn.time()), end="")
			print()
		print()


