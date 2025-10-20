# Estudo de Caso: Generic Tree (Árvore Genérica / N-ária)

> Atualização: este documento descreve duas formas de modelar uma árvore
> genealógica em C++:
>
> 1. Modelo original "Pai Primário" (cada pessoa tem no máximo um ponteiro para
>    um progenitor na estrutura de árvore) — mantém estrutura de árvore estrita.
> 2. Modelo complementar "Família como Nó" — permite representar pai + mãe de
>    forma consistente sem transformar a estrutura em um DAG de pessoas
>    (preserva travessias em árvore/floresta).

Os dois modelos coexistem conceitualmente: o modelo Pai Primário é simples e
mantém propriedades de árvore; o modelo Família como Nó é uma
extensão/reformulação que preserva travessias em árvore ao mover a relação
biparental para um nó "Família".

---

## 1. Definição (resumo)

- Generic Tree / N-ary Tree: cada nó pode ter 0..N filhos.
- Em genealogia há duas formas comuns de mapear relações bi-parentais mantendo
  comportamentos de árvore:
  - Pai Primário: cada pessoa tem um ponteiro "pai" (ou "progenitor primário") —
    simples, mantém árvore.
  - Família como Nó: cada nó representa uma família (pai + mãe) e os filhos são
    filhos dessa família — a árvore é entre famílias/gerações.

---

## 2. Modelo A — Pai Primário (modelo atual)

Descrição:

- Cada Pessoa tem no máximo um ponteiro que é usado como relação hierárquica
  “pai primário”.
- Pode-se guardar o outro progenitor como atributo simples (string ou ponteiro
  não usado para travessia) ou ignorá-lo.
- Mantém estrutura de árvore (cada nó tem no máximo um pai estrutural).

Vantagens:

- Simplicidade; travessias (DFS/BFS) e liberação de memória seguem o padrão de
  árvore.
- Fácil implementação e compatível com código já existente.

Desvantagens:

- Informação bi-parental completa não fica expressa na topologia da árvore
  (perde-se vínculo explícito em travessias).
- Se precisar de consultas que usem os dois pais como ascendência, será preciso
  lógica adicional.

Exemplo mínimo (estrutura atual simplificada):

```cpp
// Exemplo conceitual - Pai primário
struct Pessoa {
    string nome;
    string dataNascimento;
    char genero;        // 'M' ou 'F'
    Pessoa* pai;        // ponteiro para o progenitor primário
    vector<Pessoa*> filhos;
};

```

Operações típicas (mantidas do estudo original): criar pessoa, adicionar filho
(push_back), buscar por nome (DFS), exibir por gerações (BFS), contar
descendentes, destruir recursivamente.

Observação prática:

- Se quiser registrar o outro progenitor (por exemplo, a mãe) sem mudar a
  topologia, pode adicionar um campo `string maeNome` ou `Pessoa* maeInfo` que
  NÃO participa da travessia estrutural. Isso mantém a árvore.

---

## 3. Modelo B — Família como Nó (recomendado como complemento)

Ideia:

- Em vez de fazer cada Pessoa ter dois pais na topologia, cria-se um nó
  "Familia" que contém ponteiros para pai, mãe e lista de filhos.
- A árvore/floresta é construída entre Famílias por ligação entre a família de
  origem de um progenitor e a família seguinte (ex.: família do pai → família do
  filho).
- Cada Pessoa referencia sua Familia de origem (onde ela é filho), mas a
  travessia por gerações percorre Famílias.

Vantagens:

- Representa explicitamente a biparentalidade sem transformar o grafo de pessoas
  em DAG.
- Mantém propriedades de árvore/floresta para travessias, exibição por gerações
  e gerenciamento de memória.
- Evita duplicação de nós Pessoa e evita necessidade de visited/visited-delete
  complexos.

Desvantagens:

- Modelo um pouco mais indireto (introduz novo conceito de nó Família).
- Requer adaptar funções para trabalhar com nós Família ao listar gerações.

Estruturas principais (exemplo):

```cpp

struct Pessoa {
    string nome;
    string dataNascimento;
    char genero;                 // 'M' ou 'F'
    struct Familia* familiaOrigem; // ponteiro para a família onde é filho
    // outros dados pessoais...
};

struct Familia {
    Pessoa* pai;                // pode ser nullptr
    Pessoa* mae;                // pode ser nullptr
    vector<Pessoa*> filhos;     // filhos desta união
    // opcional: ponteiro para a familia ancestral (p.ex. onde pai/mae são filhos)
    vector<Familia*> familiasFilhas; // ligações em árvore entre famílias (opcional)
};
```

Exemplo de criação e ligação:

```cpp

Familia* criarFamilia(Pessoa* pai, Pessoa* mae) {
    Familia* f = new Familia();
    f->pai = pai;
    f->mae = mae;
    return f;
}

void adicionarFilhoNaFamilia(Familia* f, Pessoa* filho) {
    if (!f || !filho) return;
    f->filhos.push_back(filho);
    filho->familiaOrigem = f;
}
```

Como formar a árvore de famílias:

- A raiz pode ser uma família fundada por um progenitor sem origem conhecida.
- Para cada pessoa que se torna progenitor em outra família, podemos ligar a
  família do progenitor à nova família
  (familia->familiasFilhas.push_back(novaFamilia)). Assim temos uma árvore de
  famílias onde cada aresta representa "esta família gerou aquela família
  (através de um filho que se tornou pai/mãe)".

Travessias e consultas:

- Para exibir gerações: BFS sobre Famílias, listando os filhos de cada família.
- Para buscar uma pessoa por nome: pode manter um índice global
  (map<string,Pessoa\*>) ou percorrer Famílias/filhos.
- Para ascendentes: subir pela familiaOrigem de um progenitor e então pela
  famíliaOrigem dos pais (consistente com árvore de famílias).

Liberação de memória:

- Liberar famílias recursivamente (garantindo que cada Pessoa seja liberada
  apenas uma vez).
- Se Famílias formarem uma árvore/floresta, destruição recursiva padrão
  funciona.

---

## 4. Comparação rápida (Pai Primário vs Família como Nó)

- Informação bi-parental:

  - Pai Primário: não representada topologicamente (só um pai estrutural).
  - Família como Nó: representada explicitamente (pai + mãe no mesmo nó).

- Estrutura:

  - Pai Primário: Árvore de Pessoas (cada Pessoa tem no máximo 1 pai
    estrutural).
  - Família como Nó: Árvore de Famílias; Pessoas são itens ligados às Famílias.

- Travessias:

  - Pai Primário: simples (DFS/BFS em Pessoas).
  - Família como Nó: simples (DFS/BFS em Famílias), mantém propriedades de
    árvore.

- Complexidade de implementação:
  - Pai Primário: mais simples.
  - Família como Nó: ligeiramente mais abstrato, mas mais fiel à realidade
    biparental para muitas aplicações.

---

## 5. Exemplo combinado (modo de migração)

Estratégia prática para projeto:

1. Comece com modelo Pai Primário (se já houver código).
2. Se precisar de biparentalidade nas travessias (sem DAG), introduza Famílias
   como nó e mantenha compatibilidade:
   - Cada Pessoa continua existindo.
   - Crie Famílias para uniões conhecidas.
   - Atualize funções de exibição por gerações para operar em Famílias
     (opcionalmente mantenha versões que operam em Pessoas).
3. Para consultas rápidas por pessoa, use um índice
   (unordered_map<string,Pessoa\*>) — independente do modelo.

Exemplo de função de exibição por gerações no modelo Família como Nó
(conceitual):

```cpp

void exibirGeneracoesPorFamilias(Familia* raiz) {
    if (!raiz) return;
    queue<pair<Familia*, int>> q;
    q.push({raiz, 0});
    while (!q.empty()) {
        auto [f, nivel] = q.front(); q.pop();
        cout << "Geração " << nivel << ": ";
        for (size_t i = 0; i < f->filhos.size(); ++i) {
            if (i) cout << ", ";
            cout << f->filhos[i]->nome;
        }
        cout << "\n";
        for (Familia* filhoF : f->familiasFilhas) q.push({filhoF, nivel + 1});
    }
}
```

---

## 6. Recomendações finais

- Se seu objetivo é manter compatibilidade com o código atual e simplicidade →
  mantenha Pai Primário e, se precisar, armazene a mãe como dado adicional (não
  topológico).
- Se precisar modelar genealogia completa (pai + mãe) sem introduzir DAG de
  pessoas → adote o padrão Família como Nó. É especialmente útil para aplicações
  que:
  - exibem gerações,
  - mostram uniões (casamentos) explicitamente,
  - geram árvores sem duplicação de Pessoas.
- Ambas abordagens podem coexistir: mantenha o modelo Pai Primário para
  travessias leves e adicione Famílias como estrutura complementar quando
  precisarem de biparentalidade topológica.

---
