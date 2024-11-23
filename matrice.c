#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

int** creerMatrice(int n);
void detruire(int**matrice,int n);
void sauvegardeFichierParDefaut (char * fichier ,int** matrice,int n );
void lireFichierBinaireAndCharger(char* fichier);
void sauvegardeFichierText (char * fichier ,int** matrice,int n );




int main(int argc ,char* argv[])
{

	if(argc<5 || argc>8){
		printf("Arguments insuffisant\n");
		exit(-1);
	}
	int opt, n,i=0,j=0,k=0,l=0;
	int**mat;
	char file[256];
	while((opt = getopt(argc,argv,":cd:btaf:"))!=-1){
		switch(opt){
			case 'c' :
				printf("Creation d'une matrice aleatoire \n");
				l++;
				break;
			case 'd' :
				n = atoi(optarg);
				break;
			case 'f' : 
				strcpy(file,optarg);
				break;
			case 'b':
				k++;
				break;
			case 't':
				i++;
				break;	
			case 'a':
				j++;
				break;
			case ':':
				printf("Erreur:option %c attent un argument\n",optopt);
				break;
			case '?':
				printf("Erreur : option %c inconnu \n",optopt);
				break;	
				
		}
	}

	if(l==1){
	
  		mat =creerMatrice(n);
		if(i==1){
		  sauvegardeFichierText(file,mat,n);
			if(j==1)
			   lireFichierBinaireAndCharger(file);
			
		}
		else{
			
		   sauvegardeFichierParDefaut(file,mat,n);
		    if(j==1)
		    {
		   	lireFichierBinaireAndCharger(file);
		    }
			
		}
	}
	else{
		if(i==1){
			if(j==1)
			   lireFichierBinaireAndCharger(file);
			else{
				printf("Erreur : \n");
				exit(-1);				
			}
			
		}
		else{
			
		    if(j==1)
		    {
		   	lireFichierBinaireAndCharger(file);
		    }
		    else{
			printf("Erreur : \n");
			exit(-1);				
			}
			
			
		}
	}
	detruire(mat,n);
	return 0;
}




int** creerMatrice(int n)
{

	 srand(time(NULL));

	int** matrice=(int**)malloc(sizeof(int*)*n);
	if (!matrice)
		exit(-1);
	for (int i = 0; i < n ; i++)
	{
		matrice[i] = (int*)malloc (sizeof(int)*n);
		if(!matrice[i])
		{
			exit(-1);
		}
	}

	for(int i=0;i<n;i++)
	{
	    
	    for(int j=0;j<n;j++)
	    {
		
		matrice[i][j]=rand()%100;

	    }
	}
	

	return matrice;
}

void sauvegardeFichierParDefaut (char * fichier ,int** matrice,int n){
	int fd= open (fichier,O_WRONLY | O_CREAT,
		S_IRUSR | S_IWUSR|
		S_IRGRP|
		S_IROTH
		);
	if(fd == -1)
	{
	    perror("Erreurde creation du fichier\n ");
	    return;        
	}	

	
	printf("Fichier %s crée avec succès \n",fichier);
	int nbWrite = 0;
	for(int i=0;i<n;i++){
	    for(int j=0;j<n;j++){
		nbWrite  = write(fd,&matrice[i][j],sizeof(float));
		
		if(nbWrite <=0){
			printf("Erreur d'ecriture \n");
			return ;
		}
	    }
	}
	close(fd);
}


void lireFichierBinaireAndCharger(char* fichier)
{
	int fd =open(fichier, O_RDONLY);
	if( fd == -1){
	   perror("Erreur d'ouverture du Fichier \n");
	   return;
        }
	int buff;  
	int nbRead = 0;
	int alaligne = 0; 
	do{
	   nbRead = read (fd,&buff,sizeof(int));
	   if(nbRead > 0){
		printf("%3d ",buff);
		alaligne++;
		if(alaligne == 10){
			printf("\n");
			alaligne = 0;
		}
	   }
		
		
	}while (nbRead >0);
	printf("\n");
	close(fd);
}


void sauvegardeFichierText (char * fichier ,int** matrice,int n ){
	int fd= open (fichier,O_WRONLY | O_CREAT,
		S_IRUSR | S_IWUSR|
		S_IRGRP|
		S_IROTH
		);
	if(fd == -1)
	{
	    perror("Erreurde creation du fichier\n ");
	    return;        
	}

	

	// ajouter les donner dans le fichier
	int nbwrite = 0;
	char textBuff[255];// buffer de texte 

	for(int i=0;i<n;i++){
	    for(int j=0;j<n;j++){
		  sprintf(textBuff,"%d",matrice[i][j]);
		
		nbwrite = write(fd,textBuff,strlen (textBuff));
		
		if(nbwrite <=0){
			printf("Erreur d'ecriture \n");
			return ;
		}
	    }
	}
	

	close(fd);	

		

}
void detruire(int**matrice,int n){
	if(matrice){
		for(int i= 0;i<n;i++){
			free(matrice[i]);
		}
		free(matrice);
	}
}


