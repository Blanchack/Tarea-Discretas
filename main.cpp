#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

void readEdges(vector<vector<pair<int, int>>>& adjList){
    ifstream file("aristas.txt");

    int originVertex, destinyVertex, lenght;

    while(file>>originVertex>>destinyVertex>>lenght){
        adjList[originVertex].push_back({destinyVertex, lenght});

        //cout<<originVertex<<" "<<destinyVertex<<" "<<lenght<<endl;
    }
}

void readInput(vector<vector<pair<int, int>>>& adjList){
    ifstream file("input.txt");
    string input; getline(file, input);

    for(int i = 1; i < input.size()-1; i++){
        //Reemplazamos los espacios de los nommbres de las calles por underscores
        if(input[i] == ' ' && isalpha(input[i-1]) && isalpha(input[i+1])){
            input[i] = '_';
        }
    }
    vector<pair<string,int>> data;
    stringstream sstream(input);
    string calle; int numero;
    while(sstream >> calle >> numero){
        data.push_back({calle, numero});
    }

    cout<<input<<endl;

    string callesVertArr[] = {"Arturo_Prat", "Serrano", "Salas", "Angol", "Lincoyan", "Rengo", "Caupolican", "Anibal_Pinto", "Colo_Colo", "Castellon", "Tucapel", "Orompello", "Ongolmo", "Paicavi"};
    // 2 = ambos sentidos
    // 0 = hacia arriba
    // 1 = hacia abajo
    int sentidoCallesVert[] {0, 1,0, 1,0, 1,0, 1,0, 1,0, 1, 2};
    //string callesHorArr[] = {"Chacabuco", "Cochrane", "San_Martín", "O’Higgins", "Barros_Arana", "Freire", "Maipú", "Los_Carrera"};
    string callesHorArr[] = {"Los_Carrera", "Maipu", "Freire", "Barros_Arana", "OHiggins", "San_Martin", "Cochrane", "Chacabuco"};
    // 2 = ambos sentidos
    // 0 = hacia la derecha
    // 1 = hacia la izquierda
    int sentidoCallesHor[] {2, 0, 1, 0, 1, 0, 1, 2};

    set<string> callerVertSet;
    set<string> callerHorSet;
    for(int i = 0; i < 14; i++) callerVertSet.insert(callesVertArr[i]);
    for(int i = 0; i < 8; i++) callerHorSet.insert(callesHorArr[i]);

    for(int i = 0; i < data.size(); i++){
        //si es calle vertical
        if(callerVertSet.count(data[i].first)){
            int coorX;
            for(coorX = 0; coorX < 14; coorX++){
                if(data[i].first == callesVertArr[coorX])
                    break;
            }
            int coorY = (800-data[i].second)/100;

            int possConect1 = (coorY * 14) + coorX;     //la mas hacia arriba
            int possConect2 = ((coorY+1) * 14) + coorX;   //la mas hacia abajo

            if(sentidoCallesVert[coorX] == 2){
                adjList[113 + i].push_back({possConect1, 50});
                adjList[possConect1].push_back({113 + i, 50});

                adjList[113 + i].push_back({possConect2, 50});
                adjList[possConect2].push_back({113 + i, 50});
            }
            else if(sentidoCallesVert[coorX] == 0){
                adjList[possConect2].push_back({113 + i, 50});

                adjList[113 + i].push_back({possConect1, 50});
            }
            else if(sentidoCallesVert[coorX] == 1){
                adjList[possConect1].push_back({113 + i, 50});

                adjList[113 + i].push_back({possConect2, 50});
            }

            //cout<<coorX<<" "<<coorY<<endl;
            for(auto x : adjList[113 + i]){
                //cout<<x.first<<" "<<x.second<<endl;
            }
        }
        else{
            int coorY;
            for(coorY = 0; coorY < 8; coorY++){
                if(data[i].first == callesHorArr[coorY])
                    break;
            }
            int coorX = (data[i].second)/100;

            int possConect1 = (coorY * 14) + coorX;       //la mas hacia izq
            int possConect2 = (coorY * 14) + coorX + 1;   //la mas hacia der

            if(sentidoCallesHor[coorY] == 2){
                adjList[113 + i].push_back({possConect1, 50});
                adjList[possConect1].push_back({113 + i, 50});

                adjList[113 + i].push_back({possConect2, 50});
                adjList[possConect2].push_back({113 + i, 50});
            }
            else if(sentidoCallesHor[coorY] == 0){
                adjList[113 + i].push_back({possConect2, 50});

                adjList[possConect1].push_back({113 + i, 50});
            }
            else if(sentidoCallesHor[coorY] == 1){
                adjList[113 + i].push_back({possConect1, 50});

                adjList[possConect2].push_back({113 + i, 50});
            }

            for(auto x : adjList[113 + i]){
                //cout<<x.first<<" "<<x.second<<endl;
            }
            //cout<<coorX<<" "<<coorY<<endl;
        }
    }
}

int dijkstra(vector<vector<pair<int, int>>>& adjList, vector<int>& path, int source, int destiny){
    int INF = 1e9;
    vector<int> dist(116, INF); dist[source] = 0;
    vector<int> parent(116, -1);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.emplace(source, 0);

    while(!pq.empty()){
        pair<int, int> curr = pq.top(); pq.pop();

        if(curr.second > dist[curr.first]) continue;

        for(auto& edge : adjList[curr.first]){

            int dir = edge.first; int weight = edge.second;

            int possibleDistance = dist[curr.first] + weight;

            if(possibleDistance >= dist[dir]) continue;


            parent[dir] = curr.first;
            dist[dir] = possibleDistance;

            //cout<<dir<<endl;

            if(dir == destiny) break;

            pq.emplace(dir, dist[dir]);
        }
    }
    int curr = destiny;

    //cout<<parent[destiny];

    while(parent[curr] != -1){
        path.push_back(parent[curr]);
        curr = parent[curr];
        //cout<<curr<<endl;
    }

    return dist[destiny];
}

void printPath(vector<int>& path){
    string callesVertArr[] = {"Arturo_Prat", "Serrano", "Salas", "Angol", "Lincoyan", "Rengo", "Caupolican", "Anibal_Pinto", "Colo_Colo", "Castellon", "Tucapel", "Orompello", "Ongolmo", "Paicavi"};
    string callesHorArr[] = {"Los_Carrera", "Maipu", "Freire", "Barros_Arana", "OHiggins", "San_Martin", "Cochrane", "Chacabuco"};
    
    for(int i = path.size()-1; i >= 0; i--){
        if(path[i] > 112) continue;
        string nombreInterseccion;
        nombreInterseccion += callesVertArr[path[i]%14];
        nombreInterseccion += " ";
        nombreInterseccion += callesHorArr[path[i]/14];
        cout << nombreInterseccion << endl;
    }

}


int main(){
    //si hay vertice medio
    //Vertices 0-112 = Intersecciones de calles
    //vertice 113    = Vertice de Inicio
    //vertice 114    = Vertice medio
    //vertice 115    = Vertice final

    //---------------------------------------------------

    //si no hay vertice medio
    //Vertices 0-112 = Intersecciones de calles
    //vertice 113    = Vertice de Inicio
    //vertice 114    = Vertice final

    vector<vector<pair<int, int>>> adjList(116, vector<pair<int, int>>());

    readEdges(adjList);

    readInput(adjList);

    vector<int> path;

    int total = 0;

    if(adjList[115].empty()){
        total = dijkstra(adjList, path, 113, 114);
    }
    else{

        total += dijkstra(adjList, path, 114, 115);
        total += dijkstra(adjList, path, 113, 114);

    }

    cout<<"El camino mas corto son " << total << " metros."<<endl;
    //cout<<path.size();

    printPath(path);

    return 0;
}