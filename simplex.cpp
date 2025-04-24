#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

ifstream is("texto.txt", ios::in);
string linha;
smatch m;

bool verific()
{  

    while (getline(is, linha))
    {
        // permite que trabalhe com string como se foossem streams de entrada e saida
        stringstream strings(linha);
        string token;

        auto inicio = linha.cbegin();
        // Retorna um iterador constante para o primeiro caractere da string. começa no inicio da linha
        auto fim = linha.cend();
        // Retorna um iterador constante para um ponto após o último caractere da string.
        // Ele não aponta para um caractere válido, é só o marcador de parada dos loops com iteradores.

        // Para pegar partes especificas da string como variavel e numero
        

        while (strings >> linha)
        {

            if (regex_match(linha, m, regex("^[0-9]*\\.?[0-9]+$")))
            {

                cout << "Número encontrado: " << m[0] << endl;

                // Se quiser checar se é decimal, basta verificar se tem ponto:
                if (m[0].str().find('.') != string::npos)
                {
                    cout << "É decimal!" << endl;
                    return true;
                    /*
                    m[0]
                    É o match completo que a regex encontrou.
                    se a string tem 3.5x2, e a regex encontra 3.5x2, então m[0] == "3.5x2".

                    str()
                    Converte o match (m[0]) em um std::string, caso ele ainda esteja em formato interno da regex.

                    find('.')
                    Procura pelo caractere . (ponto) dentro da string.

                    Retorna:

                    A posição onde o ponto foi encontrado (por exemplo, 1)
                    Ou string::npos (valor especial que significa: não encontrado)

                    =string::npos
                    Verifica se o ponto foi encontrado. então é número decimal!
                    */
                }
            }
        }

        while (regex_search(inicio, fim, m, regex(R"(([0-9]*\.?[0-9]+)?(x[0-9]+))")))
        {   
            string coef = m[1].matched ? m[1].str() : "1";
            // Verifica se o grupo 1 da regex (o número) foi de fato encontrado na string. [m[1].matched]
            // se sim retorna uma string, m[1].str(). se não retorna 1, porque x2 = 1x2
            cout << "Número: " << coef << " | Variável: " << m[2] << endl;
            inicio = m.suffix().first; // Avança para o próximo match... prepara o regex_search pra buscar o próximo match a partir dali
        }
        while(regex_search(inicio, fim, m, regex(R"(>=)")))
        {
            cout << " | Variável de folga: " << m[0] << endl;
            inicio = m.suffix().first;
           
        }
        
    }
    

    return 0;
}




int main()
{
cout << verific();
    
}