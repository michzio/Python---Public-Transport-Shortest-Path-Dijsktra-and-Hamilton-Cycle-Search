/**
 * Dane wejściowe do programu: 
 * <ilosc lini>
 * <linia_1> 
 * <nazwa_przystanku>, <czas_przejazdu>, <nazwa_przystanku>, <czas_przejazdu>, ... <nazwa_przystanku>
 * ....
 * <linia_n>
 * <nazwa_przystanku>, <czas_przejazdu>, <nazwa_przystanku>, <czas_przejazdu>, ... <nazwa_przystanku>
 **/

#include<cstdio> 
#include<cstring>
#include<cstdlib>
#include<string> 
#include<unordered_map>
#include<set>
#include<algorithm>
#include<iostream>
#include<stack>
#include<vector>
#include<list>
#include<cmath>
#include<ctime>


#define DEBBUG 1 
#define DEBBUG_2 0
//domyślny rozmiar buforu
#define BUFF_SIZE 128

#define INFINITE std::numeric_limits<int>::max()

//zamienia 2 elementy w tablicy
void swap(int *elem1, int *elem2)
{
    int tmp = *elem1;
    *elem1 = *elem2;
    *elem2 = tmp;
}

struct Vertex;

/**
 * krawędzie będą elementami list sąsiedztwa przyporządkowanym
 * poszczególnym wierzchołkom. pozwala to na przechowywanie 
 * sąsiadującego wierzchołka wraz z wagą krawędzi prowadzącej do tego wierzchołka
 **/
struct Edge
{
private:
    Vertex *startStop;
    Vertex *endStop;
public:
    //numer linii komunikacji miejskiej
    int lineNo;
    //czas przejazdu
    int time;
    //wskaźnik na kolejny element listy sąsiedztwa
    Edge *next = 0;
    //konstruktor krawędzi
    Edge(int lno, Vertex *start, int t, Vertex *end)
            : lineNo(lno), startStop(start), time(t), endStop(end) { }
    /**
     * zwraca wierzchołek do którego prowadzi bieżąca krawędź
     **/
    Vertex *getEndVertex() const
    {
        return endStop;
    }
    
    Vertex *getStartVertex() const
    {
        return startStop;
    }
};

struct Vertex
{
    //indeks wierzchołka (liczbowy)
    int index;
    //etykieta wierzchołka (nazwa przystanku)
    std::string label;
    //lista sąsiedztwa wierzchołka
    Edge *adjacentVertices = 0;
   
    //konstruktor wierzchołka
    Vertex(int idx, std::string lbl) : index(idx), label(lbl) { }
    
    /**
     * dodawanie wierzchołka sąsiadującego (w postaci krawędzi łączącej wierzchołki)
     * do listy sąsiedztwa bieżącego wierzchołka. 
     * nowy wierzchołek przyległy wstawiamy zawsze na początku listy -> czas O(n)
     * można też wstawiać na początek listy nowe wierzchołki (krawędzie) to O(1)
     **/
    void addAdjacentVertex(Edge *edgeToVertex) {
     
        //gdy lista sąsiedztwa jest aktualnie pusta
        if(adjacentVertices == NULL)
        {
            adjacentVertices = edgeToVertex;
            return;
        }
        
        Edge *tmp;
        //ustawiamy wskaźnik tmp na ostatni element na liście sąsiedztwa
        for(tmp = adjacentVertices; tmp->next != NULL; tmp = tmp->next);
        
        //wstawienie nowego elementu (krawędzi prowadzącej do sąsiedniego wierzchołka)
        tmp->next = edgeToVertex;
        
    }
    
    void addAdjacentVertex(int lineNo, Vertex *start, int time, Vertex *end)
    {
        Edge *e = new Edge(lineNo, start, time, end);
        
        addAdjacentVertex(e);
    }
    
    /**
     * metoda która przeszukuje listę sąsiedztwa wierzchołka A w celu 
     * odnalezienia bezpośredniej krawędzi do wierzchołka B
     **/
    Edge *directEdge(Vertex *B)
    {
        //1. przechodzimy krawędzi poszukując wierzchołka B
        for(Edge *e = adjacentVertices; e!= NULL; e = e->next)
        {
            //odnaleźliśmy bezpośrednią krawędź z A do B
            if(e->getEndVertex()->index == B->index)
                return e;
        }
        
        //2. zwracamy NULL tj. przeszliśmy całą listę sąsiedztwa i nie
        //   znaleźliśmy B
        return NULL;
    }
    
    //przeciążenie operatora <
    bool operator< (Vertex const &q) const { return index < q.index; }
};

class Dijkstra;

class Graph
{
protected:
    //liczba wierzchołków grafu (przystanków)
    int noOfVertices = 0;
    
    //lista sąsiedztwa grafu - tablica unikalnych wierzchołków
    //z których każdy posiada listę krawędzi do bezpośrednio sąsiadujących wierzchołków
    
    //tablicę reprezentująca Listę Sąsiedztwa Grafu
    //początkowo alokowana jako pusta tablica
    //będzie realokowana metodą resize() podczas próby dodanie nowego
    //wierzchołka
    Vertex **adjacencyList = 0;
    
    //obiekt implementujący algorytm Dijkstry
    //przydatny przy znajdowaniu najkrótszej ścieżki pomiędzy
    //dwoma wskazanymi wierzchołkami (przystankami) -> rozwiazanie podpunktu 1) zadania
    Dijkstra *dijkstra = 0;
    
public:
    
    Graph() { };
    Graph(Vertex **list, int n) : adjacencyList(list), noOfVertices(n) {}
    
    void printAdjacencyList(void);
    //metoda używająca algorytmu Dijkstry znajduja najkrótsze połączenie
    int findMinPath(Vertex *a, Vertex *b, std::string *path);
    //obieranie wierzchołka grafu na podstawie numeru
    Vertex *getVertex(int i) const;
    int size(void) const;
    
    const Edge *getAdjacencyList(int vertexIdx);
    
     //metody pomocnicze w konstrukcji grafu na podstawie pliku tekstowego
    static int readLine(char * &buffer, size_t n, FILE *pFile);
    static char *trim(char *cstring);
    
protected:
    virtual bool loadGraphFromFile(FILE *pFile) = 0;
};

const Edge *Graph::getAdjacencyList(int vertexIdx)
{
    return adjacencyList[vertexIdx-1]->adjacentVertices;
}

int Graph::size(void) const
{
    return noOfVertices; 
}

char *Graph::trim(char *cstring)
{
    
    while(isspace(*cstring)) cstring++;
    
    char *end = cstring + strlen(cstring);
    while(isspace(*--end));
    *++end = '\0';
    
    return cstring;
}

/**
 * Implementacja kolejki priorytetowej na bazie std::set<> z C++ STL
 * wykorzystywana w algorytmie Dijkstry
 **/
class PriorityQueue
{
    //tablica priorytetów - mapowanie:
    //Vertex->index  ---> index tablicy ----> priorytet wierzchołka
    int size;
    static int *priorities;
    
    //funktor - komparator elementów w zbiorze (std:set<>)
    //ustawia elementy zgodnie z określonym priorytetem w kolejce priorytetowej
    
    //komparator porządkujący elementy od tego o najmniejszym priorytecie do
    //tego z największym priorytetem
    struct minPriorityComp {
        bool operator() (const Vertex *v1, const Vertex *v2) const
        {
            return priorities[v1->index-1] < priorities[v2->index-1];
        }
    };
    
    //funktor zwracający true gdy napotka w kolejce priorytetowej
    //wierzchołek grafu o danym indeksie
    struct vertexIndexEqual {
        vertexIndexEqual(int idx) : index(idx) {}
        bool operator() (const Vertex *vert) {
            return vert->index == index;
        }
        private:
        int index;
    };
    
    //analogiczny funktor zwracający true gdy napotka w kolejce priorytetowej
    //wierzchołek grafu o danej etykiecie (label)
    struct vertexLabelEqual {
        vertexLabelEqual(std::string l) : label(l) {}
        bool operator() (const Vertex *vert) {
            return vert->label == label;
        }
        private:
        std::string label;
    };
    
    //STRUKTURA DANYCH W KTÓREJ IMPLEMENTUJEMY KOLEJKE PRIORYTETOWĄ
    //wstawianie do kolejki -> O(logn)
    //zdejmowanie pierwszego elementu kolejki - O(1)
    //aktualizacja priorytetu elementu w kolejce - O(n) //uzycie find_if()
    std::multiset<const Vertex *, minPriorityComp> queue;
    
public:
    //konstruktor kolejki priorytetowej
    PriorityQueue(int s) : size(s) {
        priorities = (int *) malloc(sizeof(int) *size);
    }
    ~PriorityQueue() {}
    
    //metoda wstawiająca wierzchołek do kolejki
    void push(const Vertex *v, int priority);
    //metoda zdejmująca pierwszy element z kolejki
    const Vertex *pop(void);
    //metoda zmieniająca priorytet elementu (wierzchołka) w kolejce
    void changePriority(const Vertex *v, int priority);
    //metoda zmniejszajaca prioryte elementu (wierzchołka) w kolejce
    void decreasePriority(const Vertex *v, int decrement = 1);
    //metoda sprawdzająca czy kolejka jest pusta
    bool empty(void);
    //wypisanie zawartości kolejki priorytetowej
    void print(void);
};

//inicjalizacja statycznej zmiennej składowej - tablica priorytetów
int *PriorityQueue::priorities = 0;

/**
 * Metoda wstawia element do kolejki. Złożoność O(log(n))
 **/
void PriorityQueue::push(const Vertex *v, int priority)
{
    //zapisanie priorytetu w tablicy & dodanie wierzchołka do kolejki
    priorities[v->index -1] = priority;
    queue.insert(v);
}

/**
 * Metoda pobiera pierwszy element z kolejki o najmniejszym priorytecie
 * Złożoność 0(1)
 **/
const Vertex *PriorityQueue::pop(void)
{
    auto iterator = queue.begin();
    const Vertex *vertex = *iterator;
    queue.erase(iterator);
    
    return vertex;
}

/**
 * Metoda aktualizująca priorytet elementu w kolejce. Złożoność O(n)
 **/
void PriorityQueue::changePriority(const Vertex *v, int priority)
{
    //usunięcie elementu z kolejki jezeli istnieje
    //auto it = queue.find(v); //O(log(n))
    auto it = std::find_if(queue.begin(), queue.end(), vertexIndexEqual(v->index));  //O(n)
    if(it != queue.cend()) {
        if(DEBBUG_2)
            std::cout << "odnaleziono " << (*it)->label << std::endl;
        
        //usunięcie starego elementu
        queue.erase(it); //O(1)
        if(DEBBUG_2) {
            std::cout << "po usunięciu " << v->label << " mamy "; print();
        }
    }
    
    //wstawienie elementu z nowym priorytetem
    push(v, priority);
    
    if(DEBBUG_2) {
        std::cout << "po dodaniu " << v->label << " mamy "; print();
    }
}

/** 
 * Metoda dekrementująca priorytet kolejki o 1  lub podana wartosc
 * jako drugi argument... podajac wartość ujemna staje sie metoda inkrementujaca
 **/
void PriorityQueue::decreasePriority(const Vertex *v, int decrement)
{
    //pobieramy aktualny priorytet wskazanego wierzchołka v
    int currentPriority = priorities[v->index-1];
    //zmieniamy priorytet tego elementu w kolejce aktualicjąc o decrement
    changePriority(v, currentPriority - decrement);
}

/**
 * Metoda określajaca czy kolejka priorytetowa jest pusta
 **/
bool PriorityQueue::empty()
{
    return queue.size() == 0;
}

/**
 * klasa implementująca algorytm Dijkstry 
 * wyszukuje najkrótsze ścieżki między dwoma wierzchołkami
 **/
class Dijkstra
{
    //graf na którym algorytm Dijkstry wyszukuje minimalnych ścieżek
    const Graph *graph;
    
    //tablica z czasami przejazdów od przystanku początkowego (wierzchołek źródłowy)
    //do wszystkich innych i-tych wierzchołków grafu idąc najkrótszą ścieżką
    //odległości obliczane są jako sumy czasów przejazdu odcinków ( krawędzi )
    //pomiędzy wierzchołkami znajdującymi się na najkrótszej ścieżce z SOURCE do
    //i-tego wierzchołka
    //alokowane w zależności od wielkości grafu i inicjalizowane czasem niskończonym
    //oznaczającym brak połączenia pomiędzy przystankami
    int *travelTime;
    
    //tablica bezpośrednich krawędzi poprzedzających przystanek końcowy
    //najkrótszych ścieżek pomiędzy przystankiem poczatkowym a
    //i-tym przystankiem
    const Edge **predecessorEdge;
    
    //bieżący wierzchołek źródłowy (przystanek początkowy) - stosowany
    //do wyszukiwania ścieżek, jest ustawiany przez funkcje minPathsFrom(Vertex *src)
    const Vertex *source = 0;
    
public:
    //konstruktor
    Dijkstra(const Graph *g) : graph(g) {
        //alokowanie tablic pomocniczych  dla algorytmu Dijkstry
        travelTime = (int *) malloc(sizeof(int) * (graph->size()));
        predecessorEdge = (const Edge **) malloc(sizeof(Edge **) * (graph->size()) );
    }
    //destruktor
    ~Dijkstra();
    
    //pobranie minimalnej ścieżki na podstawie ostatnich obliczeń między (src, dest)
    int minPath(const Vertex *src, const Vertex *dest, std::list<const Edge *> &edgePath);
private:
    //metoda wyszukuje najkrótszą ścieżkę na podstawie obliczeń pomiędzy (src, dest)
    int findMinPathsFrom(const Vertex *src);
    //metoda inicjalizujaca tablice wykorzystywane przez algorytm Dijkstry
    void init(void);
};

/**
 * Metoda wypisuje aktualny graf reprezentowany w postaci listy sąsiedztwa
 **/
void Graph::printAdjacencyList(void)
{
    printf("\n Wypisanie listy sąsiedztwa: \n");
    
    for(int i=0; i<noOfVertices; i++)
    {
        printf(" %d:%s | ", adjacencyList[i]->index,
                           adjacencyList[i]->label.c_str());
        
        for(Edge *e = adjacencyList[i]->adjacentVertices; e != NULL; e = e->next)
        {
            printf("%d:%s (%d), ", e->getEndVertex()->index,
                                e->getEndVertex()->label.c_str(),
                                e->time);
            
        }
        
        printf("\n");
    }
    
    printf("\n");
}

/**
 * Metoda pomocnicza umożliwiająca wczytywanie grafu z pliku
 **/
int Graph::readLine(char * &buffer, size_t n, FILE *pFile)
{
    
    //inicjalizacja pozycji w buforze na początek bufora
    int pos = 0;
    int curr_size = n;
    //zmienna do której odczytujemy poszczególne znaki z pliku
    int ch;
    
    ch = getc(pFile);
    
    //czytamy każdą linijkę aż napotkamy znak specjalny nowej linii '\n'
    while( (ch != '\n') && (ch != EOF) ) {
        if(pos == curr_size)
        {
            if(DEBBUG)
                printf("Zapełniono buffor podczas wczytywania linii tekstu, pozycja: %d. Realokacja...\n", pos);
            curr_size *= 2; //podwojenie bieżącego rozmiaru buffora

            char *tmp = (char *) realloc(buffer, sizeof(char) * (curr_size + 1));
            
            if( tmp == NULL )
            {
                perror("Błąd podczas realokowania pamięci buffora.");
                exit(1);
            } else {
                buffer = tmp;
            }
        }
        
        buffer[pos++] = ch;
        
        ch = getc(pFile);
    }
    
    //dodanie znaku końca stringu
    buffer[pos] = '\0';
    
    //zwracamy liczbę odczytanych znaków -> pusty ciąg to 0
    return pos;
}

Vertex *Graph::getVertex(int index) const
{
    if( index > noOfVertices || index < 1)
    {
        printf("Podano wierzchołek spoza zakresu dostępnych wierzchołków. \n");
        return NULL;
    }
    return adjacencyList[index-1];
}

/**
 * Klasa reprezentujaca graf sieci komunikacji miejskiej 
 * z wierzchołkami reprezentowanymi jako przystanki.
 **/
class StopsGraph : public Graph
{
    std::unordered_map<std::string, int> labelToIndexMap;

public:
    StopsGraph(Vertex **adjacencyList, int noOfVertices);
    StopsGraph(FILE *pFile);
    ~StopsGraph();
    
    Vertex *getVertex(std::string label);
    
    //metoda pobierająca podgraf o wskazanych przystankach
    StopsGraph *getSubGraph(char **stops, int noOfStops);
    
    //metoda zwracająca minimalna wagę krawędzi (czas przejazdu) pomiędzy dwoma
    //wskazanymi przystankami lub INFINITE
    int getEdgeTravelTimeBetween(int v1, int v2);
    
private:
    //implementacja wczytująca graf dostarczony jako siatkę linii autobusowych
    bool loadGraphFromFile(FILE *pFile);
    //metoda przetwarzająca wczytaną listę przystanków i czasów przejazdów
    bool processLineDetails(int lineNo, char *stopsList);
    //metoda pomocnicza dodająca połączenie między przystankami do grafu
    bool addConnection(int lineNo, std::string startStop, int time, std::string endStop);
    //metoda  realokująca tablicę listy sąsiedztwa
    void resizeGraph();
};

/**
 * konstruktor klasy dziedziczącej po Graph specyficznej dla sieci linii autobusowych
 **/
StopsGraph::StopsGraph(FILE *pFile)
{
    if(DEBBUG)
        printf("Konstruktor StopsGraph\n");
    
    //wywołanie metody wczytującej graf z pliku specyficznej dla sieci linii autobusowej
    loadGraphFromFile(pFile);
}

/** 
 * Destruktor klasy StopsGraph
 **/
StopsGraph::~StopsGraph()
{
    if(DEBBUG)
        printf("Destrukcja obiektu klasy StopsGraph\n");
}

/**
 * Metoda zwracająca wybrany wierzchołek grafu (przystanek sieci komunikacyjnej)
 * o wskazanej etykiecie.
 **/
Vertex *StopsGraph::getVertex(std::string label)
{
    auto iterator = labelToIndexMap.find(label);
    if(iterator == labelToIndexMap.end() )
    {
        if(DEBBUG)
            printf("Nie odnaleziono wierzchołka o etykiecie: %s...\n",
                   label.c_str());
        return NULL;
    }
    
    int vertexIndex = iterator->second;
    
    return adjacencyList[vertexIndex-1];
}

/**
 * Metoda wczytuje siatke lini autobusowych dostarczona w nastepujacej postaci:
 * <ilosc lini>
 * <linia_1>
 * <nazwa_przystanku>, <czas_przejazdu>, <nazwa_przystanku>, <czas_przejazdu>, ... <nazwa_przystanku>
 * ....
 * <linia_n>
 * <nazwa_przystanku>, <czas_przejazdu>, <nazwa_przystanku>, <czas_przejazdu>, ... <nazwa_przystanku>
 **/
bool StopsGraph::loadGraphFromFile(FILE *pFile)
{
    //buffor do którego odczytujemy poszczególne linijki tekstu
    char *buffer;
    
    //alokowanie buffora, gdyby był za mały na wczytanie linijki tekstu to realokowany
    if( !(buffer = (char *) malloc(sizeof(char) * (BUFF_SIZE +1))) )
    {
        perror("Błąd podczas alokowania buffora pamięci.");
        return false;
    }
    
    if(DEBBUG)
        printf("Wczytywanie siatki linii autobusowych z pliku...\n");
    
    
    //1. Wczytywanie liczby linii komunikacji miejskiej
    int noOfLines = 0;
    
    if(readLine(buffer, BUFF_SIZE, pFile) < 1)
    {
        printf("Błąd podczas odczytywania liczby linii komunikacji miejskiej.\n");
        return false;
    }
    
    noOfLines = atoi(buffer);
    
    if(DEBBUG)
        printf("Liczba linii komunikacji miejskiej: %d\n", noOfLines);
    
    //wczytywanie danych dotyczących poszczególnych linii komunikacyjnych
    for(int i = 0; i<noOfLines; i++) {
        
        //wczytujemy numer linii
        if(readLine(buffer, BUFF_SIZE, pFile) < 1) {
            printf("Błąd podczas wczytywania %d-tego numeru linii...\n", i);
            return false;
        }
        
        int lineNo = atoi(buffer);
        
        //wczytanie listy przystanków oraz czasów przejazdów pomiędzy liniami
        if(readLine(buffer, BUFF_SIZE, pFile) < 1) {
            printf("Błąd podczas wczytywania listy przystanków linii: %d\n", lineNo);
            return false;
        }
        
        if(DEBBUG)
            printf("Lista przystanków: %s\n", buffer);
        
        //przetwarzanie danych dotyczących poszczególnych linii komunikacji miejskiej
        if(!processLineDetails(lineNo, buffer))
        {
            printf("Błąd podczas przetwarzania listy przystanków linii: %d\n", lineNo);
            return false;
        }
    }
    
    
    return true;
}

/**
 * Metoda pomocnicza która przetwarza listę przystanków i czasów przejazdów
 * @stopsList wczytaną dla linii komunikacji miejskiej o numerze @lineNo 
 * Zwraca true lub false w zależności od powodzenia procesu wczytywania
 **/
bool StopsGraph::processLineDetails(int lineNo, char *stopsList)
{
    //pomocnicze zmienne do których odczytywane są kolejne przystanki i czasy przejazdu
    std::string startStop;
    int time;
    std::string endStop;
    
    //zminna pomocnicza przy przetwarzaniu listy przystanków
    char *token;
    
    //1. wczytanie początkowego przystanku
    token = std::strtok(stopsList, ",");
    
    //2. wczytywanie kolejnych połączeń (czas, przystanek końcowy) pomiędzy przystankami
    while(token != NULL ) {
        
        //początkowy przystanek <- dotychczasowy końcowy
        startStop = trim(token);
        
        //czas przejazdu
        token = std::strtok(NULL, ",");
        if(token == NULL) //nie ma wiecej przystanków kończymy pętle
            break;
        
        time = atoi(token);
        
        //końcowy przystanek
        token = std::strtok(NULL, ",");
        
        if(token == NULL) {
            printf("Wykryto swobodną krawędź (nie ma wierzchołka końcowego)!");
            return false;
        }
        endStop = trim(token);
        
        //dodanie połączenia pomiędzy przystankami do grafu
        if(!addConnection(lineNo, startStop, time, endStop)) {
            printf("Dodawanie krawędzi połączenia między przystankami zakończone niepowodzeniem!");
            return false;
        }
    }
    
    return true;
}

/**
 * Metoda pomocnicza która dodaje nowe połączenie do grafu 
 * Krawedź pomiędzy przystankiem startStop, endStop o czasie przejazdu time lini o numerze lineNo
 **/
bool StopsGraph::addConnection(int lineNo, std::string startStop, int time, std::string endStop)
{
    printf("Dodawanie połączenia: %d (%s)-%d-(%s)\n", lineNo, startStop.c_str(), time, endStop.c_str());
    
    //1. sprawdzenie czy przystanki startStop i endStop istnieją już w grafie:
    //   - przypisanie nazwy przystanku do numeru wierzchołka
    //   - dodanie nowego wierzchołka do grafu (do tablicy z lista sasiedztwa)
    if(labelToIndexMap.find(startStop) == labelToIndexMap.end()) {
        
        //pobranie numeru indeksu dla nowego wierzchołka grafu
        int startStopIdx = ++noOfVertices;
        
        //brak mapowania nazwy przystanku na numer wierzchołka -> dodanie mapowania
        labelToIndexMap[startStop] = startStopIdx;
        
        //realokowanie rozmiaru tablicy przechowującej Listę Sąsiedztwa Grafu
        resizeGraph();
        
        //dodanie nowego wierzchołka do tablicy z Listą Sąsiedztwa Grafu
        adjacencyList[startStopIdx-1] = new Vertex(startStopIdx, startStop);
    }
    if(labelToIndexMap.find(endStop) == labelToIndexMap.end()) {
        //pobranie numeru indeksu dla nowego wierzchołka grafu
        int endStopIdx = ++noOfVertices;
        
        //brak mapowania nazwy przystanku do numeru wierzchołka -> dodanie mapowania
        labelToIndexMap[endStop] = endStopIdx;
        
        //realokowanie rozmiaru tablicy przechowującej Listę Sąsiedztwa Grafu
        resizeGraph();
        
        //dodanie nowego wierzchołka do tablicy z Listą Sąsiedztwa Grafu
        adjacencyList[endStopIdx-1] = new Vertex(endStopIdx, endStop);
    }
    
    //2. wstawienie do listy sąsiedztwa dwóch połączeń (Edge)
    //   startStop -> krawędź do endStop
    //   endStop -> krawędź do startStop
    adjacencyList[labelToIndexMap[startStop] -1]
        ->addAdjacentVertex( //utworzenie krawędzi do wierzchołka sąsiada
                lineNo,
                         adjacencyList[labelToIndexMap[startStop]-1],
                         time,
                         adjacencyList[labelToIndexMap[endStop]-1]
                         );
    
    
    adjacencyList[labelToIndexMap[endStop] -1]
        ->addAdjacentVertex( //utworzenie krawędzi do wiezchołka sąsiada
                            lineNo,
                            adjacencyList[labelToIndexMap[endStop]-1],
                            time,
                            adjacencyList[labelToIndexMap[startStop]-1]
                          );
    return true;
}

/**
 * Metoda zwiększająca, realokująca rozmiar tablicy w której przechowywana jest 
 * reprezentacja grafu w postaci listy sąsiedztwa
 **/
void StopsGraph::resizeGraph()
{
    if(DEBBUG)
        printf("Realokacja tablicy z reprezentacją grafu w postaci listy sąsiedztwa...\n");
    
    Vertex **tmp = (Vertex **) realloc(adjacencyList, noOfVertices * sizeof(Vertex *));
    
    if(tmp != NULL) {
        adjacencyList = tmp;
    } else {
        printf("Błąd podczas realokowania tablicy...\n");
        return;
    }
}

/**
 * Metoda pozwala na zainicjalizowanie tablic pomocniczych algorytmu Dijkstry
 **/
void Dijkstra::init(void)
{
    for(int i =0; i < graph->size(); i++) {
        //wypełnienie tablic najkrótszych czasów przejazdów z wierzchołka
        //źródłowego do i-tego wierzchołka grafu wartościami INFINITE
        //oznaczającymi czas nieskończony
        //podczas wykonywania algorytmu wartości tablic będą porównywane i
        //nadpisywane tymi odpowiadającymi krótszym drogą z SOURCE do i-tego
        //wierzchołka
        travelTime[i] = INFINITE;
        
        //analogicznie ustawiamy na NULL wszystkie krawędzie poprzedzające
        predecessorEdge[i] = NULL;
    }
    
    source = NULL;
}

/**
 * Metoda wykonująca główny algorytm Dijkstry wyszukujący najkrótsze ścieżki
 * z wierzchołka źródłowego SOURCE do pozostałych wierzchołków
 **/
int Dijkstra::findMinPathsFrom(const Vertex *src)
{
    if(source != NULL && src->index == source->index) {
        //jeżeli metoda została już wykonana dla bieżącego
        //wierzchołka źródłowego SOURCE to nie ma potrzeby
        //wykonywać jej ponownie
        return 1;
    }
    
    //1. inicjalizowanie tablic pomocniczych
    //   travelTime[i] <- INFINITY
    //   predecessor[i] <- NULL
    //   predecessorEdge[i] <- NULL
    //   source = NULL
    init();
    
    //2. przypisanie wierzchołka źródłowego
    source = src;
    
    //3. inicjalizacja czasu przejazdu z src do src = 0
    travelTime[source->index -1] = 0;
    
    //4. utworzenie zbioru (kolejki priorytetowej) wszystkich
    //   wierzchołków (przystanków) grafu
    PriorityQueue queue(graph->size());
    //umieszczenie pierwszego startowego wierzchołka (początkowego przystanku)
    //w kolejce priorytetowej z waga 0 oraz pozostałych z wagą infinity
    for(int i=0; i<graph->size(); i++)
    {
        queue.push(graph->getVertex(i+1), travelTime[i]);
    }
    
    const Vertex *v;
    
    //petla wykonywana dopuki beda elementy w kolejce priorytetowej
    //do kolejki dodawane sa za kazdym razem wierzchołki sąsiadujące z
    //z odpowiednim priorytetem
    while(!queue.empty()) {
        if(DEBBUG_2)
            queue.print();
        
        //pobranie wierzchołka z kolejki priorytetowej
        //o najmniejszym czasie przejazdu
        v = queue.pop();
        int vIdx = v->index -1;
        if(travelTime[vIdx] == INFINITE) {
            return 1;
              //wierzchołek niedostępny
              if(DEBBUG)
                  printf("INFINITY znalezione...\n");

              break;
        }
        
        
        //5. Pętla po wszystkich sąsiadach wierzchołka v
        for(Edge *e = v->adjacentVertices; e != NULL; e = e->next)
        {
            int wIdx = e->getEndVertex()->index -1;
            
            if(DEBBUG)
                printf("Relaksacja krawędzi (%d:%s, %d:%s)\n",
                       v->index, v->label.c_str(),
                       //e->getStartVertex()->index, e->getStartVertex()->label.c_str(),
                       e->getEndVertex()->index, e->getEndVertex()->label.c_str());
            
            //6. Relaksacja krawędzi (v, w)
            if( travelTime[wIdx] > travelTime[vIdx] + e->time)
            {
                //nadpisanie czasu przejazdu dla najkrótszej ścieżki
                //oraz poprzednika wierzchołka wIdx i krawędzi poprzedzającej wIdx
                travelTime[wIdx] = travelTime[vIdx] + e->time;
    
                predecessorEdge[wIdx] = e;
                
                //aktualizacja priorytetu wierzchołka sąsiada (w)
                queue.changePriority(e->getEndVertex(), travelTime[wIdx]);
                
                if(DEBBUG) {
                    printf("Nowa waga: %d\n", travelTime[wIdx]);
                }
            }
        }
        
    }
    
    //zwraca 1 w przypadku sukcesu
    return 1;
}

int Dijkstra::minPath(const Vertex *src, const Vertex *dest, std::list<const Edge *> &edgePath)
{
    if(! (source != NULL && source->index == src->index) )
    {
        findMinPathsFrom(src);
    }
    
    if(travelTime[dest->index-1] == INFINITE)
    {
        return -1;
    }
    
    int lastVertexIndex = -1;
    
    for(const Edge *prev = predecessorEdge[dest->index-1];
        prev != NULL; prev = predecessorEdge[prev->getStartVertex()->index-1])
    {
        if(prev->getStartVertex()->index == dest->index || prev->getStartVertex()->index == lastVertexIndex) break;
        
        edgePath.push_front(prev);
        lastVertexIndex = prev->getStartVertex()->index;
    
    }

    
    //zwróć najkrótszą ścieżkę do wierzchołka dest
    return travelTime[dest->index-1];
}

int Graph::findMinPath(Vertex *a, Vertex *b, std::string *path)
{
    if(dijkstra == NULL)
    {
        //tworzenie obiektu implementującego algorytm Dijkstry
        //dla bieżącego grafu
        dijkstra = new Dijkstra(this);
    }
    
    std::list<const Edge*> edgePath;
    
    int travelTime = dijkstra->minPath(a, b, edgePath);
    
    if(edgePath.empty())
    {
        *path = "Brak połączenia pomiędzy przystankami...";
    } else {
        
        const Edge *e;
        
        for(auto it = edgePath.begin(); it != edgePath.end(); it++)
        {
            e = *it;
            *path += e->getStartVertex()->label + "-" + std::to_string(e->lineNo) + "->";
        
        }
        
        *path += e->getEndVertex()->label;
    
    }
    
    return travelTime;
}

/**
 * Klasa implementująca algorytm heurystyczny symulowanego wyrzażania
 **/
class SimulatedAnnealing
{
    static const int INITIAL_TEMPERATURE = 150000;
    static const int INITIAL_TEMPERATURE_LENGTH = 100;
    constexpr static const double COOLING_RATIO = 0.99;
    
    StopsGraph *graph;
    
public:
    SimulatedAnnealing(StopsGraph *G) : graph(G) {

        std::srand(std::time(NULL));
    }
    
    //metoda zwracająca optymalne (minimalny) cykl Hamiltona w grafie
    int optimalHamiltonianCycle(std::string &minCycle);
private:
    //metoda pomocnicza wykonująca algorytm
    int searchMinHamiltonianCycle(std::list<const Edge*> &edgeCycle);
    //metoda pomocnicza zwraca czas przejazdy dla danego cyklu
    int hamiltonianCycleTravelTime(const int *permutation, int n);
    //metoda generująca nowe rozwiązanie na podstawie starego
    int *generateNewSolution(const int *oldSolution, int n, int &vi, int &vj);
    //metoda oblicza zmianę czasu przejazdy między rozwiązaniami starym i nowym
    int changeOfTravelTime(int currentTravelTime, const int *newSolution, int n, int vi, int vj);
    void printPermutation(const int *permutation, int n);
    double probability(double deltaTime, double T);
};

/**
 * Funkcja obługująca interakcję z użytkownikiem przy wyszukiwaniu 
 * najkrótszej ścieżki pomiędzy wybranymi przystankami w sieci komunikacyjnej
 **/
void shortestPathPrompt(StopsGraph &G)
{
    char *startStop, *endStop;
    
    //alokowanie buffora, gdyby był za mały na wczytanie linijki tekstu to realokowany
    if( !(startStop = (char *) malloc(sizeof(char) * (BUFF_SIZE +1))) )
    {
        perror("Błąd podczas alokowania buffora pamięci.");
        return;
    }
    
    //alokowanie buffora, gdyby był za mały na wczytanie linijki tekstu to realokowany
    if( !(endStop = (char *) malloc(sizeof(char) * (BUFF_SIZE +1))) )
    {
        perror("Błąd podczas alokowania buffora pamięci.");
        return;
    }
    
    //pobranie przystanku początkowego i końcowego
    printf("Podaj przystanek początkowy: \n");
    getchar();
    Graph::readLine(startStop, BUFF_SIZE, stdin);
    printf("Podaj przystanek końcowy: \n");
    Graph::readLine(endStop, BUFF_SIZE, stdin);
    
    startStop = Graph::trim(startStop);
    endStop = Graph::trim(endStop);
    
    //pobranie numerów indeksów dla nazw przystanków
    Vertex *startVertex = G.getVertex(std::string(startStop));
    Vertex *endVertex = G.getVertex(std::string(endStop));
    
    //sprawdzenie poprawności wprowadzonych wierzchołków
    if(startVertex == NULL)
    {
        printf("Podano niepoprawny przystanek początkowy: %s\n", startStop);
        return;
    }
    
    if(endVertex == NULL)
    {
        printf("Podano niepoprawny przystanek końcowy: %s\n", endStop);
        return;
    }
    
    //wyszukiwanie najkrótszej ścieżki - algorytm Dijkstry
    printf("Wyszukiwanie najkrótszej ścieżki pomiędzy przystankami: %s - %s ...\n", startStop,
                                                                               endStop);

    std::string path;
    int minCost = G.findMinPath(startVertex, endVertex, &path);
    printf("Minimalny koszt: %d\n", minCost);
    printf("Trasa: %s\n", path.c_str());
}

/**
 * Funkcja obsługująca interakcję z użytkownikiem przy wyszukiwaniu 
 * najkrótszej ścieżki pomiędzy wybranymi przystankami w sieci komunikacyjnej
 **/
void hamiltonCyclePrompt(StopsGraph &G)
{
    //tablica c-stringów z nazwami przystanków dla których wyszukiwany będzie najkrótszy cykl
    char **stops;
    int noOfStops;
    
    printf("Podaj liczbę przystanków, które mam odwiedzić: \n");
    scanf("%d", &noOfStops);
    
    //alokowanie tablicy przystanków do odwiedzenia
    if( !(stops = (char **) malloc(sizeof(char *) * (noOfStops))) )
    {
        perror("Błąd podczas alokowania tablicy przystanków do odwiedzenia.");
        return;
    }
    
    getchar();
    //wczytywanie nazw (etykiet) kolejnych przystanków
    for(int i=0; i< noOfStops; i++)
    {
        //zaalokowanie przystanku i-tego
        if( !(stops[i] = (char *) malloc(sizeof(char) * (BUFF_SIZE +1))) )
        {
            perror("Błąd podczas alokowania c-stringa dla %d przystanku.");
            return;
        }
        
        printf("Podaj nazwę %d przystanku: \n", i+1);
        Graph::readLine(stops[i], BUFF_SIZE, stdin);
    }
    
    //wypisanie testowe nazw wczytanych przystanków:
    if(DEBBUG) {
        printf("\n\n");
        printf("Wczytano następujące przystanki: \n");
        for(int i=0; i< noOfStops; i++)
        {
            printf("%s\n", stops[i]);
        }
        printf("\n");
    }
    
    //pobranie podgrafu ograniczonego do wymienionych przestanków oraz krawędzi łączących te przystanki
    StopsGraph *subG = G.getSubGraph(stops, noOfStops);
    
    //testowe wypisanie list sąsiedztwa podgrafu
    subG->printAdjacencyList();
    
    
    //wyszukiwanie najkrótszego cyklu Hamiltona w grafie
    //stosując algorytm heurystyczny - Simulated Annealing (Symulowane Wyżarzanie)
    SimulatedAnnealing sa(subG);
    
    std::string minCycle;
    //pobranie optymalnego cyklu Hamiltona
    int minCost = sa.optimalHamiltonianCycle(minCycle);
    
    printf("Optymalny czas objazdu przystanków: %d\n", minCost);
    printf("Optymalna trasa objazdu: %s\n", minCycle.c_str());
}

enum MenuOption {
    SHORTEST_PATH = 1,
    HAMILTON_CYCLE = 2
};

/***************** 2 CZĘŚĆ PROGRAMU **********************************/

/**
 * Konstruktor tworzący graf reprezenujtący sieć komunikacji miejskiej 
 * z przystankami jako wierzchołkami i połączeniami jako krawędziami
 * Konstruktor tworzy graf na podstawie przekazanej tablicy: 
 * wskaźników na wierzchołki
 **/
StopsGraph::StopsGraph(Vertex **list, int n) : Graph(list, n)
{

    //wczytanie mapy label -> index
    for(int i=0; i< noOfVertices; i++)
    {
        labelToIndexMap[adjacencyList[i]->label] = adjacencyList[i]->index;
    }
}

/**
 * Metoda wyciągająca z grafu podgraf o wskazanych nazwach wierzchołków
 * Tworzy nowy obiekt grafu z tylko wybranymi wierzchołkam i odpowiadającymi 
 * im krawędziami. Jeżeli nie istnieje wierzchołek (przystanek) o zadanej 
 * etykiecie to funkcja zwraca komunikat błędu.
 **/
StopsGraph *StopsGraph::getSubGraph(char **stops, int noOfStops)
{
    //podgraf
    StopsGraph *subGraph;
    //pomocnicza mapa: nazwa przystanku -> index w nowej liscie sąsiedztwa
    std::unordered_map<std::string, int> newLabelToIndexMap;
    
    //1. tworzenie listy sąsiedztwa podgrafu
    Vertex **subAdjacencyList = (Vertex **) malloc(noOfStops *sizeof(Vertex *));
    if(!subAdjacencyList) {
        perror("Błąd podczas alokowania listy sąsiedztwa podgrafu...");
        return NULL;
    }
    
    //2. wypełnianie list sąsiedztwa podgrafu wierzchołkami i krawędziami
    for(int i=0; i < noOfStops; i++)
    {
        //odszukanie dla nazwy kolejnego przystanku jego indeksu w dotychczasowym
        //grafie sieci komunikacyjnej
        if(labelToIndexMap.find(stops[i]) == labelToIndexMap.end() ) {
    
            printf("Nie odnaleziono podanego przystanku w grafie: %s...\n", stops[i]);
            return NULL;
        }
        
        //dodanie wierzchołka do tablicy reprezentującej
        //listę sąsiedztwa podgrafu
        subAdjacencyList[i] = new Vertex(i+1, stops[i]);
        //dodanie mapowania nazwy na indeks na nowej liscie sąsiedztwa
        newLabelToIndexMap[stops[i]] = i+1;
    
    }
    
    //pomocnicza zmienna dla przeglądania list sąsiedztwa wierzchołków
    const Vertex *v;
    
    //3.Przekopiowanie krawędzi z grafu do podgrafu
    for(int i = 0; i< noOfStops; i++)
    {
        v = adjacencyList[labelToIndexMap[stops[i]]-1];
        
        if(DEBBUG)
            printf("Dodawanie sąsiadów: %s\n", v->label.c_str());
        
        for(const Edge * e= v->adjacentVertices; e != NULL; e = e->next)
        {
            printf("Czy dodać sąsiada: %s ? ", e->getEndVertex()->label.c_str());
            
            //sprawdzanie czy sąsiad wierzchoła v grafu należy do podgrafu
            if(newLabelToIndexMap.find(e->getEndVertex()->label)
               != newLabelToIndexMap.end()) {
                
                if(DEBBUG) printf("TAK\n");
                
                int newEndIdx = newLabelToIndexMap[e->getEndVertex()->label] -1;
                
                //jeżeli sąsiad należy do podgrafu to tworzymy nową krawędź
                //i dodajmey ją do podgrafu
                subAdjacencyList[i]->addAdjacentVertex(
                    new Edge(e->lineNo, subAdjacencyList[i], e->time, subAdjacencyList[ newEndIdx ])
                );
                
                if(DEBBUG) printf("Dodano krawędź %d: %d:%s - %d -> %d:%s\n",
                                  e->lineNo,
                                  subAdjacencyList[i]->index,
                                  subAdjacencyList[i]->label.c_str(),
                                  e->time,
                                  subAdjacencyList[newEndIdx]->index,
                                  subAdjacencyList[newEndIdx]->label.c_str());
               
            } else {
                if(DEBBUG) printf("NIE\n");
            }
        }
    }
    
    //4. alokowanie nowego podgrafu na podstawie utworzonej listy sąsiedztwa
    subGraph = new StopsGraph(subAdjacencyList, noOfStops);
    
    return subGraph;
}

int SimulatedAnnealing::optimalHamiltonianCycle(std::string &minCycle)
{
    std::list<const Edge*> edgeCycle;
    
    int bestTravelTime = searchMinHamiltonianCycle(edgeCycle);
    
    if(bestTravelTime == -1)
        return -1;
    
    //rekonstrukcja optymalnego cyklu hamiltona
    std::list<const Edge *>::iterator lastIt;
    for(auto it = edgeCycle.begin(); it != edgeCycle.end(); it++)
    {
        minCycle += (*it)->getStartVertex()->label + "-" + std::to_string((*it)->lineNo) + "->";
        lastIt = it;
    }
    
    minCycle += (*lastIt)->getEndVertex()->label;
    
    return bestTravelTime;
   
    
}

/**
 * Metoda implementuje algorytm wyżarzania
 **/
int SimulatedAnnealing::searchMinHamiltonianCycle(std::list<const Edge*> &edgeCycle)
{
    
    //1. PERMUTACJA POCZĄTKOWA
    //utworzenie tablicy której elementami są indeksy wierzchołków grafu
    int *permutation = new int[graph->size()];
    //wypełnienie tablicy kolejnymi liczbami naturalnymi
    for(int i=0; i < graph->size(); i++)
        permutation[i] = i+1;
    //wygenerowanie losowaej permutacji początkowej
    std::random_shuffle(permutation, permutation + graph->size());
    
    //wypisanie testowe permutacji początkowej
    if(DEBBUG) {
        printf("Wygenerowana początkowa permutacja wierzchołków: \n");
        for(int i=0; i < graph->size(); i++)
        {
            printf("%d ", permutation[i]);
        }
        printf("\n");
    }
    
    //2. INICJALIZACJA TEMPERATURY I POCZĄTOWEGO ROZWIĄZANIA
    //parametry sterujące algorytmem
    double T = INITIAL_TEMPERATURE;
    int temperatureLength = INITIAL_TEMPERATURE_LENGTH;
    //bieżące rozwiązanie
    int *currentSolution = permutation;
    int currentTravelTime = hamiltonianCycleTravelTime(currentSolution,
                                                       graph->size());
    //aktualne najlepsze rozwiązanie
    int *bestSolution = currentSolution;
    int bestTravelTime = currentTravelTime;
    
    //3. PĘTLA OBNIŻANIA TEMPERATURY (SCHŁADANIA)
    do {
        if(DEBBUG)
            printf("Bieżące najlepsze rozwiązanie to: %d\n",
                   hamiltonianCycleTravelTime(bestSolution, graph->size()));
        if(DEBBUG)
            printf("Liczba prób dla temperatury: %g wynosi: %d\n", T, temperatureLength);
        
        if(graph->size() < 4) break;
        
        //4. PĘTLA PRÓB WYGENEROWANIA NOWEGO ROZWIĄZANIA DLA DANEJ TEMPERATURY
        for(int i=0; i < temperatureLength; i++)
        {
            //generowanie nowego rozwiązania
            int vi =0, vj = 0;
            int *newSolution = generateNewSolution(currentSolution,
                                                   graph->size(), vi, vj);
            
            //obliczenie różnicy pomiędzy rozwiązaniami
            int deltaTime = changeOfTravelTime(currentTravelTime,
                                               newSolution, graph->size(),
                                               vi, vj);
            
            if(DEBBUG)
                printf("Różnica czasów między kolejnymi rozwiązaniami: %d\n", deltaTime);
            
            if( deltaTime <= 0)
            {
                //nowe rozwiązanie ma mniejszy całkowity czas przejazdu
                //lub 0 oznaczjące, że obydwie permutacje nie są cyklami
                //                      lub miały taki sam czas przejazdu
                
                //5.1 AKCEPTUJEMY NOWĄ PERMUTACJĘ
                currentSolution = newSolution;
                currentTravelTime += deltaTime;
                //ustawiamy nową permutacje jako najlepsze dotychczasowe rozwiązanie
                bestSolution = currentSolution;
                bestTravelTime = currentTravelTime;
                
                if(DEBBUG && deltaTime == 0)
                    printf("AKCEPTUJE NIEGORSZE ROZWIĄZANIE\n");
                if(DEBBUG && deltaTime < 0)
                    printf("AKCEPTUJE LEPSZE ROZWIĄZANIE\n");

            } else {
                //nowe rozwiązanie ma gorszy (dłuższy) całkowity czas przejazdu
                //lub nie jest cyklem -> INFINITE

                
                //5.2 AKCEPTUJEMY NOWĄ PERMUTACJĘ Z PEWNYM PRAWDOPODOBIEŃSTWEM
                //    im większe wydłużenie czasu przejazdu tym mniejsze
                //    prawdopodobieństwo, im większa temperatura tym większe
                //    prawdopodobieństwo
                
                //losowanie wartości z przedziału (0,1)
                double randNum = static_cast <double>(std::rand())/1.0;
                
                //sprawdzenie czy wylosowana wartość mieści się w wyznaczonym
                //prawdopodobieństwie akceptacji rozwiązania gorszego:
                if( randNum < probability(deltaTime, T) ) {
                    //AKCEPTUJEMY GORSZE ROZWIĄZANIE
                    
                    currentSolution = newSolution;
                    currentTravelTime += deltaTime;
                    //ale nie jest to najlepsze dotychczasowe rozwiązanie
                    
                    if(DEBBUG)
                        printf("AKCEPTUJE GORSZE ROZWIĄZANIE\n");
                }
                
            }
            
        }
        
        //schładzanie - obniżanie temperatury
        T *= COOLING_RATIO;
        
        //zwiększanie ilości prób dla niższych temperatur
        double lengthRatio = (INITIAL_TEMPERATURE/T > 5.0) ? 5.0 : INITIAL_TEMPERATURE/T;
        temperatureLength = INITIAL_TEMPERATURE_LENGTH *  lengthRatio;
        
    } while(T > 1); //wykonuj do momentu schłodzenia
    
    
    if(DEBBUG)
        printf("Najlepsze znalezione rozwiązanie to: %d\n", bestTravelTime);
    if(DEBBUG)
        printPermutation(bestSolution, graph->size());
    
    if(bestTravelTime == INFINITE)
        return -1;
    
    //rekonstrukcja cyklu hamiltona na podstawie permutacji
    for(int i = 0; i < graph->size(); i++)
    {
        const Edge *bestMatch = 0;
        
        for(const Edge *e = graph->getAdjacencyList(bestSolution[i]); e != NULL; e = e->next) {
            
            if(e->getEndVertex()->index == bestSolution[(i+1)%graph->size()])
            {
                if(bestMatch == NULL || bestMatch->time > e->time)
                    bestMatch = e;
            }
        }
        
        if(bestMatch == NULL) {
            if(DEBBUG) printf("Podczas rekonstrukcji cyklu nie odnaleziono krawędzi pomiędzy: %d i %d\n", bestSolution[i], bestSolution[(i+1)%graph->size()]);
            return -1;
        }
        
        edgeCycle.push_back(bestMatch);
        bestMatch = 0; 
    }
    
    return bestTravelTime;
}

/**
 * Metoda oblicza czas przejazdu dla wskazanego cyklu hamiltona
 * jeżeli taki cykl nie istnieje (brak jednej z krawędzi)
 * to czas przejazdu zmierza do nieskończoności INFINITE
 * parametry:
 * @permutation - cykl hamiltona w postaci permutacji wierzchołków
 * @n - liczba elementów tablicy reperezenutjącej permutację
 **/
int SimulatedAnnealing::hamiltonianCycleTravelTime(const int *permutation, int n)
{
    int cycleTravelTime = 0;
    
    int edgeTravelTime = 0;
    //pętla po elementach permutacji (indeksach wierzchołków)
    //dokonujemy sprawdzenia istnienia krawędzi pomiędzy kolejną
    //parą wierzchołków (przystanków) oraz pomiędzy parą uporządkowaną
    // (last, first), w przeciwnym wypadku cykl nie istnieje i
    //zwracamy czas objazdu równy nieskończności INFINITE
    for(int i=0; i<n; i++)
    {
        //dla każdej pary wywołujemy funkcję zwracającą czas przejazdu
        //dla krawędzi łączącej wybrane 2 wierzchołki grafu, jeżeli brak
        //takiej krawędzi to zwraca INFINITE
        edgeTravelTime = graph->getEdgeTravelTimeBetween(permutation[i%n], permutation[(i+1)%n]);
        
        //brak krawędzi
        if(edgeTravelTime == INFINITE)
            return INFINITE;
        cycleTravelTime += edgeTravelTime;
    }
    
    //permutacja nie jest cyklem Hamiltona
    return cycleTravelTime;
}

/**
 * Metoda zwraca czas dla najszybszego połączenia bezpośredniego
 * (w postaci pojedynczej krawędzi łączącej 2 wierzchołki) między 
 * przystankami o indeksach v1 i v2. 
 * Jeżeli w grafie taka krawędź nie istnieje to zwracane jest INFINITE
 **/
int StopsGraph::getEdgeTravelTimeBetween(int v1, int v2)
{
    int edgeTravelTime = INFINITE;
    
    //przeglądamy listę sąsiedztwa wierzchołka o indeksie v1
    for(const Edge *e = adjacencyList[v1-1]->adjacentVertices;
        e != NULL; e = e->next)
    {
        if(e->getEndVertex()->index == v2)
            if(edgeTravelTime > e->time)
                edgeTravelTime = e->time;
    }
    
    //zwracamy czas przejazdu
    return edgeTravelTime;
}

/**
 * Metoda generująca nowe rozwiązanie (nową permutację wierzchołków) na podstawie 
 * starej permutacji wierzchołków. 
 * Parametry: 
 * @oldSolution - dotychczasowe rozwiązanie 
 * @n - długość permutacji (tablicy indeksów wierzchołków)
 * @vi - indeks i-tego wierzchołka w permutacji - zwracane przez referencje
 * @vj - indeks j-tego wierzchołka w permutacji - zwracane przez referencje
 * Nowa permutacja generowane ze starej poprzez: 
 * 1. Losowe wygenerowanie indeksów vi oraz vj 
 * 2. Odpowiednią zamianę par (vi, vi+1) i (vj, vj+1) 
 *    tak by otrzymać pary (vi, vj) i (vi+1, vj+1) 
 * 3. Odwrócenie kolejności elementów permutacji w przedziale (vi+1,..., vj) -> (vj,..., vi+1)
 * return: nowa permutacja wierzchołków tj. nowe rozwiązanie
 **/
int *SimulatedAnnealing::generateNewSolution(const int *oldSolution, int n, int &vi, int &vj)
{
    //alokowanie tablicy na nową permutację
    int *newSolution = new int[n];
    
    //przekopiowanie elementów starej permutacji do nowej tablicy
    for(int i=0; i<n; i++)
    {
        newSolution[i] = oldSolution[i];
    }
    
    //1. Losowanie indeksów vi oraz vj takich, że vi < vj
    if(n < 4) return newSolution; //za mało wierzchołków by wylosować 2 pary
    
    //vi = std::rand() % (n-3);
    //vj = std::rand() % (n-(vi+2)-1) + (vi+2);
    vi = std::rand() % (n-2);
    do {
        vj = (std::rand() % (n-(vi+1)) + (vi+2) ) %n;
    } while(vi == vj || vi == ((vj+1)%n) );
    
    
    if(DEBBUG)
        printf("Wylosowano pary (vi, vi+1) = (%d, %d) oraz (vj, vj+1) = (%d, %d)\n",
               vi, (vi+1)%n, vj, (vj+1)%n);
    //2. Zamiana par (vi, vi+1) i (vj, vj+1)
    //                    1 ----> 2                                   1       2
    //                   /|         \                               /|  \   / |\
    //                  /            \|                            /     \ /     \
    //                 6              3     (1,2) <---> (4,5)     6      / \      3
    //                |\             /      (1,4)   i   (2,5)    |\     /   \    /|
    //                  \          |/                              \  |/      \|/
    //                    5 <---- 4                                  5         4
    // otrzymujemy pary (vi, vj) oraz parę (vi+1, vj+1)
    // czyli swap(vi+1, vj)
    
    swap(newSolution + ((vi+1) %n), newSolution + vj);
    
    //3. Odwrócenie kolejności elementów w przedziale (vi+1,...,vj) -> (vj,...,v+1)
    std::reverse( (newSolution + (vi+1)%n) + 1, (newSolution + vj));
    
    if(DEBBUG) {
        printPermutation(oldSolution, n);
        printPermutation(newSolution, n);
    }
    
    return newSolution;
}

void SimulatedAnnealing::printPermutation(const int *permutation, int n)
{
    for(int i=0; i<n; i++)
        printf("%d ", permutation[i]);
    printf("\n");
}

/**
 * Metoda oblicza zmianę czasu przejazdu porównując nową permutację 
 * ze starą permutacją. Możliwe przypadki to:
 * 1) dotychczasowe rozwiązanie nie było poprawnym cyklem 
 *    nowa permutacja może być poprawnym cyklem, ale nie musi 
 *    -> potrzebujemy na nowo wyznaczyć długość permutacji 
 * 2) dotychczasowa permutacja była poprawnym cyklem
 *    po zmianie nowa permutacje może pozostać poprawnym cyklem 
 *    lub może okazać się że nie jest ona cyklem
 **/
int SimulatedAnnealing::changeOfTravelTime(int currentTravelTime, const int *newSolution, int n, int vi, int vj)
{
    int deltaTime = 0;
    
    //1. GDY DOTYCHCZASOWA PERMUTACJA NIE BYŁA CYKLEM
    if(currentTravelTime == INFINITE)
    {
        //na nowo obliczamy czas dla nowej permutacji
        int newTravelTime = hamiltonianCycleTravelTime(newSolution, n);
        //obliczenie zmiany czasu, dwie opcje:
        //1) INFINITE - INFINITE = 0 - obie permutacje nie są cyklem
        //2) (delta = num - INFINITE) < 0 - nowa permutacja jest cyklem
        deltaTime = newTravelTime - currentTravelTime;
    }
    //2. GDY DOTYHCZASOWA PERMUTACJA BYŁA CYKLEM
    else
    {
        //wyznaczamy różnicę przyrostowo ze wzoru
        //delta = time(vi, vi+1) + time(vj, vj+1)
        //       - time(vi, vj) - time(vi+1, vj+1)
        //jeżeli którakolwiek z nowych krawędzi (vi, vi+1), (vj, vj+1)
        //nie istnieje to otrzymamy delta = INFINITE
        //stare krawędzie z założenia że dotychczasowa permutacja była cyklem
        int newEdgeI = graph->getEdgeTravelTimeBetween(newSolution[vi],
                                                newSolution[(vi+1)%n] );
        int newEdgeJ = graph->getEdgeTravelTimeBetween(newSolution[vj],
                                                newSolution[(vj+1)%n] );
        
        if(DEBBUG)
            printf("Długość nowych krawędzi to: %d i %d\n", newEdgeI, newEdgeJ);
        
        if(newEdgeI == INFINITE || newEdgeJ == INFINITE) {
            //1) INFINITE -> musimy odjąć czas dotychczasowego cyklu
            deltaTime = INFINITE - currentTravelTime;
        } else {
            //1) num -> poprawna różnica czasu pomiędzy dwoma cyklami
            deltaTime = newEdgeI + newEdgeJ
                    - graph->getEdgeTravelTimeBetween(newSolution[vi],
                                               newSolution[vj])
                    - graph->getEdgeTravelTimeBetween(newSolution[(vi+1)%n],
                                               newSolution[(vj+1)%n]);
        }
    }
    
    //1) delta = 0 - obie permutacje nie są cyklem
    //2) delta << 0 - nowa permutacja jest cyklem, krótsza czas, a stara nie
    //3) delta < 0 - obie permutacje są cyklami, nowa ma krótszy czas
    //4) delta > 0 - obie permutacje są cyklami, nowa ma dłuższy czas
    //5) delta >> 0 - nowa permutacja nie jest cyklem, a stara była
    return deltaTime;
}

/**
 * Oblicza prawdopodobieństwo z jakim nowa permutacja cyklu może zostać
 * zaakceptowana. Zależy od takich parametrów jak:
 * 1)różnica czasu przejazdu między nowym i starym cyklem
 *   wydłużenie czasu przejazdu daje deltaTime > 0 
 *   im większe wydłużenie czasu to mniejsze prawdopodobieństwo
 * 2) temperatura im wyższa tym większe prawdopodobieństwo
 * wartość zwracana jest z przedziału (0, 1]
 **/
double SimulatedAnnealing::probability(double deltaTime, double T)
{
    if(deltaTime < 0) return 1;
    
    return exp( -deltaTime/T );
}

/**************** 2 CZĘŚĆ PROGRAMU ***********************************/

/**
 * Funkcja wejściowa do programu
 **/
int main(int argc, char **argv)
{
    char *filename;
    FILE *file;
    
    if(argc != 2)
    {
        printf("Wywołanie programu: ./program rozklad.txt\n");
        exit(1);
    }
    
    //pobranie nazwy pliku z argumentu wywołania programu
    filename = argv[1];
    if( !(file = fopen(filename, "r")))
    {
        perror("Błąd podczas otwierania pliku.");
        exit(1);
    }
    
    //konstrukcja grafu
    StopsGraph G(file);
    
    //wypisanie listy sąsiedztwa
    G.printAdjacencyList();
    
    int menuOption;
    
    while(1) {
        
        std::cout << "MENU:" << std::endl;
        std::cout << "1 - Najkrótsza trasa z punktu A do B" << std::endl;
        std::cout << "2 - Cykl dla wybranych przystanków" << std::endl;
        std::cout << "Wybierz opcję wyszukiwania:" << std::endl;
        scanf("%d", &menuOption);
        
        //przełączanie pomiędzy opcjami menu programu głównego
        switch(menuOption) {
            case SHORTEST_PATH:
                shortestPathPrompt(G);
                break;
            case HAMILTON_CYCLE:
                hamiltonCyclePrompt(G);
                break;
        };
       
    }


}
