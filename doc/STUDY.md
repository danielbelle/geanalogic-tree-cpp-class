# Estudo de Caso: Generic Tree (Árvore Genérica / N-ária)

## 1. Definição

Uma **Generic Tree** (ou **N-ary Tree**) é uma estrutura de dados hierárquica
onde cada nó pode ter um número ilimitado de filhos (0 a N filhos),
diferentemente de uma árvore binária que limita cada nó a no máximo 2 filhos.

### Características principais:

- **Hierarquia**: Organização em níveis (raiz, pais, filhos)
- **Flexibilidade**: Número variável de filhos por nó
- **Relações**: Um nó pai pode ter 0, 1, 2, 3... N filhos
- **Raiz única**: Existe apenas um nó raiz (ancestral comum)

## 2. Estrutura e Representação

### 2.1. Representação Visual

```text
              Avô João
              /    |    \
           Pai   Tia   Tio
          /  \    |
      Filho1 Filho2 Primo
```

### 2.2. Estrutura em C++ (usando struct e vector)

```cpp
#include <vector>
#include <string>

using namespace std;

struct Pessoa {
    string nome;
    string dataNascimento;
    char genero; // 'M' ou 'F'

    Pessoa* pai;  // Ponteiro para o pai
    vector<Pessoa*> filhos; // Lista dinâmica de filhos
};

// Função para criar uma nova pessoa
Pessoa* criarPessoa(const string& nome, const string& data, char genero) {
    Pessoa* nova = new Pessoa;
    nova->nome = nome;
    nova->dataNascimento = data;
    nova->genero = genero;
    nova->pai = nullptr;
    return nova;
}

// Função para adicionar filho
void adicionarFilho(Pessoa* pai, Pessoa* filho) {
    if (pai && filho) {
        pai->filhos.push_back(filho);
        filho->pai = pai;
    }
}
```

## 3. Implementação Completa

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

// Estrutura de dados
struct Pessoa {
    string nome;
    string dataNascimento;
    char genero;
    Pessoa* pai;
    vector<Pessoa*> filhos;
};

struct ArvoreGenealogica {
    Pessoa* raiz;
};

// ===== FUNÇÕES DE CRIAÇÃO E MANIPULAÇÃO =====

Pessoa* criarPessoa(const string& nome, const string& data, char genero) {
    Pessoa* nova = new Pessoa;
    nova->nome = nome;
    nova->dataNascimento = data;
    nova->genero = genero;
    nova->pai = nullptr;
    return nova;
}

ArvoreGenealogica* criarArvore() {
    ArvoreGenealogica* arvore = new ArvoreGenealogica;
    arvore->raiz = nullptr;
    return arvore;
}

void definirRaiz(ArvoreGenealogica* arvore, Pessoa* pessoa) {
    if (arvore) {
        arvore->raiz = pessoa;
    }
}

void adicionarFilho(Pessoa* pai, Pessoa* filho) {
    if (pai && filho) {
        pai->filhos.push_back(filho);
        filho->pai = pai;
    }
}

// ===== FUNÇÕES DE CONSULTA =====

bool ehRaiz(Pessoa* pessoa) {
    return pessoa && pessoa->pai == nullptr;
}

bool ehFolha(Pessoa* pessoa) {
    return pessoa && pessoa->filhos.empty();
}

int numFilhos(Pessoa* pessoa) {
    return pessoa ? pessoa->filhos.size() : 0;
}

// ===== BUSCA (DFS - Depth First Search) =====

Pessoa* buscarPorNomeRecursivo(Pessoa* atual, const string& nome) {
    if (!atual) return nullptr;
    if (atual->nome == nome) return atual;

    for (size_t i = 0; i < atual->filhos.size(); i++) {
        Pessoa* encontrado = buscarPorNomeRecursivo(atual->filhos[i], nome);
        if (encontrado) return encontrado;
    }
    return nullptr;
}

Pessoa* buscarPorNome(ArvoreGenealogica* arvore, const string& nome) {
    if (!arvore) return nullptr;
    return buscarPorNomeRecursivo(arvore->raiz, nome);
}

// ===== EXIBIÇÃO DE ASCENDENTES =====

void exibirAscendentes(Pessoa* pessoa) {
    if (!pessoa) {
        cout << "Pessoa não encontrada.\n";
        return;
    }

    cout << "Ascendentes de " << pessoa->nome << ":\n";
    Pessoa* atual = pessoa->pai;
    int nivel = 1;

    while (atual) {
        cout << "  Nível " << nivel << ": " << atual->nome << "\n";
        atual = atual->pai;
        nivel++;
    }

    if (nivel == 1) {
        cout << "  (Nenhum ascendente - é a raiz)\n";
    }
}

// ===== EXIBIÇÃO DE DESCENDENTES =====

void exibirDescendentesRecursivo(Pessoa* pessoa, int nivel) {
    for (size_t i = 0; i < pessoa->filhos.size(); i++) {
        // Indentação
        for (int j = 0; j < nivel; j++) {
            cout << "  ";
        }

        // Símbolo de árvore
        if (i == pessoa->filhos.size() - 1) {
            cout << "└─ ";
        } else {
            cout << "├─ ";
        }

        cout << pessoa->filhos[i]->nome << "\n";

        // Recursão para filhos
        exibirDescendentesRecursivo(pessoa->filhos[i], nivel + 1);
    }
}

void exibirDescendentes(Pessoa* pessoa) {
    if (!pessoa) {
        cout << "Pessoa não encontrada.\n";
        return;
    }

    cout << "Descendentes de " << pessoa->nome << ":\n";

    if (pessoa->filhos.empty()) {
        cout << "  (Nenhum descendente)\n";
    } else {
        exibirDescendentesRecursivo(pessoa, 0);
    }
}

// ===== EXIBIÇÃO DA ÁRVORE COMPLETA (BFS - Breadth First Search) =====

void exibirArvore(ArvoreGenealogica* arvore) {
    if (!arvore || !arvore->raiz) {
        cout << "Árvore vazia.\n";
        return;
    }

    queue<pair<Pessoa*, int>> fila;
    fila.push({arvore->raiz, 0});

    int nivelAtual = -1;

    while (!fila.empty()) {
        pair<Pessoa*, int> par = fila.front();
        fila.pop();

        Pessoa* pessoa = par.first;
        int nivel = par.second;

        // Imprimir separador de nível
        if (nivel != nivelAtual) {
            if (nivelAtual >= 0) cout << "\n";
            cout << "Nível " << nivel << ":\n";
            nivelAtual = nivel;
        }

        // Indentação
        for (int i = 0; i < nivel; i++) {
            cout << "  ";
        }
        cout << "• " << pessoa->nome << "\n";

        // Adicionar filhos na fila
        for (size_t i = 0; i < pessoa->filhos.size(); i++) {
            fila.push({pessoa->filhos[i], nivel + 1});
        }
    }
}

// ===== CONTAGEM DE DESCENDENTES =====

int contarDescendentes(Pessoa* pessoa) {
    if (!pessoa) return 0;

    int total = pessoa->filhos.size(); // Descendentes diretos

    for (size_t i = 0; i < pessoa->filhos.size(); i++) {
        total += contarDescendentes(pessoa->filhos[i]); // Descendentes indiretos
    }

    return total;
}

// ===== CÁLCULO DE DISTÂNCIA/GRAU DE PARENTESCO =====

int calcularNivel(Pessoa* ancestral, Pessoa* descendente) {
    int nivel = 0;
    Pessoa* atual = descendente;

    while (atual && atual != ancestral) {
        atual = atual->pai;
        nivel++;
    }

    return (atual == ancestral) ? nivel : -1;
}

Pessoa* encontrarAncestralComum(Pessoa* p1, Pessoa* p2) {
    if (!p1 || !p2) return nullptr;

    // Criar vetor com ancestrais de p1
    vector<Pessoa*> ancestrais1;
    Pessoa* atual = p1;
    while (atual) {
        ancestrais1.push_back(atual);
        atual = atual->pai;
    }

    // Buscar primeiro ancestral comum em p2
    atual = p2;
    while (atual) {
        for (size_t i = 0; i < ancestrais1.size(); i++) {
            if (ancestrais1[i] == atual) {
                return atual;
            }
        }
        atual = atual->pai;
    }

    return nullptr;
}

int calcularDistancia(Pessoa* p1, Pessoa* p2) {
    if (!p1 || !p2) return -1;

    // Caso especial: mesma pessoa
    if (p1 == p2) return 0;

    Pessoa* ancestralComum = encontrarAncestralComum(p1, p2);
    if (!ancestralComum) return -1; // Sem ancestral comum

    int dist1 = calcularNivel(ancestralComum, p1);
    int dist2 = calcularNivel(ancestralComum, p2);

    return dist1 + dist2;
}

// ===== MOSTRAR GERAÇÕES POR NÍVEL =====

void exibirGeracoes(ArvoreGenealogica* arvore) {
    if (!arvore || !arvore->raiz) {
        cout << "Árvore vazia.\n";
        return;
    }

    queue<pair<Pessoa*, int>> fila;
    fila.push({arvore->raiz, 0});

    int nivelAtual = -1;

    while (!fila.empty()) {
        pair<Pessoa*, int> par = fila.front();
        fila.pop();

        Pessoa* pessoa = par.first;
        int nivel = par.second;

        if (nivel != nivelAtual) {
            if (nivelAtual >= 0) cout << "\n";
            cout << "Geração " << nivel << ": ";
            nivelAtual = nivel;
        } else {
            cout << ", ";
        }

        cout << pessoa->nome;

        for (size_t i = 0; i < pessoa->filhos.size(); i++) {
            fila.push({pessoa->filhos[i], nivel + 1});
        }
    }
    cout << "\n";
}

// ===== LIBERAR MEMÓRIA =====

void destruirArvoreRecursivo(Pessoa* pessoa) {
    if (!pessoa) return;

    for (size_t i = 0; i < pessoa->filhos.size(); i++) {
        destruirArvoreRecursivo(pessoa->filhos[i]);
    }

    delete pessoa;
}

void destruirArvore(ArvoreGenealogica* arvore) {
    if (arvore) {
        destruirArvoreRecursivo(arvore->raiz);
        delete arvore;
    }
}
```

## 4. Vantagens da Generic Tree

### 4.1. Vantagens Gerais

1. **Flexibilidade**: Suporta número ilimitado de filhos por nó
2. **Representação Natural**: Modela hierarquias reais (organogramas, árvores
   genealógicas, sistemas de arquivos)
3. **Navegação Bidirecional**: Permite navegar para cima (ancestrais) e para
   baixo (descendentes)
4. **Escalabilidade**: Cresce conforme necessário, sem restrições estruturais
5. **Operações Recursivas**: Facilita implementação de algoritmos recursivos
   (busca, travessia, contagem)

### 4.2. Vantagens para Árvore Genealógica

1. **Múltiplos Filhos**: Uma pessoa pode ter quantos filhos for necessário
2. **Hierarquia Familiar**: Representa naturalmente gerações (avós → pais →
   filhos → netos)
3. **Relações Claras**: Fácil identificar ascendentes e descendentes
4. **Cálculo de Parentesco**: Permite calcular grau de parentesco entre
   quaisquer duas pessoas
5. **Consultas Rápidas**: Busca eficiente por nome, listagem de gerações

### 4.3. Vantagens da Abordagem com Vector

1. **Acesso Direto**: O(1) para acessar filho por índice
2. **Iteração Simples**: Loop `for` padrão com `size()`
3. **STL**: Usa biblioteca padrão (testada e otimizada)
4. **Flexibilidade**: Fácil adicionar/remover filhos
5. **Memória Contígua**: Melhor cache locality

## 5. Desvantagens da Generic Tree

### 5.1. Desvantagens Gerais

1. **Memória**: Uso de ponteiros e vetores consome mais memória que árvores
   binárias
2. **Complexidade**: Implementação mais complexa que estruturas lineares
   (listas, filas)
3. **Balanceamento**: Não há garantia de balanceamento (pode ficar
   desbalanceada)
4. **Busca**: Busca pode ser O(n) no pior caso (percorrer toda a árvore)
5. **Gerenciamento de Memória**: Necessário `delete` manual (risco de memory
   leaks)

### 5.2. Desvantagens da Abordagem com Vector

1. **Overhead de Memória**: Vector tem overhead adicional (capacidade vs
   tamanho)
2. **Realocação**: Push_back pode causar realocações (ainda que amortizado O(1))
3. **Cache**: Ponteiros para filhos podem estar dispersos na memória
4. **Fragmentação**: Múltiplos vectors pequenos podem fragmentar memória

## 6. Tipos de Uso e Aplicações

### 6.1. Árvores Genealógicas

- Representar famílias e linhagens
- Calcular graus de parentesco
- Listar gerações

### 6.2. Sistemas de Arquivos

- Diretórios e subdiretórios
- Navegação por pastas
- Busca de arquivos

### 6.3. Organogramas Empresariais

- Hierarquia de funcionários
- Estrutura de departamentos
- Cadeia de comando

### 6.4. Estruturas XML/HTML (DOM)

- Tags aninhadas
- Parsing de documentos
- Manipulação de árvore de elementos

### 6.5. Inteligência Artificial

- Árvores de decisão
- Árvores de jogos (game trees)
- Planejamento hierárquico

### 6.6. Compiladores

- Árvores sintáticas abstratas (AST)
- Análise de código
- Otimização de expressões

## 7. Complexidade das Operações

| Operação                | Complexidade    | Observação                       |
| ----------------------- | --------------- | -------------------------------- |
| **Criar pessoa**        | O(1)            | Alocar struct                    |
| **Inserir filho**       | O(1) amortizado | push_back no vector              |
| **Acesso a filho**      | O(1)            | Acesso por índice no vector      |
| **Buscar por nome**     | O(n)            | DFS - percorre toda árvore       |
| **Listar descendentes** | O(d)            | d = número de descendentes       |
| **Listar ascendentes**  | O(h)            | h = altura (profundidade do nó)  |
| **Calcular distância**  | O(h)            | h = altura da árvore             |
| **Contar descendentes** | O(n)            | Percorre subárvore completa      |
| **Exibir árvore (BFS)** | O(n)            | Visita cada nó uma vez           |
| **Remover nó**          | O(n)            | Depende de reajustes necessários |

## 8. Algoritmos de Travessia

### 8.1. DFS (Depth-First Search) - Busca em Profundidade

```cpp
void dfsPreOrder(Pessoa* pessoa) {
    if (!pessoa) return;

    // Visita o nó atual
    cout << pessoa->nome << "\n";

    // Visita filhos recursivamente
    for (size_t i = 0; i < pessoa->filhos.size(); i++) {
        dfsPreOrder(pessoa->filhos[i]);
    }
}
```

**Características:**

- Explora o máximo de profundidade antes de voltar
- Usa recursão (pilha implícita)
- Ordem: raiz → filhos (esquerda para direita)

### 8.2. BFS (Breadth-First Search) - Busca em Largura

```cpp
void bfs(Pessoa* raiz) {
    if (!raiz) return;

    queue<Pessoa*> fila;
    fila.push(raiz);

    while (!fila.empty()) {
        Pessoa* atual = fila.front();
        fila.pop();

        cout << atual->nome << "\n";

        // Adicionar todos os filhos na fila
        for (size_t i = 0; i < atual->filhos.size(); i++) {
            fila.push(atual->filhos[i]);
        }
    }
}
```

**Características:**

- Explora nível por nível
- Usa fila (queue do STL)
- Ordem: nível 0 → nível 1 → nível 2...

## 9. Quando Usar Generic Tree?

### ✅ Use Generic Tree quando:

- Hierarquia natural com número variável de filhos
- Precisa navegar para cima (ancestrais) e para baixo (descendentes)
- Relações pai-filho são importantes
- Estrutura pode crescer dinamicamente
- Operações de busca e travessia são frequentes
- Acesso aos filhos por índice é útil

### ❌ Não use Generic Tree quando:

- Dados são lineares (use lista ou vetor)
- Busca binária é necessária (use árvore binária de busca - BST)
- Memória é extremamente crítica (considere estruturas mais compactas)
- Acesso aleatório é predominante (use array/vector)
- Relações não são hierárquicas (use grafo)

## 10. Containers STL Úteis

### 10.1. vector (Lista Dinâmica)

```cpp
#include <vector>

using namespace std;

vector<Pessoa*> filhos;  // Vetor de ponteiros

filhos.push_back(pessoa);      // Adicionar no final - O(1) amortizado
filhos.size();                 // Tamanho - O(1)
filhos[i];                     // Acesso por índice - O(1)
filhos.empty();                // Verificar se está vazio - O(1)
filhos.clear();                // Limpar todos os elementos - O(n)
```

### 10.2. queue (Fila para BFS)

```cpp
#include <queue>

using namespace std;

queue<Pessoa*> fila;

fila.push(pessoa);             // Adicionar no final - O(1)
fila.front();                  // Acessar primeiro - O(1)
fila.pop();                    // Remover primeiro - O(1)
fila.empty();                  // Verificar se vazio - O(1)
```

### 10.3. pair (Pares de Valores)

```cpp
#include <utility>

using namespace std;

pair<Pessoa*, int> par;        // Par (pessoa, nível)
par.first;                     // Primeiro elemento
par.second;                    // Segundo elemento

// Criar par:
pair<Pessoa*, int> par = {pessoa, 0};
// Ou com make_pair:
auto par = make_pair(pessoa, 0);
```

## 11. Exemplo de Uso Completo

```cpp
int main() {
    // Criar árvore
    ArvoreGenealogica* arvore = criarArvore();

    // Criar pessoas
    Pessoa* avô = criarPessoa("João Silva", "01/01/1950", 'M');
    Pessoa* pai = criarPessoa("Carlos Silva", "15/03/1975", 'M');
    Pessoa* tia = criarPessoa("Maria Silva", "20/07/1978", 'F');
    Pessoa* filho1 = criarPessoa("Pedro Silva", "10/05/2000", 'M');
    Pessoa* filho2 = criarPessoa("Ana Silva", "22/08/2002", 'F');
    Pessoa* primo = criarPessoa("Lucas Silva", "05/12/2003", 'M');

    // Montar árvore
    definirRaiz(arvore, avô);
    adicionarFilho(avô, pai);
    adicionarFilho(avô, tia);
    adicionarFilho(pai, filho1);
    adicionarFilho(pai, filho2);
    adicionarFilho(tia, primo);

    // Exibir árvore completa
    cout << "=== ÁRVORE COMPLETA ===\n";
    exibirArvore(arvore);
    cout << "\n";

    // Exibir descendentes do avô
    cout << "=== DESCENDENTES ===\n";
    exibirDescendentes(avô);
    cout << "\n";

    // Exibir ascendentes do Pedro
    cout << "=== ASCENDENTES ===\n";
    exibirAscendentes(filho1);
    cout << "\n";

    // Contar descendentes
    int total = contarDescendentes(avô);
    cout << "Total de descendentes de " << avô->nome << ": " << total << "\n\n";

    // Calcular distância entre Pedro e Ana (irmãos)
    int dist1 = calcularDistancia(filho1, filho2);
    cout << "Distância entre " << filho1->nome << " e " << filho2->nome << ": "
              << dist1 << " graus\n";

    // Calcular distância entre Pedro e Lucas (primos)
    int dist2 = calcularDistancia(filho1, primo);
    cout << "Distância entre " << filho1->nome << " e " << primo->nome << ": "
              << dist2 << " graus\n\n";

    // Exibir gerações
    cout << "=== GERAÇÕES ===\n";
    exibirGeracoes(arvore);
    cout << "\n";

    // Buscar pessoa
    cout << "=== BUSCA ===\n";
    Pessoa* encontrada = buscarPorNome(arvore, "Ana Silva");
    if (encontrada) {
        cout << "Pessoa encontrada: " << encontrada->nome
                  << " (nascida em " << encontrada->dataNascimento << ")\n";
    }

    // Liberar memória
    destruirArvore(arvore);

    return 0;
}
```

### Saída Esperada:

```text
=== ÁRVORE COMPLETA ===
Nível 0:
• João Silva

Nível 1:
  • Carlos Silva
  • Maria Silva

Nível 2:
    • Pedro Silva
    • Ana Silva
    • Lucas Silva

=== DESCENDENTES ===
Descendentes de João Silva:
├─ Carlos Silva
  ├─ Pedro Silva
  └─ Ana Silva
└─ Maria Silva
  └─ Lucas Silva

=== ASCENDENTES ===
Ascendentes de Pedro Silva:
  Nível 1: Carlos Silva
  Nível 2: João Silva

Total de descendentes de João Silva: 5

Distância entre Pedro Silva e Ana Silva: 2 graus
Distância entre Pedro Silva e Lucas Silva: 4 graus

=== GERAÇÕES ===
Geração 0: João Silva
Geração 1: Carlos Silva, Maria Silva
Geração 2: Pedro Silva, Ana Silva, Lucas Silva

=== BUSCA ===
Pessoa encontrada: Ana Silva (nascida em 22/08/2002)
```

## 12. Dicas de Implementação

### 12.1. Gerenciamento de Memória

```cpp
// ❌ EVITAR: Memory leak
Pessoa* p = criarPessoa("João", "01/01/2000", 'M');
// ...esquece de dar delete

// ✅ BOM: Sempre liberar memória
destruirArvore(arvore);  // Libera recursivamente toda a árvore
```

### 12.2. Verificação de Ponteiros Nulos

```cpp
// ❌ PERIGOSO: Não verificar nullptr
void funcao(Pessoa* p) {
    cout << p->nome;  // CRASH se p == nullptr
}

// ✅ SEGURO: Sempre verificar
void funcao(Pessoa* p) {
    if (!p) return;
    cout << p->nome;
}
```

### 12.3. Iteração sobre Filhos

```cpp
// ✅ Usar size() do vector
for (size_t i = 0; i < pessoa->filhos.size(); i++) {
    cout << pessoa->filhos[i]->nome << "\n";
}

// ✅ Ou iterador range-based (C++11)
for (Pessoa* filho : pessoa->filhos) {
    cout << filho->nome << "\n";
}
```

## 13. Conclusão

A Generic Tree usando **struct**, **ponteiros** e **vector** do STL é uma
abordagem prática e eficiente para representar hierarquias como árvores
genealógicas.

### Resumo:

- **Struct**: Define dados (nome, data, gênero, pai, filhos)
- **Ponteiros**: Conectam nós (relações pai-filho)
- **Vector**: Armazena lista dinâmica de filhos com acesso eficiente
- **Funções**: Implementam operações (buscar, adicionar, exibir)
- **Queue**: Facilita travessia BFS (nível por nível)

### Para o projeto:

1. Use `vector<Pessoa*>` para armazenar filhos (flexível e eficiente)
2. Use DFS para buscar por nome (simples e recursivo)
3. Use BFS para exibir por gerações (nível por nível)
4. Lembre-se de liberar memória com `delete` (ou use smart pointers se
   permitido)
5. Sempre verifique ponteiros nulos antes de acessar membros

## 14. Referências e Leitura Adicional

- CORMEN, T. H. et al. _Introduction to Algorithms_. 3rd ed. MIT Press, 2009.
- SEDGEWICK, R.; WAYNE, K. _Algorithms_. 4th ed. Addison-Wesley, 2011.
- WEISS, M. A. _Data Structures and Algorithm Analysis in C++_. 4th ed.
  Pearson, 2013.
- cppreference.com: STL Containers (vector, queue, pair)
- GeeksforGeeks: N-ary Tree Data Structure
