/*Autores:
 Juan Carlos Elias Obando Valdivia nro USP: 7487156
 Lucas Fernandes de Nóbrega nro USP: 9805320
 João Pedro Doimo Torrezan nro USP: 9806933
 Enrico Vicenzo Salvador Robazza nro USP: 9806738
 */

#ifndef _REGISTRO_H
#define _REGISTRO_H

/*Estrutra que representa um registro com 4 campos(tamanho fixo).  */
typedef struct registros
{
	unsigned int iucr; //representa o primeiro campo(unsigned int): código iucr 
	char description[31]; //representa o segundo campo(30 bytes + 1 byte para o '\0'): descrição do crime
	char name[21]; //representa o terceiro campo(20 bytes + 1 byte para o '\0'): nome do criminoso
	char date[11]; //representa o quarto campo(10 bytes + 1 byte para o '\0'): data do crime
} registro;

#endif