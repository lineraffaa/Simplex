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



float detlaplace(const vector<vector<float>> &A) {
    int L = A.size();
    int C = A[0].size();

    if (L == 1) return A[0][0];

    if (L == 2) {
        return A[0][0] * A[1][1] - A[0][1] * A[1][0];
    }

    // Encontrar a linha com mais zeros
    int linhaComMaisZeros = -1, maxZeros = -1;
    for (int i = 0; i < L; i++) {
        int contZeros = 0;
        for (int j = 0; j < C; j++) {
            if (A[i][j] == 0) contZeros++;
        }
        if (contZeros > maxZeros) {
            maxZeros = contZeros;
            linhaComMaisZeros = i;
        }
    }

    cout << "Expandindo na linha: " << linhaComMaisZeros << endl;

    float determinante = 0;

    // Expandir na linha com mais zeros
    for (int j = 0; j < C; j++) {
        if (A[linhaComMaisZeros][j] == 0) continue;

        // Construir submatriz excluindo linha i e coluna j
        vector<vector<float>> submatriz;
        for (int i = 0; i < L; i++) {
            if (i == linhaComMaisZeros) continue;
            vector<float> novaLinha;
            for (int k = 0; k < C; k++) {
                if (k == j) continue;
                novaLinha.push_back(A[i][k]);
            }
            submatriz.push_back(novaLinha);
        }

        float cofator = ((linhaComMaisZeros + j) % 2 == 0 ? 1 : -1) * A[linhaComMaisZeros][j];
        float detSub = detlaplace(submatriz);
        determinante += cofator * detSub;
    }

    cout << determinante << endl;
    return determinante;
}


#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

vector<vector<float>> Matriz_inversa(const vector<vector<float>> &entrada)
{
    int n = entrada.size();

    // Copiar matriz original
    vector<vector<float>> A = entrada;

    // Criar matriz identidade
    vector<vector<float>> I(n, vector<float>(n, 0));
    for (int i = 0; i < n; i++)
        I[i][i] = 1;

    for (int i = 0; i < n; i++)
    {
        // Verifica se o pivô é zero
        if (fabs(A[i][i]) < 1e-6)
        {
            // Tenta trocar com alguma linha abaixo
            bool trocou = false;
            for (int k = i + 1; k < n; k++)
            {
                if (fabs(A[k][i]) > 1e-6)
                {
                    swap(A[i], A[k]);
                    swap(I[i], I[k]);
                    trocou = true;
                    break;
                }
            }
            if (!trocou)
            {
                cout << "Matriz não é inversível (pivô zero).\n";
                return {};
            }
        }

        // Normaliza a linha do pivô
        float pivô = A[i][i];
        for (int j = 0; j < n; j++)
        {
            A[i][j] /= pivô;
            I[i][j] /= pivô;
        }

        // Zera os outros elementos da coluna
        for (int k = 0; k < n; k++)
        {
            if (k != i)
            {
                float fator = A[k][i];
                for (int j = 0; j < n; j++)
                {
                    A[k][j] -= fator * A[i][j];
                    I[k][j] -= fator * I[i][j];
                }
            }
        }
    }

    return I;
}




int main(){
      vector<vector<float>> matrizlaplace = {
        {4, 5, -3, 0},
        {2, -1, 3, 1},
        {1, -3, 2,1},
        {0,2,-2,5}
    };

       vector<vector<float>> matriziversa = {
        {2, 5},
        {5, 3}};
    Matriz_inversa(matriziversa);

   // detlaplace(matrizlaplace);
}