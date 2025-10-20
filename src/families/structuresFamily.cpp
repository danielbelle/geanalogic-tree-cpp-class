#include <iostream>
#include <vector>

using namespace std;

struct PessoaF {
  string nome;
  string dataNascimento;
  char genero;                 // 'M' ou 'F'
  struct Familia* familiaOrigem; // ponteiro para a família onde é filho, pode ser nullptr
};

struct Familia {
  Pessoa* pai;                // pode ser nullptr
  Pessoa* mae;                // pode ser nullptr
  vector<Pessoa*> filhos;     // filhos desta união
  vector<Familia*> familiasFilhas; // ligações em árvore entre famílias caso existam
};