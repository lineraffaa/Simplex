#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <regex>
#include <vector>
#include <utility>
using namespace std;

// ifstream is("texto.txt", ios::in);
string linha;
smatch m;

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
    vector<string> c;

    while (getline(arquivo, linha))
    {

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
        while (regex_search(inicio, fim, m, regex(R"(<=|>=|=)")))
        {
            string s = m[0];
            folga.push_back(s);
            // cout << " | Variável de folga: " << s << endl;
            inicio = m.suffix().first;
        }
    }
    return folga;
}

float multiplique(float matriz[2][2], float matrizV[2][2]){
    float multi[2][2] = {0};
    int i, j;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
        for (int k = 0; k < 2; k++)
        {
            multi[i][j] += matriz[i][k]  * matrizV[k][j] ;
           
        }
        
           

        }
        
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {

            cout << multi[i][j] <<endl;
        }}

    return multi[i][j];
    
}




int main()
{
   float matriz1[2][2] = {
        {1, 2},
        {3, 4}
    };

    float matriz2[2][2] = {
        {5, 6},
        {7,8}
    };
    string nomeArquivo = "texto.txt";

    float matriz[2][3];
  //  string matriz2[100][100];

    vector<float> numerosFloat;
    vector<int> numerosInt;
    vector<int> valoresExtraidos;
    vector<string> variaveis;
    vector<string> vfolga;
    //   vector<string> sinaisEncontrados;

    // Processamento dos dados
    pair<vector<int>, vector<float>> resultado = intefloat(nomeArquivo);
    pair<vector<int>, vector<string>> resultadoVariavel = numeroevarialvel(nomeArquivo);
    vector<string> folga = variaveldefolga(nomeArquivo);

    // Atribuições dos pares
    numerosInt = resultado.first;
    numerosFloat = resultado.second;
    valoresExtraidos = resultadoVariavel.first;
    variaveis = resultadoVariavel.second;

    int index = 0;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
           
        if (index < valoresExtraidos.size())
        { 
            
            matriz[i][j] = valoresExtraidos[index];
            //cout  << matriz[i][j] << " ";
            index++;
            
        }

    }
    cout << endl;
}
multiplique(matriz1,matriz2);

/*int index2 = 0;
for (int i = 0; i < 2; i++)
{
    for (int j = 0; j < 3; j++)
    {
      
    if (index2 < variaveis.size())
    { 
        
        matriz2[i][j] = variaveis[index2];
        //cout << matriz2[i][j] << " ";
        index2++;
        
    }

}
cout << endl;
}*/


    return 0;
}