# Estudo de Caso: Generic Tree - Modelo Família como Nó

## 1. Definição

O modelo **Família como Nó** é uma abordagem para árvores genealógicas onde a
estrutura hierárquica é formada por **famílias** (uniões) em vez de pessoas
individuais. Cada nó da árvore representa uma família (casal) com seus filhos, e
as relações de parentesco são mantidas através de ligações entre famílias de
diferentes gerações.

### Características principais:

- **Biparentalidade Explícita**: Cada família representa pai + mãe de forma
  consistente
- **Árvore de Famílias**: A hierarquia é entre famílias, não entre pessoas
- **Sem DAG de Pessoas**: Evita duplicação de nós de pessoas e complexidade de
  grafos acíclicos direcionados
- **Travessias Simples**: Mantém propriedades de árvore/floresta para algoritmos
  padrão
- **Fidelidade Real**: Representa de forma mais fiel as relações familiares
  reais

## 2. Estrutura e Representação

### 2.1. Representação Visual

```text
         Família 1 (Avô + Avó)
              |
         +----+----+
         |         |
    Família 2   Família 3
    (Pai+Mãe)   (Tio+Tia)
         |         |
      +--+--+      |
      |     |      |
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
    struct Familia* familiaOrigem; // Família onde é filho
};

struct Familia {
    Pessoa* pai;  // Pode ser nullptr
    Pessoa* mae;  // Pode ser nullptr
    vector<Pessoa*> filhos; // Filhos desta união
    Familia* familiaAncestral; // Família de origem (dos pais)
    vector<Familia*> familiasFilhas; // Famílias formadas pelos filhos
};

// Função para criar uma nova pessoa
Pessoa* criarPessoa(const string& nome, const string& data, char genero) {
    Pessoa* nova = new Pessoa;
    nova->nome = nome;
    nova->dataNascimento = data;
    nova->genero = genero;
    nova->familiaOrigem = nullptr;
    return nova;
}

// Função para criar uma nova família
Familia* criarFamilia(Pessoa* pai, Pessoa* mae) {
    Familia* nova = new Familia;
    nova->pai = pai;
    nova->mae = mae;
    nova->familiaAncestral = nullptr;
    return nova;
}

// Função para adicionar filho à família
void adicionarFilhoNaFamilia(Familia* familia, Pessoa* filho) {
    if (familia && filho) {
        familia->filhos.push_back(filho);
        filho->familiaOrigem = familia;
    }
}

// Função para ligar família filha à família ancestral
void ligarFamiliaFilha(Familia* ancestral, Familia* filha) {
    if (ancestral && filha) {
        ancestral->familiasFilhas.push_back(filha);
        filha->familiaAncestral = ancestral;
    }
}
```

## 3. Implementação Completa

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

// ===== ESTRUTURAS DE DADOS =====

struct Pessoa {
    string nome;
    string dataNascimento;
    char genero;
    struct Familia* familiaOrigem;
};

struct Familia {
    Pessoa* pai;
    Pessoa* mae;
    vector<Pessoa*> filhos;
    Familia* familiaAncestral;
    vector<Familia*> familiasFilhas;
};

struct ArvoreGenealogica {
    Familia* raizFamilia;
    unordered_map<string, Pessoa*> indicePessoas; // Índice para busca rápida
};

// ===== FUNÇÕES DE CRIAÇÃO E MANIPULAÇÃO =====

Pessoa* criarPessoa(const string& nome, const string& data, char genero) {
    Pessoa* nova = new Pessoa;
    nova->nome = nome;
    nova->dataNascimento = data;
    nova->genero = genero;
    nova->familiaOrigem = nullptr;
    return nova;
}

Familia* criarFamilia(Pessoa* pai, Pessoa* mae) {
    Familia* nova = new Familia;
    nova->pai = pai;
    nova->mae = mae;
    nova->familiaAncestral = nullptr;
    return nova;
}

ArvoreGenealogica* criarArvore() {
    ArvoreGenealogica* arvore = new ArvoreGenealogica;
    arvore->raizFamilia = nullptr;
    return arvore;
}

void definirRaizFamilia(ArvoreGenealogica* arvore, Familia* familia) {
    if (arvore) {
        arvore->raizFamilia = familia;
    }
}

void adicionarFilhoNaFamilia(Familia* familia, Pessoa* filho) {
    if (familia && filho) {
        familia->filhos.push_back(filho);
        filho->familiaOrigem = familia;
    }
}

void ligarFamiliaFilha(Familia* ancestral, Familia* filha) {
    if (ancestral && filha) {
        ancestral->familiasFilhas.push_back(filha);
        filha->familiaAncestral = ancestral;
    }
}

void adicionarPessoaNoIndice(ArvoreGenealogica* arvore, Pessoa* pessoa) {
    if (arvore && pessoa) {
        arvore->indicePessoas[pessoa->nome] = pessoa;
    }
}

// ===== FUNÇÕES DE CONSULTA =====

bool ehRaizFamilia(Familia* familia) {
    return familia && familia->familiaAncestral == nullptr;
}

bool ehFamiliaFolha(Familia* familia) {
    return familia && familia->familiasFilhas.empty();
}

int numFilhos(Familia* familia) {
    return familia ? familia->filhos.size() : 0;
}

int numFamiliasFilhas(Familia* familia) {
    return familia ? familia->familiasFilhas.size() : 0;
}

// ===== BUSCA =====

Pessoa* buscarPorNome(ArvoreGenealogica* arvore, const string& nome) {
    if (!arvore) return nullptr;

    auto it = arvore->indicePessoas.find(nome);
    if (it != arvore->indicePessoas.end()) {
        return it->second;
    }
    return nullptr;
}

Familia* buscarFamiliaPorFilho(ArvoreGenealogica* arvore, const string& nomeFilho) {
    Pessoa* filho = buscarPorNome(arvore, nomeFilho);
    return filho ? filho->familiaOrigem : nullptr;
}

// ===== EXIBIÇÃO DE ASCENDENTES =====

void exibirAscendentesPessoa(Pessoa* pessoa) {
    if (!pessoa) {
        cout << "Pessoa não encontrada.\n";
        return;
    }

    cout << "Ascendentes de " << pessoa->nome << ":\n";

    Familia* familiaAtual = pessoa->familiaOrigem;
    int nivel = 1;

    while (familiaAtual) {
        cout << "  Nível " << nivel << ":\n";

        if (familiaAtual->pai) {
            cout << "    Pai: " << familiaAtual->pai->nome << "\n";
        }
        if (familiaAtual->mae) {
            cout << "    Mãe: " << familiaAtual->mae->nome << "\n";
        }

        // Subir para a família ancestral (da geração dos pais)
        familiaAtual = familiaAtual->familiaAncestral;
        nivel++;
    }

    if (nivel == 1) {
        cout << "  (Nenhum ascendente conhecido)\n";
    }
}

// ===== EXIBIÇÃO DE DESCENDENTES =====

void exibirDescendentesFamiliaRecursivo(Familia* familia, int nivel) {
    for (size_t i = 0; i < familia->familiasFilhas.size(); i++) {
        Familia* familhaFilha = familia->familiasFilhas[i];

        // Indentação
        for (int j = 0; j < nivel; j++) {
            cout << "  ";
        }

        // Símbolo de árvore
        if (i == familia->familiasFilhas.size() - 1) {
            cout << "└─ ";
        } else {
            cout << "├─ ";
        }

        // Exibir casal
        cout << "Família (";
        if (familhaFilha->pai) cout << familhaFilha->pai->nome;
        else cout << "?";
        cout << " + ";
        if (familhaFilha->mae) cout << familhaFilha->mae->nome;
        else cout << "?";
        cout << ")\n";

        // Exibir filhos desta família
        for (size_t j = 0; j < familhaFilha->filhos.size(); j++) {
            for (int k = 0; k < nivel + 1; k++) {
                cout << "  ";
            }
            cout << "  • " << familhaFilha->filhos[j]->nome << "\n";
        }

        // Recursão para famílias filhas
        exibirDescendentesFamiliaRecursivo(familhaFilha, nivel + 1);
    }
}

void exibirDescendentesFamilia(Familia* familia) {
    if (!familia) {
        cout << "Família não encontrada.\n";
        return;
    }

    cout << "Descendentes da família (";
    if (familia->pai) cout << familia->pai->nome;
    else cout << "?";
    cout << " + ";
    if (familia->mae) cout << familia->mae->nome;
    else cout << "?";
    cout << "):\n";

    // Exibir filhos diretos
    cout << "Filhos:\n";
    for (size_t i = 0; i < familia->filhos.size(); i++) {
        cout << "  • " << familia->filhos[i]->nome << "\n";
    }

    if (familia->familiasFilhas.empty()) {
        cout << "\n(Nenhuma família descendente)\n";
    } else {
        cout << "\nFamílias descendentes:\n";
        exibirDescendentesFamiliaRecursivo(familia, 0);
    }
}

// ===== EXIBIÇÃO DA ÁRVORE COMPLETA (BFS) =====

void exibirArvore(ArvoreGenealogica* arvore) {
    if (!arvore || !arvore->raizFamilia) {
        cout << "Árvore vazia.\n";
        return;
    }

    queue<pair<Familia*, int>> fila;
    fila.push({arvore->raizFamilia, 0});

    int nivelAtual = -1;

    while (!fila.empty()) {
        pair<Familia*, int> par = fila.front();
        fila.pop();

        Familia* familia = par.first;
        int nivel = par.second;

        // Imprimir separador de nível
        if (nivel != nivelAtual) {
            if (nivelAtual >= 0) cout << "\n";
            cout << "Geração " << nivel << ":\n";
            nivelAtual = nivel;
        }

        // Indentação
        for (int i = 0; i < nivel; i++) {
            cout << "  ";
        }

        // Exibir família
        cout << "• Família: ";
        if (familia->pai) cout << familia->pai->nome;
        else cout << "?";
        cout << " + ";
        if (familia->mae) cout << familia->mae->nome;
        else cout << "?";
        cout << "\n";

        // Exibir filhos
        for (size_t i = 0; i < familia->filhos.size(); i++) {
            for (int j = 0; j < nivel; j++) {
                cout << "  ";
            }
            cout << "  - Filho: " << familia->filhos[i]->nome << "\n";
        }

        // Adicionar famílias filhas na fila
        for (size_t i = 0; i < familia->familiasFilhas.size(); i++) {
            fila.push({familia->familiasFilhas[i], nivel + 1});
        }
    }
}

// ===== EXIBIÇÃO POR GERAÇÕES (SIMPLIFICADO) =====

void exibirGeracoes(ArvoreGenealogica* arvore) {
    if (!arvore || !arvore->raizFamilia) {
        cout << "Árvore vazia.\n";
        return;
    }

    queue<pair<Familia*, int>> fila;
    fila.push({arvore->raizFamilia, 0});

    int nivelAtual = -1;
    bool primeiraPessoa = true;

    while (!fila.empty()) {
        pair<Familia*, int> par = fila.front();
        fila.pop();

        Familia* familia = par.first;
        int nivel = par.second;

        if (nivel != nivelAtual) {
            if (nivelAtual >= 0) cout << "\n";
            cout << "Geração " << nivel << ": ";
            nivelAtual = nivel;
            primeiraPessoa = true;
        }

        // Listar todos os filhos desta família
        for (size_t i = 0; i < familia->filhos.size(); i++) {
            if (!primeiraPessoa) cout << ", ";
            cout << familia->filhos[i]->nome;
            primeiraPessoa = false;
        }

        // Adicionar famílias filhas na fila
        for (size_t i = 0; i < familia->familiasFilhas.size(); i++) {
            fila.push({familia->familiasFilhas[i], nivel + 1});
        }
    }
    cout << "\n";
}

// ===== CONTAGEM DE DESCENDENTES =====

int contarDescendentesFamilia(Familia* familia) {
    if (!familia) return 0;

    int total = familia->filhos.size(); // Filhos diretos

    // Contar descendentes das famílias filhas (recursivo)
    for (size_t i = 0; i < familia->familiasFilhas.size(); i++) {
        total += contarDescendentesFamilia(familia->familiasFilhas[i]);
    }

    return total;
}

// ===== CÁLCULO DE DISTÂNCIA/GRAU DE PARENTESCO =====

int calcularNivelPessoa(Pessoa* pessoa) {
    if (!pessoa) return -1;

    int nivel = 0;
    Familia* familiaAtual = pessoa->familiaOrigem;

    while (familiaAtual) {
        familiaAtual = familiaAtual->familiaAncestral;
        nivel++;
    }

    return nivel;
}

Familia* encontrarFamiliaAncestralComum(Pessoa* p1, Pessoa* p2) {
    if (!p1 || !p2) return nullptr;

    // Criar vetor com famílias ancestrais de p1
    vector<Familia*> ancestrais1;
    Familia* familiaAtual = p1->familiaOrigem;
    while (familiaAtual) {
        ancestrais1.push_back(familiaAtual);
        familiaAtual = familiaAtual->familiaAncestral;
    }

    // Buscar primeira família ancestral comum em p2
    familiaAtual = p2->familiaOrigem;
    while (familiaAtual) {
        for (size_t i = 0; i < ancestrais1.size(); i++) {
            if (ancestrais1[i] == familiaAtual) {
                return familiaAtual;
            }
        }
        familiaAtual = familiaAtual->familiaAncestral;
    }

    return nullptr;
}

int calcularDistanciaEntrePessoas(Pessoa* p1, Pessoa* p2) {
    if (!p1 || !p2) return -1;

    // Caso especial: mesma pessoa
    if (p1 == p2) return 0;

    // Caso especial: irmãos (mesma família origem)
    if (p1->familiaOrigem == p2->familiaOrigem) {
        return 2; // Irmãos: 2 graus de separação
    }

    Familia* ancestralComum = encontrarFamiliaAncestralComum(p1, p2);
    if (!ancestralComum) return -1; // Sem ancestral comum

    // Contar níveis até o ancestral comum
    int dist1 = 0;
    Familia* f1 = p1->familiaOrigem;
    while (f1 && f1 != ancestralComum) {
        f1 = f1->familiaAncestral;
        dist1++;
    }

    int dist2 = 0;
    Familia* f2 = p2->familiaOrigem;
    while (f2 && f2 != ancestralComum) {
        f2 = f2->familiaAncestral;
        dist2++;
    }

    return dist1 + dist2;
}

// ===== EXIBIR RELAÇÃO DE PARENTESCO =====

string obterRelacaoParentesco(Pessoa* p1, Pessoa* p2) {
    if (!p1 || !p2) return "Desconhecida";
    if (p1 == p2) return "Mesma pessoa";

    // Irmãos
    if (p1->familiaOrigem == p2->familiaOrigem) {
        return "Irmãos";
    }

    // Verificar se é pai/mãe e filho
    if (p1->familiaOrigem) {
        if (p1->familiaOrigem->pai == p2 || p1->familiaOrigem->mae == p2) {
            return "Filho(a) de";
        }
    }
    if (p2->familiaOrigem) {
        if (p2->familiaOrigem->pai == p1 || p2->familiaOrigem->mae == p1) {
            return "Pai/Mãe de";
        }
    }

    int dist = calcularDistanciaEntrePessoas(p1, p2);

    if (dist == 3) return "Tio(a)/Sobrinho(a)";
    if (dist == 4) return "Primos de 1º grau";
    if (dist > 4) return "Parentes distantes";

    return "Sem relação direta";
}

// ===== LIBERAR MEMÓRIA =====

void destruirFamiliaRecursivo(Familia* familia, unordered_map<Pessoa*, bool>& pessoasLiberadas) {
    if (!familia) return;

    // Liberar filhos (se ainda não foram liberados)
    for (size_t i = 0; i < familia->filhos.size(); i++) {
        Pessoa* filho = familia->filhos[i];
        if (pessoasLiberadas.find(filho) == pessoasLiberadas.end()) {
            delete filho;
            pessoasLiberadas[filho] = true;
        }
    }

    // Liberar pai e mãe (se ainda não foram liberados)
    if (familia->pai && pessoasLiberadas.find(familia->pai) == pessoasLiberadas.end()) {
        delete familia->pai;
        pessoasLiberadas[familia->pai] = true;
    }
    if (familia->mae && pessoasLiberadas.find(familia->mae) == pessoasLiberadas.end()) {
        delete familia->mae;
        pessoasLiberadas[familia->mae] = true;
    }

    // Recursivamente destruir famílias filhas
    for (size_t i = 0; i < familia->familiasFilhas.size(); i++) {
        destruirFamiliaRecursivo(familia->familiasFilhas[i], pessoasLiberadas);
    }

    // Liberar a própria família
    delete familia;
}

void destruirArvore(ArvoreGenealogica* arvore) {
    if (arvore) {
        unordered_map<Pessoa*, bool> pessoasLiberadas;
        destruirFamiliaRecursivo(arvore->raizFamilia, pessoasLiberadas);
        delete arvore;
    }
}
```

## 4. Vantagens do Modelo Família como Nó

### 4.1. Vantagens Gerais

1. **Biparentalidade Explícita**: Representa pai e mãe de forma consistente na
   estrutura
2. **Mantém Propriedades de Árvore**: Evita DAG de pessoas, mantendo travessias
   simples
3. **Fidelidade Real**: Modela mais fielmente as relações familiares reais
4. **Sem Duplicação**: Cada pessoa aparece uma única vez na estrutura
5. **Travessias Simples**: BFS/DFS funcionam normalmente em famílias

### 4.2. Vantagens para Árvore Genealógica

1. **Uniões Explícitas**: Casamentos/uniões são nós de primeira classe
2. **Consultas Bi-parentais**: Fácil acessar ambos os pais de qualquer pessoa
3. **Gerações Claras**: Famílias naturalmente representam gerações
4. **Cálculo de Parentesco**: Mais preciso com informação bi-parental
5. **Escalabilidade**: Suporta árvores genealógicas complexas

### 4.3. Vantagens Técnicas

1. **Gerenciamento de Memória**: Liberação recursiva padrão funciona
2. **Índice de Pessoas**: unordered_map permite busca O(1) por nome
3. **Flexibilidade**: Fácil adicionar novos relacionamentos
4. **Consistência**: Evita inconsistências de múltiplos pais em grafos
5. **Performance**: Travessias mantêm O(n) onde n é número de famílias

## 5. Desvantagens do Modelo Família como Nó

### 5.1. Desvantagens Gerais

1. **Complexidade Conceitual**: Mais abstrato que modelo de pessoa simples
2. **Estruturas Adicionais**: Requer struct Familia além de Pessoa
3. **Ligações Mais Complexas**: Precisa gerenciar ligações entre famílias
4. **Curva de Aprendizado**: Requer entendimento do conceito de família como nó
5. **Overhead de Memória**: Nós de família adicionais consomem memória

### 5.2. Desvantagens Técnicas

1. **Implementação Inicial**: Mais código para configurar estrutura básica
2. **Manutenção**: Requer cuidado ao adicionar/remover famílias
3. **Índice Necessário**: Busca por pessoa requer índice separado (ou travessia
   completa)
4. **Liberação de Memória**: Precisa rastrear pessoas já liberadas

## 6. Tipos de Uso e Aplicações

### 6.1. Árvores Genealógicas Completas

- Representar histórico familiar completo (ambos os pais)
- Calcular graus de parentesco precisos
- Exibir árvores genealógicas tradicionais

### 6.2. Sistemas de Genealogia Profissional

- Aplicações genealógicas que exigem biparentalidade
- Pesquisa histórica e ancestralidade
- Documentação de linhagens familiares

### 6.3. Pesquisa Genética

- Rastreamento de traços genéticos (requer ambos os pais)
- Análise de hereditariedade
- Estudos de consanguinidade

### 6.4. Sistemas de Heranças

- Cálculo de sucessão patrimonial
- Determinação de herdeiros legítimos
- Árvores de sucessão nobiliárquica

### 6.5. Estudos Demográficos

- Análise de estruturas familiares
- Pesquisa sociológica
- Estudos de população

## 7. Complexidade das Operações

| Operação                     | Complexidade | Observação                   |
| ---------------------------- | ------------ | ---------------------------- |
| **Criar pessoa**             | O(1)         | Alocar struct                |
| **Criar família**            | O(1)         | Alocar struct                |
| **Adicionar filho**          | O(1)         | push_back no vector          |
| **Ligar família filha**      | O(1)         | push_back no vector          |
| **Buscar por nome (índice)** | O(1)         | unordered_map lookup         |
| **Buscar sem índice**        | O(n\*m)      | n=famílias, m=filhos/família |
| **Listar ascendentes**       | O(h)         | h=altura (gerações)          |
| **Listar descendentes**      | O(f)         | f=famílias descendentes      |
| **Calcular distância**       | O(h)         | h=altura da árvore           |
| **Contar descendentes**      | O(f)         | f=famílias na subárvore      |
| **Exibir árvore (BFS)**      | O(f)         | Visita cada família uma vez  |
| **Destruir árvore**          | O(f+p)       | f=famílias, p=pessoas        |

## 8. Algoritmos de Travessia

### 8.1. DFS em Famílias (Depth-First Search)

```cpp
void dfsFamilias(Familia* familia) {
    if (!familia) return;

    // Visita a família atual
    cout << "Família: ";
    if (familia->pai) cout << familia->pai->nome;
    else cout << "?";
    cout << " + ";
    if (familia->mae) cout << familia->mae->nome;
    else cout << "?";
    cout << "\n";

    // Visita filhos
    for (size_t i = 0; i < familia->filhos.size(); i++) {
        cout << "  Filho: " << familia->filhos[i]->nome << "\n";
    }

    // Visita famílias filhas recursivamente
    for (size_t i = 0; i < familia->familiasFilhas.size(); i++) {
        dfsFamilias(familia->familiasFilhas[i]);
    }
}
```

**Características:**

- Explora famílias em profundidade
- Usa recursão (pilha implícita)
- Ordem: família raiz → famílias filhas → netas...

### 8.2. BFS em Famílias (Breadth-First Search)

```cpp
void bfsFamilias(Familia* raiz) {
    if (!raiz) return;

    queue<Familia*> fila;
    fila.push(raiz);

    while (!fila.empty()) {
        Familia* atual = fila.front();
        fila.pop();

        cout << "Família: ";
        if (atual->pai) cout << atual->pai->nome;
        else cout << "?";
        cout << " + ";
        if (atual->mae) cout << atual->mae->nome;
        else cout << "?";
        cout << "\n";

        // Adicionar todas as famílias filhas na fila
        for (size_t i = 0; i < atual->familiasFilhas.size(); i++) {
            fila.push(atual->familiasFilhas[i]);
        }
    }
}
```

**Características:**

- Explora famílias por geração
- Usa fila (queue do STL)
- Ordem: geração 0 → geração 1 → geração 2...

## 9. Quando Usar o Modelo Família como Nó?

### ✅ Use Família como Nó quando:

- Precisa representar ambos os pais de forma consistente
- Uniões/casamentos são importantes para o domínio
- Quer evitar DAG de pessoas (mantendo árvore simples)
- Cálculos de parentesco precisam considerar biparentalidade
- Exibir gerações de forma clara é importante
- Aplicação genealógica profissional ou acadêmica

### ❌ Não use Família como Nó quando:

- Apenas um progenitor importa (use modelo Pai Primário)
- Simplicidade é mais importante que fidelidade
- Recursos de memória são muito limitados
- Estrutura familiar é sempre monoparental
- Prototipagem rápida é necessária

## 10. Containers STL Úteis

### 10.1. vector (Lista Dinâmica)

```cpp
vector<Pessoa*> filhos;           // Filhos da família
vector<Familia*> familiasFilhas;  // Famílias descendentes

filhos.push_back(pessoa);      // Adicionar
filhos.size();                 // Tamanho
filhos[i];                     // Acesso por índice
filhos.empty();                // Verificar se vazio
```

### 10.2. unordered_map (Índice de Pessoas)

```cpp
unordered_map<string, Pessoa*> indicePessoas;

indicePessoas[nome] = pessoa;           // Adicionar
indicePessoas.find(nome);               // Buscar - O(1)
indicePessoas.count(nome);              // Verificar existência
```

### 10.3. queue (Fila para BFS)

```cpp
queue<Familia*> fila;

fila.push(familia);      // Adicionar no final
fila.front();            // Acessar primeiro
fila.pop();              // Remover primeiro
fila.empty();            // Verificar se vazio
```

## 11. Exemplo de Uso Completo

```cpp
int main() {
    // Criar árvore
    ArvoreGenealogica* arvore = criarArvore();

    // ===== GERAÇÃO 0 (Avós) =====
    Pessoa* avo = criarPessoa("João Silva", "01/01/1950", 'M');
    Pessoa* avo2 = criarPessoa("Maria Silva", "05/02/1952", 'F');

    Familia* familiaRaiz = criarFamilia(avo, avo2);
    definirRaizFamilia(arvore, familiaRaiz);

    adicionarPessoaNoIndice(arvore, avo);
    adicionarPessoaNoIndice(arvore, avo2);

    // ===== GERAÇÃO 1 (Pais) =====
    Pessoa* pai = criarPessoa("Carlos Silva", "15/03/1975", 'M');
    Pessoa* mae = criarPessoa("Ana Silva", "20/07/1977", 'F');
    Pessoa* tio = criarPessoa("Roberto Silva", "10/05/1980", 'M');
    Pessoa* tia = criarPessoa("Lucia Silva", "12/08/1982", 'F');

    // Adicionar filhos à família raiz
    adicionarFilhoNaFamilia(familiaRaiz, pai);
    adicionarFilhoNaFamilia(familiaRaiz, tio);

    adicionarPessoaNoIndice(arvore, pai);
    adicionarPessoaNoIndice(arvore, mae);
    adicionarPessoaNoIndice(arvore, tio);
    adicionarPessoaNoIndice(arvore, tia);

    // ===== GERAÇÃO 2 (Filhos) =====
    // Família do Carlos e Ana
    Familia* familia1 = criarFamilia(pai, mae);
    ligarFamiliaFilha(familiaRaiz, familia1);

    Pessoa* filho1 = criarPessoa("Pedro Silva", "10/05/2000", 'M');
    Pessoa* filho2 = criarPessoa("Julia Silva", "22/08/2002", 'F');

    adicionarFilhoNaFamilia(familia1, filho1);
    adicionarFilhoNaFamilia(familia1, filho2);

    adicionarPessoaNoIndice(arvore, filho1);
    adicionarPessoaNoIndice(arvore, filho2);

    // Família do Roberto e Lucia
    Familia* familia2 = criarFamilia(tio, tia);
    ligarFamiliaFilha(familiaRaiz, familia2);

    Pessoa* primo = criarPessoa("Lucas Silva", "05/12/2003", 'M');
    adicionarFilhoNaFamilia(familia2, primo);
    adicionarPessoaNoIndice(arvore, primo);

    // ===== EXIBIÇÕES =====

    cout << "=== ÁRVORE COMPLETA ===\n";
    exibirArvore(arvore);
    cout << "\n";

    cout << "=== DESCENDENTES DA FAMÍLIA RAIZ ===\n";
    exibirDescendentesFamilia(familiaRaiz);
    cout << "\n";

    cout << "=== ASCENDENTES DO PEDRO ===\n";
    exibirAscendentesPessoa(filho1);
    cout << "\n";

    // Contar descendentes
    int total = contarDescendentesFamilia(familiaRaiz);
    cout << "Total de descendentes da família raiz: " << total << "\n\n";

    // Calcular distância entre Pedro e Julia (irmãos)
    int dist1 = calcularDistanciaEntrePessoas(filho1, filho2);
    cout << "Distância entre " << filho1->nome << " e " << filho2->nome << ": "
         << dist1 << " graus (Relação: "
         << obterRelacaoParentesco(filho1, filho2) << ")\n";

    // Calcular distância entre Pedro e Lucas (primos)
    int dist2 = calcularDistanciaEntrePessoas(filho1, primo);
    cout << "Distância entre " << filho1->nome << " e " << primo->nome << ": "
         << dist2 << " graus (Relação: "
         << obterRelacaoParentesco(filho1, primo) << ")\n\n";

    // Exibir gerações
    cout << "=== GERAÇÕES ===\n";
    exibirGeracoes(arvore);
    cout << "\n";

    // Buscar pessoa
    cout << "=== BUSCA ===\n";
    Pessoa* encontrada = buscarPorNome(arvore, "Julia Silva");
    if (encontrada) {
        cout << "Pessoa encontrada: " << encontrada->nome
             << " (nascida em " << encontrada->dataNascimento << ")\n";
        if (encontrada->familiaOrigem) {
            cout << "  Pais: ";
            if (encontrada->familiaOrigem->pai)
                cout << encontrada->familiaOrigem->pai->nome;
            cout << " e ";
            if (encontrada->familiaOrigem->mae)
                cout << encontrada->familiaOrigem->mae->nome;
            cout << "\n";
        }
    }

    // Liberar memória
    destruirArvore(arvore);

    return 0;
}
```

### Saída Esperada:

```text
=== ÁRVORE COMPLETA ===
Geração 0:
• Família: João Silva + Maria Silva
  - Filho: Carlos Silva
  - Filho: Roberto Silva

Geração 1:
  • Família: Carlos Silva + Ana Silva
    - Filho: Pedro Silva
    - Filho: Julia Silva
  • Família: Roberto Silva + Lucia Silva
    - Filho: Lucas Silva

=== DESCENDENTES DA FAMÍLIA RAIZ ===
Descendentes da família (João Silva + Maria Silva):
Filhos:
  • Carlos Silva
  • Roberto Silva

Famílias descendentes:
├─ Família (Carlos Silva + Ana Silva)
    • Pedro Silva
    • Julia Silva
└─ Família (Roberto Silva + Lucia Silva)
    • Lucas Silva

=== ASCENDENTES DO PEDRO ===
Ascendentes de Pedro Silva:
  Nível 1:
    Pai: Carlos Silva
    Mãe: Ana Silva
  Nível 2:
    Pai: João Silva
    Mãe: Maria Silva

Total de descendentes da família raiz: 5

Distância entre Pedro Silva e Julia Silva: 2 graus (Relação: Irmãos)
Distância entre Pedro Silva e Lucas Silva: 4 graus (Relação: Primos de 1º grau)

=== GERAÇÕES ===
Geração 0: Carlos Silva, Roberto Silva
Geração 1: Pedro Silva, Julia Silva, Lucas Silva

=== BUSCA ===
Pessoa encontrada: Julia Silva (nascida em 22/08/2002)
  Pais: Carlos Silva e Ana Silva
```

## 12. Dicas de Implementação

### 12.1. Gerenciamento de Memória

```cpp
// ❌ EVITAR: Memory leak
Familia* f = criarFamilia(pai, mae);
// ...esquece de liberar

// ✅ BOM: Sempre liberar recursivamente
destruirArvore(arvore);  // Libera tudo corretamente
```

### 12.2. Verificação de Ponteiros Nulos

```cpp
// ❌ PERIGOSO: Não verificar nullptr
void funcao(Familia* f) {
    cout << f->pai->nome;  // CRASH se f ou f->pai == nullptr
}

// ✅ SEGURO: Sempre verificar
void funcao(Familia* f) {
    if (!f) return;
    if (f->pai) cout << f->pai->nome;
    else cout << "Pai desconhecido";
}
```

### 12.3. Uso de Índice para Busca

```cpp
// ❌ LENTO: Busca sem índice (O(n*m))
Pessoa* buscarSemIndice(Familia* raiz, string nome) {
    // Precisa percorrer toda árvore
}

// ✅ RÁPIDO: Busca com índice (O(1))
Pessoa* p = arvore->indicePessoas[nome];
```

### 12.4. Iteração sobre Famílias

```cpp
// ✅ Iterar sobre famílias filhas
for (size_t i = 0; i < familia->familiasFilhas.size(); i++) {
    processar(familia->familiasFilhas[i]);
}

// ✅ Ou iterador range-based (C++11)
for (Familia* familiaFilha : familia->familiasFilhas) {
    processar(familiaFilha);
}
```

## 13. Comparação: Modelo Pai Primário vs Família como Nó

| Aspecto                | Pai Primário              | Família como Nó          |
| ---------------------- | ------------------------- | ------------------------ |
| **Biparentalidade**    | Apenas 1 pai estrutural   | Pai + mãe explícitos     |
| **Complexidade**       | Simples                   | Moderada                 |
| **Fidelidade Real**    | Baixa (monoparental)      | Alta (biparental)        |
| **Travessias**         | DFS/BFS em Pessoas        | DFS/BFS em Famílias      |
| **Memória**            | Menor                     | Maior (nós de família)   |
| **Busca Pessoa**       | DFS O(n)                  | Índice O(1)              |
| **Cálculo Parentesco** | Impreciso (só um lado)    | Preciso (ambos os lados) |
| **Aplicações**         | Protótipos, casos simples | Genealogia profissional  |

## 14. Conclusão

O modelo **Família como Nó** é a escolha ideal quando a biparentalidade é
essencial para a aplicação. Apesar de ser conceitualmente mais complexo que o
modelo Pai Primário, ele oferece:

### Resumo:

- **Struct Pessoa**: Define dados individuais (nome, data, gênero)
- **Struct Familia**: Define união (pai + mãe + filhos)
- **Ponteiros entre Famílias**: Conectam gerações (familiaAncestral,
  familiasFilhas)
- **Índice de Pessoas**: unordered_map para busca rápida O(1)
- **Travessias em Famílias**: BFS/DFS percorrem famílias, não pessoas
- **Sem DAG**: Cada pessoa aparece uma vez, estrutura mantém propriedades de
  árvore

### Para o projeto:

1. Use `Familia` como nó da árvore, não `Pessoa`
2. Mantenha índice `unordered_map<string, Pessoa*>` para buscas rápidas
3. Implemente travessias (BFS/DFS) sobre famílias
4. Ao calcular parentesco, considere ambos os lados da família
5. Libere memória cuidadosamente (rastrear pessoas já liberadas)
6. Use vector para listas de filhos e famílias filhas

## 15. Referências e Leitura Adicional

- CORMEN, T. H. et al. _Introduction to Algorithms_. 3rd ed. MIT Press, 2009.
- SEDGEWICK, R.; WAYNE, K. _Algorithms_. 4th ed. Addison-Wesley, 2011.
- WEISS, M. A. _Data Structures and Algorithm Analysis in C++_. 4th ed.
  Pearson, 2013.
- cppreference.com: STL Containers (vector, queue, unordered_map, pair)
- GeeksforGeeks: N-ary Tree Data Structure
- Family Tree Data Structures:
  https://stackoverflow.com/questions/tagged/genealogy
