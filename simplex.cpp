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
#include <set>
using namespace std;

string nomeArquivo = "texto.txt";
// ifstream is("texto.txt", ios::in);
string linha;
smatch m;
int L, C, LinhaB, ColunaB;
int contador;

//* Matriz A sendo declarada
vector<vector<float>> MatrizA(L, vector<float>(C, 0.0));
//* Matriz B sendo declarada
vector<vector<float>> MatrizB(LinhaB, vector<float>(ColunaB, 0.0));

vector<float> intefloat(const string &Arquivo)
{
    ifstream arquivo(Arquivo);
    vector<float> todosNumeros;
    string linha;
    smatch m;

    while (getline(arquivo, linha))
    {
        stringstream strings(linha);
        string token;

        while (strings >> token)
        {
            if (regex_match(token, m, regex(R"(^[+-]?[0-9]+$)")))
            {
                // Número inteiro
                float numero = stof(m[0]);
                todosNumeros.push_back(numero);
                //  cout << numero << " (int)\n";
            }
            else if (regex_match(token, m, regex(R"(^[+-]?[0-9]*\.[0-9]+$)")))
            {
                // Número decimal
                float numero = stof(m[0]);
                todosNumeros.push_back(numero);
                //   cout << numero << " (float)\n";
            }
        }
    }

    return todosNumeros;
}

pair<vector<float>, vector<string>> floatvarialvel(const string &Arquivo)
{
    ifstream arquivo(Arquivo);
    vector<string> todasVariaveis;
    vector<float> todosCoeficientes;

    string linha;
    bool primeiraLinha = true;

    //!  encontrar todas as variáveis (na função objetivo)
    if (getline(arquivo, linha))
    {
        smatch m;
        auto ini = linha.cbegin();
        auto fim = linha.cend();
        while (regex_search(ini, fim, m, regex(R"(x[0-9]+)")))
        {
            string var = m.str();
            if (find(todasVariaveis.begin(), todasVariaveis.end(), var) == todasVariaveis.end())
            {
                todasVariaveis.push_back(var);
            }
            ini = m.suffix().first;
        }
    }

    // Agora: processar as demais linhas (restrições)
    while (getline(arquivo, linha))
    {
        // Vetor temporário da linha com zeros
        vector<float> linhaCoef(todasVariaveis.size(), 0.0);

        smatch m; //! guarda partes da string que "casaram" com a expressão regular.
        auto ini = linha.cbegin();
        auto fim = linha.cend();

        while (regex_search(ini, fim, m, regex(R"(([+-]?[0-9]*\.?[0-9]+)?(x[0-9]+))")))
        {
            //! m armazena os grupos capturados pelo regex
            string coefstr = m[1].str(); //! pega o coeficiente como string
            string var = m[2].str();     //! pega a variável como string

            float coef = 1.0;
            if (coefstr == "-")
                coef = -1.0;
            else if (!coefstr.empty())
                coef = stof(coefstr);

            //! find() para encontra a posição
            auto it = find(todasVariaveis.begin(), todasVariaveis.end(), var);
            if (it != todasVariaveis.end())
            {
                int pos = distance(todasVariaveis.begin(), it);
                linhaCoef[pos] = coef;
            }
            //! it vai apontar para "x2" dentro do vetor

            //! distance(todasVariaveis.begin(), it) vai retornar 1 (índice da variável)

            //! Então linhaCoef[1] = coef; vai atualizar o coeficiente da variável "x2".

            ini = m.suffix().first;
        }
        //*  linhaCoef.begin() é o início do vetor — aponta para o primeiro elemento.

        //* linhaCoef.end() é o fim, ou melhor, a posição logo após o último elemento do vetor.

        // Adiciona os coeficientes da linha ao vetor final
        todosCoeficientes.insert(todosCoeficientes.end(), linhaCoef.begin(), linhaCoef.end());
    }
    // !Na função insert do vetor, o primeiro parâmetro é onde você vai inserir os novos elementos.

    //!  todosCoeficientes.end() significa: insere no final do vetor todosCoeficientes.

    //! Os próximos dois parâmetros são um intervalo de elementos que você quer inserir:

    //!  linhaCoef.begin() — início do vetor linhaCoef

    //! linhaCoef.end() — fim do vetor linhaCoef

    //! Ou seja, insere todos os elementos de linhaCoef (do começo ao fim) no final de todosCoeficientes.
    return {todosCoeficientes, todasVariaveis};
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

vector<vector<float>> multiplique(const vector<vector<float>> &A, const vector<vector<float>> &B)
{
    int linhasA = A.size();
    int colunasA = A[0].size();
    int linhasB = B.size();
    int colunasB = B[0].size();

    // Matriz resultado inicializada com zeros
    vector<vector<float>> resultado(linhasA, vector<float>(colunasB, 0.0));
    // TODO colocar a verificação se matrizA 2x3 é igual matrizB 3x4

    // Multiplicação
    for (int i = 0; i < linhasA; i++)
    {
        for (int j = 0; j < colunasB; j++)
        {
            for (int k = 0; k < colunasA; k++)
            {
                resultado[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    for (int i = 0; i < linhasA; i++)
    {
        for (int j = 0; j < colunasB; j++)
        {

            cout << setw(6) << resultado[i][j] << " ";
        }
        cout << endl;
    }

    return resultado;
}

vector<float> X1(const string &Arquivo)
{

    ifstream arquivo(Arquivo);
    regex re("x(\\d+)");
    string linha;
    smatch match;
    set<int> expoentesUnicos;

    while (getline(arquivo, linha))
    {
        auto inicio = sregex_iterator(linha.begin(), linha.end(), re);
        auto fim = sregex_iterator();

        for (auto i = inicio; i != fim; ++i)
        {
            //! Um loop que percorre todos os matches encontrados pela regex na linha.
            smatch match = *i; //! aqui estou acessando o conteudo ao qual aponto *it
            //! o que esta sendo capturado no arquivo como x2
            int expoente = stoi(match[1]);    //! esta sendo capturado 2
            expoentesUnicos.insert(expoente); //! é para ignorar as repetções

            //  cout << "Expoente encontrado: " << expoente << endl;
        }
    }
    vector<float> X(expoentesUnicos.begin(), expoentesUnicos.end());

    return X;
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
// TODO fazer o laplaca, colocar a verificação para matriz 1 por e 2 por 2
/*vector<float> pegarColuna(const vector<vector<float>> &A, int j)
{
    vector<float> coluna;
    for (int i = 0; i < A.size(); i++)
    {
        coluna.push_back(A[i][j]);
    }
    return coluna;
}*/

float detlaplace(const vector<vector<float>> &A)
{

    // int TAM = A.size();

    int L = A.size();
    int C = A[0].size(), contextra = 0;
    //! Encontrar a linha  com mais zeros
    int linhaComMaisZeros = -1;
    int maxZeros = -1;

    if (A.size() == 0)
    {

        return A[0][0];
    }

    if (A.size() == 2)
    {
        return A[0][0] * A[1][1] - A[0][1] * A[1][0];
    }

    for (int i = 0; i < L; i++)
    {
        int contZeros = 0;
        //* Para verificar qual linha, possui mais zeros
        for (int j = 0; j < C; j++)
        {

            if (A[i][j] == 0)
            {
                contZeros++;
            }

            if (contZeros > maxZeros)
            {

                maxZeros = contZeros;
                linhaComMaisZeros = i;
            }
        }
    }

    //! fixa a linha com mais zeros, vai percorrendo a colunas
    //  cout << "Linha com mais zeros: " << linhaComMaisZeros << endl;
    // cout << "Coluna com mais zeros: " << colunamaiszeros << endl;
    float determinante = 0;
    //! Ja verifiquei a linha e coluna com mais zeros, agora preciso retirar a linha e coluna com mais zeros
    //* trcho de codigo para fazer esse etapa
    for (int j = 0; j < C; j++)
    {
        if (A[linhaComMaisZeros][j] == 0) //! pula os elementos que são zero
            continue;

        // Construir submatriz excluindo linha i e coluna j
        vector<vector<float>> submatriz;
        for (int i = 0; i < L; i++)
        {
            if (i == linhaComMaisZeros)
                continue;
            //! a linha usada na expansão de Laplace. Você não quer incluí-la na submatriz.
            // cout << i << "i" << endl;
            //  cout << linhaComMaisZeros << "debug" << endl;
            vector<float> novaLinha;
            for (int k = 0; k < C; k++)
            {
                if (k == j) //! Ignora a coluna do elemento atual da expansão índice j.
                    continue;
                novaLinha.push_back(A[i][k]);
            }
            submatriz.push_back(novaLinha);
        }

        //? que ganbiarra foi feita aqui
        //! se as somas de linhacommais zeros e j, forem par é positvo a soma é + se não é -
        float cofator;
        // cout << linhaComMaisZeros << "linhas com zeros \n";
        // cout << j << "j \n";
        // cout <<  A[linhaComMaisZeros][j] << "matriz A \n";
        //! primeiro é somado os indices da matriz A (original), é fixado a linha com mais zeros, é ignorado a coluna do valor j
        // ! é formado a submatriz, calculado o determinante
        if ((linhaComMaisZeros + j) % 2 == 0)
        {

            //  cout << cofator << "cofator \n";
            cofator = 1 * A[linhaComMaisZeros][j];
        }
        else
        {
            cofator = -1 * A[linhaComMaisZeros][j];
        }

        //! aqui recebe det da sub matriz
        float detSub = detlaplace(submatriz);

        determinante += cofator * detSub;
    }
    // cout << determinante << "det \n";
    return determinante;

    //! calcular o det da matriz 3 por 3

    //* para pegar a diagonal principal, fixar a coluna 1 e 0
}

vector<vector<float>> Matriz_inversa(vector<vector<float>> matrizI)
{
    int L = matrizI.size();
    int C = matrizI[0].size();

    vector<vector<float>> matrizidentidade(L, vector<float>(C, 0));
    for (int i = 0; i < L; i++)
        matrizidentidade[i][i] = 1;

    for (int i = 0; i < L; i++)
    {
        float divisor = matrizI[i][i];

        if (fabs(divisor) < 1e-6)
        {
            cout << "Erro: pivo zero. Matriz pode n ser inversível.\n";
            return {};
        }

        // Dividindo a linha pelo pivô
        for (int j = 0; j < C; j++)
        {
            matrizI[i][j] /= divisor;
            matrizidentidade[i][j] /= divisor;
        }

        // Zerando os outros elementos da coluna i
        for (int k = 0; k < L; k++)
        {
            if (k != i)
            {
                float fator = matrizI[k][i];
                for (int j = 0; j < C; j++)
                {
                    matrizI[k][j] -= fator * matrizI[i][j];
                    matrizidentidade[k][j] -= fator * matrizidentidade[i][j];
                }
            }
        }
    }

    // Print para debug
    cout << "Matriz inversa:\n";
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < C; j++)
        {
            // cout << fixed << setprecision(4) << matrizidentidade[i][j] << " ";
        }
        cout << "\n";
    }

    return matrizidentidade;
}

void FaseI(const string &Arquivo)
{
    ifstream arquivo(Arquivo);

    string linha;
    getline(arquivo, linha);

    regex regexMax("max", regex_constants::icase); // detecta "max"
    string novaLinha;

    // Verifica se é um problema de maximização
    if (regex_search(linha, regexMax))
    {
        linha = regex_replace(linha, regexMax, "min"); //! troca "max" por "min"

        // Separa parte antes e depois do '='
        size_t Igual = linha.find('=');
        if (Igual == string::npos)
        {
            cerr << "Formato inválido da função objetivo.\n";
            return;
        }

        string esquerda = linha.substr(0, Igual); //! Ex: "min z"
        string direita = linha.substr(Igual + 1); //! Ex: "2x1 + x2"
        string Modificada;

        //! Para encontrar coeficientes e variáveis
        regex coef("([+-]?\\s*\\d*\\.?\\d*)\\s*([a-zA-Z]\\d*)");
        sregex_iterator iter(direita.begin(), direita.end(), coef);
        sregex_iterator end;

        //*percorrer a primeira a linha
        size_t fim = 0;
        for (iter; iter != end; iter++)
        {
            string letra = iter->str();
            cout << letra << "AQUIII" << endl;
            size_t ultimo = iter->position();

            // cout << letra <<endl;
            // cout << ultimo << endl;

            Modificada += direita.substr(fim, ultimo - fim);
            //! aqui estou pegando a parte do min z

            string valores = iter->str(1);
            string x = iter->str(2);

            //! para remover os espaços dos valores
            valores.erase(remove(valores.begin(), valores.end(), ' '), valores.end());
            //! para verifcar os valores como x1
            if (valores.empty() || valores == "+" || valores == "-")
            {
                valores += "1";
            }

            //! para converter uma string para um float
            float valor = stof(valores);
            valor *= -1;

            if (valor >= 0 && !Modificada.empty() && Modificada.back() != '-')
            {
                Modificada += "+";
            }

            ostringstream saida; //! permite escrever em uma string como se fosse um cout
            saida << fixed << setprecision(0) << valor;
            Modificada += saida.str() + x;

            fim = ultimo + letra.length();
        }
        //! substr é para pegar uma parte da string, acessa o inicio ou o final
        Modificada += direita.substr(fim);

        novaLinha = esquerda + " = " + Modificada;

        cout << novaLinha << endl;
    }
}

vector<float> custos(const string &Arquivo, int C)
{
    ifstream arquivo(Arquivo);

    string linha;
    getline(arquivo, linha);

    //  vector <float> novaLinha;
    const int TOTAL_VARIAVEIS = C;
    vector<float> novaLinha(TOTAL_VARIAVEIS, 0.0);
    regex coef("([+-]?\\s*\\d*\\.?\\d*)\\s*([a-zA-Z])(\\d+)");
    // Verifica se é um problema de maximização
    if (regex_search(linha, coef))
    {

        // Separa parte antes e depois do '='
        size_t Igual = linha.find('=');
        if (Igual == string::npos)
        {
            cerr << "Formato inválido da função objetivo.\n";
            //  return;
        }

        //  string esquerda = linha.substr(0, Igual); //! Ex: "min z"
        string direita = linha.substr(Igual + 1); //! Ex: "2x1 + x2"
        string Modificada;

        //! Para encontrar coeficientes e variáveis

        sregex_iterator iter(direita.begin(), direita.end(), coef);
        sregex_iterator end;

        //*percorrer a primeira a linha
        size_t fim = 0;
        for (iter; iter != end; iter++)
        {
            string letra = iter->str();

            size_t ultimo = iter->position();
            int varIndice = stoi(iter->str(3)) - 1;

            //! aqui estou pegando a parte do min z

            string valores = iter->str(1);

            //  cout << "valores " << valores <<< endl;
            string x = iter->str(2);

            //! para remover os espaços dos valores
            valores.erase(remove(valores.begin(), valores.end(), ' '), valores.end());
            //! para verifcar os valores como x1
            if (valores.empty() || valores == "+")
            {
                valores += "1";
            }
            else if (valores == "-")
                valores = "-1";

            //! para converter uma string para um float
            float coef = stof(valores);
            // novaLinha.push_back(coef);
            if (varIndice >= 0 && varIndice < TOTAL_VARIAVEIS)
            {
                novaLinha[varIndice] = coef;
            }
        }
        //! substr é para pegar uma parte da string, acessa o inicio ou o final
        //  Modificada += direita.substr(fim);

        for (int i = 0; i < novaLinha.size(); i++)
        {
            //     cout << "Nova Linha " << novaLinha[i] << endl;
        }
    }

    return novaLinha;
}

vector<float> matrizXvetor(vector<vector<float>> matrizz, vector<float> vetor)
{

    int linhas = matrizz.size();
    int colunas = matrizz[0].size();

    if (vetor.size() != colunas)
    {
        throw runtime_error("Erro: número de colunas da matriz deve ser igual ao tamanho do vetor.");
    }

    vector<float> resultado(linhas, 0.0);
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {

            // cout << "mult \n";
            resultado[i] += matrizz[i][j] * vetor[j];
            // cout << matrizz[i][j] << endl;
            // cout << vetor[k] <<"vetor" <<endl;
        }
    }

    for (int i = 0; i < resultado.size(); i++)
    {

        cout << resultado[i] << endl;
    }

    return resultado;
}

vector<float> multivetor(vector<float> a, vector<float> b)
{
    size_t tamanho_min = min(a.size(), b.size());
    vector<float> resultado(tamanho_min);

    for (int i = 0; i < a.size(); i++)
    {

        resultado[i] = a[i] * b[i];
    }

    return resultado;
}
// TODO matrizBasica, matrizB, escolhidas, naoescolhidasindices, matriznaoBasica
void FaseII(const string &Arquivo, vector<vector<float>> basica, vector<vector<float>> matrizB, vector<float> custosbasicos, vector<float> custosnbasicos, vector<vector<float>> matrizNb)
{
    //! Aqui é para ser o passo I
    vector<vector<float>> inversaB;
    inversaB = Matriz_inversa(basica); // multiplicação

    //! 2.1 {vetor multiplicador simplex}
    vector<float> lambd;
    lambd = matrizXvetor(inversaB, custosbasicos);

    cout << "AQUI\n";

    // TODO multiplicação de vetor com vetor
    vector<float> custorelativo;
    vector<float> resultadoMult;
    resultadoMult = matrizXvetor(matrizNb, lambd); //! coluna da não basica
                                                   //! 2.2 {custos relativos}

    for (int i = 0; i < custorelativo.size(); i++)
    {

        custorelativo[i] = custosnbasicos[i] - resultadoMult[i];
    }

    //!  {determina¸c˜ao da vari´avel a entrar na base}
    auto it = min_element(custorelativo.begin(), custorelativo.end());
    float menorValor = *it;
    int indiceMenor = distance(custorelativo.begin(), it);

    // Verificação
    if (menorValor >= 0) {
      //  break;
    
    }

    // Agora usamos o índice para acessar a coluna da matriz não-básica
    int colunanaobasica = indiceMenor;
    vector <float> Colunaescolhida;
    cout << "Coluna da matrizNb escolhida (índice " << colunanaobasica << "):\n";
    for (int i = 0; i < matrizNb.size(); ++i) {
        cout << matrizNb[i][colunanaobasica] << endl;
        Colunaescolhida.push_back(matrizNb[i][colunanaobasica]);
    }
    


    vector<float> y;
    y = matrizXvetor(inversaB, Colunaescolhida);

    for (int i = 0; i < y.size(); ++i)
    {

            if (y[i] <= 0)
            {
                continue;
            }
            else
            {

                float menor2 = numeric_limits<float>::max();

                for (size_t i = 0; i < inversaB.size(); ++i) {
                    if (y[i] == 0) {
                        cerr << "Erro: divisão por zero na linha " << i << endl;
                        continue;
                    }
            
                    for (size_t j = 0; j < inversaB[i].size(); ++j) {
                        float valor = inversaB[i][j] / y[i];
                        if (valor < menor2) {
                            menor2 = valor;
                        }
                    }
               
                //! pegar o menor valor realizado pela divisao
            }

    }
}
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
    vector<float> CB;

    //* matriz para calcular o laplace

    //* Para receber valores inteiros
    vector<float> MB;

    //! variavel para contar as linhas
    int lines = 0;

    //! função para recerber os numeros de restrições referente ao arquivo
    lines = contarRestricoes(nomeArquivo);

    //! Declarei uma variavel para receber os valores da função numerovariavel
    //* numerovariavel retorna os numeros que estão acompanhados de X e a variavel tambem
    pair<vector<float>, vector<string>> resultadoVariavel = numeroevarialvel(nomeArquivo);

    //! Recebe as variaveis de folga
    vector<string> Vfolga = variaveldefolga(nomeArquivo);

    //! Recebe os valores sem variavel
    MB = intefloat(nomeArquivo);

    //! Recebe os valores do tipo float que possuem variaveis
    pair<vector<float>, vector<string>> resulf = floatvarialvel(nomeArquivo);

    //* referente a função numerovariavel
    variaveis = resultadoVariavel.second;
    numero = resultadoVariavel.first;

    //*referente a função floatvariavel
    numerof = resulf.first;
    fn = resulf.second;

    //* referente a função intfloat
    // MB = numint.first;
    // nf = numint.second;

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

    // cout << nVariaveis <<"variaveis n"<<endl;
    //! Vetor para receber as variaveis de folga
    vector<string> folga;
    //* variaveis contadores que não podem mudar de lugar
    int indx = 0, cont = 0, inteiros = 0, decimal = 0;

    for (int i = 0; i < MB.size(); i++)
    {
        //! Para receber as variaveis referente a intfloat

        //   cout << MB[i] << "  numeros inteiros \n";
        inteiros++;
    }
    if (nf.size() > 0)
    {
        for (int i = 0; i < nf.size(); i++)
        {
            //! Para receber as variaveis referente a intfloat

            //    cout << nf[i] << "  numeros decimal \n";
            decimal++;
        }
    }

    ColunaB = inteiros + decimal, LinhaB = lines;
    for (int i = 0; i < lines - 1; i++)
    {

        //   cout << variaveis[i] << "  Variaveis\n ";

        cont++;
    }
    int cvariaveis = 0;
    for (int i = 0; i < fn.size(); i++)
    {

        cout << fn[i] << "  Variaveis\n ";
        cvariaveis++;
    }

    for (int i = 0; i < numerof.size(); i++)
    {
        //   cout << numerof[i] << " I numeros float\n";
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

    //! Para inserir os valores referentes a variaveis de folga
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

    //* Matriz A sendo declarada
    vector<vector<float>> MatrizA(L, vector<float>(C, 0.0));
    //* Matriz B sendo declarada
    vector<vector<float>> MatrizB(LinhaB, vector<float>(ColunaB, 0.0));

    //* Matriz Basica sendo declarada
    vector<vector<float>> MatrizBasica(lines, vector<float>(lines, 0.0));
    int idx = 0;
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < nVariaveis; j++)
        {
            if (idx < numerof.size())
            {
                //* MatrizA recebendo valores
                // cout << "aqui a matriz" << numerof[idx] << endl;
                MatrizA[i][j] = numerof[idx++];
                // cout << MatrizA[i][j] <<"Matriz A" << endl;
            }
        }
    }

    //! Contador da MatrizB
    int idx2 = 0;
    for (int i = 0; i < LinhaB; i++)
    {
        for (int j = 0; j < ColunaB / 2; j++)
        {
            if (idx2 < MB.size())
            {
                if (MB[idx2] != 0)
                {
                    MatrizB[i][j] = MB[idx2++];
                }
            }
        }
    }

    //! Contador da MatrizB

    int idxFolga = 1;          //! é necessario que comece com 1
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
                // cout << MatrizA[i][colFolga] << "Mais um teste"<<endl;
            }
        }
        idxFolga++;
    }

    //?Arrumar a matriz A
    cout << "Matriz A \n";
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < C; j++)
        {
            cout << setw(6) << MatrizA[i][j] << "";
        }
        cout << endl;
    }
    custos(nomeArquivo, C);
    cout << "Matriz B \n";
    for (int i = 0; i < LinhaB; i++)
    {
        for (int j = 0; j < ColunaB; j++)
        {

            if (MatrizB[i][j] != 0)
            {
                cout << setw(MatrizB.size()) << MatrizB[i][j] << "\n";
            }
        }
    }

    // TODO fazer a matriz basica, escolher aleatoriamente 3 culunas e linhas da matriz A.
    // TODO determinante, inversa e soma
    int colunaA = C;
    //* Vetor de indices
    vector<int> indices(colunaA);

    for (int i = 0; i < colunaA; i++)
    {
        indices[i] = i;
    }

    random_device rd;
    default_random_engine gerador(rd());
    //? verificar o valor que esta sendo sorteado
    shuffle(indices.begin(), indices.end(), gerador);
    int colunasSelecionadas = lines;

    vector<int> Colescolhidas(indices.begin(), indices.begin() + colunasSelecionadas);
    vector<float> escolhidas(colunasSelecionadas);

    set<int> escolhidasSet(Colescolhidas.begin(), Colescolhidas.end());
    vector<float> naoEscolhidasIndices;

    for (int i = 0; i < colunaA; i++)
    {
        if (escolhidasSet.find(i) == escolhidasSet.end())
        {
            naoEscolhidasIndices.push_back(i);
        }
    }

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < colunaA; j++)
        {

            int coluna_escolhida = Colescolhidas[j];
            escolhidas[j] = coluna_escolhida;
            MatrizBasica[i][j] = MatrizA[i][coluna_escolhida];
        }
    }

    //! Imprimindo matriz basica
    cout << "Matriz Basica \n";
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < lines; j++)
        {

            cout << setw(6) << MatrizBasica[i][j];
        }
        cout << endl;
    }

    int Lnaobasica = lines, Cnaobasica = lines - 1;

    // cout << Lnaobasica << "L";
    // cout << Cnaobasica << "c";
    vector<int> todas_colunas(C);

    //? verificar a linha 849
    iota(todas_colunas.begin(), todas_colunas.end(), 0);

    for (int idx : Colescolhidas)
    {
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

            cout << setw(2) << MatriznaoBasica[i][j];
        }
        cout << endl;
    }

    // cout << "AQUII \n";

    vector<vector<float>> matriz2 = {
        {2, 1},
        {1, 2}};

    //    vector<vector<float>> matrizResultado = multiplique(matriz, matriz2);
    vector<vector<float>> matrizlaplace = {
        {-3, 1, -2, 1},
        {5, 2, 2, 3},
        {7, 4, -5, 0},
        {1, -1, 11, 2}};
    cout << detlaplace(matrizlaplace) << endl;

    vector<vector<float>> matriziversa = {
        {1, 2, 3},
        {0, 1, 4},
        {0, 0, 1}};
    // FaseI(nomeArquivo);

    //!  CB = custos(nomeArquivo, C); essa função não é para custos

    // Matriz_inversa(matriziversa);

    //!

    // pot = X1(nomeArquivo);

    for (int i = 0; i < escolhidas.size(); i++)
    {
        cout << escolhidas[i] << "custos basico" << endl;
    }

    for (int i = 0; i < naoEscolhidasIndices.size(); i++)
    {
        cout << naoEscolhidasIndices[i] << "custos nao basico" << endl;
    }
    // TODO  matrizXvetor(matrizZ, vetorR);
    // TODO matrizBasica, matrizB, escolhidas, naoescolhidasindices, matriznaoBasica
    FaseII(nomeArquivo, MatrizBasica, MatrizB, escolhidas, naoEscolhidasIndices, MatriznaoBasica);

    return 0;
}
