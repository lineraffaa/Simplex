#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <regex>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
using namespace std;

string nomeArquivo = "texto.txt";
// ifstream is("texto.txt", ios::in);
string linha;
smatch m;
int L, C, ColunaB, LinhaB;
int contador;
 //* Matriz A sendo declarada 
 vector<vector<float>> MatrizA(L, vector<float>(C, 0.0));
 //* Matriz B sendo declarada 
 vector<vector<float>> MatrizB(LinhaB, vector<float>(ColunaB, 0.0));

pair<vector<int>, vector<float>> intefloat(const string &Arquivo)
{
    
//*Para recerber os numeros int e float sem variaveis 
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


            }

           else if (regex_match(token, m, regex(R"(^([+-]?[0-9]*\.[0-9]+)$)")))
            {
                float numero = stof(m[1].str());
                F.push_back(numero);

            }
        }
    }

    return {seq, F};
}



pair<vector<float>, vector<string>> floatvarialvel(const string &Arquivo)
{
    // para receber os numeros  floats que possuem variaveis 
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

        while (regex_search(inicio, fim, m, regex(R"(([+-]?(?:[0-9]+(?:\.[0-9]+)?)?)(x[0-9]+))")))
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



vector<string> variaveldefolga(const string &Arquivo)
{
    // função para receber as variavies de folga 
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

float multiplique(float MatrizA[L][C], float matrizV[2][2])
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

int contarRestricoes(const string &Arquivo)
{
    ifstream arquivo(Arquivo);

    int total = 0;

    while (getline(arquivo, linha))
    {

        total++;
    }
    return total - 1; //*desconsidera objetivo e última linha
}

int main()
{
    //*Para recerber as variaveis (tipo String)
    vector<string> variaveis;
    vector<string> fn;

    //* Para receber valores decimais 
    vector<float> numero;
    vector<float> numerof;
    vector<float> nf;
    vector<float> f;
    vector<string> vf;

   //* Para receber valores inteiros
    vector<int> nint;
    
    //! variavel para contar as linhas 
    int lines = 0;

    //! função para recerber os numeros de restrições referente ao arquivo
    lines = contarRestricoes(nomeArquivo);
    cout << lines << "Restri \n";
   

    //! Declarei uma variavel para receber os valores da função numerovariavel 
    //* numerovariavel retorna os numeros que estão acompanhados de X e a variavel tambem
    pair<vector<float>, vector<string>> resultadoVariavel = numeroevarialvel(nomeArquivo);

    //! Recebe as variaveis de folga 
    vector<string> Vfolga = variaveldefolga(nomeArquivo);

    //! Recebe os valores sem variavel
    pair<vector<int>, vector<float>> numint = intefloat(nomeArquivo);

    //! Recebe os valores do tipo float que possuem variaveis 
    pair<vector<float>, vector<string>> resulf = floatvarialvel(nomeArquivo);

    //* referente a função numerovariavel
    variaveis = resultadoVariavel.second;
    numero = resultadoVariavel.first;

    //*referente a função floatvariavel
    numerof = resulf.first;
    fn = resulf.second;

    //* referente a função intfloat
    nint = numint.first;
    nf = numint.second;

    //! contador de variaveis 
    int nVariaveis = 0;

    //! Para contar quando as variaveis são encontradas, esse numero ajuda a montar o tamanho da matriz
    ifstream arquivo(nomeArquivo);
    string primeiraLinha;
    if (getline(arquivo, primeiraLinha))
    {
        auto inicio = primeiraLinha.cbegin();
        auto fim = primeiraLinha.cend();

        while (regex_search(inicio, fim, m, regex(R"(([+-]?[0-9]*\.?[0-9]+)?(x[0-9]+))")))
        {
            nVariaveis++;

            inicio = m.suffix().first;
        }
    }
    arquivo.close();

    //! Vetor para receber as variaveis de folga
    vector<string> folga;
    //* variaveis contadores que não podem mudar de lugar 
    int indx = 0, cont = 0, inteiros = 0, decimal = 0;

   
    
    for (int i = 0; i < nint.size(); i++)
    {   
        //! Para receber as variaveis referente a intfloat

        //cout << nint[i] << "  numeros intiros \n";
        inteiros++; 
    }
if (nf.size() > 0)
{
    for (int i = 0; i < nf.size(); i++)
    {
        //! Para receber as variaveis referente a intfloat
     
          
    //  cout << nf[i] << "  numeros decimal \n";
        decimal++;
    } 
}

    

    ColunaB = inteiros + decimal, LinhaB = lines;
    for (int i = 0; i < lines - 1; i++)
    {

      //  cout << variaveis[i] << "  Variaveis\n ";

        cont++;

    }
   

    for (int i = 0; i < numerof.size(); i++)
    {
      //?  cout << numerof[i] << "numeros float\n";
    }

    int cont2 = 0, v = 0;
  
   
    for (int i = 0; i < Vfolga.size(); i++)
    {
        

        if (Vfolga[i] == ">=" || Vfolga[i] == "<=")
        {
            //  cout << Vfolga[i] << " Variaveis de Folga \n";

            cont2++;
        }
      
    }

    
    //!Para inserir os valores referentes a variaveis de folga 
    for (int i = 0; i < Vfolga.size(); i++)
    {

        if (Vfolga[i] == "<=")
        {
            f.push_back(1);

          
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

    //! Linhas e colunas referente a matriz A
    L = lines;
    C = nVariaveis + cont2;
 
    //! Essa variavel não pode ser alterada de lugar
    int idx = nVariaveis; 
    
    
   
    //* Matriz Basica sendo declarada
    vector<vector<float>> MatrizBasica(lines, vector<float>(lines, 0.0));
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < nVariaveis; j++)
        {
            if (idx < numerof.size())
            {   
                //* MatrizA recebendo valores
                MatrizA[i][j] = numerof[idx++];
            }
        }
    }

    //!Contador da MatrizB
    int idx2 = 0;
    for (int i = 0; i < LinhaB; i++)
    {
        for (int j = 0; j < ColunaB/2; j++)
        {
            if (idx2 < nint.size())
            {

                MatrizB[i][j] = nint[idx2++];
            }
        }
    }

    //!Contador da MatrizB
    int idx3 = 0;
    for (int i = 0; i < LinhaB; i++)
    {
        for (int j = ColunaB/2; j < ColunaB; j++)
        {
            if (idx3 < nf.size())
            {

                MatrizB[i][j] = nf[idx3++];
            }
        }
    }


 

    int idxFolga = 1; //! é necessario que comece com 1
    int colFolga = nVariaveis; //* Para receber a quantidade de variaveis

    for (int i = 0; i < L && idxFolga < f.size(); i++)
    {

        if (idxFolga < f.size())
        {
            if (f[idxFolga] != 0)
            {
               // cout << f[idxFolga] << endl;
                MatrizA[i][colFolga] = f[idxFolga];
                colFolga++; // só avança se for folga válida
            }
        }
        idxFolga++;
    }

    cout << "Matriz A \n";
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < C; j++)
        {
            cout << setw(6) << MatrizA[i][j] << "";
        }
        cout << endl;
    }


    cout << "Matriz B \n";
    for (int i = 0; i < LinhaB; i++)
    {
        for (int j = 0; j < ColunaB; j++)
        {
            if(MatrizB[i][j] != 0){
                cout << setw(5) << MatrizB[i][j] << "\n";
            }
           
        }
      
    }

    //TODO fazer a matriz basica, escolher aleatoriamente 3 culunas e linhas da matriz A. 
    //TODO determinante, inversa e soma
    
    //* Vetor de indices 
    vector<int> indices(lines); 
    
    for (int i = 0; i < lines; i++)
    {
        indices[i] = i;
    }
    
    random_device rd;
    default_random_engine gerador(rd());

    shuffle(indices.begin(),indices.end(), gerador);

    vector<int> colunas_escolhidas(indices.begin(), indices.begin() + lines);


  for (int i = 0; i < lines; i++)
  {
    for (int j = 0; j < lines; j++)
    {
        
        
        int coluna_escolhida = colunas_escolhidas[j];
        MatrizBasica[i][j] = MatrizA[i][coluna_escolhida];
    }
    
   
  }
  
//! Imprimindo matriz basica   
cout << "Matriz Basica \n";
    for (int i = 0; i < lines; i++)
    {
       for (int j = 0; j < lines; j++)
       {
        
        cout <<setw(6)<<MatrizBasica[i][j];
       
        

       }
       cout << endl;
    }
    
    int Lnaobasica = lines , Cnaobasica =  lines - 1; 

   // cout << Lnaobasica << "L";
    //cout << Cnaobasica << "c";
    vector<int> todas_colunas(C);

    iota(todas_colunas.begin(), todas_colunas.end(), 0); 
    
    for (int idx : colunas_escolhidas) {
        todas_colunas.erase(remove(todas_colunas.begin(), todas_colunas.end(), idx), todas_colunas.end());
    }
    
    vector<vector<float>> MatriznaoBasica(Lnaobasica, vector<float>(Cnaobasica, 0.0));
    
    for (int i = 0; i < Lnaobasica; i++)
    {
       for (int j = 0; j < Cnaobasica; j++)
       {
        
        int coluna = todas_colunas[j];
            MatriznaoBasica[i][j] = MatrizA[i][coluna]; 
           
        
        

       }

    }


    cout << "Matriz nao basica \n";
    for (int i = 0; i < Lnaobasica; i++)
    {
       for (int j = 0; j < Cnaobasica; j++)
       {
        
        cout <<setw(2)<<MatriznaoBasica[i][j];
       
        

       }
       cout << endl;
    }


    return 0;
}


