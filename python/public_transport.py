#-*-coding:utf-8-*-
import graph 
import dijkstra


class Stop(graph.Vertex):
    """ Klasa reprezentuje przystanek. """
    def __repr__(self):
        return 'Stop("' + str(self) + '")'
    def __hash__(self): 
        return hash(repr(self))


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
        return graph.Edge.weight(self) 

    def line_number(self):
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
        
        file = open(filename, "r")
        try: 
            no_of_lines = int(file.readline())
        except ValueError: 
            print("Niepoprawna liczba linii autobusowych!")
            file.close()
            return None

        for i in range(no_of_lines): 
            try: 
                line_no = int(file.readline())
            except ValueError: 
                print("Niepoprawny numer linii!")
                file.close()
                return None
            
            line_schedule = file.readline()
            tokens = line_schedule.split(",")
            end_stop = tokens[0]
            for j in range(1,len(tokens)-2, 2): # parsing each schedule line
                start_stop = end_stop
                
                try: 
                    travel_time = int(tokens[j])
                except ValueError: 
                    print("Błedna wartość czasu przejazdu pomiędzy przystankami!")
                    file.close()
                    return None 

                end_stop = tokens[j+1]
                #print ("%s - %s - %s" % (start_stop, travel_time, end_stop))
                network.add_undirected_connection(line_no, Stop(start_stop.strip()), Stop(end_stop.strip()), travel_time)
        file.close()
        return network 

    def __str__(self): 
        """ Reprezentacja tekstowa sieci transportowej."""
        output = "Sieć transportu publicznego: "
        for key in self.graph: 
            output += "%s: " % str(key) + "\n"
            for conn in self.graph[key]: # pętla po połączeniach
                output += "%s (linia:%d, czas:%d), " % (
                    conn.end_stop(), conn.line_number(), conn.time()) 
                output += "\n"
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

    def add_undirected_connection(self, line_no, stop1, stop2, travel_time):
        """ Metoda dodaje połączenie pomiędzy przystankami o zadanym czasie 
            jednocześnie w obu kierunkach stop1->stop2 i stop2->stop1 
            dla linii line_no. """
        self.add_connection(Connection(line_no, stop1, travel_time, stop2))
        self.add_connection(Connection(line_no, stop2, travel_time, stop1))

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

    def get_stops(self): 
        return graph.Graph.vertices(self)

    def find_shortest_connection(self, stop1, stop2): 

        if not self.dijkstra: 
            self.dijkstra = dijkstra.Dijkstra(self) 

        min_travel_time, min_connections_path = self.dijkstra.min_path(stop1, stop2)

        if not min_connections_path: 
            print("Brak połączenia pomiędzy przystankami....")
            return (None, None)
        else: 
            shortest_connection = [ str(conn.start_stop()) + "(" + str(conn.line_number()) + ")" for conn in min_connections_path]
            shortest_connection.append(str(min_connections_path[-1].end_stop()))
            return (min_travel_time, shortest_connection)
