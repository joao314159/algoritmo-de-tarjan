#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>

#define vizinho_do_node graph.nodes[graph.vizinhos[node.valor][i]]

using namespace std;

int min(int a,int b){
  if(a > b){
    return b;
  }
  else{
    return a;
  }
}

class Node{
public:

  Node(){
    this->visitado = false;
    this->pai = -1;
  }

  Node(int valor){
    this->visitado = false;
    this->valor = valor;
    this->pai = -1;
  }

  int valor;

  //informações úteis para alguns algoritmos
  bool visitado;
  int pai;
  int tempo;
  int menor_tempo;

  void print(){
    cout<<"vértice "<<this->valor<<endl;
  }
};

class Edge{
public:

  Node node1;
  Node node2;

};

class Graph1{
public:

  vector<Node> nodes;
  vector<Edge> edges;

};


class Graph2{
public:

Graph2(){
  this->nodes = {};
  this->vizinhos = {};
  this->conexo = 0;
}
Graph2(int maximo_vertices){
  //garantimos poder alocar corretamente todos os possíveis nodes do grafo, inicializando o vector com maximo_vertices
  this->nodes = vector<Node>(maximo_vertices);
  //o mesmo conta para vizinhos, já que haverá no máximo n listas de vizinhos, (n = número de vértices)
  this->vizinhos = vector<vector<int>>(maximo_vertices);

  this->conexo = 0;
}

//aqui acessamos facilmente todos os vizinhos de cada node
vector<vector<int>> vizinhos;


//a posição do node corresponde ao seu valor, portanto não se deve usar push_back para adicionar nodes.
// por exemplo o node cujo valor é 8 fica em nodes[8], e é o nono node do grafo
vector<Node> nodes;

//para definir se é conexo. 0 = desconexo 1 = 1-conexo 2 = 2-conexo 3 = 3-conexo ou mais
int conexo;

//a saída são os vizinhos de cada vértices
void print(){
  for(int i = 0; i < vizinhos.size(); i++){
    cout<<"vizinhos do vértice "<<i<<" : ";
    for(int i2 =0; i2 < vizinhos[i].size(); i2++){
      cout<<vizinhos[i][i2]<<" ";
    }
    cout<<endl;
  }
}


};


//lê o arquivo e salva o resultado em um objeto da classe grafo
Graph2 ler_arquivo1(string nome){


  ifstream arquivo(nome);

  if(!arquivo.is_open()){
    cout<<"Erro ao abrir o arquivo!"<<endl;
  }
  else{

    int n;
    int m;

    //leitura da primeira linha
    arquivo>>n>>m;

    Graph2 graph(n);

    int number;

    int number1;
    int number2;

    //lendo as próximas linhas
    bool nova_linha = true;

    while(arquivo>>number){
      if(nova_linha){
        number1 = number;
        nova_linha = false;
      }
      else{

        number2 = number;

        graph.vizinhos[number1].push_back(number2);
        graph.vizinhos[number2].push_back(number1);

        graph.nodes[number2] = Node(number2);
        graph.nodes[number1] = Node(number1);

        nova_linha = true;
      }

    }
    arquivo.close();
    return graph;
  }
}



//retorno grafo com os pais dos vértices de acordo com a ordem do percorrimento
// e os tempos de início de cada vértice

//tempo inicia em zero
void busca_em_profundidade_marcando_vertices2(Graph2& graph, Node node, int& tempo,bool raiz){


  if(raiz){
    graph.nodes[node.valor].visitado = true;
    //se for raiz marca como visitado
    //não possui pai
  }

  tempo++;
  //salvo o tempo em que comecei a explorar o vértice
  graph.nodes[node.valor].tempo = tempo;

  //na primeira vez que exploramos um vértice, seu menor_tempo será igual ao tempo
  graph.nodes[node.valor].menor_tempo = tempo;


  for(int i = 0; i<graph.vizinhos[node.valor].size(); i++){
    //cout<<"vizinho de "<<node.valor<<" : "<<graph.vizinhos[node.valor][i].valor<<" "<<endl;

    //#define vizinho_do_node graph.nodes[graph.vizinhos[node.valor][i]]

    if(not vizinho_do_node.visitado){

      vizinho_do_node.visitado = true;
      //salvo o node pai
      vizinho_do_node.pai = node.valor;

      busca_em_profundidade_marcando_vertices2(graph,vizinho_do_node,tempo, false);

      graph.nodes[node.valor].menor_tempo  = min(graph.nodes[node.valor].menor_tempo,vizinho_do_node.menor_tempo);
      //graph.nodes[vizinho_do_node.pai].menor_tempo = min(graph.nodes[vizinho_do_node.pai].menor_tempo,vizinho_do_node.menor_tempo);

    }
    else{
      // se o vértice filho não já foi visitado, então a aresta que liga o pai com o filho é um backedge
      //e o filho não é exatamente filho. Não confundir!!
      if(graph.nodes[node.valor].pai == -1){
        cout<<"ERRO!!!!!"<<endl;
      }
      //se o vizinho não é o pai
      if(graph.nodes[node.valor].pai != vizinho_do_node.valor){
        graph.nodes[node.valor].menor_tempo = min(vizinho_do_node.tempo,graph.nodes[node.valor].menor_tempo );
      }
    }

  }
  //se n for a raiz
  if(graph.nodes[node.valor].pai != -1 ){
    if(graph.nodes[node.valor].menor_tempo >= graph.nodes[graph.nodes[node.valor].pai].menor_tempo){
      //grafo não é 2-conexo, pois tem vértice de corte
      graph.conexo = 1;
    }
  }
}


Graph2 busca_em_profundidade_marcando_vertices(Graph2 graph, Node node){

    Graph2 graph2 = graph;

    int tempo = 0;

    busca_em_profundidade_marcando_vertices2(graph2, node, tempo, true);

    cout<<"tempo "<<tempo<<endl;


    return graph2;

}

void k_conexo(Graph2 graph){
  //printa se o grafo é 0-conexo, 1-conexo, 2-conexo ou mais
  //algoritmo de tarjan

  //grafo 2-conexo se tirar qualquer 1 vértice, o grafo continua conexo

  //se no final essa lista não estiver vazia então o grafo é 2-conexo
  vector<Node> vertices_de_corte = {};

  //grafo modificado
  Graph2 grafo2 = busca_em_profundidade_marcando_vertices(graph, graph.nodes[0]);

  if(grafo2.conexo == 1){
    cout<<"GRAFO NÃO É 2-CONEXO"<<endl;
  }

  //ao fazer a DFS será necessário salvar:
  //o tempo de chegada no vértice (atributo tempo)
  //o menor tempo de chegada alcançado pelo filho de cada vértice (atributo menor_tempo)


}


int main(){


  Graph2 graph = ler_arquivo1("entradas/entrada.txt");

  cout<<graph.vizinhos[1][0]<<endl;

  cout<<"teste 2"<<endl;
  graph.print();

  k_conexo(graph);

  return 0;

}
