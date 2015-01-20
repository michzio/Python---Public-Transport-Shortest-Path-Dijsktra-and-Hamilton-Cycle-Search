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

import sys
import public_transport
from enum import Enum

class MenuOptions(Enum): 
	SHORTEST_PATH = 1
	HAMILTONIAN_CYCLE = 2

def shortest_path_prompt(network):

	try: 

		start_stop = public_transport.Stop(str(input("Podaj przystanek początkowy: ")).strip())

		if start_stop not in network.vertices(): 
			print("Podany przystanek nie istnieje!\n")
			return 

		end_stop = public_transport.Stop(str(input("Podaj przystanek końcowy: ")).strip())
		if end_stop not in network.vertices(): 
			print("Podany przystanek nie istnieje!\n")
			return 

		# wyszukiwanie najkrótszej ścieżki, algorytm Dijkstry
		print("Wyszukiwanie najszybszego połączenia pomiędzy %s -> %s " % (start_stop, end_stop))
		min_travel_time, shortest_connection = network.find_shortest_connection(start_stop, end_stop)
		print("Minimalny czas przejazdu: %d" % min_travel_time)
		print("Najszybsza trasa:")
		print("->".join(shortest_connection))
		print()
		
	except ValueError: 
		print("Błąd wczytywania nazw przystanków!\n")
		return

def hamiltonian_cycle_prompt(network): 
	pass

# MAIN PROGRAM

if len(sys.argv) != 2: 
	print("Wywołanie programu: python program.py rozklad.txt")
	sys.exit(1)

# pobranie nazwy pliku z siatką połączeń 
filename = sys.argv[1]
# konstrukcja grafu 
network = public_transport.TransportNetwork.load_from_file(filename)

if not network: 
	print("Nie udało się utworzyć grafu sieci transportowej.") 
	sys.exit(1)

# wypisanie grafu 
network.print_network()
 
while True:
	print("MENU:")
	print("1 - najkrótsza trasa z przystanku A do B")
	print("2 - najkrótszy cykl dla wybranych przystanków (problem komiwojażera)")
	print("dowolny inny klawisz - wyjście z programu")

	try: 
		menu_option = int(input("Wybierz opcję wyszukiwania: ")) 
		print("Wybrano: %d" % menu_option)

	except ValueError: 
		print("wyjście z programu\n")
		sys.exit(0)

	if MenuOptions(menu_option) == MenuOptions.SHORTEST_PATH: 
		shortest_path_prompt(network)
	elif MenuOptions(menu_option) == MenuOptions.HAMILTONIAN_CYCLE: 
		hamiltonian_cycle_prompt(network)
	else: 
		print("Wyjście z programu\n")
		sys.exit(0)


