#-*-coding:utf-8-*-
import heapq

class Dijkstra: 
    """ Klasa implementująca algorytm Dijkstry 
        który wyszukuje najkrótsze ścieżki między dwoma
        wierzchołkami w grafie. """

    INFINITY = float('inf')

    def __init__(self, graph):
        # kompozycja, zapisanie w atrybucie przeszukiwanego grafu 
        self.graph = graph
        self.init()
        
    def init(self): 
        """ Metoda inicajlizująca kontenery wykorzystywane przez algorytm Dijkstry."""
        # dict z kosztem przejścia od wierzchołka źródłowego 
        # do wszystkich pozostałych wierzchołków grafu 
        # idąc po najkrótszej ścieżce. Inicjalizacja wartością INFINITY
        self.path_cost = { node : Dijkstra.INFINITY for node in self.graph.vertices() }
        # dict z krawędziami bezpośrednio poprzedzającymi i-ty 
        # wierzchołek końcowy na najkrótszej ścieżce z wierzchołka źródłowego
        self.predecessor_edge = { node : None for node in self.graph.vertices() }
        # bieżący wierzchołek źródłowy - ustawiane przez metodę min_paths_from()
        self.source_node = None 

    def min_path(self, source, target):
        """ Metoda pobiera ścieżkę o minimalnym koszcie między source i target 
            na podstawie ostatnich obliczeń. """
        if self.source_node != source: 
            self.find_min_paths_from(source)

        #print(self.path_cost)
        if self.path_cost[target] == Dijkstra.INFINITY: 
            return (None, None) # koszt = None, ścieżka = None 

        path = []

        last_node = None 
        prev_edge = self.predecessor_edge[target] 
        while prev_edge: 
            if prev_edge.source() == target or prev_edge.source() == last_node: 
                break
            path.insert(0,prev_edge) # push front prev_edge
            last_node = prev_edge.source()
            prev_edge = self.predecessor_edge[prev_edge.source()] # iterowanie wstecz ścieżki

        return (self.path_cost[target], path) # koszt, ścieżka


    def find_min_paths_from(self, source): 
        """ Metoda wyszukuje najkrótsze ścieżki (min koszt) z source do pozostałych
            wierzchołków grafu. """

        if self.source_node == source: 
            return True # obiekt został już wykonany dla wierzchołka source! 

        # 1. inicjalizacja kontenerów 
        self.init() 

        # 2. ustawienie wierzchołka źródłowego 
        self.source_node = source 

        # 3. inicjalizacja czasu przejścia z source-source na 0
        self.path_cost[source] = 0 

        # 4. utworzenie kolejki priorytetowej o rozmiarze równym liczbie wierzchołków grafu
        #    i wstawienie do kolejki poszczególnych wierzchołków + koszt dojścia do nich z source 
        #    na kolejke priorytetową wstawiamy tuple (priorytet, item) = (koszt, wierzchołek)
        queue = [ ] 
        for node in self.graph.vertices(): 
            heapq.heappush(queue, (self.path_cost[node], node) ) 

        # 5. pętla dla elementów zdejmowanych z kolejki priorytetowej 
        #    wykonuje się aż do momentu zdjęcia wszystkich elementów z kolejki
        #    na kolejkę dostawiamy za każdym razem wierzchołki sąsiadujące z odpowiednim kosztem dojścia
        while queue: 
            node = heapq.heappop(queue)[1] # za każdym razem zdejmujemy wierzchołek z min. kosztem 
            if self.path_cost[node] == Dijkstra.INFINITY: 
                # wierzchołek nieosiągalny
                return True
            # pętla po wszystkich sąsiadach wierzchołka node 
            for edge in self.graph.get_edges_from(node):
                neighbour = edge.target()

                # 6. relaksacja krawędzi (node, neighbour)
                if self.path_cost[neighbour] > self.path_cost[node] + edge.weight():
                    # aktualizacja kosztu dojścia do neighbour oraz krawędzi poprzedzającej
                    self.path_cost[neighbour] = self.path_cost[node] + edge.weight()
                    self.predecessor_edge[neighbour] = edge 
                    # wstawienie sąsiada z nowym priorytetem 
                    heapq.heappush(queue, (self.path_cost[neighbour], neighbour))

        return True 

    @staticmethod
    def printable(path): 
        """ Przekształca ścieżkę zwracaną przez algorytm Dijkstry na wersje czytelną."""
        output = ""
        for edge in path: 
            output += edge.source() + "-" + str(edge.weight()) + "-"
        output += edge.target()
        return output

