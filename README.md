# Python---Public-Transport-Shortest-Path-Dijsktra-and-Hamilton-Cycle-Search
This is program that uses Dijkstra algorithm in order to find shortest connection
in public transport network implemented as directed graph with time as edges weight. 
Program also enable to find Hamilton cycle resolving "Travelling salesman problem" 
using Simulated annealing algorithm.  

Problem Definition: 

1) There is graph (V,E)
2) Nodes in this graph represents public transport stops, 
   while edges represent bus (or tram) connections between them. 
3) Edges has weights that represents travel time between stops. 
4) Task has two parts: 
   a) client types two stops and the algorithm have to find 
      the shortest path (connection) taking into account travel time (edges weights) - this is Dijkstra algorithm 
   b) client types list of stops and the algorithm have to find 
      shortest cycle visiting each stop on the list. If it is possible 
      each stop should be visited only once. - this is hamilton cycle for graph constructed from typed stops list (travelling salesman problem), if there isn't hamilton cycle we allow for repeating vertices in cycle. This problem will be solved by generating possible vertices permutation that represents 
      cycle and use heuristic algorithm -> Simulated annealing. 
 5) INPUT: 
<numbers of lines>
<line number>
<stop_name>, <travel_time>, <stop_name>, <travel_time>, ..., <stop_name>
... 
<linia number>
<stop_name>, <travel_time>, <stop_name, <travel_time>, ..., <stop_name> 

6) EXAMPLE INPUT: 
5
18
Czerwone_Maki, 2, Chmieleniec, 1, Kampus_UJ, 2, ..., 3, Krowodrza Górka 
...
164
Piaski_Nowe, 2, Nowosądecka, 1, ..., 2, Górka Narodowa

6) EXAMPLE OUTPUT - PROBLEM 1: 
Type starting stop: Kampus_UJ
Type ending stop: Teatr_Bagatela
Shortest connection: Kampus_UJ, Ruczaj, Norymberska, ..., Filharmonia, Teatr_Bagatela (travel time 30)
52 → 11 → 18

7) EXAMPLE OUTPUT - PROBLEM 2: 
Type number of stops to visit: 5
Type name of 1st stop: Kampus_UJ
...
Type name of 5th stop: Teatr_Bagatela
Shortest cycle visiting each stop: Kampus_UJ, ..., Kampus_UJ
52 → 11 → 18 → 11 → 52