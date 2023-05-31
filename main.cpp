#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
using namespace std;

#define ESC    	27

//local para os structs
typedef struct _blocosColisao
{
  int x, y, altura, largura, tipo;
  bool colidido, cliqueMouse, coletado;
  void *spriteMascara;
  void *sprite;
}BlocoDeColisao;

//criando o vetor para blocos de colis�o
BlocoDeColisao *blocosColisao;
int qntBlocos = 10;

//checagem de itens coletados
int qntItensColetados = 0;
 
//Area para funcoes
void* carregarImagem(const char *sprite, int largura, int altura , int x, int y);

bool ChecagemDeColisao(int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu);

void LidandoComFases(int &numItensColetados, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal);

void DesenhandoBotao( int xImagem, int yImagem, void *sprites,void *spritesMascara);

//=====================> Tempo de Espera para Clique do Mouse <=====================
int Espera = 500;
double TempoDecorrido;
long long Inicio, Agora;
					
//===============================> Mouse <===============================
HWND janela;//coletando a janela
POINT P;//posi��o do Mouse
int indexItemColidido = 0;//coletandoBlocoColidido
bool colisaoMouse = false;//verificando se o mouse colidiu com algum bloco
  
//===============================> Invent�rio <===============================
int InvLargura = 90;
int InvAltura = 720;
int xInv = 1280 - InvLargura;
int yInv = 0;
  
//=================================================> JOGO <=================================================
int main()
{
  //===============================> Lidando com a janela <===============================
  int pg = 1;
  char tecla;	
  unsigned long long gt1, gt2;//Clocks do Computador
  int fps = 60;
  
  initwindow(1280, 720, "Projeto Alma");
  janela = GetForegroundWindow();
  setactivepage(pg);//Deixa ativa a pagina do la�o para desenhar nela
  setvisualpage(pg);//Deixa visual para mostrar o que foi desenhado na tela
  tecla = 0;//index do teclado
  
  //===============================> Fases <===============================
  int fases = 1;
  bool pegouMissao = false;

  //===============================> Blocos de Colis�o para o Mouse <===============================
  //Vetor que est� guardando os blocos
  blocosColisao = NULL;
  blocosColisao = (BlocoDeColisao *) malloc(sizeof(BlocoDeColisao) * qntBlocos);
  
  //===============================> Bot�es para troca de fases <===============================
  //bot�o para cima
  blocosColisao[0].x = 640;
  blocosColisao[0].y = 20;
  blocosColisao[0].altura = 64;
  blocosColisao[0].largura = 64;
  blocosColisao[0].tipo = 0;
  blocosColisao[0].colidido = false;
  blocosColisao[0].cliqueMouse = false;
  blocosColisao[0].coletado = false;
  blocosColisao[0].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Cima_Mascara.bmp", blocosColisao[0].largura, blocosColisao[0].altura, 0, 0);
  blocosColisao[0].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Cima.bmp", blocosColisao[0].largura, blocosColisao[0].altura, 0, 0);
  
  //bot�o para direita
  blocosColisao[1].altura = 64;
  blocosColisao[1].largura = 64;
  blocosColisao[1].x = 1250 - blocosColisao[1].largura;
  blocosColisao[1].y = 360;
  blocosColisao[1].tipo = 1;
  blocosColisao[1].colidido = false;
  blocosColisao[1].cliqueMouse = false;
  blocosColisao[1].coletado = false;
  blocosColisao[1].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Direita_Mascara.bmp", blocosColisao[1].largura, blocosColisao[1].altura, 0, 0);
  blocosColisao[1].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Direita.bmp", blocosColisao[1].largura, blocosColisao[1].altura, 0, 0);
  
  //bot�o para esquerda
  blocosColisao[2].altura = 64;
  blocosColisao[2].largura = 64;
  blocosColisao[2].x = 30;
  blocosColisao[2].y = 360;
  blocosColisao[2].tipo = 2;
  blocosColisao[2].colidido = false;
  blocosColisao[2].cliqueMouse = false;
  blocosColisao[2].coletado = false;
  blocosColisao[2].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Esquerda_Mascara.bmp", blocosColisao[2].largura, blocosColisao[2].altura, 0, 0);
  blocosColisao[2].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Esquerda.bmp", blocosColisao[2].largura, blocosColisao[2].altura, 0, 0);
  
  //bot�o para baixo
  blocosColisao[3].altura = 64;
  blocosColisao[3].largura = 64;
  blocosColisao[3].x = 640;
  blocosColisao[3].y = 720 - blocosColisao[3].altura - 20;
  blocosColisao[3].tipo = 3;
  blocosColisao[3].colidido = false;
  blocosColisao[3].cliqueMouse = false;
  blocosColisao[3].coletado = false;
  blocosColisao[3].spriteMascara = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Baixo_Mascara.bmp", blocosColisao[3].largura, blocosColisao[3].altura, 0, 0);
  blocosColisao[3].sprite = carregarImagem(".//Artes//Botoes//botao_trocaDeFases_Baixo.bmp", blocosColisao[3].largura, blocosColisao[3].altura, 0, 0);
  
  //invent�rio
  blocosColisao[4].altura = 64;
  blocosColisao[4].largura = 64;
  blocosColisao[4].tipo = 4;
  blocosColisao[4].x = 1280 - blocosColisao[4].largura;
  blocosColisao[4].y = 30;
  blocosColisao[4].colidido = false;
  blocosColisao[4].cliqueMouse = false;
  blocosColisao[4].coletado = false;
  blocosColisao[4].spriteMascara = carregarImagem(".//Artes//Botoes//icone_Inventario_Mascara.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  blocosColisao[4].sprite = carregarImagem(".//Artes//Botoes//icone_Inventario.bmp", blocosColisao[4].largura, blocosColisao[4].altura, 0, 0);
  
  //Itens
  blocosColisao[5].x = 320;
  blocosColisao[5].y = 230;
  blocosColisao[5].altura = 64;
  blocosColisao[5].largura = 64;
  blocosColisao[5].tipo = 5;
  blocosColisao[5].colidido = false;
  blocosColisao[5].cliqueMouse = false;
  blocosColisao[5].coletado = false;
  blocosColisao[5].sprite = NULL;
  blocosColisao[5].spriteMascara = NULL;
  
  blocosColisao[6].x = 900;
  blocosColisao[6].y = 540;
  blocosColisao[6].altura = 64;
  blocosColisao[6].largura = 64;
  blocosColisao[6].tipo = 5;
  blocosColisao[6].colidido = false;
  blocosColisao[6].cliqueMouse = false;
  blocosColisao[6].coletado = false;
  blocosColisao[6].sprite = NULL;
  blocosColisao[6].spriteMascara = NULL;
  
  blocosColisao[7].x = 670;
  blocosColisao[7].y = 240;
  blocosColisao[7].altura = 64;
  blocosColisao[7].largura = 64;
  blocosColisao[7].tipo = 5;
  blocosColisao[7].colidido = false;
  blocosColisao[7].cliqueMouse = false;
  blocosColisao[7].coletado = false;
  blocosColisao[7].sprite = NULL;
  blocosColisao[7].spriteMascara = NULL;
  
  blocosColisao[8].x = 1000;
  blocosColisao[8].y = 400;
  blocosColisao[8].altura = 64;
  blocosColisao[8].largura = 64;
  blocosColisao[8].tipo = 5;
  blocosColisao[8].colidido = false;
  blocosColisao[8].cliqueMouse = false;
  blocosColisao[8].coletado = false;
  blocosColisao[8].sprite = NULL;
  blocosColisao[8].spriteMascara = NULL;
  
  blocosColisao[9].x = 1100;
  blocosColisao[9].y = 130;
  blocosColisao[9].altura = 64;
  blocosColisao[9].largura = 64;
  blocosColisao[9].tipo = 5;
  blocosColisao[9].colidido = false;
  blocosColisao[9].cliqueMouse = false;
  blocosColisao[9].coletado = false;
  blocosColisao[9].sprite = NULL;
  blocosColisao[9].spriteMascara = NULL;

  
  //===============================> Cen�rios <===============================
  void *cenario1;
  cenario1 = carregarImagem(".//Artes//Cenarios//sala_tutas.bmp", 1280, 600, 0, 0);
  
  void *cenario2;
  cenario2 = carregarImagem(".//Artes//Cenarios//quarto_tutas.bmp", 1280, 600, 0, 0);
  
  void *cenario3;
  cenario3 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_1.bmp", 1280, 600, 0, 0);
  
  void *cenario4;
  cenario4 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_2.bmp", 1280, 600, 0, 0);
  
  void *cenario5;
  cenario5 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_3.bmp", 1280, 600, 0, 0);
  
  void *cenario6;
  cenario6 = carregarImagem(".//Artes//Cenarios//fase_2_cenario_4.bmp", 1280, 600, 0, 0);
  
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
	  if(pg == 1) pg = 2; else pg = 1;//t�cnica de pagina��o
	  setactivepage(pg);
	  cleardevice();
	  
      //=================> Lidando com a troca de fases <=================
      if(fases == 1)
      {
        putimage(0, 0, cenario1, COPY_PUT);
        LidandoComFases(qntItensColetados, fases, false, false, true, false, true, 5, 6, 5, 9);
        if(qntItensColetados >= 5)
          fases = 4;
	  }//n�o h� segunda fase pela l�gica que criei de transi��o de fase
	  else if(fases == 3)
	  {
	    putimage(0, 0, cenario2, COPY_PUT);
	    LidandoComFases(qntItensColetados, fases, false, false, false, true, true, 7, 9, 5, 9);
	    if(qntItensColetados >= 5)
          fases = 4;
	  }
	  else if(fases == 4)
	  {
	    putimage(0, 0, cenario3, COPY_PUT);
	    LidandoComFases(qntItensColetados, fases, false, false, true, false, false, 7, 9, 5, 9);
	  }
	  else if(fases == 6)
	  {
	    putimage(0, 0, cenario4, COPY_PUT);
	    LidandoComFases(qntItensColetados, fases, true, false, pegouMissao, true, false, 7, 9, 5, 9);
	  }
	  else if(fases == 7)
	  {
	    putimage(0, 0, cenario5, COPY_PUT);
	    LidandoComFases(qntItensColetados, fases, false, true, true, false, false, 7, 9, 5, 9);
	  }
	  else if(fases == 9)
	  {
	    putimage(0, 0, cenario6, COPY_PUT);
	    LidandoComFases(qntItensColetados, fases, false, false, false, true, false, 7, 9, 5, 9);
	  }
	  
      setvisualpage(pg);
    }
    
    //===============================> Input para sa�da do la�o <===============================
    fflush(stdin);
    if (kbhit())
	{
      tecla = getch();
    }
  }
  
  printf("\n\nFim do Programa");
  for(int i = 0; i<qntBlocos; i++)
  {
    free(blocosColisao[i].sprite);
  	free(blocosColisao[i].spriteMascara);
  }
  free(blocosColisao);
  free(cenario1);
  free(cenario2);
  free(cenario3);
  free(cenario4);
  free(cenario5);
  free(cenario6);
  closegraph();
  return 0; 
}

void* carregarImagem(const char *sprite, int largura, int altura , int x, int y)
{	
  void *image;//vetor para bits brutos
  int aux = imagesize(x, y, largura, altura);//tamanho da imagem
  image = malloc(aux);//abrindo espa�o de mem�ria para o vetor com base no tamanho da imagem
  readimagefile(sprite, x, y, largura, altura);//lendo a imagem que queremos
  getimage(x, y, largura, altura, image);
  
  return image;
}

void DesenhandoBotao( int xImagem, int yImagem, void *sprites,void *spritesMascara)
{
  putimage(xImagem, yImagem, spritesMascara, AND_PUT);	
  putimage(xImagem, yImagem, sprites, OR_PUT);
}

void LidandoComFases(int &numItensColetados, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool inventario, int blocoInicial, int blocoFinal, int InvIndexInicial, int InvIndexFinal)
{
  int controleItensInventario = InvAltura/(InvIndexFinal - InvIndexInicial + 1);//Criando vari�vel para controlar a localidade de cada item no invent�rio.
  int salvandoPosicaoInicial;
  
  //===============================> Desenhando os Itens <===============================
  setfillstyle(1,RGB(255, 255, 0));
  for(int i = blocoInicial; i < blocoFinal + 1; i++)//conferindo quais itens tem na fase
  {
	if(!blocosColisao[i].coletado)
	  bar(blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].x + blocosColisao[i].largura, blocosColisao[i].y + blocosColisao[i].altura);
  }
  
  //===============================> Lidando com os Blocos <===============================
  setfillstyle(1,RGB(255, 0, 0));
  for(int i = 0; i <= 4; i++)//pegando os bot�es bases para troca de fase e invent�rio
  {
    //===============================> Blocos para Mudan�a de Fase <===============================
    if(i == 0 && fasePraCima == true)//conferindo para quais lados ser�o as pr�ximas fases / fases anteriores
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
    if(i == 1 && fasePraDireita == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
    if(i == 2 && fasePraEsquerda == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
    if(i == 3 && fasePraBaixo == true)
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
      
    //===============================> Bot�o do Invent�rio <===============================
    if(i == 4 && inventario == true)
    {
      DesenhandoBotao( blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].sprite, blocosColisao[i].spriteMascara);
	}
      
    //==================> A��es dos Bloco de Colis�o que n�o sejam colet�veis (Bot�es de mudan�a de fases e invent�rio) <==================
    if(blocosColisao[i].tipo == 0 && blocosColisao[i].cliqueMouse == true && fasePraCima == true)//bot�o fase Acima
	{
	  blocosColisao[i].cliqueMouse = false;
	  fase += 1;
	}
	else if(blocosColisao[i].tipo == 1 && blocosColisao[i].cliqueMouse == true && fasePraDireita == true)//bot�o fase para � Direita
	{	
	  blocosColisao[i].cliqueMouse = false;
	  fase += 2;
	}
	else if(blocosColisao[i].tipo == 2 && blocosColisao[i].cliqueMouse == true && fasePraEsquerda == true)//bot�o fase � Esquerda
	{	
	  blocosColisao[i].cliqueMouse = false;
	  fase -= 2;
	}
    else if(blocosColisao[i].tipo == 3 && blocosColisao[i].cliqueMouse == true && fasePraBaixo == true)//bot�o fase Abaixo
    {
	  blocosColisao[i].cliqueMouse = false;
	  fase -= 1;
	}
	else if(blocosColisao[i].tipo == 4 && blocosColisao[i].cliqueMouse == true)//bot�o para invent�rio
    {
      if(!blocosColisao[i].coletado)//abrindo o invent�rio
      {
        blocosColisao[i].x = xInv - blocosColisao[i].largura;
        blocosColisao[i].coletado = true;
	  }
	  else//fechando o invent�rio
	  {
	    blocosColisao[i].x = 1280 - blocosColisao[i].largura;
	    blocosColisao[i].coletado = false;
	  }
	  blocosColisao[i].cliqueMouse = false;
	}
	
	//==================> Criando o Invent�rio <==================
    if(blocosColisao[i].tipo == 4 && blocosColisao[i].coletado == true)
    {
      setfillstyle(1,RGB(50, 125, 255));
      bar(blocosColisao[i].x + blocosColisao[i].largura, 0, 1280, 720);//desenhando o invent�rio
      
      for(int j = InvIndexInicial; j < InvIndexFinal + 1; j++)
      {
	    if(!blocosColisao[j].coletado)//modificando a imagem c base se o item foi coletado ou n�o
	    {
	      setfillstyle(1,RGB(50, 125, 50));
	      bar(xInv + (InvLargura/2) - (blocosColisao[i].largura/2), controleItensInventario*(j - InvIndexInicial + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), xInv + (InvLargura/2) + (blocosColisao[i].largura/2), controleItensInventario*(j - InvIndexInicial + 1) - (controleItensInventario/2) + (blocosColisao[j].largura/2));
		}
	    else
	    {
	      setfillstyle(1,RGB(0, 0, 0));
	      bar(xInv + (InvLargura/2) - (blocosColisao[i].largura/2), controleItensInventario*(j - InvIndexInicial + 1) - (controleItensInventario/2) - (blocosColisao[j].largura/2), xInv + (InvLargura/2) + (blocosColisao[i].largura/2), controleItensInventario*(j - InvIndexInicial + 1) - (controleItensInventario/2) + (blocosColisao[j].largura/2));
		}
      }
	}
  }
  
  //===============================> Colis�o do Mouse <===============================
  (GetCursorPos(&P));
  (ScreenToClient(janela, &P));
      
  for(int i = 0; i < qntBlocos; i++)
  {
    ChecagemDeColisao(P.x, P.y, blocosColisao[i].x, blocosColisao[i].y, blocosColisao[i].largura, blocosColisao[i].altura, blocosColisao[i].colidido);//Coletando a informa��o de colis�o e guardando dentro do item.
  }
  if(colisaoMouse == false)
  {
    for(int i = 0; i < qntBlocos; i++)//checagem dos Itens para ver se h� algum colidido e Qual foi.
	{
	  colisaoMouse = blocosColisao[i].colidido;
	  if(colisaoMouse == true)//se h�, coleta qual item
	  {
	    indexItemColidido = i;
	    break;
	  }
	}
  }
  if(colisaoMouse == true)
  {
	for(int i = 0; i < qntBlocos; i++)//checagem se n�o h� mais colis�o.
	{
	  if(blocosColisao[i].colidido == true)//se h�, ent�o ignora a verifica��o
		break;
	  else if(i == qntBlocos - 1)//se n�o h�. ent�o mouse n�o est� colidido
	  {
		if(blocosColisao[i].colidido == false) colisaoMouse = false;
	  }
	}
  }
  //===============================> Captura de Inputs <===============================
  if(GetKeyState(VK_LBUTTON)&0x80 && colisaoMouse == true)//Input do Mouse
  {
    if(TempoDecorrido > Espera)
    {
      Inicio = GetTickCount();//resetando o tempo de espera
      blocosColisao[indexItemColidido].cliqueMouse = true;
      if(blocosColisao[indexItemColidido].tipo == 5 && blocosColisao[indexItemColidido].coletado == false)//Captura de Colet�veis
      {	
        numItensColetados++;//registrando itens coletados
        blocosColisao[indexItemColidido].coletado = true;//impossibilitando de clicar novamente
	  }	
	}
  }
}

bool ChecagemDeColisao( int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu)
{
  if( xColisor >= xColidido && yColisor >= yColidido )//chegagem se o mouse est� � direita e em baixo do v�rtice superior esquerdo
  {
    if( xColisor <= (xColidido + larguraColidido) && yColisor <= (yColidido + alturaColidido) )//chegagem se o mouse est� � esquerda e em cima do v�rtice inferior direito
    {
      colidiu = true;
	}
   else
	  colidiu = false;
  }
  else
	  colidiu = false;
}

/*
largura de uma letra na tela = 4 pixeis;
strcpy() -> Copia a string para dentro da vari�vel
strcat() -> Junta duas strings
strlen() -> Nos fala o tamanho de uma string
strcmp() -> Compara duas strings
gets()   -> Input para strings, muito melhor que scanf
puts()   -> Print para string, mais f�cil que printf
*/
 
