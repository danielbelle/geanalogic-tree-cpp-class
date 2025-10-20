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