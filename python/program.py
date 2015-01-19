#-*-coding:utf-8-*-
"""
 Dane wejściowe: 
 <ilość linii> 
 <linia 1> 
 <nazwa przystanku>, <czas przejazdu>, <nazwa przystanku>, <czas przejazdu>, 
 <nazwa przystanku, ... , <nazwa przystanku> 
 ...
 <linia n> 
 <nazwa przystanku>, <czas przejazdu>, <nazwa przystanku>, <czas przejazdu>, 
 <nazwa przystanku, ... , <nazwa przystanku> 
"""

import public_transport


network = public_transport.TransportNetwork.load_from_file("rozklad2.txt")

network.print_network()
print(network.find_shortest_connection(public_transport.Stop("Czerwone Maki"), public_transport.Stop("Most Grunwaldzki")) )


