

#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
#include <string>

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
  this->conexo = -1;

  this->vertice_de_corte  = -1;

}
Graph2(int maximo_vertices){
  //garantimos poder alocar corretamente todos os possíveis nodes do grafo, inicializando o vector com maximo_vertices
  this->nodes = vector<Node>(maximo_vertices);
  //o mesmo conta para vizinhos, já que haverá no máximo n listas de vizinhos, (n = número de vértices)
  this->vizinhos = vector<vector<int>>(maximo_vertices);

  this->conexo = -1;

  this->vertice_de_corte = -1;

}

//aqui acessamos facilmente todos os vizinhos de cada node
vector<vector<int>> vizinhos;

int quantidade_vertices;

//a posição do node corresponde ao seu valor, portanto não se deve usar push_back para adicionar nodes.
// por exemplo o node cujo valor é 8 fica em nodes[8], e é o nono node do grafo
vector<Node> nodes;

//para definir se é conexo. 0 = desconexo 1 = 1-conexo 2 = 2-conexo 3 = 3-conexo ou mais
int conexo;

//variável específica para usar no teste de grafo 3-convexo
int vertice_de_corte;


void remover_vertice(int node){
  this->vertice_de_corte = node;

  vector<vector<int>> vizinhos2 = {};

  for(int i = 0; i< this->vizinhos.size(); i++){
    vector<int> lista = {};
    if(i != node){
      for(int i2 = 0; i2 < this->vizinhos[i].size(); i2++){
        if(this->vizinhos[i][i2] != node){
          lista.push_back(this->vizinhos[i][i2]);
        }
      }
      vizinhos2.push_back(lista);
    }
  }

  this->vizinhos = vizinhos2;

  for(int i = 0; i< this->vizinhos.size(); i++){
    for(int i2 = 0; i2 < this->vizinhos[i].size(); i2++){
      if(this->vizinhos[i][i2] > node){
        this->vizinhos[i][i2]--;
      }
    }
  }

  this->nodes.erase(this->nodes.begin() + node);

}

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



//gera grafos com um vértice a menos
vector<Graph2> Gerar_grafos(Graph2 graph){

  vector<Graph2> grafos;

  for(int i = 0; i< graph.quantidade_vertices; i++){
    Graph2 graph2 = graph;
    graph2.remover_vertice(i);
    grafos.push_back(graph2);

  }
  return grafos;
}


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

    graph.quantidade_vertices = n;

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
void busca_em_profundidade_marcando_vertices2(Graph2& graph, Node node, int& tempo,bool raiz,vector<int>& vertices_de_corte){

  //para tratar o caso particular da raiz como vértice de corte
  int filhos_da_raiz = 0;

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

    if(raiz){
      filhos_da_raiz++;
    }


    //#define vizinho_do_node graph.nodes[graph.vizinhos[node.valor][i]]

    if(not vizinho_do_node.visitado){

      vizinho_do_node.visitado = true;
      //salvo o node pai
      vizinho_do_node.pai = node.valor;

      busca_em_profundidade_marcando_vertices2(graph,vizinho_do_node,tempo, false, vertices_de_corte);

      graph.nodes[node.valor].menor_tempo  = min(graph.nodes[node.valor].menor_tempo,vizinho_do_node.menor_tempo);
      //graph.nodes[vizinho_do_node.pai].menor_tempo = min(graph.nodes[vizinho_do_node.pai].menor_tempo,vizinho_do_node.menor_tempo);

      if (!raiz && vizinho_do_node.menor_tempo >= graph.nodes[node.valor].tempo) {
          //node.valor é vértice de corte
          vertices_de_corte.push_back(graph.nodes[node.valor].valor);

          graph.conexo = 1;
      }

    }
    else{
      // se o vértice filho não já foi visitado, então a aresta que liga o pai com o filho é um backedge
      //e o filho não é exatamente filho. Não confundir!!

      //se o vizinho não é o pai
      if(graph.nodes[node.valor].pai != vizinho_do_node.valor){
        graph.nodes[node.valor].menor_tempo = min(vizinho_do_node.tempo,graph.nodes[node.valor].menor_tempo );
      }
    }
/*
    //se a raiz é vértice de corte
    if(raiz && filhos_da_raiz >=2){
      vertices_de_corte.push_back(node.valor);
      graph.conexo = 1;
    }
*/
  }

}


Graph2 busca_em_profundidade_marcando_vertices(Graph2 graph, Node node,vector<int>& vertices_de_corte){

    Graph2 graph2 = graph;

    int tempo = 0;

    busca_em_profundidade_marcando_vertices2(graph2, node, tempo, true, vertices_de_corte);

    if(tempo < graph2.quantidade_vertices){
      //se o grafo é desconexo
      graph2.conexo  = 0;
    }

    return graph2;

}

//testa se o grafo é 3-conexo
int busca_em_profundidade_marcando_vertices3(Graph2 graph, vector<vector<int>>& vertices_de_corte){



  //para cada remoção de vértice executar o outro código
  vector<Graph2> grafos = Gerar_grafos(graph);
  vector<int> vertices_de_corte2 = {};
  vector<int> par;


  for(int i = 0; i< grafos.size(); i++){


    Graph2 grafos2 = busca_em_profundidade_marcando_vertices(grafos[i], grafos[i].nodes[0],vertices_de_corte2);

    for(int i2 = 0; i2< vertices_de_corte2.size(); i2++){

      par.push_back(grafos[i].vertice_de_corte);
      par.push_back(vertices_de_corte2[i2]);


    }
  }


  //se não há nenhum par de vértices de corte então o grafo é 3-conexo
  if(par.size() == 0){
    return 3;
  }
  else{
    return 2;
  }


}

void k_conexo(Graph2 graph){
  //printa se o grafo é 0-conexo, 1-conexo, 2-conexo ou mais
  //algoritmo de tarjan

  //grafo 2-conexo se tirar qualquer 1 vértice, o grafo continua conexo

  //se no final essa lista estiver vazia então o grafo é 2-conexo
  vector<int> vertices_de_corte = {};

  //se no final essa lista estiver vazia então o grafo é 3-conexo
  //vector<vector<int>> vertices_de_corte2 = {};

  //grafo modificado
  Graph2 grafo2 = busca_em_profundidade_marcando_vertices(graph, graph.nodes[0], vertices_de_corte);

  //para testar se é 3-conexo
  //Graph2 grafo3 = busca_em_profundidade_marcando_vertices3(graph,vertices_de_corte2);

  if(grafo2.conexo == 0){
    cout<<"Grafo desconexo"<<endl;
  }
  else if(grafo2.conexo == 1){
    cout<<"Grafo 1-conexo"<<endl;

    vector<int> ordenacao = vector<int>(graph.quantidade_vertices + 1,-1);

    for(int i = 0; i<vertices_de_corte.size();i++){
      ordenacao[vertices_de_corte[i]] = vertices_de_corte[i];
    }

    for(int i = 0; i< graph.quantidade_vertices + 1; i++){
      if(ordenacao[i] != -1){
        cout<<ordenacao[i]<<endl;
      }
    }

  }
  else{
    cout<<"Grafo 2-conexo"<<endl;

    vector<vector<int>> vertices_de_corte = {};

    int a = busca_em_profundidade_marcando_vertices3(grafo2, vertices_de_corte);
    if(a == 3){
      cout<<"Grafo é 3-conexo!"<<endl;
    }
  }



  //ao fazer a DFS será necessário salvar:
  //o tempo de chegada no vértice (atributo tempo)
  //o menor tempo de chegada alcançado pelo filho de cada vértice (atributo menor_tempo)


}


int main(){

  vector<string> arquivos = {};

  for(int i = 0; i < 14; i++){
    string number = to_string(i+1);
    string texto = "entradas/" + number + ".in";
    cout<<texto<<endl;

    if((i+1) != 9){
      arquivos.push_back(texto);
    }
  }


  for(int i = 0; i< 15; i++){

  Graph2 graph = ler_arquivo1(arquivos[i]);
  cout<<"arquivo: "<<arquivos[i]<<endl;
  k_conexo(graph);

  }

  return 0;

}
