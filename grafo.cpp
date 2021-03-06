#include "grafo.h"

/*André Viana Sena de Souza - 25037
**Projeto e Análise de Algoritmos - ECO027
**Projeto #4 - YODA - 03/12/2014
**Arquivo: grafo.cpp
**Descrição: Implementação do cabeçalho grafo.h.
*/

/** \file grafo.cpp
    \brief Implementações de grafo.h.
*/

int Grafo::Aresta::Comparador(const void* a, const void* b)
{
    //Retorna 1 se a>b;
    //Retorna -1 se a<b;
    //Retorna 0 se a==b;

    Aresta* aa = (Aresta*) a;
    Aresta* bb = (Aresta*) b;
    return (aa->peso - bb->peso);
}

void Grafo::Vertice::AdicionarArestaAdj(Grafo::Aresta a_adj)
{
    if(!adj)
        adj=new std::vector<Aresta>;
    adj->push_back(a_adj);
}
bool Grafo::Vertice::ProximaArestaAdj(Grafo::Aresta* a_adj)
{
    //Iterar pela próxima aresta do vector ou inicializar o iterador
    // se a_adj == NULL.

    static int atual=-1;
    if(adj && a_adj)
    {
        atual++;
        if(atual>=adj->size())
            return false;
        *a_adj=adj->at(atual);
        return true;
    }
    if(!a_adj)
        atual=-1;
    return false;
}

Grafo::Vertice::Vertice()
{
    peso=-1;
    adj=NULL;
    ant=-1;
}
Grafo::Vertice::~Vertice()
{
    if(adj)
        delete adj;
}

Grafo::Grafo()
{
    a=NULL;
    v=NULL;
    a_agm=NULL;
    nv=0;
    na=0;
}
Grafo::~Grafo()
{
    if(v)
        delete [] v;
    if(a)
        delete [] a;
    if(a_agm)
        delete a_agm;
}
void Grafo::AdicionarArestaAGM(Aresta aresta)
{
    if(!a_agm)
        a_agm=new std::vector<Aresta>;
    a_agm->push_back(aresta);
}
bool Grafo::ProximaArestaAGM(Grafo::Aresta* aresta)
{
    //Iterar pela próxima aresta do vector ou inicializar o iterador
    // se aresta == NULL.

    static int atual=-1;
    if(aresta && a_agm)
    {
        atual++;
        if(atual>=a_agm->size())
            return false;
        *aresta=a_agm->at(atual);
        return true;
    }
    if(!aresta)
        atual=-1;
    return false;
}

Grafo* CarregarArquivo(const char* arquivo)
{
    //Tentando ler arquivo.
    FILE* file = fopen(arquivo, "r");

    //Se o arquivo estiver aberto.
    if(file)
    {
        //Inicializando variáveis.
        Grafo* ret = new Grafo;
        int aux1, aux2, aux3;
        char c;
        fpos_t pos;

        //Recuperando a quantidade de vértices.
        fscanf(file, "%d", &ret->nv);

        //Instanciando conteiner dos vértices.
        ret->v=new Grafo::Vertice[ret->nv];

        //Lendo a quantidade de tropas em cada vértice.
        for(int c=0; c<ret->nv; c++)
        {
            fscanf(file, "%d -> %d", &aux1, &aux2);
            ret->v[aux1].tropas = aux2;
        }

        //Contando a quantidade de arestas.
        fgetpos(file, &pos);
        ret->na=-1;
        while(true)
        {
            c=fgetc(file);
            if(c=='\n') ret->na++;
            else if(c==EOF)
                break;
        }

        if(ret->na>-1)
        {
            fsetpos(file, &pos);

            //Instanciando conteiner de arestas.
            ret->a=new Grafo::Aresta[ret->na];

            //Atribuindo valor às arestas.
            for(int c=0; c<ret->na; c++)
                fscanf(file, "%d %d %d",
                       &ret->a[c].origem, &ret->a[c].destino, &ret->a[c].peso);

            //Inserindo arestas nas listas internas dos vértices correspondentes.
            for(int c=0; c<ret->na; c++)
                ret->v[ret->a[c].origem].AdicionarArestaAdj(ret->a[c]);
        }
        //Fechando arquivo.
        fclose(file);
        //Retornando grafo.
        return ret;
    }

    //Retorno default.
    return NULL;
}

void ImprimirGrafo(Grafo* grafo)
{
    if(grafo)
    {
        //Imprimindo vértices do grafo.
        printf("Vertices:\n"
               "VERTICE\t|PESO\t|ANTERIOR |TROPAS\n");

        for(int c=0; c<grafo->nv; c++)
        {
            if(grafo->v[c].peso == INT_MAX)
                printf("%d\t| \u221E\t| %d\t  | %d\n", c,
                       grafo->v[c].ant, grafo->v[c].tropas);
            else
                printf("%d\t| %d\t| %d\t  | %d\n",
                       c, grafo->v[c].peso,
                       grafo->v[c].ant,
                       grafo->v[c].tropas);
        }

        printf("\n");

        //Imprimindo arestas do grafo.
        printf("Arestas:\n"
               "ORIGEM\t|DESTINO |DISTANCIA\n");

        for(int c=0; c<grafo->na; c++)
            printf("%d\t| %d\t | %d\n",
                   grafo->a[c].origem,
                   grafo->a[c].destino,
                   grafo->a[c].peso);

        printf("\n");

        //Se o grafo ter uma AGM.
        if(grafo->a_agm)
        {
            //Imprimir árvore geradora mínima.
            printf("Arestas da arvore geradora minima:\n"
                   "ORIGEM\t|DESTINO |DISTANCIA\n");

            for(int c=0; c<grafo->a_agm->size(); c++)
                printf("%d\t| %d\t | %d\n",
                       grafo->a_agm->at(c).origem,
                       grafo->a_agm->at(c).destino,
                       grafo->a_agm->at(c).peso);
        }
        else
            printf("Arvore geradora minima nao calculada.\n");
    }
}

