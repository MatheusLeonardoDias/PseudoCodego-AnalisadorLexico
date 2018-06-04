#include <bits/stdc++.h>
using namespace std;

#define TAB_SIZE 4

class Token
{
  private:
  public:
    string tipo;
    string conteudo;
    int linha;
    int coluna;
    Token(string tipo, string conteudo, int linha, int coluna)
    {
        this->tipo = tipo;
        this->conteudo = conteudo;
        this->linha = linha;
        this->coluna = coluna;
    }

    Token(string tipo, int linha, int coluna)
    {
        this->tipo = tipo;
        this->conteudo = tipo;
        this->linha = linha;
        this->coluna = coluna;
    }
};

class Erro
{
  public:
    string message;
    int linha;
    int coluna;

    Erro(string message, int linha, int coluna)
    {
        this->message = message;
        this->linha = linha;
        this->coluna = coluna;
    }
};

FILE *entrada;
vector<Token> tokens;
vector<Erro> erros;

void logger(string message)
{
    cout << message << endl;
}

char le_caracter(FILE *arquivo)
{
    char letra = fgetc(arquivo);
    if (feof(arquivo))
    {
        return -1;
    }
    else
        return letra;
}

bool checa_letra(char caracter)
{
    if (caracter >= 0x41 && caracter <= 0x5A)
        return true;
    if (caracter >= 0x61 && caracter <= 0x7A)
        return true;
    else
        return false;
}

bool checa_numero(char caracter)
{
    if (caracter >= 0x30 && caracter <= 0x39)
        return true;
    else
        return false;
}

void imprime_tokens(vector<Token> tokens)
{
    for (int tk = 0; tk < tokens.size(); tk++)
    {
        if (tokens[tk].tipo == "QUEBRA_LINHA")
        {
            cout << endl;
        }
        else
            cout << "<" << tokens[tk].tipo << ", " << tokens[tk].conteudo << ", " << tokens[tk].linha << ", " << tokens[tk].coluna << "> ";
    }
}

void imprime_erros(vector<Erro> erros)
{
    cout << endl
         << "ERROS: " << endl;
    for (int err = 0; err < erros.size(); err++)
    {
        cout << "<" << erros[err].message << ", " << erros[err].linha << ", " << erros[err].coluna << ">" << endl;
    }
}

char lookahead(FILE *entrada)
{
    char letra = le_caracter(entrada);
    if (letra == -1)
    {
        logger("Erro: Fim do Arquivo inesperado. ");
        return -1;
    }

    return letra;
}

void adiciona_token(string identifier, int linha, int coluna)
{
    /* Logicas */
    if (identifier == "e")
    {
        tokens.push_back(Token("LOG_E", linha, coluna));
    }
    else if (identifier == "ou")
    {
        tokens.push_back(Token("LOG_OU", linha, coluna));
    }
    else if (identifier == "ouexcluivo")
    {
        tokens.push_back(Token("LOG_OUEXCLUSIVO", linha, coluna));
    }
    else if (identifier == "nao")
    {
        tokens.push_back(Token("LOG_NAO", linha, coluna));
    }

    /* Tipos */
    else if (identifier == "inteiro")
    {
        tokens.push_back(Token("TIPO_INTEIRO", linha, coluna));
    }
    else if (identifier == "real")
    {
        tokens.push_back(Token("TIPO_REAL", linha, coluna));
    }
    else if (identifier == "caracter")
    {
        tokens.push_back(Token("TIPO_CARACTER", linha, coluna));
    }
    else if (identifier == "logico")
    {
        tokens.push_back(Token("TIPO_LOGICO", linha, coluna));
    }

    /* Condicional */
    else if (identifier == "se")
    {
        tokens.push_back(Token("COND_SE", linha, coluna));
    }
    else if (identifier == "senao")
    {
        tokens.push_back(Token("COND_SENAO", linha, coluna));
    }
    else if (identifier == "escolha")
    {
        tokens.push_back(Token("COND_ESCOLHA", linha, coluna));
    }
    else if (identifier == "caso")
    {
        tokens.push_back(Token("COND_CASO", linha, coluna));
    }

    /* Laço */
    else if (identifier == "enquanto")
    {
        tokens.push_back(Token("REP_ENQUANTO", linha, coluna));
    }
    else if (identifier == "para")
    {
        tokens.push_back(Token("REP_PARA", linha, coluna));
    }

    /* Escopo */
    else if (identifier == "inicia")
    {
        tokens.push_back(Token("ESC_INICIA", linha, coluna));
    }
    else if (identifier == "finaliza")
    {
        tokens.push_back(Token("ESC_FINALIZA", linha, coluna));
    }
    else if (identifier == "faca")
    {
        tokens.push_back(Token("ESC_FACA", linha, coluna));
    }
    else if (identifier == "fim")
    {
        tokens.push_back(Token("ESC_FIM", linha, coluna));
    }
    else if (identifier == "funcao")
    {
        tokens.push_back(Token("ESC_FUNCAO", linha, coluna));
    }

    /* Impressão */
    else if (identifier == "imprime")
    {
        tokens.push_back(Token("IMP_IMPRIME", linha, coluna));
    }
    else if (identifier == "leia")
    {
        tokens.push_back(Token("IMP_LEIA", linha, coluna));
    }

    /* Variável */
    else
    {
        tokens.push_back(Token("ID", identifier, linha, coluna));
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        logger("Erro: Para execução do analisador léxico você deve informar o arquivo a ser analisado.");
        exit(-1);
    }

    entrada = fopen(argv[1], "r");
    if (entrada == NULL)
    {
        logger("Erro: Impossível abrir o arquivo.");
        exit(-1);
    }

    // Tokens serão armazenados em formato <Tipo, Conteúdo, Linha, Coluna>
    // Tipos de tokens = CLOSE_PAR, OPEN_PAR, DIV, MUL, SUB, SUM, MAIOR, MAIOR_IGUAL, MENOR, MENOR_IGUAL, DIF, ATRIB, LOG_E, LOG_OU, LOG_IGUAL, LOG_OUEXCLUSIVO, LOG_NAO, TIPO_CARACTER, TIPO_LOGICO, TIPO_REAL, TIPO_INTEIRO, COND_CASO, COND_ESCOLHA, COND_SE, COND_SENAO, REP_ENQUANTO, REP_PARA, ESC_FACA, ESC_FIM, ESC_FINALIZA, ESC_FUNCAO, ESC_INICIA, IMP_IMPRIME, IMP_LEIA, ID, NUM_FLOAT, NUM_INT
    // vector<Token> tokens;

    int linha_atual = 1;
    int coluna_atual = 1;
    while (true)
    {
        char letra = le_caracter(entrada);
        if (letra == -1)
            break;

        /* Tokens que não necessitam de lookahead. */
        if (letra == ')')
        {
            tokens.push_back(Token("CLOSE_PAR", linha_atual, coluna_atual));
        }
        else if (letra == '(')
        {
            tokens.push_back(Token("OPEN_PAR", linha_atual, coluna_atual));
        }
        else if (letra == '/')
        {
            tokens.push_back(Token("DIV", "OP_ARIT",linha_atual, coluna_atual));
        }
        else if (letra == '*')
        {
            tokens.push_back(Token("MUL", "OP_ARIT",linha_atual, coluna_atual));
        }
        else if (letra == '+')
        {
            tokens.push_back(Token("SUM", "OP_ARIT",linha_atual, coluna_atual));
        }
        else if (letra == '-')
        {
            tokens.push_back(Token("SUB", "OP_ARIT", linha_atual, coluna_atual));
        }

        /* Tokens que necessitam de lookahead. */
        else if (letra == '>')
        {
            if (lookahead(entrada) == '=')
            {
                tokens.push_back(Token("MAIOR_IGUAL", linha_atual, coluna_atual));
                coluna_atual++;
            }
            else
            {
                fseek(entrada, -1, SEEK_CUR);
                tokens.push_back(Token("MAIOR", linha_atual, coluna_atual));
            }
        }
        else if (letra == '<')
        {
            if (lookahead(entrada) == '=')
            {
                tokens.push_back(Token("MENOR_IGUAL", linha_atual, coluna_atual));
                coluna_atual++;
            }
            else
            {
                fseek(entrada, -1, SEEK_CUR);
                tokens.push_back(Token("MENOR", linha_atual, coluna_atual));
            }
        }
        else if (letra == '!')
        {
            if (lookahead(entrada) == '=')
            {
                tokens.push_back(Token("DIF", linha_atual, coluna_atual));
                coluna_atual++;
            }
            else
            {
                fseek(entrada, -1, SEEK_CUR);
                tokens.push_back(Token("LOG_NAO", linha_atual, coluna_atual));
            }
        }
        else if (letra == '=')
        {
            if (lookahead(entrada) == '=')
            {
                tokens.push_back(Token("LOG_IGUAL", linha_atual, coluna_atual));
                coluna_atual++;
            }
            else
            {
                fseek(entrada, -1, SEEK_CUR);
                tokens.push_back(Token("ATRIB", linha_atual, coluna_atual));
            }
        }
        else if (checa_numero(letra))
        {
            int linha_token = linha_atual, coluna_token = coluna_atual;
            string number;
            number += letra;

            bool real = false;
            while (true)
            {
                char digito = le_caracter(entrada);
                coluna_atual++;
                // if (digito == -1)
                //     goto end;

                if (digito == '.')
                {
                    if (real)
                    {
                        erros.push_back(Erro("Sintaxe Inválida: Mais de um ponto identificado no mesmo número.", linha_atual, coluna_atual));
                    }
                    else
                    {
                        number += '.';
                        real = true;
                    }
                }
                else if (checa_numero(digito))
                {
                    number += digito;
                }
                else
                {
                    if (real)
                    {
                        tokens.push_back(Token("NUM_FLOAT", number, linha_token, coluna_token));
                    }
                    else
                    {
                        tokens.push_back(Token("NUM_INT", number, linha_token, coluna_token));
                    }
                    fseek(entrada, -1, SEEK_CUR);
                    break;
                }
            }
            continue;
        }
        else if (checa_letra(letra))
        {
            int linha_token = linha_atual, coluna_token = coluna_atual;
            string identifier;
            identifier += letra;

            while (true)
            {
                char digito = le_caracter(entrada);
                coluna_atual++;
                // if (digito == -1)
                //     goto end;

                if (checa_letra(digito) || checa_numero(digito))
                {
                    identifier += digito;
                }
                else
                {
                    adiciona_token(identifier, linha_token, coluna_token);
                    fseek(entrada, -1, SEEK_CUR);
                    break;
                }
            }
            continue;
        }

        /* Comentario */
        else if (letra == '#')
        {
            while (true)
            {
                char digito = le_caracter(entrada);
                if (digito == -1 || digito == '\n')
                {
                    fseek(entrada, -1, SEEK_CUR);
                    break;
                }
            }
        }

        else if (letra == '\n')
        {
            tokens.push_back(Token("QUEBRA_LINHA", linha_atual, coluna_atual));
            coluna_atual = 1;
            linha_atual++;
            continue;
        }
        else
        {
            if (letra != ' ' && letra != '\t')
            {
                string message = "Caracter indefinido: ";
                message += letra;
                erros.push_back(Erro(message, linha_atual, coluna_atual));
            }
            if (letra == '\t')
            {
                coluna_atual += TAB_SIZE;
                continue;
            }
        }
        coluna_atual++;
    }

    imprime_tokens(tokens);
    imprime_erros(erros);
    return 0;
}