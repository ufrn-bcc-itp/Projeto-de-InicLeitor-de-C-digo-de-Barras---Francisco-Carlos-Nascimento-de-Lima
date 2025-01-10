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

public:
    CodigoDeBarras(const std::string &id, int largura = 3, int altura = 50, int espaco = 4)
        : identificador(id), largura_por_digito(largura), altura(altura), espaco_lateral(espaco)
    {
        if (!validarIdentificador(id))
        {
            throw std::invalid_argument("Identificador inválido. Deve conter 8 dígitos numéricos.");
        }
    }

    std::vector<std::string> gerarImagemPBM() const
    {
        int largura_total = espaco_lateral * 2 + identificador.size() * largura_por_digito;
        std::vector<std::string> imagem(altura, std::string(largura_total, '0'));

        for (int i = 0; i < altura; ++i)
        {
            for (int j = 0; j < espaco_lateral; ++j)
            {
                imagem[i][j] = '0';
                imagem[i][largura_total - j - 1] = '0';
            }
        }

        for (size_t i = 0; i < identificador.size(); ++i)
        {
            int inicio = espaco_lateral + i * largura_por_digito;
            char bit = (identificador[i] - '0') % 2 == 0 ? '1' : '0';
            for (int x = 0; x < largura_por_digito; ++x)
            {
                for (int y = 0; y < altura; ++y)
                {
                    imagem[y][inicio + x] = bit;
                }
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
        size_t largura_por_digito = (largura - espaco_lateral * 2) / 8;
        for (size_t i = 0; i < 8; ++i)
        {
            size_t inicio = espaco_lateral + i * largura_por_digito;
            char bit = imagemPBM[0][inicio];
            identificador += (bit == '1') ? '0' : '1';
        }

        return identificador;
    }

    static bool validarIdentificador(const std::string &id)
    {
        if (id.size() != 8)
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
};

#endif // CODIGO_BARRAS_H
