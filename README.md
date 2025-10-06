Trabalho 1

Entrega: até 03/11/2025

Objetivo: Implementar um sistema/software em C++ para representar e manipular
uma árvore genealógica, aplicando estruturas de dados de dados hierárquicas,
como árvores, ou filas e listas.

Funcionalidades Obrigatórias: 1- Adicionar pessoa (nome, data de nascimento,
gênero). 2- Definir relações (pai, mãe, filho, filha). 3- Exibir ASCENDENTES e
DESCENDENTES de uma pessoa. 4- Listar toda a árvore a partir de um ancestral. 5-
Buscar pessoa pelo nome. (hash?)

Funcionalidades Opcionais:

- Exibir nível de parentesco entre duas pessoas (distância entre elas, "grau"
  ver Obs).
- Contagem de descendentes diretos e indiretos. (contar filhos, netos,
  bisnetos...)
- Mostrar gerações por nível (geração). (mesma geração, mesmo nível)
- Gravar e carregar de um arquivo de texto.

Avaliação: 40% - Estrutura de Dados corretas e adequadas. 30% - Funcionalidades
obrigatórias. 20% - Clareza e organização do código. 10% - Documentação e
testes. 30% - Opcionais (apenas, se obrigatórias completas).

Listagem: tree model

Pessoa |-- filho1 | |-- neto1 | |-- neto2 |-- filho2 |-- neto3

Obs: primo é parente de quarto grau. Tio é parente de terceiro grau. Avô é
parente de segundo grau. Pai é parente de primeiro grau. \*/
