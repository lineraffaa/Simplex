#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <regex>
#include <vector>
#include <utility>
using namespace std;

string nomeArquivo = "texto.txt";
// ifstream is("texto.txt", ios::in);
string linha;
smatch m;
int L, C;
pair<vector<int>, vector<float>> intefloat(const string &Arquivo)
{

    ifstream arquivo(Arquivo);
    vector<int> seq;
    vector<float> F;
    while (getline(arquivo, linha))
    {

        stringstream strings(linha);
        string token;

        while (strings >> token)
        {
            if (regex_match(token, m, regex(R"(^[+-]?[0-9]+$)")))
            {
                int numero = stoi(m[0]);
                seq.push_back(numero);

                // cout << "Número encontrado: " << numero << endl;
            }

            if (regex_match(token, m, regex(R"(^[+-]?[0-9]*\.[0-9]+$)")))
            {
                float numero = stof(m[0]);
                F.push_back(numero);

                //  cout << "Número decimal encontrado: " << numero << endl;
            }
        }
    }

    return {seq, F};
}

pair<vector<int>, vector<string>> numeroevarialvel(const string &Arquivo)
{
    ifstream arquivo(Arquivo);
    vector<int> n;
    vector<string> linhas;
    vector<string> c;

    string temp;

    // Armazena todas as linhas do arquivo
    while (getline(arquivo, linha)) {
    
    //armazena todas as linhas do arquivo

   
        auto inicio = linha.cbegin();
        auto fim = linha.cend();
    

        while (regex_search(inicio, fim, m, regex(R"(([+-]?[0-9]*\.?[0-9]+)?(x[0-9]+))")))
        {
            string coef = m[1].matched ? m[1].str() : "1";
            if (coef == "+" || coef == "-")
                coef += "1";
            int numero = stoi(coef);
            n.push_back(numero);

            string variavel = m[2];
            c.push_back(variavel);
            // Verifica se o grupo 1 da regex (o número) foi de fato encontrado na string. [m[1].matched]
            // se sim retorna uma string, m[1].str(). se não retorna 1, porque x2 = 1x2
            // cout << "Número: " << coef << " | Variável: " << variavel << endl;
            inicio = m.suffix().first; // Avança para o próximo match... prepara o regex_search pra buscar o próximo match a partir dali
        }
    }

    return {n, c};
}

vector<string> variaveldefolga(const string &Arquivo)
{
    ifstream arquivo(Arquivo);
    vector<string> folga;
    while (getline(arquivo, linha))
    {

        auto inicio = linha.cbegin();

        auto fim = linha.cend();
        while (regex_search(inicio, fim, m, regex(R"(<|>|=)")))
        {
            string s = m[0];
            folga.push_back(s);
            // cout << " | Variável de folga: " << s << endl;
            inicio = m.suffix().first;
        }
    }
    return folga;
}

float multiplique(float matriz[2][2], float matrizV[2][2])
{
    float multi[2][2] = {0};
    int i, j;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                multi[i][j] += matriz[i][k] * matrizV[k][j];
            }
        }
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {

            cout << multi[i][j] << endl;
        }
    }

    return multi[i][j];
}

int linhas(const string &Arquivo)
{
    ifstream arquivo(Arquivo);
    int i = 0;
    while (getline(arquivo, linha))
    {
        i++;
    }
    int cont = i;

    cont = cont - 1;

    return cont;
}
// mais 1 para mais uma variavel
int colunas(const string &Arquivo)
{

    fstream arquivo(Arquivo);

    vector<string> folga = variaveldefolga(nomeArquivo);
    int cont2 = 0;
    while (getline(arquivo, linha))
    {

        auto inicio = linha.cbegin();

        auto fim = linha.cend();

            if (folga[cont2] != "=>" || folga[cont2] == "<=")
            {
                cout << folga[cont2] << " Variaveis de Folga \n";
                cont2++;
                cont2 = cont2 + 1;
            }

        
    }

    return cont2;
}

int contarRestricoes(const string &Arquivo) {
    ifstream arquivo(Arquivo);
    int total = 0;
    while (getline(arquivo, linha)) total++;
    return total - 2; // desconsidera objetivo e última linha
}

int main()
{

   

    vector<string> variaveis;
    vector<int> numero;
    int lines;
  // cout << colunas(nomeArquivo) << endl; 
    
    pair<vector<int>, vector<string>> resultadoVariavel = numeroevarialvel(nomeArquivo);
    vector<string> Vfolga = variaveldefolga(nomeArquivo);

    variaveis = resultadoVariavel.second;
    numero = resultadoVariavel.first;

    int cont = 0;

    for (int i = 0; i < lines - 1; i++)
    {

        cout << variaveis[i] << "  Variaveis\n ";

        cont++;
        // soma os valores das variaveis com as variaveis de folga
    }
    cout << cont << endl;

    for (int i = 0; i < numero.size(); i++)
    {
        cout << numero[i] << "numeros"<<endl;
        
    }
    

    int cont2 = 0;
    int v = 0;
    for (int i = 0; i < Vfolga.size(); i++)
    {

        if (Vfolga[i] == ">" || Vfolga[i] == "<")
        {
            cout << Vfolga[i] << " Variaveis de Folga \n";
            cont2++;
           
        }

        
    }

    cout << cont + cont2 << "colunas" << endl;
    

    L = cont;
    C = cont + cont2;
    
  

    int idx = 0;
    vector<vector<float>> MatrizA(L, vector<float>(C, 0.0)); 
   

    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < C; j++)
        {
            MatrizA[i][j] = numero[idx++];
        }
        


    }
    

    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            cout << setw(6) << MatrizA[i][j] << "";
        }
        cout << endl;
    }
    

    

    // 3 linhas, coluna em relação as variaveis
    // aumenta numero de colunas de for maior e menor





    return 0;
}