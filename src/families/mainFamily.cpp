#include <vector>
#include <string>
#include <iostream>

using namespace std;

import struct Familia;
import struct Pessoa;

// Função para criar uma nova pessoa
Pessoa* criarPessoa(const string& nome, const string& data, char genero, Familia* familiaOrigem) {

  Pessoa* novaPessoa = new Pessoa;

  novaPessoa->nome = nome;
  novaPessoa->dataNascimento = data;
  novaPessoa->genero = genero;
  novaPessoa->familiaOrigem = familiaOrigem;

  return novaPessoa;
}

Familia* criarFamilia(Pessoa* pai, Pessoa* mae, Familia* familiaOrigem) {

  Familia* novaFamilia = new Familia;

  novaFamilia->pai = pai;
  novaFamilia->mae = mae;
  novaFamilia->familiaOrigem = familiaOrigem;

  return novaFamilia;
}

// Função para adicionar filho
void adicionarFilho(Pessoa* pai, Pessoa* filho) {
  if (pai && filho) {
    pai->filhos.push_back(filho);
    filho->pai = pai;
  }
}


void imprimirArvoreRec(const Pessoa* no, const std::string& prefixo, bool ultimo) {
  if (!no) return;
  std::cout << prefixo << (ultimo ? "└── " : "├── ")
    << no->nome << " (" << no->dataNascimento << ", " << no->genero << ")\n";
  for (std::size_t i = 0; i < no->filhos.size(); ++i) {
    bool eUltimo = (i + 1 == no->filhos.size());
    imprimirArvoreRec(no->filhos[i], prefixo + (ultimo ? "    " : "│   "), eUltimo);
  }
}

void imprimirArvore(const Pessoa* raiz) {
  if (!raiz) return;
  std::cout << raiz->nome << " (" << raiz->dataNascimento << ", " << raiz->genero << ")\n";
  for (std::size_t i = 0; i < raiz->filhos.size(); ++i) {
    bool eUltimo = (i + 1 == raiz->filhos.size());
    imprimirArvoreRec(raiz->filhos[i], "", eUltimo);
  }
}

void destruirArvore(Pessoa* raiz) {
  if (!raiz) return;
  for (Pessoa* f : raiz->filhos) destruirArvore(f);
  delete raiz;
}

int main() {
  Pessoa* avo = criarPessoa("Avô", "1945-01-01", 'M');
  Pessoa* pai = criarPessoa("Pai", "1970-06-15", 'M');
  Pessoa* tia = criarPessoa("Tia", "1972-09-20", 'F');
  adicionarFilho(avo, pai);
  adicionarFilho(avo, tia);

  Pessoa* filho1 = criarPessoa("Filho", "1995-03-10", 'M');
  Pessoa* filha2 = criarPessoa("Filha", "1998-11-05", 'F');
  adicionarFilho(pai, filho1);
  adicionarFilho(pai, filha2);

  imprimirArvore(avo);

  destruirArvore(avo);
  return 0;
}