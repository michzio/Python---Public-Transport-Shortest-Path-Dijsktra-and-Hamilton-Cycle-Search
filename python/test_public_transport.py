#-*-coding:utf-8-*-
import unittest
from public_transport import *

class TestPublicTransport(unittest.TestCase): 

    def setUp(self): 
        self.network = TransportNetwork.load_from_file("rozklad2.txt")
    
    def test_load_from_file(self): 
        """ Testowanie czy sieć komunikacyjna została wczytana."""
        self.assertNotEqual(self.network, None)

    def test_str(self):
        self.assertTrue(len(str(self.network)) > 0)
    
    def test_number_of_stops(self): 
        N = TransportNetwork() 
        N.add_stop(Stop("Ruczaj"))
        N.add_stop(Stop("Chmieleniec"))
        N.add_stop(Stop("Czerwone Maki"))
        self.assertTrue( N.number_of_stops() == 3)

    def test_stop(self): 
        self.assertEqual(Stop("Ruczaj"), Stop("Ruczaj"))
        self.assertEqual(str(Stop("Ruczaj")), str(Stop("Ruczaj")))

    def test_connection(self): 
        C = Connection(18, Stop("Kampus UJ"), 1, Stop("Ruczaj")) 
        self.assertEqual(C.line_number(), 18) 
        self.assertEqual(C.time(), 1) 
        self.assertEqual(C.start_stop(), Stop("Kampus UJ"))
        self.assertEqual(C.end_stop(), Stop("Ruczaj"))

    def test_number_of_connections(self): 
        N = TransportNetwork()
        N.add_connection(Connection(18, Stop("Chmieleniec"), 2, Stop("Kampus UJ")))
        N.add_connection(Connection(18, Stop("Czerwone Maki"), 1, Stop("Chmieleniec")))
        N.add_connection(Connection(18, Stop("Kampus UJ"), 1,  Stop("Ruczaj")))
        self.assertTrue(N.number_of_connections() == 3)
        self.assertTrue(N.number_of_stops() == 4)

    def test_add_stop(self): 
        self.network.add_stop(Stop("Norymberska"))
        self.assertTrue(Stop("Norymberska") in self.network.get_stops())

    def test_add_connection(self): 
        self.network.add_connection(Connection(179, Stop("Łagiewniki"), 2, Stop("Tischnera")))
        self.assertTrue(Connection(179, Stop("Łagiewniki"), 2, Stop("Tischnera")) 
            in self.network.get_connections_from(Stop("Łagiewniki")))

    def test_add_undirected_connection(self): 
        self.network.add_undirected_connection(179, Stop("Tischnera"), Stop("Powstańsców Śląskich"), 4)
        self.assertTrue(Connection(179, Stop("Tischnera"), 4, Stop("Powstańsców Śląskich")) 
            in self.network.get_connections_from(Stop("Tischnera")))
        self.assertTrue(Connection(179, Stop("Powstańsców Śląskich"), 4, Stop("Tischnera")) 
            in self.network.get_connections_from(Stop("Powstańsców Śląskich")))

    def test_travel_time(self): 
        self.network.add_undirected_connection(179, Stop("Tischnera"), Stop("Powstańsców Śląskich"), 4)
        self.assertEqual(self.network.travel_time_between(Stop("Powstańsców Śląskich"), Stop("Tischnera")), 4) 

    def tearDown(self): 
        return 

if __name__ == "__main__":
    unittest.main()
