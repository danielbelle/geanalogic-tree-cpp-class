# Trabalho 1 - Árvore Genealógica

## Definição do projeto

Implementar um sistema/software em C++ para representar e manipular uma árvore
genealógica.

## Visão geral

**Entrega:** até 03/11/2025

## Objetivo

Implementar um sistema/software em C++ para representar e manipular uma árvore
genealógica, aplicando estruturas de dados hierárquicas, como árvores, ou filas
e listas.

## Funcionalidades Obrigatórias

1. Adicionar pessoa (nome, data de nascimento, gênero).
2. Definir relações (pai, mãe, filho, filha).
3. Exibir ASCENDENTES e DESCENDENTES de uma pessoa.
4. Listar toda a árvore a partir de um ancestral.
5. Buscar pessoa pelo nome.

## Funcionalidades Opcionais

- Exibir nível de parentesco entre duas pessoas (distância entre elas, "grau"
  ver Obs).
- Contagem de descendentes diretos e indiretos. (contar filhos, netos,
  bisnetos...)
- Mostrar gerações por nível (geração). (mesma geração, mesmo nível)
- Gravar e carregar de um arquivo de texto.

## Critérios de Avaliação

- 40% - Estrutura de Dados corretas e adequadas.
- 30% - Funcionalidades obrigatórias.
- 20% - Clareza e organização do código.
- 10% - Documentação e testes.
- 30% - Opcionais (apenas, se obrigatórias completas).

## Exemplo de listagem (modelo em árvore)

Listagem: tree model

```text
Pessoa
├─ filho1
│  ├─ neto1
│  └─ neto2
└─ filho2
    └─ neto3
```

## Observações

Obs: primo é parente de quarto grau. Tio é parente de terceiro grau. Avô é
parente de segundo grau. Pai é parente de primeiro grau.

# Abordagem à solução:

Separando-as em:

- Funcionais F1 a F5
- Opcionais O1 a O4

# TODO

- [x] Estudar N-ary tree / generic tree tree/florest;
- [x] Ver como aplicar struct na solução;
- [x] Entender para que ela é otimizada;
- [x] Como montar a estrutura da arvore generica em c++;
- [ ] Entender como fazer um CRUD na arvore generica;
- [ ] Adicionar pessoa (nome, data nasci e genero) (F1);
- [ ] Adicionar relacional (cima ou baixo) node (F2);
- [ ] Encontrar pessoa (F5);
- [ ] Exibir todos os acima e abaixo da pessoa pesquisada (F3);
- [ ] Exibir todos os descendentes a partir da pessoa pesquisada (F4);
- [ ] Criar arquivo com dados para alimentar a generic tree (O4);
- [ ] Distancia entre 2 parentes (O1);
- [ ] Mostrar gerações por nível (filhos ou netos ou bisnetos) (O3);
- [ ] Contagem de descendentes diretos e indiretos (O2);
- [ ] Ajustar testes unitários;
- [ ] Testes de funcionamento completo;
- [ ] Testes de feature;
- [ ] Finalizar Documentação;
