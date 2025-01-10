#include "codigo_barras.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: ./gerar_codigo <identificador> [largura_por_digito altura espaco_lateral]" << std::endl;
        return 1;
    }

    std::string identificador = argv[1];
    int largura = (argc > 2) ? std::stoi(argv[2]) : 3;
    int altura = (argc > 3) ? std::stoi(argv[3]) : 50;
    int espaco = (argc > 4) ? std::stoi(argv[4]) : 4;

    try
    {
        CodigoDeBarras codigo(identificador, largura, altura, espaco);
        auto imagem = codigo.gerarImagemPBM();

        std::ofstream arquivo("codigo_barras.pbm");
        arquivo << "P1\n";
        arquivo << imagem[0].size() << " " << imagem.size() << "\n";
        for (const auto &linha : imagem)
        {
            arquivo << linha << "\n";
        }
        arquivo.close();

        std::cout << "Imagem PBM gerada com sucesso em 'codigo_barras.pbm'." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}