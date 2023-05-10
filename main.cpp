#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<time.h>
using namespace std;

#define ESC    	27

/*
  FAZER COM QUE QUANDO DER FALSE NOS BOTOES DE MUDANÇA DE FASE ELES N APAREÇAM NA TELA
*/

//local para os structs
typedef struct _blocosColisao
{
  int x, y, altura, largura, tipo;
  bool colidido, cliqueMouse;
}BlocoDeColisao;

//Area para funcoes
void* loadimage(const char *sprite, int largura, int altura , int x, int y);

bool ChecagemDeColisao(int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu);

void LidandoComFases(BlocoDeColisao *blocos, int qntBlocos, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, int blocoInicial, int blocoFinal);

void ExibeTempo(int x, int y, char *msg, long long t);

//===============================> Mouse <===============================
  HWND janela;//coletando a janela
  POINT P;//Posição do Mouse
  int indexItemColidido = 0;//ColetandoBlocoColidido
  bool colisaoMouse = false;

//=================================================> JOGO <=================================================
int main()
{
  int pg = 1;
  char tecla;	
  unsigned long long gt1, gt2;//Clocks do Computador
  int fps = 60;
  
  //===============================> Fases <===============================
  int fases = 0;
  
  //===============================> Blocos de Colisão para o Mouse <===============================
  //Vetor que está guardando os blocos
  BlocoDeColisao *blocosColisao;
  int qntBlocos = 19;
  blocosColisao = NULL;
  blocosColisao = (BlocoDeColisao *) malloc(sizeof(BlocoDeColisao) * qntBlocos);

  //Botões para troca de fases
  blocosColisao[0].x = 640;
  blocosColisao[0].y = 20;
  blocosColisao[0].altura = 20;
  blocosColisao[0].largura = 20;
  blocosColisao[0].tipo = 0;
  blocosColisao[0].colidido = false;
  blocosColisao[0].cliqueMouse = false;
  
  blocosColisao[1].x = 1230;
  blocosColisao[1].y = 360;
  blocosColisao[1].altura = 20;
  blocosColisao[1].largura = 20;
  blocosColisao[1].tipo = 1;
  blocosColisao[1].colidido = false;
  blocosColisao[1].cliqueMouse = false;
  
  blocosColisao[2].x = 30;
  blocosColisao[2].y = 360;
  blocosColisao[2].altura = 20;
  blocosColisao[2].largura = 20;
  blocosColisao[2].tipo = 2;
  blocosColisao[2].colidido = false;
  blocosColisao[2].cliqueMouse = false;
  
  blocosColisao[3].x = 640;
  blocosColisao[3].y = 680;
  blocosColisao[3].altura = 20;
  blocosColisao[3].largura = 20;
  blocosColisao[3].tipo = 3;
  blocosColisao[3].colidido = false;
  blocosColisao[3].cliqueMouse = false;
  
  blocosColisao[4].x = 500;
  blocosColisao[4].y = 500;
  blocosColisao[4].altura = 20;
  blocosColisao[4].largura = 20;
  blocosColisao[4].tipo = 4;
  blocosColisao[4].colidido = false;
  blocosColisao[4].cliqueMouse = false;
  
  //Itens
  
  //===============================> Tempo de Espera <===============================
  int espera = 2000;
  double tempoDecorrido;
  unsigned long long inicio, agora;
  bool contar = false;

  //===============================> Lidando com a janela <===============================
  initwindow(1280, 720, "Projeto Alma");
  janela = GetForegroundWindow();
  setactivepage(pg);//Deixa ativa a pagina do laço para desenhar nela
  setvisualpage(pg);//Deixa visual para mostrar o que foi desenhado na tela
  tecla = 0;
  
  //===============================> Salvando o tick do computador Inicialmente <===============================
  gt1 = GetTickCount();
  inicio = GetTickCount();
  agora = inicio + espera;
  
  while(tecla != ESC)
  {
    //===============================> Coletando o Tick ao passar do tempo <===============================
    gt2 = GetTickCount();
    agora = GetTickCount();
    
	tempoDecorrido = agora - inicio;
	
    if (gt2-gt1 > 1000/fps)//setando teto para frames
	{
      gt1 = gt2;
	  if(pg == 1) pg = 2; else pg = 1;
	  setactivepage(pg);
	  cleardevice();
      
      
      LidandoComFases(blocosColisao, qntBlocos, fases, false, true, true, true, 4, 4);
      ExibeTempo(10, 10, "Fase = ", fases);
      
      setvisualpage(pg);
    }
    
    //===============================> Input para saída do laço <===============================
    fflush(stdin);    
    if (kbhit())
	{
      tecla = getch();
    }
  }

  printf("\n\nFim do Programa");
  free(blocosColisao);
  closegraph();
  return 0; 
}

void* loadimage(const char *sprite, int largura, int altura , int x, int y)
{	
  void *image;
  int aux = imagesize(x, y, largura, altura);
  image = malloc(aux);
  readimagefile(sprite, x, y, largura, altura);
  getimage(x, y, largura, altura, image);
  cleardevice();
  
  return image;
}

void LidandoComFases(BlocoDeColisao *blocos, int qntBlocos, int &fase, bool fasePraCima, bool fasePraBaixo, bool fasePraDireita, bool fasePraEsquerda, int blocoInicial, int blocoFinal)
{
  //===============================> Desenhando os Itens <===============================
  setfillstyle(1,RGB(255, 255, 0));
  for(int i = blocoInicial; i < blocoFinal + 1; i++)
  {
	if(!blocos[i].cliqueMouse)
	  bar(blocos[i].x, blocos[i].y, blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura);
  }
  
  //===============================> Blocos para Mudança de Fase <===============================
  setfillstyle(1,RGB(255, 0, 0));
  for(int i = 0; i <= 3; i++)
  {
    bar(blocos[i].x, blocos[i].y, blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura);
    if(blocos[i].tipo == 3 && blocos[i].cliqueMouse == true && fasePraBaixo == true)
    {	
	  fase -= 1;
	  blocos[i].cliqueMouse = false;
	}
	else if(blocos[i].tipo == 1 && blocos[i].cliqueMouse == true && fasePraDireita == true)
	{	
	  fase += 2;
	  blocos[i].cliqueMouse = false;
	}
	else if(blocos[i].tipo == 2 && blocos[i].cliqueMouse == true && fasePraEsquerda == true)
	{	
	  fase -= 2;
	  blocos[i].cliqueMouse = false;
	}
	else if(blocos[i].tipo == 0 && blocos[i].cliqueMouse == true && fasePraCima == true)
	{
	  fase += 1;
	  blocos[i].cliqueMouse = false;	
	}
  }
  
  //===============================> Colisão do Mouse <===============================
  (GetCursorPos(&P));
  (ScreenToClient(janela, &P));
      
  for(int i = 0; i < qntBlocos; i++)
  {
    ChecagemDeColisao(P.x, P.y, blocos[i].x, blocos[i].y, blocos[i].largura, blocos[i].altura, blocos[i].colidido);//Coletando a informação de colisão e guardando dentro do item.
  }
  if(colisaoMouse == false)
  {
    for(int i = 0; i < qntBlocos; i++)//Checagem dos Itens para ver se há algum colidido e Qual foi.
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
	for(int i = 0; i < qntBlocos; i++)//Checagem se não há mais colisão.
	{
	  if(blocos[i].colidido == true) 
		break;
	  else if(i == qntBlocos - 1)
	  {
		if(blocos[i].colidido == false) colisaoMouse = false;
	  }
	} 
  }
  //===============================> Captura de Inputs <===============================
  if(GetKeyState(VK_LBUTTON)&0x80 && colisaoMouse == true)//Input do Mouse
  {
    blocos[indexItemColidido].cliqueMouse = true;
  }
}

bool ChecagemDeColisao( int xColisor, int yColisor, int xColidido, int yColidido, int larguraColidido, int alturaColidido, bool &colidiu)
{
  if( xColisor >= xColidido && yColisor >= yColidido )
  {
    if( xColisor <= (xColidido + larguraColidido) && yColisor <= (yColidido + alturaColidido) )
    {
      colidiu = true;
	}
   else
    colidiu = false;
  }
  else
    colidiu = false;
}

void ExibeTempo(int x, int y, char *msg, long long t) 
{
  char st[50], S[100];
  itoa(t, st, 10);
  strcpy(S, msg);
  strcat(S, st);
  outtextxy(x, y, S);
}
/*
strcpy() -> Copia a string para dentro da variável
strcat() -> Junta duas strings
strlen() -> Nos fala o tamanho de uma string
strcmp() -> Compara duas strings
gets()   -> Input para strings, muito melhor que scanf
puts()   -> Print para string, mais fácil que printf
*/
 
