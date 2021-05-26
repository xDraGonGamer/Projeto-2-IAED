#include <stdio.h>        /*ist199333 Tiago Santos*/
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "caminho.h"
#define HELPDESC   "Imprime os comandos disponiveis."
#define QUITDESC   "Termina o programa."
#define SETDESC    "Adiciona ou modifica o valor a armazenar."
#define PRINTDESC  "Imprime todos os caminhos e valores."
#define FINDDESC   "Imprime o valor armazenado."
#define LISTDESC   "Lista todos os componentes de um caminho."
#define SEARCHDESC "Procura o caminho dado um valor."
#define DELETEDESC "Apaga um caminho e todos os subcaminhos."
#define HELP   "help"        /*     estrutura de dados utilizada  */
#define QUIT   "quit"     /* arvore em que cada no tem n caminhos e cada */
#define SET    "set"   /*caminho tem uma outra arvore que sao os seus subcaminhos*/
#define PRINT  "print"
#define FIND   "find"
#define LIST   "list"
#define SEARCH "search"
#define DELETE "delete"
#define MAXCMDSIZE 65535
#define NOMEMORYERROR "No memory."
#define DENTRO 1
#define FORA 0
#define HASHPRIME 13007
#define DELIM "/"
#define ERROR_NOTFOUND "not found"
#define ERROR_NODATA "no data"

int nCaminhos, searchAux = FORA;

char* leCaminho(){
	char cmd[MAXCMDSIZE];
	char *res;
	if(getchar() == '\n'){ /*se o comando for introduzido sem argumentos, retorna NULL*/
		return NULL;
	}
	if(scanf("%s", cmd) == 0){ /*se nao conseguir ler e porque passa dos limites*/
		printf(NOMEMORYERROR);
                return NULL;
	}
	res = (char*) malloc((strlen(cmd) + 1) * sizeof(char));
	strcpy(res, cmd);
	return res;
}

char* simplificaCaminho(char* caminho){
	char aux[MAXCMDSIZE];
	char* res = caminho;
	int estado = DENTRO, index;
	if(*caminho == '/'){ /*se o primeiro caracter for a barra, comeca do inicio*/
		index = 0;
	}else{
		aux[0] = '/'; /*caso contrario adiciona a barra e comeca do segundo caracter*/
		index = 1;
	}
	while(*caminho != '\0'){ 
		if(*caminho == '/'){ /*se for barra e a variavel de controlo for dentro, copia a barra*/
			if(estado == DENTRO){
				estado = FORA;
				aux[index++] = *caminho;
			} /*se a variavel de controlo for fora, ignora a barra*/
			caminho++;
		}else{ /*se nao for barra, adiciona sempre*/
			estado = DENTRO;
			aux[index++] = *caminho;
			caminho++;
		}
	}
	aux[index] = '\0';
	free(res);
	res = (char*) malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(res, aux);
	return res;
}

void helpCommand(){ /*mostra todos os comandos e os seus propositos*/
	printf(HELP": " HELPDESC "\n");
	printf(QUIT": " QUITDESC "\n");
	printf(SET": " SETDESC "\n");
	printf(PRINT": " PRINTDESC "\n");
	printf(FIND": " FINDDESC "\n");
	printf(LIST": " LISTDESC "\n");
	printf(SEARCH": " SEARCHDESC "\n");
	printf(DELETE": " DELETEDESC "\n");
}

char* leValor(){
	char aux[MAXCMDSIZE], c, *res;
	int index = 0;
	getchar();
	while((c = getchar()) == ' ' || c == '\t'){/*remove os espacos iniciais*/
	}
	while(c != '\n'){
		aux[index++] = c;
		c = getchar();
	}
	aux[index--] = '\0';
	while(aux[index] == ' ' || aux[index] == '\t'){ /*remove os espacos finais*/
		aux[index--] = '\0';
	}
	res = (char*) malloc((strlen(aux) + 1));
	strcpy(res, aux);
	return res;
}

void setCommand(Arvore* ordenada, char* caminho, char* valor){
	char *token, *acumulator;
	Caminho *caminhoAuxOrdenado = NULL;
	acumulator = (char*) malloc(sizeof(char));
	token = strtok(caminho, DELIM);
	while(token != NULL){
		acumulator = (char*) realloc(acumulator, strlen(acumulator) + strlen(token) + 2);
		strcat(acumulator, DELIM);
		strcat(acumulator, token);
		caminhoAuxOrdenado = get_caminho_ordenado(ordenada, acumulator, nCaminhos);
		if(caminhoAuxOrdenado != NULL){ /*se a posicao estiver ocupada*/
			if(get_subcaminhos(caminhoAuxOrdenado) != NULL){ /*se tem filhos*/
				/*avanca-se para as sublistas a seguir*/
				ordenada = get_subcaminhos(caminhoAuxOrdenado);
			}else{    /*se nao tem filhos, cria uma lista de filhos*/
				set_subcaminhos(caminhoAuxOrdenado, initArvore(HASHPRIME));
				ordenada = get_subcaminhos(caminhoAuxOrdenado);
			}
		}else{ /*se o caminho nao pertencer*/
			nCaminhos++;
			/*cria se um novo caminho e avanca-se para a subarvore desse caminho*/
			caminhoAuxOrdenado = newCaminho(NULL, acumulator);
			set_subcaminhos(caminhoAuxOrdenado, initArvore(HASHPRIME));
			add_ordered_caminho(ordenada, caminhoAuxOrdenado);
			ordenada = get_subcaminhos(caminhoAuxOrdenado);
		}
		token = strtok(NULL, DELIM);
	}
	set_val(caminhoAuxOrdenado, valor);
}

void printCommand(Arvore* ordenada){
	int i = 0;
	if(get_caminho(ordenada, 0) == NULL){ /*base da recursao*/
		return;
	}
	for(; i < nCaminhos; i++){
		/*se for caminho nao nulo de valor nao nulo*/
		if(get_caminho(ordenada, i) != NULL && get_val(get_caminho(ordenada, i)) != NULL){
			printf("%s ", get_cam(get_caminho(ordenada, i)));
			printf("%s\n", get_val(get_caminho(ordenada, i)));
		}
		/*chamada recursiva*/
		if(get_caminho(ordenada, i) != NULL){
			printCommand(get_subcaminhos(get_caminho(ordenada, i)));
		}
	}
}

void findCommand(Arvore* ordenado, char* caminho){
	char *token, *acumul;
	Caminho* caminhoAux;
        acumul = (char*) malloc(sizeof(char));
	token = strtok(caminho, DELIM);
	while(token != NULL){
		acumul = (char*) realloc(acumul, strlen(acumul) + strlen(token) + 2);
		strcat(acumul, DELIM);
                strcat(acumul, token);
		/*caminho atualizado para o proximo caminho de caminho acumul*/
		caminhoAux = get_caminho_ordenado(ordenado, acumul, nCaminhos);
		if(caminhoAux == NULL){
			printf("%s\n", ERROR_NOTFOUND);
			return;
		}
		ordenado = get_subcaminhos(caminhoAux); /*proxima subarvore*/
		token = strtok(NULL, DELIM);
	}
	if(get_val(caminhoAux) == NULL){
		printf("%s\n", ERROR_NODATA);
		return;
	}
	free(acumul);
	printf("%s\n", get_val(caminhoAux));
}

void swapStr(char** str1, char** str2){
	char* aux;
	aux = *str1;
	*str1 = *str2;
	*str2 = aux;
}

void sortComponentes(char** list, int size){
	int i, a = 0;
	if(size <= 1){
		return;
	}
	for(i = 0; i < size; i++){
		if(strcmp(list[i], list[size - 1]) < 0){
			swapStr(list + i, list + a++);
		}
	}
	swapStr(list + a, list + size - 1);
	sortComponentes(list, a++);
	sortComponentes(list + a, size - a);
}

void listCommand(Arvore* ordenada, char* str){
	int i = 0, j = 0;
	char *token, *acumulator, aux[MAXCMDSIZE];
	Caminho* caminhoAux;
	char** lista = (char**) malloc(nCaminhos * sizeof(char*));
	if(str == NULL){ /*se list for chamado sem argumentos*/
		while(i < nCaminhos){ /*printa todos os componentes da raiz*/
			caminhoAux = get_caminho(ordenada, i);
			if(caminhoAux == NULL){
				break;
			}
			/*printf("%s\n", get_cam(caminhoAux));*/
			lista[i] = get_cam(caminhoAux) + 1;
			i++;
		}
		sortComponentes(lista, i);
	}else{
		strcpy(aux, str);
		printf("%s\n", str);
		acumulator = (char*) malloc(sizeof(char));
        	token = strtok(str, DELIM);
		while(token != NULL){
			acumulator = (char*) realloc(acumulator, strlen(acumulator) + strlen(token) + 2);
                	strcat(acumulator, DELIM);
                	strcat(acumulator, token);
			caminhoAux = get_caminho_ordenado(ordenada, acumulator, nCaminhos);
			if(caminhoAux == NULL || get_subcaminhos(caminhoAux) == NULL){
				printf("%s\n", ERROR_NOTFOUND);
				return;
			}
			ordenada = get_subcaminhos(caminhoAux);
			token = strtok(NULL, DELIM);
		}
		i = 0;
		while(i < nCaminhos){
			caminhoAux = get_caminho(ordenada, i);
                        if(caminhoAux == NULL){
                                break;
                        }
                        /*printf("%s\n", get_cam(caminhoAux) + strlen(str) + 1);*/
			lista[i] = get_cam(caminhoAux) + strlen(aux) + 1;
			i++;
		}
		sortComponentes(lista, i);
	}
	for(; j < i; j++){
		printf("%s\n", lista[j]);
	}
	free(lista);
}

int searchCommand(Arvore* ordenada, char* val){
	int i = 0;/*algoritmo de travessia igual ao do print*/
        if(get_caminho(ordenada, 0) == NULL){
                return searchAux;
        }
        for(; i < nCaminhos; i++){
		/*se for um caminho nao nulo com valor nao nulo*/
                if(get_caminho(ordenada, i) != NULL && get_val(get_caminho(ordenada, i)) != NULL){
			if(strcmp(get_val(get_caminho(ordenada, i)), val) == 0){
				if(searchAux == FORA){
					searchAux = DENTRO;
					printf("%s\n", get_cam(get_caminho(ordenada, i)));
				}
				return searchAux;
			}
                }
                /*chamada recursiva*/
                if(get_caminho(ordenada, i) != NULL){
                        searchCommand(get_subcaminhos(get_caminho(ordenada, i)), val);
                }
        }
	return searchAux;
}

void deleteFunc(Caminho* caminho){
	int i;
	free(get_val(caminho));
	free(get_cam(caminho));
	if(get_subcaminhos(caminho) == NULL){
		free(caminho);
		return;
	}
	for(i = 0; i < nCaminhos; i++){ /*percorremos os subcaminhos de caminho*/
		if(get_caminho(get_subcaminhos(caminho), i) == NULL){
			break;
		}
		deleteFunc(get_caminho(get_subcaminhos(caminho), i));
	}
	deleteTree(get_subcaminhos(caminho), HASHPRIME);
}

void deleteCommand(Arvore* ordenado, char* caminho){
	int i;
	Caminho* caminhoAux;
	char *token, *acumulator;
	if(caminho == NULL){
		for(i = 0; i < nCaminhos; i++){
			if(get_caminho(ordenado, i) == NULL){
				return;
			}
			deleteFunc(get_caminho(ordenado, i));
		}
		free(ordenado);
	}else{
		acumulator = (char*) malloc(sizeof(char));
		token = strtok(caminho, DELIM);
		while(token != NULL){
			acumulator = (char*) realloc(acumulator, strlen(acumulator) + strlen(token) + 2);
			strcat(acumulator, DELIM);
			strcat(acumulator, token);
			caminhoAux = get_caminho_ordenado(ordenado, acumulator, nCaminhos);
			if(caminhoAux == NULL){
				printf("%s\n", ERROR_NOTFOUND);
				return;
			}
			ordenado = get_subcaminhos(caminhoAux);
			token = strtok(NULL, DELIM);
		}
		deleteFunc(caminhoAux);
	}
}

int main(){
	char cmd[6];
	char *str, *valor, c;
	Arvore *ordenada;
	scanf("%s", cmd);
	ordenada = initArvore(HASHPRIME);
	while(strcmp(cmd, QUIT) != 0){
		if(strcmp(cmd, HELP) == 0){
			helpCommand();
		}else if(strcmp(cmd, SET) == 0){
			str = leCaminho();
			str = simplificaCaminho(str);
			valor = leValor();
			setCommand(ordenada, str, valor);
		}else if(strcmp(cmd, PRINT) == 0){
			printCommand(ordenada);
		}else if(strcmp(cmd, FIND) == 0){
			str = leCaminho();
                        str = simplificaCaminho(str);
			findCommand(ordenada, str);
		}else if(strcmp(cmd, LIST) == 0){
			str = leCaminho();
			if(str != NULL){
				str = simplificaCaminho(str);
			}
			listCommand(ordenada, str);
		}else if(strcmp(cmd, SEARCH) == 0){
			valor = leValor();
			searchAux = FORA;
			if(searchCommand(ordenada, valor) == FORA){
				printf("%s\n", ERROR_NOTFOUND);
			}
		}else if(strcmp(cmd, DELETE) == 0){
			if((c = getchar()) == '\n'){
				str = NULL;
			}else{
				str = leCaminho();
				str = simplificaCaminho(str);
			}
			deleteCommand(ordenada, str);
			if(c == '\n'){
				ordenada = initArvore(HASHPRIME);
			}
		}
		scanf("%s", cmd);
	}
	for(nCaminhos = 0; get_caminho(ordenada, nCaminhos) != NULL; nCaminhos++){
		deleteFunc(get_caminho(ordenada, nCaminhos));
	}
	for(; nCaminhos < HASHPRIME; nCaminhos++){
		free(get_caminho(ordenada, nCaminhos));
	}
	free(ordenada);
	return 0;
}
