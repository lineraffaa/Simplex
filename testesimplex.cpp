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
int contador;

pair<vector<int>, vector<float>> intefloat(const string &Arquivo)
{
    cout << "Aqui \n";

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

                cout << "Número encontrado: " << numero << endl;
            }

           else if (regex_match(token, m, regex(R"(^([+-]?[0-9]*\.[0-9]+)$)")))
            {
                float numero = stof(m[1].str());
                F.push_back(numero);
                cout << "Número decimal encontrado: " << numero << endl;
            }
        }
    }

    return {seq, F};
}



pair<vector<float>, vector<string>> floatvarialvel(const string &Arquivo)
{
    ifstream arquivo(Arquivo);
    vector<float> n;
    vector<string> linhas;
    vector<string> c;

    string temp;

    // Armazena todas as linhas do arquivo
    while (getline(arquivo, linha))
    {

        // armazena todas as linhas do arquivo

        auto inicio = linha.cbegin();
        auto fim = linha.cend();

        while (regex_search(inicio, fim, m, regex(R"(([+-]?(?:[0-9]+(?:\.[0-9]+)?)?)(x[0-9]+))"

                                                  )))
        {
            string coefstr = m[1].str();

            float coef;

            if (coefstr.empty() || coefstr == "+")
            {
                coef = 1.0;
            }
            else if (coefstr == "-")
            {
                coef = -1.0;
            }
            else
            {
                coef = stof(coefstr); // Converte corretamente para float
            }

            string variavel = m[2].str();
            n.push_back(coef);
            c.push_back(variavel);

            inicio = m.suffix().first;
        }
    }

    return {n, c};
}

pair<vector<float>, vector<string>> numeroevarialvel(const string &Arquivo)
{
    ifstream arquivo(Arquivo);

    vector<float> n;
    vector<string> linhas;
    vector<string> c;

    string temp;

    // Armazena todas as linhas do arquivo
    while (getline(arquivo, linha))
    {

        // armazena todas as linhas do arquivo

        auto inicio = linha.cbegin();
        auto fim = linha.cend();

        while (regex_search(inicio, fim, m, regex(R"(([+-]?[0-9]*\.?[0-9]+)?(x[0-9]+))")))
        {
            string coefstr = m[1].str();

            float coef;

            if (coefstr.empty() || coefstr == "+")
            {
                coef = 1.0;
            }
            else if (coefstr == "-")
            {
                coef = -1.0;
            }
            else
            {
                coef = stof(coefstr); // Converte corretamente para float
            }

            string variavel = m[2].str();
            n.push_back(coef);
            c.push_back(variavel);

            inicio = m.suffix().first;
        }
    }

    return {n, c};
}

/*pair<vector<int>, vector<string>> numeroevarialvel(const string &Arquivo)
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


        while (regex_search(inicio, fim, m, regex(regex(R"(([+-]?[0-9]*\.?[0-9]+)?(x[0-9]+))")
    )))
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
}*/

vector<string> variaveldefolga(const string &Arquivo)
{
    ifstream arquivo(Arquivo);
    vector<string> folga;
    while (getline(arquivo, linha))
    {

        auto inicio = linha.cbegin();

        auto fim = linha.cend();
        while (regex_search(inicio, fim, m, regex(R"((<=|>=|<|>|=))")))
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
/*int colunas(const string &Arquivo)
{

    fstream arquivo(Arquivo);

    vector<string> folga = variaveldefolga(nomeArquivo);
    int cont2 = 0;
    while (getline(arquivo, linha))
    {

        auto inicio = linha.cbegin();

        auto fim = linha.cend();

            if (folga[cont2] != ">" || folga[cont2] == "<")
            {
                cout << folga[cont2] << " Variaveis de Folga \n";
                cont2++;
                cont2 = cont2 + 1;
            }


    }

    return cont2;
}
*/

int contarRestricoes(const string &Arquivo)
{
    ifstream arquivo(Arquivo);

    int total = 0;

    while (getline(arquivo, linha))
    {

        total++;
    }
    return total - 1; // desconsidera objetivo e última linha
}

int main()
{

    vector<string> variaveis;
    vector<float> numero;
    vector<float> numerof;
    vector<string> fn;
    vector<int> nint;
    vector<float> nf;
    int lines = 0;
    lines = contarRestricoes(nomeArquivo);
    // cout << colunas(nomeArquivo) << endl;
    cout << lines << "linha \n";
    pair<vector<float>, vector<string>> resultadoVariavel = numeroevarialvel(nomeArquivo);
    vector<string> Vfolga = variaveldefolga(nomeArquivo);

    cout << "Agora \n";
    pair<vector<int>, vector<float>> numint = intefloat(nomeArquivo);

    pair<vector<float>, vector<string>> resulf = floatvarialvel(nomeArquivo);

    variaveis = resultadoVariavel.second;
    numero = resultadoVariavel.first;
    numerof = resulf.first;
    fn = resulf.second;

    nint = numint.first;
    nf = numint.second;

    int nVariaveis = 0;
    ifstream arquivo(nomeArquivo);
    string primeiraLinha;
    if (getline(arquivo, primeiraLinha))
    {
        auto inicio = primeiraLinha.cbegin();
        auto fim = primeiraLinha.cend();

        while (regex_search(inicio, fim, m, regex(R"(([+-]?[0-9]*\.?[0-9]+)?(x[0-9]+))")))
        {
            nVariaveis++;
            // para contar quado as variaveis são encontradas
            inicio = m.suffix().first;
        }
    }
    arquivo.close();

    vector<string> folga;
    int indx = 0;

    int cont = 0;

    for (int i = 0; i < lines - 1; i++)
    {

        cout << nint[i] << "  numeros intiros";

        // soma os valores das variaveis com as variaveis de folga
    }

    for (int i = 0; i < lines - 1; i++)
    {

        cout << nf[i] << "  numeros decimal";
    }

    for (int i = 0; i < lines - 1; i++)
    {

        cout << variaveis[i] << "  Variaveis\n ";

        cont++;
        // soma os valores das variaveis com as variaveis de folga
    }
    cout << cont << endl;

    /*   for (int i = 0; i < numero.size(); i++)
      {
          cout << numero[i] << "numeros"<<endl;

          // os numeros em relação as variaveis

      }*/

    for (int i = 0; i < numerof.size(); i++)
    {
        cout << numerof[i] << "numeros float\n";
    }

    int cont2 = 0;
    int v = 0;
    vector<float> f;
    vector<string> vf;
   
    for (int i = 0; i < Vfolga.size(); i++)
    {
        // cout << Vfolga[i] << " Variaveis de Folga2 \n";

        if (Vfolga[i] == ">=" || Vfolga[i] == "<=")
        {
            //  cout << Vfolga[i] << " Variaveis de Folga \n";

            cont2++;
        }
        //  cout << vf[i] << "vf" << endl;
    }

    cout << "AAAAaaqqqui \n";
    cout << Vfolga.size() << endl;
    cout << f.size() << endl;

    for (int i = 0; i < Vfolga.size(); i++)
    {

        if (Vfolga[i] == "<=")
        {
            f.push_back(1);

            //   cout << Vfolga[i] << "AAaAqqui" << endl;
        }
        else if (Vfolga[i] == ">=")
        {
            f.push_back(-1);
        }
        else
        {
            f.push_back(0);
        }
        // cout << f[i] << "valores de f \n";
    }

    L = lines;
    C = nVariaveis + cont2;

    cout << nVariaveis << "nv \n";

    int idx = nVariaveis;
    vector<vector<float>> MatrizA(L, vector<float>(C, 0.0));

    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < nVariaveis; j++)
        {
            if (idx < numerof.size())
            {

                MatrizA[i][j] = numerof[idx++];
            }
        }
    }

    /*int  idxx = 0;
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < nVariaveis; j++)
        {
            if (idxx < numerof.size()) {
                MatrizA[i][j] = numerof[idxx++];
            }

        }

    }*/

    // Agora insere 1 ou -1 nas colunas de folga
 

    int idxFolga = 0;
    int colFolga = nVariaveis;

    for (int i = 0; i < L && idxFolga < f.size(); i++)
    {

        if (idxFolga < f.size())
        {
            if (f[idxFolga] != 0)
            {
                cout << f[idxFolga] << endl;
                MatrizA[i][colFolga] = f[idxFolga];
                colFolga++; // só avança se for folga válida
            }
        }
        idxFolga++;
    }

    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < C; j++)
        {
            cout << setw(6) << MatrizA[i][j] << "";
        }
        cout << endl;
    }

    return 0;
}

/*int index = C;
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < indx; j++)
        {


                if (MatrizA[i][j] == 0)
                {
                    MatrizA[i][j] = f[index++];

                }







        }

    }*/

// 3 linhas, coluna em relação as variaveis
// aumenta numero de colunas de for maior e menor
