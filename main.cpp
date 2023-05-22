#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
using namespace std;

#define ESC    	27

/*
  FAZER COM QUE QUANDO DER FALSE NOS BOTOES DE MUDAN�A DE FASE ELES N APARE�AM NA TELA
*/

//local para os structs
typedef struct _blocosColisao
{
  int x, y, altura, largura, tipo;
  bool colidido, cliqueMouse, coletado;
  void *sprite;
}BlocoDeColisao;

//Area para funcoes
void* carregarImagem(const char *sprite, int largura, int altura , int x, int y);

bool ChecagemDeColisao(int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu);

void LidandoComFases(BlocoDeColisao *blocos, int &numItensColetados, int qntBlocos, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool caixinhaTexto, int blocoInicial, int blocoFinal);

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
  
  //===============================> Blocos de Colis�o para o Mouse <===============================
  //Vetor que est� guardando os blocos
  BlocoDeColisao *blocosColisao;
  int qntBlocos = 10;
  blocosColisao = NULL;
  blocosColisao = (BlocoDeColisao *) malloc(sizeof(BlocoDeColisao) * qntBlocos);

  //Bot�es para troca de fases
  //bot�o para cima
  blocosColisao[0].x = 640;
  blocosColisao[0].y = 20;
  blocosColisao[0].altura = 20;
  blocosColisao[0].largura = 20;
  blocosColisao[0].tipo = 0;
  blocosColisao[0].colidido = false;
  blocosColisao[0].cliqueMouse = false;
  blocosColisao[0].coletado = false;
  
  //bot�o para direita
  blocosColisao[1].x = 1230;
  blocosColisao[1].y = 360;
  blocosColisao[1].altura = 20;
  blocosColisao[1].largura = 20;
  blocosColisao[1].tipo = 1;
  blocosColisao[1].colidido = false;
  blocosColisao[1].cliqueMouse = false;
  blocosColisao[1].coletado = false;
  
  //bot�o para esquerda
  blocosColisao[2].x = 30;
  blocosColisao[2].y = 360;
  blocosColisao[2].altura = 20;
  blocosColisao[2].largura = 20;
  blocosColisao[2].tipo = 2;
  blocosColisao[2].colidido = false;
  blocosColisao[2].cliqueMouse = false;
  blocosColisao[2].coletado = false;
  blocosColisao[2].sprite = carregarImagem(".//Artes//Cenarios//sala_tutas.bmp", 20, 20, blocosColisao[2].x, blocosColisao[2].y);
  
  //bot�o para baixo
  blocosColisao[3].x = 640;
  blocosColisao[3].y = 680;
  blocosColisao[3].altura = 20;
  blocosColisao[3].largura = 20;
  blocosColisao[3].tipo = 3;
  blocosColisao[3].colidido = false;
  blocosColisao[3].cliqueMouse = false;
  blocosColisao[3].coletado = false;
  
  //invent�rio
  blocosColisao[4].x = 1230;
  blocosColisao[4].y = 30;
  blocosColisao[4].altura = 20;
  blocosColisao[4].largura = 20;
  blocosColisao[4].tipo = 4;
  blocosColisao[4].colidido = false;
  blocosColisao[4].cliqueMouse = false;
  blocosColisao[4].coletado = false;
  
  //Itens
  int qntItensColetados = 0;
  
  blocosColisao[5].x = 320;
  blocosColisao[5].y = 230;
  blocosColisao[5].altura = 20;
  blocosColisao[5].largura = 20;
  blocosColisao[5].tipo = 5;
  blocosColisao[5].colidido = false;
  blocosColisao[5].cliqueMouse = false;
  blocosColisao[5].coletado = false;
  
  blocosColisao[6].x = 900;
  blocosColisao[6].y = 540;
  blocosColisao[6].altura = 20;
  blocosColisao[6].largura = 20;
  blocosColisao[6].tipo = 5;
  blocosColisao[6].colidido = false;
  blocosColisao[6].cliqueMouse = false;
  blocosColisao[6].coletado = false;
  
  blocosColisao[7].x = 670;
  blocosColisao[7].y = 240;
  blocosColisao[7].altura = 20;
  blocosColisao[7].largura = 20;
  blocosColisao[7].tipo = 5;
  blocosColisao[7].colidido = false;
  blocosColisao[7].cliqueMouse = false;
  blocosColisao[7].coletado = false;
  
  blocosColisao[8].x = 1000;
  blocosColisao[8].y = 400;
  blocosColisao[8].altura = 20;
  blocosColisao[8].largura = 20;
  blocosColisao[8].tipo = 5;
  blocosColisao[8].colidido = false;
  blocosColisao[8].cliqueMouse = false;
  blocosColisao[8].coletado = false;
  
  blocosColisao[9].x = 1100;
  blocosColisao[9].y = 130;
  blocosColisao[9].altura = 20;
  blocosColisao[9].largura = 20;
  blocosColisao[9].tipo = 5;
  blocosColisao[9].colidido = false;
  blocosColisao[9].cliqueMouse = false;
  blocosColisao[9].coletado = false;
  
  
  
  //===============================> Cen�rios <===============================
  void *cenario1;
  cenario1 = carregarImagem(".//Artes//Cenarios//sala_tutas.bmp", 1280, 600, 0, 0);
  
  void *cenario2;
  cenario2 = carregarImagem(".//Artes//Cenarios//quarto_tutas.bmp", 1280, 600, 0, 0);
  
  
  //=======================> Salvando o tick do computador Inicialmente <=======================
  gt1 = GetTickCount();
  
  while(tecla != ESC)
  {
    //===============================> Coletando o Tick ao passar do tempo <===============================
    gt2 = GetTickCount();
	
    if (gt2-gt1 > 1000/fps)//setando teto para frames
	{
      gt1 = gt2;
	  if(pg == 1) pg = 2; else pg = 1;//t�cnica de pagina��o
	  setactivepage(pg);
	  cleardevice();
      
      //=================> Lidando com a troca de fases <=================
      if(fases == 1)
      {
        putimage(0, 0, cenario1, COPY_PUT);
        LidandoComFases(blocosColisao, qntItensColetados, qntBlocos, fases, false, false, true, false, true, 5, 6);
	  }
	  else if(fases == 3)//n�o h� segunda fase pela l�gica q criei de transi��o de fase
	  {
	  	putimage(0, 0, cenario2, COPY_PUT);
	    LidandoComFases(blocosColisao, qntItensColetados, qntBlocos, fases, false, false, false, true, true, 7, 9);
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
    free(blocosColisao[i].sprite);
  free(blocosColisao);
  free(cenario1);
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

void LidandoComFases(BlocoDeColisao *blocos	, int &numItensColetados, int qntBlocos, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, bool caixinhaTexto, int blocoInicial, int blocoFinal)
{
  int controleItensInventario = InvAltura/(blocoFinal - blocoInicial + 2);//Criando vari�vel para controlar a localidade de cada item no invent�rio.
  
  //===============================> Desenhando os Itens <===============================
  setfillstyle(1,RGB(255, 255, 0));
  for(int i = blocoInicial; i < blocoFinal + 1; i++)
  {
	if(!blocos[i].coletado)
	  bar(blocos[i].x, blocos[i].y, blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura);
  }
  
  //===============================> Lidando com os Blocos <===============================
  setfillstyle(1,RGB(255, 0, 0));
  for(int i = 0; i <= 4; i++)
  {
    //===============================> Blocos para Mudan�a de Fase <===============================
    if(i == 0 && fasePraCima == true)//conferindo para quais lados ser�o as pr�ximas fases / fases anteriores
      bar(blocos[i].x, blocos[i].y, blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura);//desenhando os bot�es
    if(i == 1 && fasePraDireita == true)
      bar(blocos[i].x, blocos[i].y, blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura);
    if(i == 2 && fasePraEsquerda == true)
      bar(blocos[i].x, blocos[i].y, blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura);
    if(i == 3 && fasePraBaixo == true)
      bar(blocos[i].x, blocos[i].y, blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura);
    if(i == 3 && fasePraBaixo == true)
      bar(blocos[i].x, blocos[i].y, blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura);
      
    //===============================> Blocos de Texto <===============================
    if(i == 4 && caixinhaTexto == true)
      bar(blocos[i].x, blocos[i].y, blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura);
      
    //==================> Verifica��o do tipo do Bloco de Colis�o <==================
    if(blocos[i].tipo == 0 && blocos[i].cliqueMouse == true && fasePraCima == true)//bot�o fase Acima
	{
	  fase += 1;
	  blocos[i].cliqueMouse = false;	
	}
	else if(blocos[i].tipo == 1 && blocos[i].cliqueMouse == true && fasePraDireita == true)//bot�o fase para � Direita
	{	
	  fase += 2;
	  blocos[i].cliqueMouse = false;
	}
	else if(blocos[i].tipo == 2 && blocos[i].cliqueMouse == true && fasePraEsquerda == true)//bot�o fase � Esquerda
	{	
	  fase -= 2;
	  blocos[i].cliqueMouse = false;
	}
    else if(blocos[i].tipo == 3 && blocos[i].cliqueMouse == true && fasePraBaixo == true)//bot�o fase Abaixo
    {
	  fase -= 1;
	  blocos[i].cliqueMouse = false;
	}
	else if(blocos[i].tipo == 4 && blocos[i].cliqueMouse == true)//bot�o para invent�rio
    {
      if(!blocos[i].coletado)//abrindo o invent�rio
      {
        blocos[i].x = xInv - blocos[i].largura;
        blocos[i].coletado = true;
	  }
	  else//fechando o invent�rio
	  {
	    blocos[i].x = 1230;
	    blocos[i].coletado = false;
	  }
	  blocos[i].cliqueMouse = false;
	}
	
	//==================> Criando o Invent�rio <==================
    if(blocos[i].tipo == 4 && blocos[i].coletado == true)
    {
      setfillstyle(1,RGB(50, 125, 255));
      bar(blocos[i].x + blocos[i].largura, 0, 1280, 720);//desenhando o invent�rio
      
      for(int j = blocoInicial; j < blocoFinal + 1; j++)
      {
	    if(!blocos[j].coletado)//modificando a imagem c base se o item foi coletado ou n�o
	    {
	      setfillstyle(1,RGB(50, 125, 50));
	      bar(xInv + (InvLargura/2) - (blocos[i].largura/2), controleItensInventario*(j - blocoInicial + 1), xInv + (InvLargura/2) + (blocos[i].largura/2), controleItensInventario*(j - blocoInicial + 1) + blocos[j].largura);
		}
	    else
	    {
	      setfillstyle(1,RGB(0, 0, 0));
	      bar(xInv + (InvLargura/2) - (blocos[i].largura/2), controleItensInventario*(j - blocoInicial + 1), xInv + (InvLargura/2) + (blocos[i].largura/2), controleItensInventario*(j - blocoInicial + 1) + blocos[j].largura);
		}
      }
	}
  }
  
  //===============================> Colis�o do Mouse <===============================
  (GetCursorPos(&P));
  (ScreenToClient(janela, &P));
      
  for(int i = 0; i < qntBlocos; i++)
  {
    ChecagemDeColisao(P.x, P.y, blocos[i].x, blocos[i].y, blocos[i].largura, blocos[i].altura, blocos[i].colidido);//Coletando a informa��o de colis�o e guardando dentro do item.
  }
  if(colisaoMouse == false)
  {
    for(int i = 0; i < qntBlocos; i++)//checagem dos Itens para ver se h� algum colidido e Qual foi.
	{
	  colisaoMouse = blocos[i].colidido;
	  if(colisaoMouse == true)
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
	  if(blocos[i].colidido == true)//se h�, ent�o ignora a verifica��o
		break;
	  else if(i == qntBlocos - 1)//se n�o h�. ent�o mouse n�o est� colidido
	  {
		if(blocos[i].colidido == false) colisaoMouse = false;
	  }
	}
  }
  //===============================> Captura de Inputs <===============================
  if(GetKeyState(VK_LBUTTON)&0x80 && colisaoMouse == true)//Input do Mouse
  {
    blocos[indexItemColidido].cliqueMouse = true;
    if(blocos[indexItemColidido].tipo == 5 && blocos[indexItemColidido].coletado == false)
    {	
      numItensColetados++;//registrando itens coletados
      blocos[indexItemColidido].coletado = true;//impossibilitando de clicar novamente
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
 
