// Arquivo: codigo_barras.h
#ifndef CODIGO_BARRAS_H
#define CODIGO_BARRAS_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>

class CodigoDeBarras
{
private:
    std::string identificador;
    int largura_por_digito;
    int altura;
    int espaco_lateral;

    static constexpr int MARCADOR_INICIO = 3;
    static constexpr int MARCADOR_CENTRAL = 5;
    static constexpr int MARCADOR_FIM = 3;
    static constexpr int DIGITOS = 8;
    static constexpr int LARGURA_CODIFICACAO = 7;

public:
    CodigoDeBarras(const std::string &id, int largura = 7, int altura = 50, int espaco = 4)
        : identificador(id), largura_por_digito(largura), altura(altura), espaco_lateral(espaco)
    {
        if (!validarIdentificador(id))
        {
            throw std::invalid_argument("Identificador inválido. Deve conter 8 dígitos numéricos.");
        }
    }

    std::vector<std::string> gerarImagemPBM() const
    {
        int largura_total = espaco_lateral * 2 +
                            MARCADOR_INICIO +
                            DIGITOS * LARGURA_CODIFICACAO +
                            MARCADOR_CENTRAL +
                            MARCADOR_FIM;

        // Adiciona altura extra para exibir o número abaixo do código de barras
        int altura_total = altura + 8; // 8 linhas para o número
        std::vector<std::string> imagem(altura_total, std::string(largura_total, '0'));

        // Adiciona o marcador de início
        preencherArea(imagem, espaco_lateral, MARCADOR_INICIO, '1', 0, altura);

        // Adiciona os dígitos codificados
        for (size_t i = 0; i < identificador.size() / 2; ++i)
        {
            int inicio = espaco_lateral + MARCADOR_INICIO + i * LARGURA_CODIFICACAO;
            preencherArea(imagem, inicio, LARGURA_CODIFICACAO, (identificador[i] - '0') % 2 == 0 ? '1' : '0', 0, altura);
        }

        // Adiciona o marcador central
        preencherArea(imagem, espaco_lateral + MARCADOR_INICIO + DIGITOS / 2 * LARGURA_CODIFICACAO, MARCADOR_CENTRAL, '1', 0, altura);

        // Adiciona os dígitos codificados restantes
        for (size_t i = identificador.size() / 2; i < identificador.size(); ++i)
        {
            int inicio = espaco_lateral + MARCADOR_INICIO + MARCADOR_CENTRAL + (i - 4) * LARGURA_CODIFICACAO;
            preencherArea(imagem, inicio, LARGURA_CODIFICACAO, (identificador[i] - '0') % 2 == 0 ? '1' : '0', 0, altura);
        }

        // Adiciona o marcador de fim
        preencherArea(imagem, largura_total - espaco_lateral - MARCADOR_FIM, MARCADOR_FIM, '1', 0, altura);

        // Adiciona o número abaixo do código de barras
        for (size_t i = 0; i < identificador.size(); ++i)
        {
            char digito = identificador[i];
            int inicio = espaco_lateral + MARCADOR_INICIO + i * LARGURA_CODIFICACAO + LARGURA_CODIFICACAO / 2;
            if (inicio < largura_total)
            {
                imagem[altura + 1][inicio] = digito; // Posiciona o caractere na linha extra
            }
        }

        return imagem;
    }

    static std::string extrairIdentificador(const std::vector<std::string> &imagemPBM)
    {
        if (imagemPBM.empty())
        {
            throw std::invalid_argument("Imagem PBM inválida ou vazia.");
        }

        size_t largura = imagemPBM[0].size();
        size_t altura = imagemPBM.size();
        size_t espaco_lateral = 0;

        for (size_t j = 0; j < largura; ++j)
        {
            bool vazio = true;
            for (size_t i = 0; i < altura; ++i)
            {
                if (imagemPBM[i][j] == '1')
                {
                    vazio = false;
                    break;
                }
            }
            if (vazio)
            {
                ++espaco_lateral;
            }
            else
            {
                break;
            }
        }

        std::string identificador;
        size_t largura_por_digito = LARGURA_CODIFICACAO;
        for (size_t i = 0; i < DIGITOS; ++i)
        {
            size_t inicio = espaco_lateral + MARCADOR_INICIO + i * largura_por_digito;
            char bit = imagemPBM[0][inicio];
            identificador += (bit == '1') ? '0' : '1';
        }

        return identificador;
    }

    static bool validarIdentificador(const std::string &id)
    {
        if (id.size() != DIGITOS)
        {
            return false;
        }
        for (char c : id)
        {
            if (!isdigit(c))
            {
                return false;
            }
        }
        return true;
    }

private:
    void preencherArea(std::vector<std::string> &imagem, int inicio, int largura, char valor, int inicio_altura, int fim_altura) const
    {
        for (int i = inicio_altura; i < fim_altura; ++i)
        {
            for (int j = 0; j < largura; ++j)
            {
                imagem[i][inicio + j] = valor;
            }
        }
    }
};

#endif // CODIGO_BARRAS_H
