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


public_transport.TransportNetwork.load_from_file("file.txt")

