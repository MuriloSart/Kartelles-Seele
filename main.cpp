#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
#include <windows.h>
using namespace std;

#define ESC    	27

/*
	Colocar MostrarColetaveisMissao() para mostrar as moedas ou doces das respectiveis fases,
	Criar função para receber item ao completar
*/
//local para os structs
typedef struct _blocosColisao
{
  int x, y, altura, largura, tipo, texto;
  bool colidido, cliqueMouse, coletado;
  void *spriteMascara;
  void *sprite;
}BlocoDeColisao;

//criando o vetor para blocos de colisão
BlocoDeColisao *blocosColisao;
int qntBlocos = 85;

//=> Fases
int fases = 21;
bool pegouMissao = false;
bool entrou = false;
bool jaEntrou = false;
bool jaEntrou2 = false;
bool jaEntrou3 = false;
bool tocou = false;
bool trocarDeFase = true;

//=> Inventário
void *inventarioImagem;
bool inventario = true;

//=> Texto
int texto = 41;
bool rolarTexto = true;

//checagem de itens coletados
int qntItensColetados = 0;
int qntMoedasColetadas = 0;
int qntDocesColetados = 0;
 
//variável para controlar qual personagem terá no mapa 
int variavelDeControle = 18;

//=====================> Tempo de Espera para Clique do Mouse <=====================
int Espera = 500;
double TempoDecorrido;
long long Inicio, Agora;
					
//===============================> Mouse <===============================
HWND janela;//coletando a janela
POINT P;//posição do Mouse
int indexItemColidido = 0;//coletandoBlocoColidido
bool colisaoMouse = false;//verificando se o mouse colidiu com algum bloco
  
//===============================> Inventário <===============================
int InvLargura = 400;
int InvAltura = 720;
int xInv = 1280 - InvLargura;
int yInv = 0;
 
//=====================> Funções <=====================
//=> Área para Colisões
bool ChecagemDeColisao(int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu);

void ChecagemDeColisaoDoMouse();

void ZerandoColisoes();

//= > Área para Desenhar e Criar
void CriandoInvetario(int IndexInicial, int IndexFinal, int index);

void DesenhandoBotao( int xImagem, int yImagem, void *sprites,void *spritesMascara);

void* carregarImagem(const char *sprite, int largura, int altura , int x, int y);

void CriandoPersonagem(int index);

void MostrarColetaveisMissao(int indexMenor, int indexMaior);

void MostrarColetaveis(int index);

//= > Área para Lidar com Leveis
void LidandoComFases(void *cenario, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal, bool TemPersonagem);

void Menu(void *cenario, int &fases);

void SaindoDoTutorial();

void AtivandoFinal();

void CutScene(void *cenario);

void CondicionaisDeFases(void **cenarios);

//= > Área para Lidar com Personagens
void LidandoComPersonagem(int &index);

void LidandoComAsFolhas(int &index);

void LidandoComABruxa(int primeiroItemMissao, int ultimoItemMissao);

void LidandoComMissoes(int tipo);

void FinalizandoMissoes();

void FinalMissao(int circunstancia, int contador);

//= > Área para Coletas de Itens
void ColetarItensFase();

void ColetarItensMissoes(int tipo, int &quantidade, int indexItemColididos);

//=> Textos
void CaixaDeTexto(int index);

void LidandoComTextos();

void TextoAoInteragir(int qualTexto);

void CondicionaisTextos();

void IndicacaoParaTrocaDeTexto();

//=================================================> JOGO <=================================================
int main()
{
  //===============================> Carrega som<=========================
  mciSendString("open .\\sons\\Kartelles_Seele.mp3 type MPEGVideo alias jogo", NULL, 0, 0); 
    
  //===============================> Lidando com a janela <===============================
  int pg = 1;
  char tecla;	
  unsigned long long gt1, gt2;//Clocks do Computador
  int fps = 60;
  
  initwindow(1280, 720, "Kartelles Seele");
  janela = GetForegroundWindow();
  setactivepage(pg);//Deixa ativa a pagina do laço para desenhar nela
  setvisualpage(pg);//Deixa visual para mostrar o que foi desenhado na tela
  tecla = 0;//index do teclado

  //===============================> Blocos de Colisão para o Mouse <===============================
  //Vetor que está guardando os blocos
  blocosColisao = NULL;
  blocosColisao = (BlocoDeColisao *) malloc(sizeof(BlocoDeColisao) * qntBlocos);
  
  //===============================> Botões para troca de fases <===============================
  //botão para cima
  blocosColisao[0].x = 640;
  blocosColisao[0].y = 20;
  blocosColisao[0].altura = 64;
  blocosColisao[0].largura = 64;
  blocosColisao[0].tipo = 0;
  blocosColisao[0].texto = 40;
  blocosColisao[0].colidido = false;
  blocosColisao[0].cliqueMouse = false;
  blocosColisao[0].coletado = false;
  blocosColisao[0].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Cima_Mascara.bmp", blocosColisao[0].largura, blocosColisao[0].altura, 0, 0);
  blocosColisao[0].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Cima.bmp", blocosColisao[0].largura, blocosColisao[0].altura, 0, 0);
  
  //botão para direita
  blocosColisao[1].altura = 64;
  blocosColisao[1].largura = 64;
  blocosColisao[1].x = 1250 - blocosColisao[1].largura;
  blocosColisao[1].y = 360;
  blocosColisao[1].tipo = 1;
  blocosColisao[1].texto = 40;
  blocosColisao[1].colidido = false;
  blocosColisao[1].cliqueMouse = false;
  blocosColisao[1].coletado = false;
  blocosColisao[1].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Direita_Mascara.bmp", blocosColisao[1].largura, blocosColisao[1].altura, 0, 0);
  blocosColisao[1].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Direita.bmp", blocosColisao[1].largura, blocosColisao[1].altura, 0, 0);
  
  //botão para esquerda
  blocosColisao[2].altura = 64;
  blocosColisao[2].largura = 64;
  blocosColisao[2].x = 30;
  blocosColisao[2].y = 360;
  blocosColisao[2].tipo = 2;
  blocosColisao[2].texto = 40;
  blocosColisao[2].colidido = false;
  blocosColisao[2].cliqueMouse = false;
  blocosColisao[2].coletado = false;
  blocosColisao[2].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Esquerda_Mascara.bmp", blocosColisao[2].largura, blocosColisao[2].altura, 0, 0);
  blocosColisao[2].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Esquerda.bmp", blocosColisao[2].largura, blocosColisao[2].altura, 0, 0);
  
  //botão para baixo
  blocosColisao[3].altura = 64;
  blocosColisao[3].largura = 64;
  blocosColisao[3].x = 640;
  blocosColisao[3].y = 600 - blocosColisao[3].altura - 10;
  blocosColisao[3].tipo = 3;
  blocosColisao[3].texto = 40;
  blocosColisao[3].colidido = false;
  blocosColisao[3].cliqueMouse = false;
  blocosColisao[3].coletado = false;
  blocosColisao[3].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Baixo_Mascara.bmp", blocosColisao[3].largura, blocosColisao[3].altura, 0, 0);
  blocosColisao[3].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Baixo.bmp", blocosColisao[3].largura, blocosColisao[3].altura, 0, 0);
  
  //inventário
  blocosColisao[4].altura = 64;
  blocosColisao[4].largura = 64;
  blocosColisao[4].tipo = 4;
  blocosColisao[4].texto = 40;
  blocosColisao[4].x = 1280 - blocosColisao[4].largura;
  blocosColisao[4].y = 30;
  blocosColisao[4].colidido = false;
  blocosColisao[4].cliqueMouse = false;
  blocosColisao[4].coletado = false;
  blocosColisao[4].spriteMascara = carregarImagem(".//Artes//Botoes//icone_Inventario_Mascara.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[4].sprite = carregarImagem(".//Artes//Botoes//icone_Inventario.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //===============================> ITENS <===============================
  //=>Ervas
  blocosColisao[5].x = 318;
  blocosColisao[5].y = 198;
  blocosColisao[5].altura = 54;
  blocosColisao[5].largura = 96;
  blocosColisao[5].tipo = 5;
  blocosColisao[5].texto = 48;
  blocosColisao[5].colidido = false;
  blocosColisao[5].cliqueMouse = false;
  blocosColisao[5].coletado = false;
  blocosColisao[5].sprite = carregarImagem(".//Artes//Itens//ervas.bmp", blocosColisao[5].largura, blocosColisao[5].altura, 0, 0);
  blocosColisao[5].spriteMascara = carregarImagem(".//Artes//Itens//ervas_masc.bmp", blocosColisao[5].largura, blocosColisao[5].altura, 0, 0);
  
  //=>Pote de Sal
  blocosColisao[6].x = 900;
  blocosColisao[6].y = 527;
  blocosColisao[6].altura = 54;
  blocosColisao[6].largura = 96;
  blocosColisao[6].tipo = 5;
  blocosColisao[6].texto = 50;
  blocosColisao[6].colidido = false;
  blocosColisao[6].cliqueMouse = false;
  blocosColisao[6].coletado = false;
  blocosColisao[6].sprite = carregarImagem(".//Artes//Itens//pote_de_sal.bmp", blocosColisao[6].largura, blocosColisao[6].altura, 0, 0);
  blocosColisao[6].spriteMascara = carregarImagem(".//Artes//Itens//pote_de_sal_masc.bmp", blocosColisao[6].largura, blocosColisao[6].altura, 0, 0);
  
  //=>Relicário
  blocosColisao[7].x = 518;
  blocosColisao[7].y = 185;
  blocosColisao[7].altura = 54;
  blocosColisao[7].largura = 96;
  blocosColisao[7].tipo = 5;
  blocosColisao[7].texto = 46;
  blocosColisao[7].colidido = false;
  blocosColisao[7].cliqueMouse = false;
  blocosColisao[7].coletado = false;
  blocosColisao[7].sprite = carregarImagem(".//Artes//Itens//relicario.bmp", blocosColisao[7].largura, blocosColisao[7].altura, 0, 0);
  blocosColisao[7].spriteMascara = carregarImagem(".//Artes//Itens//relicario_masc.bmp", blocosColisao[7].largura, blocosColisao[7].altura, 0, 0);
  
  //=>Boneca
  blocosColisao[8].x = 965;
  blocosColisao[8].y = 387;
  blocosColisao[8].altura = 54;
  blocosColisao[8].largura = 96;
  blocosColisao[8].tipo = 5;
  blocosColisao[8].texto = 44;
  blocosColisao[8].colidido = false;
  blocosColisao[8].cliqueMouse = false;
  blocosColisao[8].coletado = false;
  blocosColisao[8].sprite = carregarImagem(".//Artes//Itens//boneca.bmp", blocosColisao[8].largura, blocosColisao[8].altura, 0, 0);
  blocosColisao[8].spriteMascara = carregarImagem(".//Artes//Itens//boneca_masc.bmp", blocosColisao[8].largura, blocosColisao[8].altura, 0, 0);
  
  //=>Estrela Judaica
  blocosColisao[9].x = 920;
  blocosColisao[9].y = 90;
  blocosColisao[9].altura = 54;
  blocosColisao[9].largura = 96;
  blocosColisao[9].tipo = 5;
  blocosColisao[9].texto = 51;
  blocosColisao[9].colidido = false;
  blocosColisao[9].cliqueMouse = false;
  blocosColisao[9].coletado = false;
  blocosColisao[9].sprite = carregarImagem(".//Artes//Itens//caixa_estrela.bmp", blocosColisao[9].largura, blocosColisao[9].altura, 0, 0);
  blocosColisao[9].spriteMascara = carregarImagem(".//Artes//Itens//caixa_estrela_masc.bmp", blocosColisao[9].largura, blocosColisao[9].altura, 0, 0);
  
  //Parte depois do Tutorial
  //=>COGUMELO
  blocosColisao[10].x = 900;
  blocosColisao[10].y = 255;
  blocosColisao[10].altura = 54;
  blocosColisao[10].largura = 96;
  blocosColisao[10].tipo = 5;
  blocosColisao[10].texto = 40;
  blocosColisao[10].colidido = false;
  blocosColisao[10].cliqueMouse = false;
  blocosColisao[10].coletado = true;
  blocosColisao[10].sprite = carregarImagem(".//Artes//Itens//cogumelo.bmp", blocosColisao[10].largura, blocosColisao[10].altura, 0, 0);
  blocosColisao[10].spriteMascara = carregarImagem(".//Artes//Itens//cogumelo_masc.bmp", blocosColisao[10].largura, blocosColisao[10].altura, 0, 0);
  
  //=>Caveira
  blocosColisao[11].x = 23;
  blocosColisao[11].y = 198;
  blocosColisao[11].altura = 96;
  blocosColisao[11].largura = 54;
  blocosColisao[11].tipo = 5;
  blocosColisao[11].texto = 40;
  blocosColisao[11].colidido = false;
  blocosColisao[11].cliqueMouse = false;
  blocosColisao[11].coletado = true;
  blocosColisao[11].sprite = carregarImagem(".//Artes//Itens//caveira.bmp", blocosColisao[11].largura, blocosColisao[11].altura, 0, 0);
  blocosColisao[11].spriteMascara = carregarImagem(".//Artes//Itens//caveira_masc.bmp", blocosColisao[11].largura, blocosColisao[11].altura, 0, 0);
  
  //=>Olho Mágico
  blocosColisao[12].x = 1050;
  blocosColisao[12].y = 350;
  blocosColisao[12].altura = 54;
  blocosColisao[12].largura = 96;
  blocosColisao[12].tipo = 5;
  blocosColisao[12].texto = 40;
  blocosColisao[12].colidido = false;
  blocosColisao[12].cliqueMouse = false;
  blocosColisao[12].coletado = true;
  blocosColisao[12].sprite = carregarImagem(".//Artes//Itens//olho_magico.bmp", blocosColisao[12].largura, blocosColisao[12].altura, 0, 0);
  blocosColisao[12].spriteMascara = carregarImagem(".//Artes//Itens//olho_magico_masc.bmp", blocosColisao[12].largura, blocosColisao[12].altura, 0, 0);
  
  //=>Cálice
  blocosColisao[13].x = 403;
  blocosColisao[13].y = 168;
  blocosColisao[13].altura = 54;
  blocosColisao[13].largura = 96;
  blocosColisao[13].tipo = 5;
  blocosColisao[13].texto = 40;
  blocosColisao[13].colidido = false;
  blocosColisao[13].cliqueMouse = false;
  blocosColisao[13].coletado = true;
  blocosColisao[13].sprite = carregarImagem(".//Artes//Itens//calice.bmp", blocosColisao[13].largura, blocosColisao[13].altura, 0, 0);
  blocosColisao[13].spriteMascara = carregarImagem(".//Artes//Itens//calice_masc.bmp", blocosColisao[13].largura, blocosColisao[13].altura, 0, 0);
  
  //=>Giz
  blocosColisao[14].x = 1200;
  blocosColisao[14].y = 480;
  blocosColisao[14].altura = 54;
  blocosColisao[14].largura = 96;
  blocosColisao[14].tipo = 5;
  blocosColisao[14].texto = 40;
  blocosColisao[14].colidido = false;
  blocosColisao[14].cliqueMouse = false;
  blocosColisao[14].coletado = true;
  blocosColisao[14].sprite = carregarImagem(".//Artes//Itens//giz.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  blocosColisao[14].spriteMascara = carregarImagem(".//Artes//Itens//giz_masc.bmp", blocosColisao[14].largura, blocosColisao[14].altura, 0, 0);
  
  //=>incenso
  blocosColisao[15].x = 900;
  blocosColisao[15].y = 425;
  blocosColisao[15].altura = 64;
  blocosColisao[15].largura = 64;
  blocosColisao[15].tipo = 5;
  blocosColisao[15].texto = 40;
  blocosColisao[15].colidido = false;
  blocosColisao[15].cliqueMouse = false;
  blocosColisao[15].coletado = false;
  blocosColisao[15].sprite = carregarImagem(".//Artes//Itens//incenso.bmp", blocosColisao[15].largura, blocosColisao[15].altura, 0, 0);
  blocosColisao[15].spriteMascara = carregarImagem(".//Artes//Itens//incenso_masc.bmp", blocosColisao[15].largura, blocosColisao[15].altura, 0, 0);
  
  //=>vela
  blocosColisao[16].x = 1205;
  blocosColisao[16].y = 270;
  blocosColisao[16].altura = 64;
  blocosColisao[16].largura = 64;
  blocosColisao[16].tipo = 5;
  blocosColisao[16].texto = 40;
  blocosColisao[16].colidido = false;
  blocosColisao[16].cliqueMouse = false;
  blocosColisao[16].coletado = false;
  blocosColisao[16].sprite = carregarImagem(".//Artes//Itens//vela.bmp", blocosColisao[16].largura, blocosColisao[16].altura, 0, 0);
  blocosColisao[16].spriteMascara = carregarImagem(".//Artes//Itens//vela_masc.bmp", blocosColisao[16].largura, blocosColisao[16].altura, 0, 0);
  
  //=>adaga
  blocosColisao[17].x = 1050;
  blocosColisao[17].y = 100;
  blocosColisao[17].altura = 64;
  blocosColisao[17].largura = 64;
  blocosColisao[17].tipo = 5;
  blocosColisao[17].texto = 40;
  blocosColisao[17].colidido = false;
  blocosColisao[17].cliqueMouse = false;
  blocosColisao[17].coletado = false;
  blocosColisao[17].sprite = carregarImagem(".//Artes//Itens//adaga.bmp", blocosColisao[17].largura, blocosColisao[17].altura, 0, 0);
  blocosColisao[17].spriteMascara = carregarImagem(".//Artes//Itens//adaga_masc.bmp", blocosColisao[17].largura, blocosColisao[17].altura, 0, 0);
  
  //===============================> PERSONAGENS <===============================
  //=>Folhas
  blocosColisao[18].x = 500;
  blocosColisao[18].y = 300;
  blocosColisao[18].altura = 173;
  blocosColisao[18].largura = 343;
  blocosColisao[18].tipo = 6;
  blocosColisao[18].texto = 40;
  blocosColisao[18].colidido = false;
  blocosColisao[18].cliqueMouse = false;
  blocosColisao[18].coletado = false;
  blocosColisao[18].sprite = carregarImagem(".//Artes//Personagens//folhas.bmp", blocosColisao[18].largura, blocosColisao[18].altura, 0, 0);
  blocosColisao[18].spriteMascara = carregarImagem(".//Artes//Personagens//folhas_masc.bmp", blocosColisao[18].largura, blocosColisao[18].altura, 0, 0);
  
  //=>Folhas Outline
  blocosColisao[19].x = blocosColisao[18].x;
  blocosColisao[19].y = blocosColisao[18].y;
  blocosColisao[19].altura = blocosColisao[18].altura;
  blocosColisao[19].largura = blocosColisao[18].largura;
  blocosColisao[19].tipo = blocosColisao[18].tipo;
  blocosColisao[19].texto = blocosColisao[18].texto;
  blocosColisao[19].colidido = false;
  blocosColisao[19].cliqueMouse = false;
  blocosColisao[19].coletado = false;
  blocosColisao[19].sprite = carregarImagem(".//Artes//Personagens//folhas_out.bmp", blocosColisao[18].largura, blocosColisao[18].altura, 0, 0);
  blocosColisao[19].spriteMascara = carregarImagem(".//Artes//Personagens//folhas_out_masc.bmp", blocosColisao[18].largura, blocosColisao[18].altura, 0, 0);
  
  //=>Lontra
  blocosColisao[20].x = blocosColisao[18].x + 100;
  blocosColisao[20].y = blocosColisao[18].y;
  blocosColisao[20].altura = 255;
  blocosColisao[20].largura = 255;
  blocosColisao[20].tipo = 7;
  blocosColisao[20].texto = 55;
  blocosColisao[20].colidido = false;
  blocosColisao[20].cliqueMouse = false;
  blocosColisao[20].coletado = false;
  blocosColisao[20].sprite = carregarImagem(".//Artes//Personagens//lontra.bmp", blocosColisao[20].largura, blocosColisao[20].altura, 0, 0);
  blocosColisao[20].spriteMascara = carregarImagem(".//Artes//Personagens//lontra_masc.bmp", blocosColisao[20].largura, blocosColisao[20].altura, 0, 0);
  
  //=>Lontra Outline
  blocosColisao[21].x = blocosColisao[20].x;
  blocosColisao[21].y = blocosColisao[20].y;
  blocosColisao[21].altura = blocosColisao[20].altura;
  blocosColisao[21].largura = blocosColisao[20].largura;
  blocosColisao[21].tipo = blocosColisao[20].tipo;
  blocosColisao[21].tipo = blocosColisao[20].tipo;
  blocosColisao[21].colidido = false;
  blocosColisao[21].cliqueMouse = false;
  blocosColisao[21].coletado = false;
  blocosColisao[21].sprite = carregarImagem(".//Artes//Personagens//lontra_outline.bmp", blocosColisao[20].largura, blocosColisao[20].altura, 0, 0);
  blocosColisao[21].spriteMascara = carregarImagem(".//Artes//Personagens//lontra_outline_masc.bmp", blocosColisao[20].largura, blocosColisao[20].altura, 0, 0);
  
  //=>Bruxa
  blocosColisao[22].x = 480;
  blocosColisao[22].y = 8;
  blocosColisao[22].altura = 300;
  blocosColisao[22].largura = 300;
  blocosColisao[22].tipo = 8;
  blocosColisao[22].texto = 71;
  blocosColisao[22].colidido = false;
  blocosColisao[22].cliqueMouse = false;
  blocosColisao[22].coletado = false;
  blocosColisao[22].sprite = carregarImagem(".//Artes//Personagens//bruxa.bmp", blocosColisao[22].largura, blocosColisao[22].altura, 0, 0);
  blocosColisao[22].spriteMascara = carregarImagem(".//Artes//Personagens//bruxa_masc.bmp", blocosColisao[22].largura, blocosColisao[22].altura, 0, 0);
  
  //=>Bruxa Outline
  blocosColisao[23].x = blocosColisao[22].x;
  blocosColisao[23].y = blocosColisao[22].y;
  blocosColisao[23].altura = blocosColisao[22].altura;
  blocosColisao[23].largura = blocosColisao[22].largura;
  blocosColisao[23].tipo = blocosColisao[22].tipo;
  blocosColisao[23].texto = blocosColisao[22].texto;
  blocosColisao[23].colidido = false;
  blocosColisao[23].cliqueMouse = false;
  blocosColisao[23].coletado = false;
  blocosColisao[23].sprite = carregarImagem(".//Artes//Personagens//bruxa_outline.bmp", blocosColisao[22].largura, blocosColisao[22].altura, 0, 0);
  blocosColisao[23].spriteMascara = carregarImagem(".//Artes//Personagens//bruxa_outline_masc.bmp", blocosColisao[22].largura, blocosColisao[22].altura, 0, 0);
  
  //=>Bruxa2
  blocosColisao[24].x = 600;
  blocosColisao[24].y = 265;
  blocosColisao[24].altura = 137;
  blocosColisao[24].largura = 137;
  blocosColisao[24].tipo = 7;
  blocosColisao[24].texto = 84;
  blocosColisao[24].colidido = false;
  blocosColisao[24].cliqueMouse = false;
  blocosColisao[24].coletado = false;
  blocosColisao[24].sprite = carregarImagem(".//Artes//Personagens//bruxa2.bmp", blocosColisao[24].largura, blocosColisao[24].altura, 0, 0);
  blocosColisao[24].spriteMascara = carregarImagem(".//Artes//Personagens//bruxa2_masc.bmp", blocosColisao[24].largura, blocosColisao[24].altura, 0, 0);
  
  //=>Bruxa2 Outline
  blocosColisao[25].x = blocosColisao[24].x;
  blocosColisao[25].y = blocosColisao[24].y;
  blocosColisao[25].altura = blocosColisao[24].altura;
  blocosColisao[25].largura = blocosColisao[24].largura;
  blocosColisao[25].tipo = blocosColisao[24].tipo;
  blocosColisao[25].texto = blocosColisao[24].texto;
  blocosColisao[25].colidido = false;
  blocosColisao[25].cliqueMouse = false;
  blocosColisao[25].coletado = false;
  blocosColisao[25].sprite = carregarImagem(".//Artes//Personagens//bruxa2_outline.bmp", blocosColisao[24].largura, blocosColisao[24].altura, 0, 0);
  blocosColisao[25].spriteMascara = carregarImagem(".//Artes//Personagens//bruxa2_out_masc.bmp", blocosColisao[24].largura, blocosColisao[24].altura, 0, 0);
  
  //=>Guaxinim
  blocosColisao[26].x = 350;
  blocosColisao[26].y = 300;
  blocosColisao[26].altura = 255;
  blocosColisao[26].largura = 255;
  blocosColisao[26].tipo = 9;
  blocosColisao[26].texto = 40;
  blocosColisao[26].colidido = false;
  blocosColisao[26].cliqueMouse = false;
  blocosColisao[26].coletado = false;
  blocosColisao[26].sprite = carregarImagem(".//Artes//Personagens//guaxi.bmp", blocosColisao[26].largura, blocosColisao[26].altura, 0, 0);
  blocosColisao[26].spriteMascara = carregarImagem(".//Artes//Personagens//guaxi_masc.bmp", blocosColisao[26].largura, blocosColisao[26].altura, 0, 0);
  
  //=>Guaxinim Outline
  blocosColisao[27].x = blocosColisao[26].x;
  blocosColisao[27].y = blocosColisao[26].y;
  blocosColisao[27].altura = blocosColisao[26].altura;
  blocosColisao[27].largura = blocosColisao[26].largura;
  blocosColisao[27].tipo = blocosColisao[26].tipo;
  blocosColisao[27].texto = blocosColisao[26].texto;
  blocosColisao[27].colidido = false;
  blocosColisao[27].cliqueMouse = false;
  blocosColisao[27].coletado = false;
  blocosColisao[27].sprite = carregarImagem(".//Artes//Personagens//guaxi_outline.bmp", blocosColisao[26].largura, blocosColisao[26].altura, 0, 0);
  blocosColisao[27].spriteMascara = carregarImagem(".//Artes//Personagens//guaxi_outline_masc.bmp", blocosColisao[26].largura, blocosColisao[26].altura, 0, 0);
  
  //=>Cat
  blocosColisao[28].x = 550;
  blocosColisao[28].y = 30;
  blocosColisao[28].altura = 400;
  blocosColisao[28].largura = 170;
  blocosColisao[28].tipo = 10;
  blocosColisao[28].texto = 40;
  blocosColisao[28].colidido = false;
  blocosColisao[28].cliqueMouse = false;
  blocosColisao[28].coletado = false;
  blocosColisao[28].sprite = carregarImagem(".//Artes//Personagens//cat.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  blocosColisao[28].spriteMascara = carregarImagem(".//Artes//Personagens//cat_masc.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  
  //=>Cat Outline
  blocosColisao[29].x = blocosColisao[28].x;
  blocosColisao[29].y = blocosColisao[28].y;
  blocosColisao[29].altura = blocosColisao[28].altura;
  blocosColisao[29].largura = blocosColisao[28].largura;
  blocosColisao[29].tipo = blocosColisao[28].tipo;
  blocosColisao[29].texto = blocosColisao[28].texto;
  blocosColisao[29].colidido = false;
  blocosColisao[29].cliqueMouse = false;
  blocosColisao[29].coletado = false;
  blocosColisao[29].sprite = carregarImagem(".//Artes//Personagens//cat_out.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  blocosColisao[29].spriteMascara = carregarImagem(".//Artes//Personagens//cat_out_masc.bmp", blocosColisao[28].largura, blocosColisao[28].altura, 0, 0);
  
  //===============================> ITENS DE MISSÕES <===============================
  //=>Moedas 1
  blocosColisao[30].x = 900;
  blocosColisao[30].y = 130;
  blocosColisao[30].altura = 64;
  blocosColisao[30].largura = 64;
  blocosColisao[30].tipo = 11;
  blocosColisao[30].colidido = false;
  blocosColisao[30].cliqueMouse = false;
  blocosColisao[30].coletado = true;
  blocosColisao[30].sprite = carregarImagem(".//Artes//Itens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[30].spriteMascara = carregarImagem(".//Artes//Itens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Moedas 2
  blocosColisao[31].x = 500;
  blocosColisao[31].y = 130;
  blocosColisao[31].altura = blocosColisao[30].altura;
  blocosColisao[31].largura = blocosColisao[30].largura;
  blocosColisao[31].tipo = blocosColisao[30].tipo;
  blocosColisao[31].colidido = false;
  blocosColisao[31].cliqueMouse = false;
  blocosColisao[31].coletado = true;
  blocosColisao[31].sprite = carregarImagem(".//Artes//Itens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[31].spriteMascara = carregarImagem(".//Artes//Itens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Moedas 3
  blocosColisao[32].x = 600;
  blocosColisao[32].y = 300;
  blocosColisao[32].altura = blocosColisao[30].altura;
  blocosColisao[32].largura = blocosColisao[30].largura;
  blocosColisao[32].tipo = blocosColisao[30].tipo;
  blocosColisao[32].colidido = false;
  blocosColisao[32].cliqueMouse = false;
  blocosColisao[32].coletado = true;
  blocosColisao[32].sprite = carregarImagem(".//Artes//Itens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[32].spriteMascara = carregarImagem(".//Artes//Itens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Moedas 4
  blocosColisao[33].x = 900;
  blocosColisao[33].y = 500;
  blocosColisao[33].altura = blocosColisao[30].altura;
  blocosColisao[33].largura = blocosColisao[30].largura;
  blocosColisao[33].tipo = blocosColisao[30].tipo;
  blocosColisao[33].colidido = false;
  blocosColisao[33].cliqueMouse = false;
  blocosColisao[33].coletado = true;
  blocosColisao[33].sprite = carregarImagem(".//Artes//Itens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[33].spriteMascara = carregarImagem(".//Artes//Itens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Moedas 5
  blocosColisao[34].x = 1100;
  blocosColisao[34].y = 20;
  blocosColisao[34].altura = blocosColisao[30].altura;
  blocosColisao[34].largura = blocosColisao[30].largura;
  blocosColisao[34].tipo = blocosColisao[30].tipo;
  blocosColisao[34].colidido = false;
  blocosColisao[34].cliqueMouse = false;
  blocosColisao[34].coletado = true;
  blocosColisao[34].sprite = carregarImagem(".//Artes//Itens//moeda.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  blocosColisao[34].spriteMascara = carregarImagem(".//Artes//Itens//moeda_masc.bmp", blocosColisao[30].largura, blocosColisao[30].altura, 0, 0);
  
  //=>Doce 1
  blocosColisao[35].x = 900;
  blocosColisao[35].y = 400;
  blocosColisao[35].altura = 64;
  blocosColisao[35].largura = 64;
  blocosColisao[35].tipo = 12;
  blocosColisao[35].colidido = false;
  blocosColisao[35].cliqueMouse = false;
  blocosColisao[35].coletado = true;
  blocosColisao[35].sprite = carregarImagem(".//Artes//Itens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[35].spriteMascara = carregarImagem(".//Artes//Itens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //=>Doce 2
  blocosColisao[36].x = 150;
  blocosColisao[36].y = 500;
  blocosColisao[36].altura = blocosColisao[35].altura;
  blocosColisao[36].largura = blocosColisao[35].largura;
  blocosColisao[36].tipo = blocosColisao[35].tipo;
  blocosColisao[36].colidido = false;
  blocosColisao[36].cliqueMouse = false;
  blocosColisao[36].coletado = true;
  blocosColisao[36].sprite = carregarImagem(".//Artes//Itens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[36].spriteMascara = carregarImagem(".//Artes//Itens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //=>Doce 3
  blocosColisao[37].x = 300;
  blocosColisao[37].y = 50;
  blocosColisao[37].altura = blocosColisao[35].altura;
  blocosColisao[37].largura = blocosColisao[35].largura;
  blocosColisao[37].tipo = blocosColisao[35].tipo;
  blocosColisao[37].colidido = false;
  blocosColisao[37].cliqueMouse = false;
  blocosColisao[37].coletado = true;
  blocosColisao[37].sprite = carregarImagem(".//Artes//Itens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[37].spriteMascara = carregarImagem(".//Artes//Itens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //=>Doce 4
  blocosColisao[38].x = 120;
  blocosColisao[38].y = 400;
  blocosColisao[38].altura = blocosColisao[35].altura;
  blocosColisao[38].largura = blocosColisao[35].largura;
  blocosColisao[38].tipo = blocosColisao[35].tipo;
  blocosColisao[38].colidido = false;
  blocosColisao[38].cliqueMouse = false;
  blocosColisao[38].coletado = true;
  blocosColisao[38].sprite = carregarImagem(".//Artes//Itens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[38].spriteMascara = carregarImagem(".//Artes//Itens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //=>Doce 5
  blocosColisao[39].x = 500;
  blocosColisao[39].y = 400;
  blocosColisao[39].altura = blocosColisao[35].altura;
  blocosColisao[39].largura = blocosColisao[35].largura;
  blocosColisao[39].tipo = blocosColisao[35].tipo;
  blocosColisao[39].colidido = false;
  blocosColisao[39].cliqueMouse = false;
  blocosColisao[39].coletado = true;
  blocosColisao[39].sprite = carregarImagem(".//Artes//Itens//doce.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  blocosColisao[39].spriteMascara = carregarImagem(".//Artes//Itens//doce_masc.bmp", blocosColisao[35].largura, blocosColisao[35].altura, 0, 0);
  
  //===========> Caixas de Textos <===========
  
  //Caixa de Texto Vazia
  blocosColisao[40].x = 0;
  blocosColisao[40].y = 600;
  blocosColisao[40].altura = 120;
  blocosColisao[40].largura = 1280;
  blocosColisao[40].tipo = 13;
  blocosColisao[40].colidido = false;
  blocosColisao[40].cliqueMouse = false;
  blocosColisao[40].coletado = true;
  blocosColisao[40].sprite = carregarImagem(".//Artes//Textos//caixa_de_texto_Vazia.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[40].spriteMascara = NULL;
  
  //Tutorial Textos 1
  blocosColisao[41].x = 0;
  blocosColisao[41].y = 600;
  blocosColisao[41].altura = 120;
  blocosColisao[41].largura = 1280;
  blocosColisao[41].tipo = blocosColisao[35].tipo;
  blocosColisao[41].colidido = false;
  blocosColisao[41].cliqueMouse = false;
  blocosColisao[41].coletado = true;
  blocosColisao[41].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_tutorial_1.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[41].spriteMascara = NULL;
  
  //Tutorial Textos 2
  blocosColisao[42].x = blocosColisao[40].x;
  blocosColisao[42].y = blocosColisao[40].y;
  blocosColisao[42].altura = blocosColisao[40].altura;
  blocosColisao[42].largura = blocosColisao[40].largura;
  blocosColisao[42].tipo = blocosColisao[40].tipo;
  blocosColisao[42].colidido = false;
  blocosColisao[42].cliqueMouse = false;
  blocosColisao[42].coletado = true;
  blocosColisao[42].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_tutorial_2.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[42].spriteMascara = NULL;
  
  //Tutorial Textos 3
  blocosColisao[43].x = blocosColisao[40].x;
  blocosColisao[43].y = blocosColisao[40].y;
  blocosColisao[43].altura = blocosColisao[40].altura;
  blocosColisao[43].largura = blocosColisao[40].largura;
  blocosColisao[43].tipo = blocosColisao[40].tipo;
  blocosColisao[43].colidido = false;
  blocosColisao[43].cliqueMouse = false;
  blocosColisao[43].coletado = true;
  blocosColisao[43].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_tutorial_3.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[43].spriteMascara = NULL;
  
  //Texto Boneca 1
  blocosColisao[44].x = blocosColisao[40].x;
  blocosColisao[44].y = blocosColisao[40].y;
  blocosColisao[44].altura = blocosColisao[40].altura;
  blocosColisao[44].largura = blocosColisao[40].largura;
  blocosColisao[44].tipo = blocosColisao[40].tipo;
  blocosColisao[44].colidido = false;
  blocosColisao[44].cliqueMouse = false;
  blocosColisao[44].coletado = true;
  blocosColisao[44].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_tutorial_boneca_1.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[44].spriteMascara = NULL;
  
  //Texto Boneca 2
  blocosColisao[45].x = blocosColisao[40].x;
  blocosColisao[45].y = blocosColisao[40].y;
  blocosColisao[45].altura = blocosColisao[40].altura;
  blocosColisao[45].largura = blocosColisao[40].largura;
  blocosColisao[45].tipo = blocosColisao[40].tipo;
  blocosColisao[45].colidido = false;
  blocosColisao[45].cliqueMouse = false;
  blocosColisao[45].coletado = true;
  blocosColisao[45].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_tutorial_boneca_2.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[45].spriteMascara = NULL;
  
  //Texto Relicário 1
  blocosColisao[46].x = blocosColisao[40].x;
  blocosColisao[46].y = blocosColisao[40].y;
  blocosColisao[46].altura = blocosColisao[40].altura;
  blocosColisao[46].largura = blocosColisao[40].largura;
  blocosColisao[46].tipo = blocosColisao[40].tipo;
  blocosColisao[46].colidido = false;
  blocosColisao[46].cliqueMouse = false;
  blocosColisao[46].coletado = true;
  blocosColisao[46].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_5_relicario_1.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[46].spriteMascara = NULL;
  
  //Texto Relicário 2
  blocosColisao[47].x = blocosColisao[40].x;
  blocosColisao[47].y = blocosColisao[40].y;
  blocosColisao[47].altura = blocosColisao[40].altura;
  blocosColisao[47].largura = blocosColisao[40].largura;
  blocosColisao[47].tipo = blocosColisao[40].tipo;
  blocosColisao[47].colidido = false;
  blocosColisao[47].cliqueMouse = false;
  blocosColisao[47].coletado = true;
  blocosColisao[47].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_5_relicario_2.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[47].spriteMascara = NULL;
  
  //Texto Ervas 1
  blocosColisao[48].x = blocosColisao[40].x;
  blocosColisao[48].y = blocosColisao[40].y;
  blocosColisao[48].altura = blocosColisao[40].altura;
  blocosColisao[48].largura = blocosColisao[40].largura;
  blocosColisao[48].tipo = blocosColisao[40].tipo;
  blocosColisao[48].texto = 40;
  blocosColisao[48].colidido = false;
  blocosColisao[48].cliqueMouse = false;
  blocosColisao[48].coletado = true;
  blocosColisao[48].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_6_ervas_1.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[48].spriteMascara = NULL;
  
  //Texto Ervas 2
  blocosColisao[49].x = blocosColisao[40].x;
  blocosColisao[49].y = blocosColisao[40].y;
  blocosColisao[49].altura = blocosColisao[40].altura;
  blocosColisao[49].largura = blocosColisao[40].largura;
  blocosColisao[49].tipo = blocosColisao[40].tipo;
  blocosColisao[49].colidido = false;
  blocosColisao[49].cliqueMouse = false;
  blocosColisao[49].coletado = true;
  blocosColisao[49].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_6_ervas_2.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[49].spriteMascara = NULL;
  
  //Texto Sal 
  blocosColisao[50].x = blocosColisao[40].x;
  blocosColisao[50].y = blocosColisao[40].y;
  blocosColisao[50].altura = blocosColisao[40].altura;
  blocosColisao[50].largura = blocosColisao[40].largura;
  blocosColisao[50].tipo = blocosColisao[40].tipo;
  blocosColisao[50].colidido = false;
  blocosColisao[50].cliqueMouse = false;
  blocosColisao[50].coletado = true;
  blocosColisao[50].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_7_sal.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[50].spriteMascara = NULL;
  
  //Texto Estrela 
  blocosColisao[51].x = blocosColisao[40].x;
  blocosColisao[51].y = blocosColisao[40].y;
  blocosColisao[51].altura = blocosColisao[40].altura;
  blocosColisao[51].largura = blocosColisao[40].largura;
  blocosColisao[51].tipo = blocosColisao[40].tipo;
  blocosColisao[51].colidido = false;
  blocosColisao[51].cliqueMouse = false;
  blocosColisao[51].coletado = true;
  blocosColisao[51].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_8_estrela.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[51].spriteMascara = NULL;
  
  //Texto Ao Pegar Todos os Itens Tutorial 1
  blocosColisao[52].x = blocosColisao[40].x;
  blocosColisao[52].y = blocosColisao[40].y;
  blocosColisao[52].altura = blocosColisao[40].altura;
  blocosColisao[52].largura = blocosColisao[40].largura;
  blocosColisao[52].tipo = blocosColisao[40].tipo;
  blocosColisao[52].colidido = false;
  blocosColisao[52].cliqueMouse = false;
  blocosColisao[52].coletado = true;
  blocosColisao[52].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_9_todos_itens.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[52].spriteMascara = NULL;
  
  //Texto Ao Pegar Todos os Itens Tutorial 2
  blocosColisao[53].x = blocosColisao[40].x;
  blocosColisao[53].y = blocosColisao[40].y;
  blocosColisao[53].altura = blocosColisao[40].altura;
  blocosColisao[53].largura = blocosColisao[40].largura;
  blocosColisao[53].tipo = blocosColisao[40].tipo;
  blocosColisao[53].colidido = false;
  blocosColisao[53].cliqueMouse = false;
  blocosColisao[53].coletado = true;
  blocosColisao[53].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_10_final.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[53].spriteMascara = NULL;
  
  //Garantindo saída do tutorial
  blocosColisao[54].x = blocosColisao[40].x;
  blocosColisao[54].y = blocosColisao[40].y;
  blocosColisao[54].altura = blocosColisao[40].altura;
  blocosColisao[54].largura = blocosColisao[40].largura;
  blocosColisao[54].tipo = blocosColisao[40].tipo;
  blocosColisao[54].colidido = false;
  blocosColisao[54].cliqueMouse = false;
  blocosColisao[54].coletado = true;
  blocosColisao[54].sprite = carregarImagem(".//Artes//Textos//Tutorial//texto_10_final.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[54].spriteMascara = NULL;
  
  //Conversa com a Lontra 1
  blocosColisao[55].x = blocosColisao[40].x;
  blocosColisao[55].y = blocosColisao[40].y;
  blocosColisao[55].altura = blocosColisao[40].altura;
  blocosColisao[55].largura = blocosColisao[40].largura;
  blocosColisao[55].tipo = blocosColisao[40].tipo;
  blocosColisao[55].colidido = false;
  blocosColisao[55].cliqueMouse = false;
  blocosColisao[55].coletado = true;
  blocosColisao[55].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_11.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[55].spriteMascara = NULL;
  
  //Conversa com a Lontra 2
  blocosColisao[56].x = blocosColisao[40].x;
  blocosColisao[56].y = blocosColisao[40].y;
  blocosColisao[56].altura = blocosColisao[40].altura;
  blocosColisao[56].largura = blocosColisao[40].largura;
  blocosColisao[56].tipo = blocosColisao[40].tipo;
  blocosColisao[56].colidido = false;
  blocosColisao[56].cliqueMouse = false;
  blocosColisao[56].coletado = true;
  blocosColisao[56].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_12.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[56].spriteMascara = NULL;
  
  //Conversa com a Lontra 3
  blocosColisao[57].x = blocosColisao[40].x;
  blocosColisao[57].y = blocosColisao[40].y;
  blocosColisao[57].altura = blocosColisao[40].altura;
  blocosColisao[57].largura = blocosColisao[40].largura;
  blocosColisao[57].tipo = blocosColisao[40].tipo;
  blocosColisao[57].colidido = false;
  blocosColisao[57].cliqueMouse = false;
  blocosColisao[57].coletado = true;
  blocosColisao[57].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_13.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[57].spriteMascara = NULL;

  //Conversa com a Lontra 4
  blocosColisao[58].x = blocosColisao[40].x;
  blocosColisao[58].y = blocosColisao[40].y;
  blocosColisao[58].altura = blocosColisao[40].altura;
  blocosColisao[58].largura = blocosColisao[40].largura;
  blocosColisao[58].tipo = blocosColisao[40].tipo;
  blocosColisao[58].colidido = false;
  blocosColisao[58].cliqueMouse = false;
  blocosColisao[58].coletado = true;
  blocosColisao[58].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_14.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[58].spriteMascara = NULL;
  
  //Conversa com a Lontra 5
  blocosColisao[59].x = blocosColisao[40].x;
  blocosColisao[59].y = blocosColisao[40].y;
  blocosColisao[59].altura = blocosColisao[40].altura;
  blocosColisao[59].largura = blocosColisao[40].largura;
  blocosColisao[59].tipo = blocosColisao[40].tipo;
  blocosColisao[59].colidido = false;
  blocosColisao[59].cliqueMouse = false;
  blocosColisao[59].coletado = true;
  blocosColisao[59].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_15.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[59].spriteMascara = NULL;
  
  //Conversa com a Lontra 6
  blocosColisao[60].x = blocosColisao[40].x;
  blocosColisao[60].y = blocosColisao[40].y;
  blocosColisao[60].altura = blocosColisao[40].altura;
  blocosColisao[60].largura = blocosColisao[40].largura;
  blocosColisao[60].tipo = blocosColisao[40].tipo;
  blocosColisao[60].colidido = false;
  blocosColisao[60].cliqueMouse = false;
  blocosColisao[60].coletado = true;
  blocosColisao[60].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_16.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[60].spriteMascara = NULL;
  
  //Conversa com a Lontra 7
  blocosColisao[61].x = blocosColisao[40].x;
  blocosColisao[61].y = blocosColisao[40].y;
  blocosColisao[61].altura = blocosColisao[40].altura;
  blocosColisao[61].largura = blocosColisao[40].largura;
  blocosColisao[61].tipo = blocosColisao[40].tipo;
  blocosColisao[61].colidido = false;
  blocosColisao[61].cliqueMouse = false;
  blocosColisao[61].coletado = true;
  blocosColisao[61].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_17.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[61].spriteMascara = NULL;
  
  //Conversa com a Lontra 8
  blocosColisao[62].x = blocosColisao[40].x;
  blocosColisao[62].y = blocosColisao[40].y;
  blocosColisao[62].altura = blocosColisao[40].altura;
  blocosColisao[62].largura = blocosColisao[40].largura;
  blocosColisao[62].tipo = blocosColisao[40].tipo;
  blocosColisao[62].colidido = false;
  blocosColisao[62].cliqueMouse = false;
  blocosColisao[62].coletado = true;
  blocosColisao[62].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_18.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[62].spriteMascara = NULL;
  
  //Conversa com a Lontra 9
  blocosColisao[63].x = blocosColisao[40].x;
  blocosColisao[63].y = blocosColisao[40].y;
  blocosColisao[63].altura = blocosColisao[40].altura;
  blocosColisao[63].largura = blocosColisao[40].largura;
  blocosColisao[63].tipo = blocosColisao[40].tipo;
  blocosColisao[63].colidido = false;
  blocosColisao[63].cliqueMouse = false;
  blocosColisao[63].coletado = true;
  blocosColisao[63].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_19.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[63].spriteMascara = NULL;
  
  //Conversa com a Lontra 10
  blocosColisao[64].x = blocosColisao[40].x;
  blocosColisao[64].y = blocosColisao[40].y;
  blocosColisao[64].altura = blocosColisao[40].altura;
  blocosColisao[64].largura = blocosColisao[40].largura;
  blocosColisao[64].tipo = blocosColisao[40].tipo;
  blocosColisao[64].colidido = false;
  blocosColisao[64].cliqueMouse = false;
  blocosColisao[64].coletado = true;
  blocosColisao[64].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_20.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[64].spriteMascara = NULL;
  
  //Conversa com a Lontra 11
  blocosColisao[65].x = blocosColisao[40].x;
  blocosColisao[65].y = blocosColisao[40].y;
  blocosColisao[65].altura = blocosColisao[40].altura;
  blocosColisao[65].largura = blocosColisao[40].largura;
  blocosColisao[65].tipo = blocosColisao[40].tipo;
  blocosColisao[65].colidido = false;
  blocosColisao[65].cliqueMouse = false;
  blocosColisao[65].coletado = true;
  blocosColisao[65].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_21.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[65].spriteMascara = NULL;
  
  //Conversa com a Lontra levando ela para bruxa
  blocosColisao[66].x = blocosColisao[40].x;
  blocosColisao[66].y = blocosColisao[40].y;
  blocosColisao[66].altura = blocosColisao[40].altura;
  blocosColisao[66].largura = blocosColisao[40].largura;
  blocosColisao[66].tipo = blocosColisao[40].tipo;
  blocosColisao[66].colidido = false;
  blocosColisao[66].cliqueMouse = false;
  blocosColisao[66].coletado = true;
  blocosColisao[66].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_22.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[66].spriteMascara = NULL;
  
  //Conversa com a Lontra convencendo a entrar na casa 1
  blocosColisao[67].x = blocosColisao[40].x;
  blocosColisao[67].y = blocosColisao[40].y;
  blocosColisao[67].altura = blocosColisao[40].altura;
  blocosColisao[67].largura = blocosColisao[40].largura;
  blocosColisao[67].tipo = blocosColisao[40].tipo;
  blocosColisao[67].colidido = false;
  blocosColisao[67].cliqueMouse = false;
  blocosColisao[67].coletado = true;
  blocosColisao[67].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_23.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[67].spriteMascara = NULL;
  
  //Conversa com a Lontra convencendo a entrar na casa 2
  blocosColisao[68].x = blocosColisao[40].x;
  blocosColisao[68].y = blocosColisao[40].y;
  blocosColisao[68].altura = blocosColisao[40].altura;
  blocosColisao[68].largura = blocosColisao[40].largura;
  blocosColisao[68].tipo = blocosColisao[40].tipo;
  blocosColisao[68].colidido = false;
  blocosColisao[68].cliqueMouse = false;
  blocosColisao[68].coletado = true;
  blocosColisao[68].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_24.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[68].spriteMascara = NULL;
  
  //Conversa com a Lontra convencendo a entrar na casa 3
  blocosColisao[69].x = blocosColisao[40].x;
  blocosColisao[69].y = blocosColisao[40].y;
  blocosColisao[69].altura = blocosColisao[40].altura;
  blocosColisao[69].largura = blocosColisao[40].largura;
  blocosColisao[69].tipo = blocosColisao[40].tipo;
  blocosColisao[69].colidido = false;
  blocosColisao[69].cliqueMouse = false;
  blocosColisao[69].coletado = true;
  blocosColisao[69].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_25.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[69].spriteMascara = NULL;
  
  //Conversa com a Lontra convencendo a entrar na casa 4
  blocosColisao[70].x = blocosColisao[40].x;
  blocosColisao[70].y = blocosColisao[40].y;
  blocosColisao[70].altura = blocosColisao[40].altura;
  blocosColisao[70].largura = blocosColisao[40].largura;
  blocosColisao[70].tipo = blocosColisao[40].tipo;
  blocosColisao[70].colidido = false;
  blocosColisao[70].cliqueMouse = false;
  blocosColisao[70].coletado = true;
  blocosColisao[70].sprite = carregarImagem(".//Artes//Textos//encontro_Lontra//texto_26.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[70].spriteMascara = NULL;
  
  //Conversa com a Bruxa 1
  blocosColisao[71].x = blocosColisao[40].x;
  blocosColisao[71].y = blocosColisao[40].y;
  blocosColisao[71].altura = blocosColisao[40].altura;
  blocosColisao[71].largura = blocosColisao[40].largura;
  blocosColisao[71].tipo = blocosColisao[40].tipo;
  blocosColisao[71].colidido = false;
  blocosColisao[71].cliqueMouse = false;
  blocosColisao[71].coletado = true;
  blocosColisao[71].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_27.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[71].spriteMascara = NULL;
  
  //Conversa com a Bruxa 2
  blocosColisao[72].x = blocosColisao[40].x;
  blocosColisao[72].y = blocosColisao[40].y;
  blocosColisao[72].altura = blocosColisao[40].altura;
  blocosColisao[72].largura = blocosColisao[40].largura;
  blocosColisao[72].tipo = blocosColisao[40].tipo;
  blocosColisao[72].colidido = false;
  blocosColisao[72].cliqueMouse = false;
  blocosColisao[72].coletado = true;
  blocosColisao[72].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_28.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[72].spriteMascara = NULL;
  
  //Conversa com a Bruxa 3
  blocosColisao[73].x = blocosColisao[40].x;
  blocosColisao[73].y = blocosColisao[40].y;
  blocosColisao[73].altura = blocosColisao[40].altura;
  blocosColisao[73].largura = blocosColisao[40].largura;
  blocosColisao[73].tipo = blocosColisao[40].tipo;
  blocosColisao[73].colidido = false;
  blocosColisao[73].cliqueMouse = false;
  blocosColisao[73].coletado = true;
  blocosColisao[73].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_29.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[73].spriteMascara = NULL;
  
  //Conversa com a Bruxa 4
  blocosColisao[74].x = blocosColisao[40].x;
  blocosColisao[74].y = blocosColisao[40].y;
  blocosColisao[74].altura = blocosColisao[40].altura;
  blocosColisao[74].largura = blocosColisao[40].largura;
  blocosColisao[74].tipo = blocosColisao[40].tipo;
  blocosColisao[74].colidido = false;
  blocosColisao[74].cliqueMouse = false;
  blocosColisao[74].coletado = true;
  blocosColisao[74].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_30.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[74].spriteMascara = NULL;
  
  //Conversa com a Bruxa 5
  blocosColisao[75].x = blocosColisao[40].x;
  blocosColisao[75].y = blocosColisao[40].y;
  blocosColisao[75].altura = blocosColisao[40].altura;
  blocosColisao[75].largura = blocosColisao[40].largura;
  blocosColisao[75].tipo = blocosColisao[40].tipo;
  blocosColisao[75].colidido = false;
  blocosColisao[75].cliqueMouse = false;
  blocosColisao[75].coletado = true;
  blocosColisao[75].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_31.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[75].spriteMascara = NULL;
  
  //Conversa com a Bruxa 6
  blocosColisao[76].x = blocosColisao[40].x;
  blocosColisao[76].y = blocosColisao[40].y;
  blocosColisao[76].altura = blocosColisao[40].altura;
  blocosColisao[76].largura = blocosColisao[40].largura;
  blocosColisao[76].tipo = blocosColisao[40].tipo;
  blocosColisao[76].colidido = false;
  blocosColisao[76].cliqueMouse = false;
  blocosColisao[76].coletado = true;
  blocosColisao[76].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_32.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[76].spriteMascara = NULL;
  
  //Conversa com a Bruxa 7
  blocosColisao[77].x = blocosColisao[40].x;
  blocosColisao[77].y = blocosColisao[40].y;
  blocosColisao[77].altura = blocosColisao[40].altura;
  blocosColisao[77].largura = blocosColisao[40].largura;
  blocosColisao[77].tipo = blocosColisao[40].tipo;
  blocosColisao[77].colidido = false;
  blocosColisao[77].cliqueMouse = false;
  blocosColisao[77].coletado = true;
  blocosColisao[77].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_33.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[77].spriteMascara = NULL;
  
  //Conversa com a Bruxa 8
  blocosColisao[78].x = blocosColisao[40].x;
  blocosColisao[78].y = blocosColisao[40].y;
  blocosColisao[78].altura = blocosColisao[40].altura;
  blocosColisao[78].largura = blocosColisao[40].largura;
  blocosColisao[78].tipo = blocosColisao[40].tipo;
  blocosColisao[78].colidido = false;
  blocosColisao[78].cliqueMouse = false;
  blocosColisao[78].coletado = true;
  blocosColisao[78].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_34.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[78].spriteMascara = NULL;
  
  //Conversa com a Bruxa 9
  blocosColisao[79].x = blocosColisao[40].x;
  blocosColisao[79].y = blocosColisao[40].y;
  blocosColisao[79].altura = blocosColisao[40].altura;
  blocosColisao[79].largura = blocosColisao[40].largura;
  blocosColisao[79].tipo = blocosColisao[40].tipo;
  blocosColisao[79].colidido = false;
  blocosColisao[79].cliqueMouse = false;
  blocosColisao[79].coletado = true;
  blocosColisao[79].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_35.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[79].spriteMascara = NULL;
  
  //Conversa com a Bruxa 10
  blocosColisao[80].x = blocosColisao[40].x;
  blocosColisao[80].y = blocosColisao[40].y;
  blocosColisao[80].altura = blocosColisao[40].altura;
  blocosColisao[80].largura = blocosColisao[40].largura;
  blocosColisao[80].tipo = blocosColisao[40].tipo;
  blocosColisao[80].colidido = false;
  blocosColisao[80].cliqueMouse = false;
  blocosColisao[80].coletado = true;
  blocosColisao[80].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_36.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[80].spriteMascara = NULL;
  
  //Conversa com a Bruxa 11
  blocosColisao[81].x = blocosColisao[40].x;
  blocosColisao[81].y = blocosColisao[40].y;
  blocosColisao[81].altura = blocosColisao[40].altura;
  blocosColisao[81].largura = blocosColisao[40].largura;
  blocosColisao[81].tipo = blocosColisao[40].tipo;
  blocosColisao[81].colidido = false;
  blocosColisao[81].cliqueMouse = false;
  blocosColisao[81].coletado = true;
  blocosColisao[81].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_37.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[81].spriteMascara = NULL;
  
  //Conversa com a Bruxa 12
  blocosColisao[82].x = blocosColisao[40].x;
  blocosColisao[82].y = blocosColisao[40].y;
  blocosColisao[82].altura = blocosColisao[40].altura;
  blocosColisao[82].largura = blocosColisao[40].largura;
  blocosColisao[82].tipo = blocosColisao[40].tipo;
  blocosColisao[82].colidido = false;
  blocosColisao[82].cliqueMouse = false;
  blocosColisao[82].coletado = true;
  blocosColisao[82].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_38.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[82].spriteMascara = NULL;
  
  //Conversa com a Bruxa 13
  blocosColisao[83].x = blocosColisao[40].x;
  blocosColisao[83].y = blocosColisao[40].y;
  blocosColisao[83].altura = blocosColisao[40].altura;
  blocosColisao[83].largura = blocosColisao[40].largura;
  blocosColisao[83].tipo = blocosColisao[40].tipo;
  blocosColisao[83].colidido = false;
  blocosColisao[83].cliqueMouse = false;
  blocosColisao[83].coletado = true;
  blocosColisao[83].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_39.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[83].spriteMascara = NULL;
  
  //Conversa com a Bruxa2
  blocosColisao[84].x = blocosColisao[40].x;
  blocosColisao[84].y = blocosColisao[40].y;
  blocosColisao[84].altura = blocosColisao[40].altura;
  blocosColisao[84].largura = blocosColisao[40].largura;
  blocosColisao[84].tipo = blocosColisao[40].tipo;
  blocosColisao[84].colidido = false;
  blocosColisao[84].cliqueMouse = false;
  blocosColisao[84].coletado = true;
  blocosColisao[84].sprite = carregarImagem(".//Artes//Textos//encontro_bruxa//texto_27.bmp", blocosColisao[40].largura, blocosColisao[40].altura, 0, 0);
  blocosColisao[84].spriteMascara = NULL;
  
  //===============================> Cenários <===============================
  void **cenarios;
  int qntDeCenarios = 26;
  cenarios = (void **) malloc(sizeof(void *) * qntDeCenarios);

  cenarios[0] = carregarImagem(".//Artes//Cenarios//sala_tutas.bmp", 1280, 600, 0, 0);
  
  cenarios[1] = carregarImagem(".//Artes//Cenarios//quarto_tutas.bmp", 1280, 600, 0, 0);
  
  cenarios[2] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_1.bmp", 1280, 600, 0, 0);
  
  cenarios[3] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_2.bmp", 1280, 600, 0, 0);
  
  cenarios[4] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_3.bmp", 1280, 600, 0, 0);
  
  cenarios[5] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_4.bmp", 1280, 600, 0, 0);
  
  cenarios[6] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_5.bmp", 1280, 600, 0, 0);
  
  cenarios[7] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_6.bmp", 1280, 600, 0, 0);
  
  cenarios[8] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_7.bmp", 1280, 600, 0, 0);
  
  cenarios[9] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_8.bmp", 1280, 600, 0, 0);
  
  cenarios[10] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_9.bmp", 1280, 600, 0, 0);
  
  cenarios[11] = carregarImagem(".//Artes//Cenarios//fase_2_cenario_10.bmp", 1280, 600, 0, 0);
  
  cenarios[12] = carregarImagem(".//Artes//Cenarios//menu.bmp", 1280, 720, 0, 0);
  
  cenarios[13] = carregarImagem(".//Artes//Cenarios//tela_1_final.bmp", 1280, 720, 0, 0);
  
  cenarios[14] = carregarImagem(".//Artes//Cenarios//tela_2_final.bmp", 1280, 720, 0, 0);
  
  cenarios[15] = carregarImagem(".//Artes//Cenarios//tela_3_final.bmp", 1280, 720, 0, 0);
  
  cenarios[16] = carregarImagem(".//Artes//Cenarios//tela_4_final.bmp", 1280, 720, 0, 0);
  
  cenarios[17] = carregarImagem(".//Artes//Cenarios//cena1_pt1.bmp", 1280, 720, 0, 0);
  
  cenarios[18] = carregarImagem(".//Artes//Cenarios//cena1_pt2.bmp", 1280, 720, 0, 0);
  
  cenarios[19] = carregarImagem(".//Artes//Cenarios//cena2_pt1.bmp", 1280, 720, 0, 0);
  
  cenarios[20] = carregarImagem(".//Artes//Cenarios//cena2_pt2.bmp", 1280, 720, 0, 0);
  
  cenarios[21] = carregarImagem(".//Artes//Cenarios//cena3.bmp", 1280, 720, 0, 0);
  
  cenarios[22] = carregarImagem(".//Artes//Cenarios//cena4.bmp", 1280, 720, 0, 0);
  
  cenarios[23] = carregarImagem(".//Artes//Cenarios//cena5_pt1.bmp", 1280, 720, 0, 0);
  
  cenarios[24] = carregarImagem(".//Artes//Cenarios//cena5_pt2.bmp", 1280, 720, 0, 0);
  
  cenarios[25] = carregarImagem(".//Artes//Cenarios//cena_fim_tutorial.bmp", 1280, 720, 0, 0);
  
  //Imagem do Inventario
  inventarioImagem = carregarImagem(".//Artes//Itens//inventario.bmp", InvLargura, InvAltura, 0, 0);
  
  //=======================> Salvando o tick do computador Inicialmente <=======================
  gt1 = GetTickCount();//registrando o tick inicial do computador para intervalo de frames
  Inicio = GetTickCount();//registrando o tick inicial do computador para tempo de espera do click
  
  while(tecla != ESC)
  {
    //===============================> Coletando o Tick ao passar do tempo <===============================
    gt2 = GetTickCount();//coletando o tick atual do computador
	
    if (gt2-gt1 > 1000/fps)//setando teto para frames
	{
      gt1 = gt2;
      TempoDecorrido = gt2 - Inicio;
	  if(pg == 1) pg = 2; else pg = 1;//técnica de paginação
	  setactivepage(pg);
	  cleardevice();
      
      //=================> Garantindo que o Mouse Fique Dentro da Tela <=================
      GetCursorPos(&P);
      ScreenToClient(janela, &P);
      printf("\n%d", blocosColisao[18].coletado);
      //=================> Lidando com a troca de fases <=================
      CondicionaisDeFases(cenarios);
      CondicionaisTextos();
	  FinalizandoMissoes();
	  
      setvisualpage(pg);
    }//Fim do Condicional para Barramento de Frames
    
    //===============================> Input para saída do laço <===============================
    fflush(stdin);
    if (kbhit())
	{
      tecla = getch();
    }
  }//Fim do Laço
  
  printf("\n\nFim do Programa");
  
  //===>liberando memória dos blocos
  for(int i = 0; i < qntBlocos; i++)
  {
    free(blocosColisao[i].sprite);
    free(blocosColisao[i].spriteMascara);
  }
  free(blocosColisao);
  
  //===>liberando memória dos cenários
  for(int i = 0; i < qntDeCenarios; i++)
  {
    free(cenarios[i]);
  }
  free(cenarios);
  
  closegraph();
  return 0;
}

//======================= > Área para Desenhar e Criar < =======================

void CriandoPersonagem(int index)
{
  ChecagemDeColisao( P.x, P.y, blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].largura, blocosColisao[index].altura, blocosColisao[index].colidido);
  
  if(!blocosColisao[index].colidido)//Detectando colisão para por outline
    DesenhandoBotao( blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].sprite, blocosColisao[index].spriteMascara);
  else
    DesenhandoBotao( blocosColisao[index+1].x, blocosColisao[index+1].y, blocosColisao[index+1].sprite, blocosColisao[index+1].spriteMascara);
}

void CriandoInvetario(int IndexInicial, int IndexFinal, int index)
{
  int controleItensInventario = InvAltura/5;//Criando variável para controlar a localidade de cada item no inventário.
  if(blocosColisao[index].tipo == 4 && blocosColisao[index].coletado == true)
  {
    putimage(xInv, yInv, inventarioImagem, COPY_PUT);//desenhando o inventário
    for(int j = IndexInicial; j < IndexFinal + 1; j++)//pegando quais itens tem pela fase para aparecer no inventário
    {
	  if(!blocosColisao[j].coletado)//modificando a imagem com base se o item foi coletado ou não
	  {
	  	if((j - IndexInicial + 1) <= 5)
	  	{
	      DesenhandoBotao(xInv + 50, controleItensInventario*(j - IndexInicial + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), blocosColisao[j].sprite, blocosColisao[j].spriteMascara);	
		}
	    else if((j - IndexInicial + 1) > 5)
	    {
	      DesenhandoBotao(xInv + 200, controleItensInventario*(j - (IndexInicial + 5) + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), blocosColisao[j].sprite, blocosColisao[j].spriteMascara);
        }
      }
    }	
  }
}

void* carregarImagem(const char *sprite, int largura, int altura , int x, int y)
{	
  void *image;//vetor para bits brutos
  int aux = imagesize(x, y, largura, altura);//tamanho da imagem
  image = malloc(aux);//abrindo espaço de memória para o vetor com base no tamanho da imagem
  readimagefile(sprite, x, y, largura, altura);//lendo a imagem que queremos
  getimage(x, y, largura, altura, image);
  
  return image;
}

void DesenhandoBotao( int xImagem, int yImagem, void *sprites,void *spritesMascara)
{
  putimage(xImagem, yImagem, spritesMascara, AND_PUT);	
  putimage(xImagem, yImagem, sprites, OR_PUT);
}

void MostrarColetaveis(int index)
{
  if(!blocosColisao[index].coletado)
  {
	DesenhandoBotao( blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].sprite, blocosColisao[index].spriteMascara);
  }
  ChecagemDeColisao( P.x, P.y, blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].largura, blocosColisao[index].altura, blocosColisao[index].colidido);
}

void MostrarColetaveisMissao(int indexMenor, int indexMaior)
{
	for(int i = indexMenor; i < indexMaior + 1; i++)
	{
	  MostrarColetaveis(i);
	}
}

//===================================================================== >  ÁREA PARA LIDAR COM LEVEIS < =====================================================================

void LidandoComFases(void *cenario, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal, bool TemPersonagem)
{
  //Desenhando Cenário
  putimage(0, 0, cenario, COPY_PUT);
  ChecagemDeColisao( P.x, P.y, blocosColisao[texto].x, blocosColisao[texto].y, blocosColisao[texto].largura, blocosColisao[texto].altura, blocosColisao[texto].colidido);
  CaixaDeTexto(texto);
  IndicacaoParaTrocaDeTexto();
  
  //Lidando com Personagem da fase, se houver
  if(TemPersonagem == true)
    LidandoComPersonagem(variavelDeControle);
  //===============================> Desenhando os Itens <===============================
  for(int i = blocoInicial; i < blocoFinal + 1; i++)//conferindo quais itens tem na fase
  {
    MostrarColetaveis(i);
  }
  //===============================> Lidando com os Blocos <===============================
  for(int i = 0; i <= 4; i++)//pegando os botões bases para troca de fase e inventário
  { 
    //===============================> Desenhando os Blocos para Mudança de Fase <===============================
    if(blocosColisao[i].tipo == 0 && fasePraCima == true && trocarDeFase == true)
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);//Coletando a informação de colisão e guardando dentro do item.
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}
      
    if(blocosColisao[i].tipo == 1 && fasePraDireita == true && trocarDeFase == true)
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}
      
    if(blocosColisao[i].tipo == 2 && fasePraEsquerda == true && trocarDeFase == true)
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}
      
    if(blocosColisao[i].tipo == 3 && fasePraBaixo == true && trocarDeFase == true)
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}
 
    //===============================> Desenhando o Botão do Inventário <===============================
    if(i == 4 && inventario == true)
    {
      ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}

    //==================> Ações dos Bloco de Colisão que não sejam coletáveis (Botões de mudança de fases e inventário) <==================
    //Conferindo para quais lados serão as próximas fases / fases anteriores
    if(blocosColisao[i].tipo == 0 && blocosColisao[i].cliqueMouse == true && fasePraCima == true && trocarDeFase == true)//botão fase Acima
	{
	  fases += 1;
	  texto = 40;
	  ZerandoColisoes();
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 1 && blocosColisao[i].cliqueMouse == true && fasePraDireita == true && trocarDeFase == true)//botão fase para à Direita
	{	
	  fases += 2;
	  texto = 40;
	  ZerandoColisoes();
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 2 && blocosColisao[i].cliqueMouse == true && fasePraEsquerda == true && trocarDeFase == true)//botão fase à Esquerda
	{	
	  fases -= 2;
	  texto = 40;
	  ZerandoColisoes();
	  blocosColisao[i].cliqueMouse = false;
	}
    else if(blocosColisao[i].tipo == 3 && blocosColisao[i].cliqueMouse == true && fasePraBaixo == true && trocarDeFase == true)//botão fase Abaixo
    {
	  fases -= 1;
	  texto = 40;
	  ZerandoColisoes();
	  blocosColisao[i].cliqueMouse = false;
	}
	else if(blocosColisao[i].tipo == 4 && inventario == true && blocosColisao[i].cliqueMouse == true)//botão para inventário
    {
      if(!blocosColisao[i].coletado)//abrindo o inventário
      {
        blocosColisao[i].x = xInv - blocosColisao[i].largura;//mudando a posição do ícone do inventário
        blocosColisao[i].coletado = true;
	  }
	  else//fechando o inventário
	  {
	    blocosColisao[i].x = 1280 - blocosColisao[i].largura;//reajustando a posição do ícone do inventário
	    blocosColisao[i].coletado = false;
	  }
	  blocosColisao[i].cliqueMouse = false;
	}
	
	//Criando o Inventário
    CriandoInvetario(InvIndexInicial, InvIndexFinal, i);
  }
  //===============================> Colisão do Mouse <===============================
  for(int i = blocoInicial; i < blocoFinal + 1; i++)//conferindo a colisão para cada Bloco de Colisão
  {	
    ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);//Coletando a informação de colisão e guardando dentro do item.
  }
  ChecagemDeColisaoDoMouse();
}

void Menu(void *cenario, int &fases)//Criando  o Menu
{
  putimage(0, 0, cenario, COPY_PUT);
  if(GetKeyState(VK_LBUTTON)&0x80)
  {
	Inicio = GetTickCount();
	mciSendString("play jogo repeat", NULL, 0, 0); 
	fases ++;
  }
}

void SaindoDoTutorial()//Verificando se o Player coletou todos os itens para mudar de etapa
{
  fases = 2;
  texto = 40;
  inventario = false;
  ZerandoColisoes();
}

void AtivandoFinal()
{
  if(fases == 9)
     fases = 16;
}

void CutScene(void *cenario)
{
  putimage(0, 0, cenario, COPY_PUT);
  if(TempoDecorrido > Espera)
  {
    if(GetKeyState(VK_LBUTTON)&0x80)
	{
	  Inicio = GetTickCount();
	  if(fases != 2)
	    fases ++;
	  else
	  {
	    fases = 4;
        texto = 40;	
	  }
	}
  }
}

void CondicionaisDeFases(void **cenarios)
{
  if(fases == 0)//menu
	Menu(cenarios[12], fases);
  if(fases == 1)
  {
    LidandoComFases(cenarios[0], false, false, true, false, inventario, 5, 6, 5, 9, false);
  }//não há segunda fase pela lógica que criei de transição de fase
  else if(fases == 2)
  {
    CutScene(cenarios[25]);
  }
  else if(fases == 3)
  {
	LidandoComFases(cenarios[1], false, false, false, true, inventario, 7, 9, 5, 9, false);
  }
  else if(fases == 4)
  {
    LidandoComFases(cenarios[2], false, false, true, false, inventario, 10, 10, 10, 17, true);
  }	
  else if(fases == 6)
  {
    variavelDeControle = 18;
	LidandoComFases(cenarios[3], true, false, pegouMissao, true, inventario, 11, 11, 10, 17, false);
    if(!jaEntrou2)
    {
      TextoAoInteragir(66);
      jaEntrou2 = true;
	}
  }
  else if(fases == 7)
  {
    LidandoComFases(cenarios[4], false, true, true, false, inventario, 12, 12, 10, 17, false);
    if(!jaEntrou)
    {
      TextoAoInteragir(67);
      jaEntrou = true;
	}
  }
  else if(fases == 8)
  {
    LidandoComFases(cenarios[6], false, false, true, true, inventario, 14, 14, 10, 17, false);
    MostrarColetaveisMissao(30,30);
  }
  else if(fases == 9)
  {
	if(!entrou)
	{	
	  pegouMissao = true;
	  entrou = true;
	}
	variavelDeControle = 22;
	LidandoComFases(cenarios[5], false, false, false, true, inventario, 13, 13, 10, 17, true);
  }
  else if(fases == 10)
  {
  	rolarTexto = false;
    variavelDeControle = 24;
	LidandoComFases(cenarios[7], false, false, true, true, inventario, 7, 9, 10, 17, true);
	MostrarColetaveisMissao(31,31);
	MostrarColetaveisMissao(35,35);
  }
  else if(fases == 12)
  {
	LidandoComFases(cenarios[8], true, false, true, true, inventario, 7, 9, 10, 17, false);
	MostrarColetaveisMissao(32,33);
	MostrarColetaveisMissao(36,36);
  }
  else if(fases == 13)
  {
    variavelDeControle = 26;
	LidandoComFases(cenarios[9], false, true, true, false, inventario, 15, 15, 10, 17, true);
	MostrarColetaveisMissao(37,37);
  }
  else if(fases == 14)
  {
    variavelDeControle = 28;
	LidandoComFases(cenarios[11], false, false, false, true, inventario, 17, 17, 10, 17, true);
	MostrarColetaveisMissao(39,39);
	MostrarColetaveisMissao(34,34);
  }
  else if(fases == 15)
  {
    LidandoComFases(cenarios[10], false, false, false, true, inventario, 16, 16, 10, 17, false);
	MostrarColetaveisMissao(38,38);
  }
  //CutScenes do final do jogo
  else if(fases == 16)
	CutScene(cenarios[13]);
  else if(fases == 17)
	CutScene(cenarios[14]);
  else if(fases == 18)
	CutScene(cenarios[15]);
  else if(fases == 19)
	putimage(0,0, cenarios[16], COPY_PUT);
	  
  //CutScenes do começo do jogo
  else if(fases == 21)
	CutScene(cenarios[17]);
  else if(fases == 22)
	CutScene(cenarios[18]);
  else if(fases == 23)
	CutScene(cenarios[19]);
  else if(fases == 24)
	CutScene(cenarios[20]);
  else if(fases == 25)
	CutScene(cenarios[21]);
  else if(fases == 26)
	CutScene(cenarios[22]);
  else if(fases == 27)
	CutScene(cenarios[23]);
  else if(fases == 28)
	CutScene(cenarios[24]);
  else if(fases == 29)
	fases = 0;
}

//======================= > Área para Lidar com Colisões < =======================

void ChecagemDeColisaoDoMouse()
{
  if(colisaoMouse == false)
  {
    for(int i = 0; i < qntBlocos; i++)//checagem dos Itens para ver se há algum colidido e Qual foi.
	{
	  colisaoMouse = blocosColisao[i].colidido;
	  if(colisaoMouse == true)//se há, coleta qual item
	  {
	    indexItemColidido = i;
	    break;
	  }
	}
  }
  if(colisaoMouse == true)
  {
	for(int i = 0; i < qntBlocos; i++)//checagem se não há mais colisão.
	{
	  if(blocosColisao[i].colidido == true)//se há, então ignora a verificação
		break;
	  else if(i == qntBlocos - 1)//se não há. então mouse não está colidido
	  {
		if(blocosColisao[i].colidido == false) colisaoMouse = false;
	  }
	}
  }
  
  //===============================> Captura de Inputs <===============================
  if(TempoDecorrido > Espera)
  {
	if(GetKeyState(VK_LBUTTON)&0x80 && colisaoMouse == true)//Input do Mouse
	{
	  Inicio = GetTickCount();//resetando o tempo de espera
	  blocosColisao[indexItemColidido].cliqueMouse = true;
	  ColetarItensFase();
	  ColetarItensMissoes(11, qntMoedasColetadas, indexItemColidido);
	  ColetarItensMissoes(12, qntDocesColetados, indexItemColidido);
	  if(blocosColisao[indexItemColidido].tipo == 5)
	    TextoAoInteragir(blocosColisao[indexItemColidido].texto);
	  LidandoComTextos();
	}
  }
}

void ZerandoColisoes()
{
  //Fazendo com que todos os itens desverifiquem as colisões(útil para situações em que o item acabou saindo da tela com a informação q está cólidido e bug o jogo)
  for(int i = 0; i < qntBlocos; i++)
    blocosColisao[i].colidido = false;
}

bool ChecagemDeColisao( int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu)
{
  if( xColisor >= xColidido && yColisor >= yColidido )//checagem se o mouse está à direita e em baixo do vértice superior esquerdo
  {
    if( xColisor <= (xColidido + larguraColidido) && yColisor <= (yColidido + alturaColidido) )//checagem se o mouse está à esquerda e em cima do vértice inferior direito
      colidiu = true;
    else
      colidiu = false;
  }
  else
    colidiu = false;
}

//======================= > Área para Lidar com Personagens < =======================

void LidandoComPersonagem(int &index)//Colentando o tipo de personagem que teve interação e reagindo conforme devido
{
  CriandoPersonagem(index);
  if(blocosColisao[index].cliqueMouse == true && blocosColisao[index].coletado == false)
  {
  	printf("\nentrou");
  	blocosColisao[index].coletado = true;
	switch(blocosColisao[index].tipo)
	{
	  case 6:
	    LidandoComAsFolhas(index);//Interação com as folhas de onde sai a Lontra
	    break;
	  case 7:
	    TextoAoInteragir(blocosColisao[index].texto);
	    break;
	  case 8:
	    LidandoComABruxa(10, 17);
	    break;
	  case 9:
	    LidandoComMissoes(11);
	    break;
	  case 10:
	    LidandoComMissoes(12);
	    break;
	}
  }
}

void LidandoComAsFolhas(int &index)//Interação com as folhas de onde sai a Lontra
{
  index += 2;
  ZerandoColisoes();
  TextoAoInteragir(55);
}

void LidandoComABruxa(int primeiroItemMissao, int ultimoItemMissao)//Ativando os itens a serem coletados
{
  for(int i = primeiroItemMissao; i < ultimoItemMissao + 1; i++)
  {
    blocosColisao[i].coletado = false;
  }
  inventario = true;
  TextoAoInteragir(71);
}

void LidandoComMissoes(int tipo)//Ativando os itens a serem coletados adicionalmente para recompensa no final
{
  for(int i = 0; i < qntBlocos; i++)
  {
    if(blocosColisao[i].tipo == tipo)
      blocosColisao[i].coletado = false;
  }
}

void FinalizandoMissoes()//Conferindo circunstâncias para finalizar missões do jogo
{
  if(qntItensColetados >= 15)
    AtivandoFinal();
  if(qntDocesColetados >= 5)
    FinalMissao(15, qntDocesColetados);
  if(qntMoedasColetadas >= 5)
    FinalMissao(14, qntMoedasColetadas);
}

void FinalMissao(int circunstancia, int contador)
{
  if(fases == circunstancia)
  {
  	qntItensColetados++;
  	contador = 0;
  }
}

//======================= > Área para Coletas de Itens < =======================

void ColetarItensFase()
{
  if(blocosColisao[indexItemColidido].tipo == 5 && blocosColisao[indexItemColidido].coletado == false)//Captura de Coletáveis da Fase
  {	
    qntItensColetados++;//registrando itens coletados
	blocosColisao[indexItemColidido].coletado = true;//impossibilitando de clicar novamente
  }
}

void ColetarItensMissoes(int tipo, int &quantidade, int indexItemColididos)
{
  if(blocosColisao[indexItemColidido].tipo == tipo && blocosColisao[indexItemColidido].coletado == false)//Captura de Coletáveis da Missão
  {
  	quantidade++;
    blocosColisao[indexItemColidido].coletado = true;//impossibilitando de clicar novamente
  }
}

//======================= > Área para Textos < =======================

void LidandoComTextos()//Mudar De Texto ao Clicar na Caixa De Texto
{
  if(rolarTexto == true)
  {
    if(blocosColisao[texto].cliqueMouse == true)
    {
      ZerandoColisoes();
	  texto++;
      colisaoMouse = false;
	}
  }
}

void TextoAoInteragir(int qualTexto)
{
  rolarTexto = true;
  texto = qualTexto;
  if(fases <= 3 && qntItensColetados == 5 && texto < 52)
    texto = 52;
}

void CaixaDeTexto(int index)//Desenhando Caixa de Texto
{
  putimage(blocosColisao[index].x, blocosColisao[index].y, blocosColisao[index].sprite , COPY_PUT);
}

void CondicionaisTextos()
{
  if(texto == 40)
    rolarTexto = false;
  if(texto == 43)
    rolarTexto = false;
  if(texto == 45)
    rolarTexto = false;
  if(texto == 47)
    rolarTexto = false;
  if(texto == 49)
    rolarTexto = false;
  if(texto == 50)
    rolarTexto = false;
  if(texto == 51)
    rolarTexto = false;
  if(texto == 52)
    trocarDeFase = false;
  if(texto == 54)
    SaindoDoTutorial();//Verificando se o Player coletou todos os itens e clicou na última caixa de texto para mudar de etapa
  if(texto == 65)
  {
    trocarDeFase = true;
    rolarTexto = false;
  }
  if(texto == 66)
    rolarTexto = false;
  if(texto == 70)
    rolarTexto = false;
  if(texto == 83)
  {
    trocarDeFase = true;
    rolarTexto = false;
  }
  if(texto == 83)
    rolarTexto = false;
}

void IndicacaoParaTrocaDeTexto()
{
  if(rolarTexto == true)
    outtextxy(1215, 695, "Passe >>");
}


