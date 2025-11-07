#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<ctype.h>
typedef struct{
	int d,m,a;
}DATA;
typedef struct{
	char nome[60],fone[15],email[40];
	DATA aniv;
}CONTATO;
int menu()
{
	int opcao;
	printf("\n1- Inserir: ");
	printf("\n2- Exibir: ");
	printf("\n3- Alterar: ");
	printf("\n4- Excluir: ");
	printf("\n5- Sair: ");
	printf("\nInforme a opcao: ");
	scanf("%d",&opcao);
	return opcao;
}
int busca(FILE *arquivo,char nome[])
{
	CONTATO ctt;
	rewind(arquivo);
	fread(&ctt,sizeof(CONTATO),1,arquivo);
	while(!feof(arquivo)&&stricmp(nome,ctt.nome)!=0)
	{
		fread(&ctt,sizeof(CONTATO),1,arquivo);
	}
	if(!feof(arquivo))
	  return(ftell(arquivo)-sizeof(CONTATO));
	else
	 return -1;  
}
void inserir()
{
	FILE *arquivo;
	CONTATO ctt;
	int pos;
	arquivo=fopen("agenda.bin","ab+");
	if(arquivo==NULL)
      printf("Erro no Arquivo");
	else
	{
		do{
			system("cls");
			printf("\nNome: "); fflush(stdin);
			gets(ctt.nome);
			pos=busca(arquivo,ctt.nome);
			if(pos==-1)
			{
				printf("\nTelefone: "); fflush(stdin);
				gets(ctt.fone);
				printf("\nE-mail: "); fflush(stdin);
				gets(ctt.email);
				printf("\nAniversario dd mm aaaa: ");
				scanf("%d%d%d",&ctt.aniv.d,&ctt.aniv.m,&ctt.aniv.a);
				fwrite(&ctt,sizeof(CONTATO),1,arquivo);
			}
			else{
				fseek(arquivo,pos,0);
				fread(&ctt,sizeof(CONTATO),1,arquivo);
				printf("\n%sO contato ja esta cadastrado",ctt.nome);
			}
				printf("\nDeseja continuar S/N? ");
		}while(toupper(getche())=='S');
		fclose(arquivo);
	}  	
}
void exibir()
{
	FILE *arquivo;
	CONTATO ctt;
	arquivo=fopen("agenda.bin","rb");
	if(arquivo==NULL)
	  printf("Erro no arquivo");
	else
	{
		system("cls");
		while(fread(&ctt, sizeof(CONTATO),1,arquivo)==1)
		{
			printf("\nNome: %s",ctt.nome);
			printf("\nTelefone: %s",ctt.fone);
			printf("\nE-mail: %s",ctt.email);
			printf("\nAniversario: %d/%d/%d",ctt.aniv.d,ctt.aniv.m,ctt.aniv.a);
			printf("\n_______________\n");
		}
		fclose(arquivo);
	}  
	system("pause");
	system("cls");
}
void alterar()
{
	FILE *arquivo;
	CONTATO ctt;
	int pos,op;
	arquivo=fopen("agenda.bin","rb+");
	if(arquivo==NULL)
		printf("Erro no arquivo");
	else
	{
		printf("Alterar os dados do nome: "); fflush(stdin);
		gets(ctt.nome);
		while(stricmp(ctt.nome,"\0")!=0)//strIcmp o I faz a letra poder ser maiuscula ou minuscula
		{
			pos=busca(arquivo,ctt.nome);
			if(pos==-1)
				printf("Contato nao encontrado\n");
			else
			{
				fseek(arquivo,pos,0);
				fread(&ctt,sizeof(CONTATO),1,arquivo);
				printf("\n-------------------------------");
				printf("\nNome : %s",ctt.nome);
				printf("\nTelefone: %s",ctt.fone);
				printf("\nEmail: %s",ctt.email);
				printf("\nO que deseja Alterar? \n 1-Telefone \n -2E-mail? ");
				scanf("%s",&op);
				if(op==1)
				{
					printf("\nNovo Telefone: "); fflush(stdin);
					gets(ctt.fone);
					fseek(arquivo,pos,0);
					fwrite(&ctt,sizeof(CONTATO),1,arquivo);
					system("pause");
					system("cls");
				}
				if(op==2)
				{
					printf("\nNovo Email: "); fflush(stdin);
					gets(ctt.email);
					fseek(arquivo,pos,0);
					fwrite(&ctt,sizeof(CONTATO),1,arquivo);
					system("pause");
					system("cls");
				}
			}
			printf("\nQual o nome para alterar: "); fflush(stdin);
			gets(ctt.nome);
		}
	}
	fclose(arquivo);
}
void excluir()
{
	FILE *arquivo;
	CONTATO ctt;
	int pos;
	char nome[30];
	arquivo=fopen("agenda.bin","rb");
	if(arquivo==NULL)
		printf("Erro no arquivo");
	else
	{
		printf("\nInformer o nome a excluir:"); fflush(stdin);
		gets(nome);
		if(stricmp(nome,"\0")!=0)
		{
			pos=busca(arquivo,nome);
			if(pos== -1)
				printf("\nRegistro não encontrado");
			else
			{
				fseek(arquivo,pos,0);
				fread(&ctt,sizeof(CONTATO),1,arquivo);
				printf("\nNome: %s",ctt.nome);
				printf("\nTelefone: %s",ctt.fone);
				printf("\nEmail: %s",ctt.email);
				printf("\nAniversario: %d/%d/%d",ctt.aniv.d,ctt.aniv.m,ctt.aniv.a);
				printf("\nDeseja Excluir S/N?");
				
				if(toupper(getche())=='S')
					{
						FILE *temp;
						temp=fopen("temp.bin","wb");
						if(temp==NULL)
							printf("Erro no Arquivo");
						else
						{
							rewind(arquivo);
							fread(&ctt,sizeof(CONTATO),1,arquivo);
							while(!feof(arquivo))
							{
								if(strcmp(nome,ctt.nome)!=0)
									fwrite(&ctt,sizeof(CONTATO),1,temp);
								fread(&ctt,sizeof(CONTATO),1,arquivo);
							}
						}
						fclose(arquivo);
						fclose(temp);
						remove("agenda.bin");
						rename("temp.bin","agenda.bin");
					}
			}
		}
	}
	fclose(arquivo);
}
main()
{
	int opc;
	do{
		opc=menu();
		switch(opc)
		{
			case 1:inserir();
			     break;
			case 2:exibir();     
			     break;
		    case 3:alterar();
			     break;
		    case 4:excluir();
			     break;
			case 5:printf("Encerrando a agenda...");
			     break;
			default:printf("Opcao invalida.");	 		 	     
		}
	}while(opc!=5);
}
