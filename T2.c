/// Programa desenvolvido por Roberto L. e Pablo B.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EXPT 100

//Struct de uma variável
typedef struct avl{
   char nome;
   int valor;
   int altura;
   struct avl *esq;
   struct avl *dir;
}Var;

typedef struct arv_var{
   Var* prim;
}Arv_Var;

Arv_Var* cria_var(void){
   Arv_Var* a = (Arv_Var*) malloc(sizeof(Arv_Var));
   a->prim = NULL;
   return a;
}

//Nó que armazena os números
typedef struct num {
   char* info; //Contém um número
   struct num* prox; //Aponta para o próximo operador da pilha
}Num;

//Início da pilha de números
typedef struct pilhan {
   Num* prim; //Aponta para o topo de uma pilha de números
}Pilhan;

//Nó que armazena as operadores
typedef struct op {
   char opera; //Contém o operando
   struct op* prox; //Aponta para o próximo operando da pilha
}Op;

//Início da plilha de operadores
typedef struct pilhao {
   Op* prim; //Aponta para o topo de uma pilha de operadores
}Pilhao;

//Retorna erro
void erro(void){
   printf("ERRO FATAL\nAbortando o programa\n");
   exit(0);
}

//Função que verifica e retorna o maior valor entre dois
int maior(int a, int b){
   if(a>b)
      return a;
   return b;
}

//Função que faz a rotação para a direita na árvore
Var* rotacaoDireita(Var* K2 ){
   Var* K1;
   K1 = K2->esq;
   K2->esq = K1->dir;
   K1->dir = K2;
   K2->altura = maior( altura( K2->esq ), altura( K2->dir ) ) + 1;
   K1->altura = maior( altura( K1->esq ), K2->altura ) + 1;
   return K1;
}

//Função que faz a rotação para a esquerda na árvore
Var* rotacaoEsquerda(Var* K1 ){
   Var* K2;
   K2 = K1->dir;
   K1->dir = K2->esq;
   K2->esq = K1;
   K1->altura = maior( altura( K1->esq ), altura( K1->dir ) ) + 1;
   K2->altura = maior( altura( K2->dir ), K1->altura ) + 1;
   return K2;
}

//Função que faz uma rotação para a esquerda seguida de uma rotação para a direita na árvore
Var* rotacaoEsquerdaDireita(Var* K3 ){
   K3->esq = rotacaoEsquerda( K3->esq );
   return rotacaoDireita( K3 );
}

//Função que faz uma rotação para a direita seguida de uma rotação para a esquerda na árvore
Var* rotacaoDireitaEsquerda(Var* K1 ){
   K1->dir;
   K1->dir = rotacaoDireita( K1->dir );
   K1->dir;
   return rotacaoEsquerda( K1 );
}

//Função que retorna a altura da árvore ou de uma sub-árvore
int altura (Var* a){
   if(a == NULL)
      return -1;
   return a->altura;
}

//Função que aloca um nó de variável
Var* aloca(char nome, int valor){
   Var* arv;
   arv = (Var*)malloc(sizeof(Var));
   arv->nome= nome;
   arv->valor=valor;
   arv->altura = 0;
   arv->esq = NULL;
   arv->dir = NULL;
   return arv;
}


//Retira uma variável da árvore
Var* retira_var (Var* v, char nome){
   if(v==NULL){
      return NULL;
   }
   else{
      if(v->nome>nome){
         v->esq=retira_var(v->esq, nome);
      }
      else{
         if(v->nome<nome){
            v->dir=retira_var(v->dir, nome);
         }
         else{
            if((v->esq==NULL) && (v->dir==NULL)){
               free(v);
               return NULL;
            }
            else{
               if(v->dir==NULL){
                  Var* aux=v;
                  v=v->esq;
                  free(aux);
               }
               else{
                  if(v->esq==NULL){
                     Var* aux=v;
                     v=v->dir;
                     free(aux);
                  }
                  else{
                     Var* pai=v;
                     Var* filho=v->esq;
                     while(filho->dir!=NULL){
                        pai=filho;
                        filho=filho->dir;
                     }
                     v->nome=filho->nome;
                     v->valor=filho->valor;
                     filho->nome=nome;
                     v->esq=retira_var(v->esq, nome);
                  }
                  return v;
               }
            }
         }
      }
   }
}


//Função que insere um elemento novo na árvore de variáveis e faz as rotações
Var* insere_var(Var* arv, char nome, int valor){
   if( arv == NULL ){
      arv=aloca(nome, valor);
   }
   else{
      if(arv->nome == nome){
         if(valor == 0)
            arv = retira_var(arv,nome);
         else
            arv->valor = valor;
         return arv;
      }
      if(nome < arv->nome){
         arv->esq = insere_var( arv->esq, nome, valor );
         if( altura( arv->esq ) - altura( arv->dir ) == 2 ){
            if( nome< arv->esq->nome){
               arv = rotacaoDireita( arv );
            }
            else{
               arv = rotacaoEsquerdaDireita( arv );
            }
         }
      }
      else{
         if( nome> arv->nome){
            arv->dir;
            arv->dir = insere_var( arv->dir, nome, valor );
            if( altura( arv->dir ) - altura( arv->esq ) == 2 ){
               if( nome> arv->dir->nome){
                  arv = rotacaoEsquerda( arv );
               }
               else{
                  arv = rotacaoDireitaEsquerda( arv );
               }
            }
         }
      }
   }
   arv->altura = maior( altura( arv->esq ), altura( arv->dir )) + 1;
   return arv;
}

//Procura uma variável pelo nome e retorna seu valor, se a variável não existir retorna zero
int busca_var(Var* raiz, char i){
   if (raiz == NULL) 
       return 0;
   else if (i < raiz->nome) 
       return busca_var(raiz->esq, i);
   else if (i > raiz->nome) 
       return busca_var(raiz->dir, i);
   else return raiz->valor;
}

//Função que escreve a lista de variáveis
void imprime_var(Var* a){
   if (a==NULL)
      return;
   imprime_var(a->esq);
   printf("%c = %d, ", a->nome, a->valor);
   imprime_var(a->dir);
}

//Remove árvore
void limpa_arvore(Var* v){
   if(v==NULL)
      return;
   limpa_arvore(v->esq);
   limpa_arvore(v->dir);
   free(v);
}


//Inicializa uma pilha vazia de números
Pilhan* cria_num(void){
   Pilhan* p = (Pilhan*) malloc(sizeof(Pilhan));
   p->prim = NULL;
   return p;
}

//Insere um número na lista de números
Num* insere_num(Num* l, char* i){
   Num* p = (Num*)malloc(sizeof(Num));
   char* aux = (char*)malloc(strlen(i)*sizeof(char));
   strcpy(aux, i);
   p->info = aux;
   p->prox = l;
   return p;
}

//Remove um nó do começo de uma lista de números
Num* ret_ini_num(Num* l){
   Num* p = l->prox;
   free(l->info);
   free(l);
   return p;
}

//Inicializa uma pilha vazia de operadores
Pilhao* cria_op(void){
   Pilhao* p = (Pilhao*) malloc(sizeof(Pilhao));
   p->prim = NULL;
   return p;
}

//Insere um operador na lista de operadores
Op* insere_op(Op* l, char op){
   Op* p = (Op*)malloc(sizeof(Op));
   p->opera = op;
   p->prox = l;
   return p;
}

//Faz push na pilha de operadores
void empilha_operador(Pilhao* p, char op){
   p->prim = insere_op(p->prim, op);
}


//Remove um nó do começo de uma lista de caracteres
Op* ret_ini_op(Op* l){
   Op* p = l->prox;
   free(l);
   return p;
}

//Faz push na pilha de operando
void empilha_operando(Pilhan* p, char* i){
   p->prim = insere_num(p->prim,i);
}

//Faz pop na pilha de operandos
char* pop_num(Pilhan* p){
   if (p->prim==NULL){
      erro();
   }
   char* i = (char*)malloc(strlen(p->prim->info)*sizeof(char));
   strcpy(i, p->prim->info);
   p->prim = ret_ini_num(p->prim);
   return i;
}

//Faz pop na pilha de operadores
char pop_op(Pilhao* p){
   if(p->prim==NULL){
      erro();
   }
   char c;
   c = p->prim->opera;
   p->prim = ret_ini_op(p->prim);
   return c;
}

//Trabalha para o caso da expressão começar com espaço
int caso_zero(char *atomo, char *string){
   if (*string==' '){ //Se espaço
      return 0;
   }
   else{
      if (((*string>=65) && (*string<=90)) ||  ((*string>=97) && (*string<=122))){ //Se letra
         *atomo=*string;
         return 1;
      }
      else{
         if ((*string>=48) && (*string<=57)){ //Se numero
            *atomo=*string;
            return 2;
         }
         else{
            if((*string=='+') || (*string=='-') || (*string=='*') || (*string=='/') || (*string=='^')  || (*string=='(') || (*string==')') || (*string=='=')){ //Se operador
               *atomo=*string;
               return 4;
            }
            else{
               return 3;
            }
         }
      }
   }
}

//Trabalha para o caso da expressão começar com uma letra
int caso_um(char *atomo, char *string){
    if (((*string>=65) && (*string<=90)) ||  ((*string>=97) && (*string<=122))){ //Se letra
        *atomo=*string;
        return 1;
    }
    else{
        return 6;
    }
}

//Trabalha para o caso da expressão começar com um sinal ariimético
int caso_dois(char *atomo, char *string){
    if((*string>=48) && (*string<=57)){ //Se numero
        *atomo=*string;
        return 2;
    }
    else{
        return 5;
    }
}

//Função que transforma um atomo em uma string e a retorna
void le_atomo(char *string, int *i, char* atomo){
    int estado=0, j=-1, k=-1;
    while(estado<=3){
        k++;
        j++;
        switch(estado){
            case 0:
                estado=caso_zero(&atomo[j], &string[*i+k]);
                if(estado==0)
                    j--;
                if(estado==4){
                    k=1;
                    j++;
                }
                break;
            case 1:
                estado=caso_um(&atomo[j], &string[*i+k]);
                break;
            case 2:
                estado=caso_dois(&atomo[j], &string[*i+k]);
                break;
            case 3:
                erro;
                break;
            case 4:
            case 5:
            case 6:
            default:
                break;
        }
    }
    atomo[j]='\0';
    *i=*i+k;
}

void troca(char s[], int i, int j)
{
char x;
x = s[i];
s[i] = s[j];
s[j] = x;
}

void inverte(char s[])
{
	int i,j;
	for(i=0, j=strlen(s)-1; i<j; i++, j--){
		troca(s,i,j);
	}
}


void itoa(int n, char s[])
{	
	int i,d, sinal;
	char c;
	i = 0;
	if(n < 0)
		sinal = -1;
	else
		sinal = 1;
	n *= sinal;
	do{
		d = n%10;
		n = n/10;
		c = d + '0';
		s[i++] = c;
	}while( n != 0);
	if(sinal == -1){
		s[i++] = '-';
	}
	s[i] = '\0';
	inverte (s);
}

//Tranforma um int num char

void atribui(Arv_Var* v, char nome, int valor){
   if(valor==0){
      v->prim=retira_var(v->prim, nome);
   }
   else{
      v->prim=insere_var(v->prim, nome, valor);
   }
}

int powa(int a, int b){
   int i = 0;
   int l = 1;
   for(i=0;i<b;i++){
      l *= a;
   }
   return l;
}

//Função que executa um cálculo
void calcula(Pilhan* n, char op, Arv_Var* variaveis){
   int i1, i2, i=0;
   char* aux=(char*)malloc(EXPT*sizeof(char));
   if((n->prim->info[0]>=48) && (n->prim->info[0]<=59)){//Se número
      i2=atoi(pop_num(n));
   }
   else{
      aux=pop_num(n);
      i2=busca_var(variaveis->prim, aux[0]);
   }
   if(n->prim==NULL){
      i1=0;
   }
   else{
      if((n->prim->info[0]>=48) && (n->prim->info[0]<=59)){//Se número
         i1=atoi(pop_num(n));
      }
      else{
         aux=pop_num(n);
         i1=busca_var(variaveis->prim, aux[0]);
      }
   }
   switch (op){
      case '+':
         i = i1 + i2;
         break;
      case '-':
         i = i1 - i2;
         break;
      case '*':
         i = i1*i2;
         break;
      case '/':
         if(i2==0)
            erro();
         i = i1/i2;
         break;
      case '^':
         i=powa(i1, i2);
         break;
      case '=':
         atribui(variaveis, aux[0], i2);
         i=i2;
         break;
   }
   //itoa(i, aux, 10); //windowns
   itoa(i,aux); //linux
   empilha_operando(n, aux);
}

//Função que executa as operações da pilha de operandos
void opera(Pilhao* calculo, Pilhan* numeros, Arv_Var* variaveis){
   char op;
   while((calculo->prim!=NULL) && (numeros->prim!=NULL)){
      op=pop_op(calculo);
      calcula(numeros, op, variaveis);
   }
}

//Função que faz operações no caso da existância de parênteses
void opera_parenteses(Pilhao* calculo, Pilhan* numeros, Arv_Var* variaveis){
   char op=0;
   while((calculo->prim!=NULL) && (numeros->prim!=NULL) && (op!='(')){
      op=pop_op(calculo);
      if(op!='(')
         calcula(numeros, op, variaveis);
   }
}

//Função que analisa e manda empilhar os operadores
void trabalha_operadores(Pilhao* calculo, Pilhan* numeros, char *atomo, Arv_Var* variaveis){
   switch(*atomo){
      case '+': case '-':
         if(calculo->prim!=NULL){
            if((calculo->prim->opera!='(') && (calculo->prim->opera!='='))
               opera(calculo, numeros, variaveis);
         }
         empilha_operador(calculo, *atomo);
         break;
      case '*': case '/':
         if(calculo->prim!=NULL){
            if((calculo->prim->opera!='(') && (calculo->prim->opera!='+') && (calculo->prim->opera!='-') && (calculo->prim->opera!='='))
               opera(calculo, numeros, variaveis);
         }
         empilha_operador(calculo, *atomo);
         break;
      case '^': case '(':
         empilha_operador(calculo, *atomo);
         break;
      case ')':
         opera_parenteses(calculo,numeros, variaveis);
         break;
      case '=':
         if(numeros->prim!=NULL){
            if(((numeros->prim->info[0]>=65) && (numeros->prim->info[0]<=90)) ||  ((numeros->prim->info[0]>=97) && (numeros->prim->info[0]<=122))){ //Se variável
               empilha_operador(calculo, *atomo);
            }
         }
         else{
            erro();
         }
         break;
   }
}

//Função que interpreta a expressão recebida
void interpreta_expressao(char *string, Pilhao* calculo, Pilhan* numeros, Arv_Var* variaveis){
   char atomo[EXPT];
   int i=0;
   while(string[i]!='\0'){
      le_atomo(string, &i, atomo);
      if((atomo[0]>=48) && (atomo[0]<=59)){ //Se numero
         empilha_operando(numeros, atomo);
      }
      else{
         if((atomo[0]=='+') || (atomo[0]=='-') || (atomo[0]=='*') || (atomo[0]=='/') || (atomo[0]=='^') || (atomo[0]=='%') || (atomo[0]=='(') || (atomo[0]==')') || (atomo[0]=='=')){ //Se operador
            trabalha_operadores(calculo, numeros, atomo, variaveis);
         }
         else{
            if (((atomo[0]>=65) && (atomo[0]<=90)) ||  ((atomo[0]>=97) && (atomo[0]<=122))){ //Se variável
               empilha_operando(numeros, atomo);
            }
            else{ //Se alguma outra coisa
               erro();
            }
         }
      }
   }
   opera(calculo, numeros, variaveis);
}

//Função que lê uma string do tecado
void le_calculo(char *string){
   gets(string);
}



//Função principal que contém o laço que executa o programa em si
int main (void){
   char conta[EXPT];
   Pilhao* operadores=cria_op();
   Pilhan* operandos=cria_num();
   Arv_Var* variaveis=cria_var();
   char* res;
   do{
      le_calculo(conta);
      if(conta[0]!='\0'){
         interpreta_expressao(conta, operadores, operandos, variaveis);

         res=pop_num(operandos);
         printf("Resposta: %s\n", res);
         free(res);

         printf("Variáveis: ");
         imprime_var(variaveis->prim);
         printf("\n");
      }
   }while(conta[0]!='\0');
   limpa_arvore(variaveis->prim);
   free(operadores);
   free(operandos);
   printf("Programa finalizado com sucesso!\n");
}
