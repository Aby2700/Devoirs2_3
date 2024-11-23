
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>



void split(char *fichier,int nombrefichier);
 void join(char* newfichier,int n,char *fichier[]);



int main(int argc ,char* argv[])
{
	
	if(argc=3)
	{
		int taille = atoi(argv[2]);
		split(argv[1],taille);
	
	}
	if(argc>3){
		int n=argc-2;
		join(argv[1],n,&argv[2]);
	}
	if(argc<3){
		printf("erreur:\n");
		printf("revoyez vos arguments please\n");
		exit(-1);
	}
	return 0;
}




	

void split(char *fichier, int nombrefichier) {	
    int fd = open(fichier, O_RDONLY);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier\n");
        return;
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0) {
        perror("Erreur : Impossible de lire les informations du fichier\n");
        close(fd);
        return;
    }
	
    long taillefichier = file_stat.st_size;
    int taille_part = taillefichier / nombrefichier;
    int reste = taillefichier % nombrefichier;
	int n;
    for (int i = 0; i <  nombrefichier; i++) {
        char resultfile[30];
        snprintf(resultfile, sizeof(resultfile), "F%d%s", i + 1,fichier);
        int fd_result = open(resultfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

        if (fd_result == -1) {
            perror("Erreur : Impossible de créer le fichier de sortie\n");
            close(fd);
            return;
        }
	

	if(i==nombrefichier-1 && reste >0){
		n=taille_part +reste;
	}
	else{
		n = taille_part;
	}
        char *buffer = malloc(n);
        if (!buffer) {
            perror("Erreur : Allocation mémoire échouée\n");
            close(fd_result);
            close(fd);
            return;
        }

       int nbRead = read(fd, buffer,n);
	if(nbRead==-1){
		printf("Erreur de lecture du fichier initiale \n");
		close(fd_result);
            	close(fd);
		free(buffer);
            	return;
	}

	if (nbRead < n) {
   		 printf("Avertissement : La lecture a renvoyé moins d'octets que prévu.\n");
	}

        int nbWrite=write(fd_result, buffer, n);

	if(nbWrite==-1){
		printf("Erreur d'ecrture  dans le   fichier %d \n",i+1);
		close(fd_result);
            	close(fd);
		free(buffer);
            	return;
	}

        free(buffer);
        close(fd_result);
   }

}
 void join(char* newfichier,int n,char *fichier[]){
	long taillefichier ;
	char *buffer=NULL; 
	int nbRead,nbWrite;
	int fdDst= open(newfichier,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	
	if(fdDst==-1){
		printf("Erreur de creation du fichier join \n");
		exit(-1);
	
	}

	for(int i=0;i<n;i++){
		int fdScr=open(fichier[i],O_RDONLY);
		if(fdScr == -1){
			printf("Erreur d'ouverture du fichier join \n");
			exit(-1);
		}
		 struct stat file_stat;
	 	 if (fstat(fdScr, &file_stat) < 0) {
			perror("Erreur : Impossible de lire les informations du fichier\n");
			close(fdScr);
			return  	;
	   	 }
	
    		 taillefichier = file_stat.st_size;
		buffer = malloc (taillefichier);
		nbRead = read(fdScr,buffer,taillefichier);
		if(nbRead==-1){
			printf("Erreur de lecture du ficher\n");
			close(fdScr);
			close(fdDst);
			free(buffer);
			exit(-1);
		}
		nbWrite= write (fdDst,buffer,taillefichier);
		if(nbWrite==-1){
			printf("Erreur de lecture du ficher\n");
			close(fdScr);
			close(fdDst);
			free(buffer);
			exit(-1);
		}
		free(buffer);
		close(fdScr);
	}
	close(fdDst);
}


