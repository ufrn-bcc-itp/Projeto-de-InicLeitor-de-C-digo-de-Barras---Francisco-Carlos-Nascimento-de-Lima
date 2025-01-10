#include "codigo_barras.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: ./extrair_codigo <arquivo_pbm>" << std::endl;
        return 1;
    }

    std::string arquivo_pbm = argv[1];

    try
    {
        std::ifstream arquivo(arquivo_pbm);
        if (!arquivo.is_open())
        {
            throw std::runtime_error("Não foi possível abrir o arquivo PBM.");
        }

        std::string linha;
        std::vector<std::string> imagem;
        std::getline(arquivo, linha); // P1
        std::getline(arquivo, linha); // Dimensões
        while (std::getline(arquivo, linha))
        {
            imagem.push_back(linha);
        }
        arquivo.close();

        std::string identificador = CodigoDeBarras::extrairIdentificador(imagem);
        std::cout << "Identificador extraído: " << identificador << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
