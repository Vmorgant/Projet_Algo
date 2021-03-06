#include<stdio.h>
#include<stdlib.h> 
#include <unistd.h>
#include <string.h>
#include "../includes/global.h"
#include "../includes/ges_equipes.h"
#include "../includes/listes_ptr.h"
#include "../includes/Init_map.h"
#include "../includes/ges_partie.h"
#include <dirent.h>

/**
*\file save.c
*\brief Ce fichier contient les définitions des fonctions permettant de sauvegarder et charger une partie
*\author Corentin Petit
*\version 1.0
*\date 05/12/2016
*/

int charger_save( char  nomsave[34], t_liste * ordre_action, int * Nb_tours) {
/**
 * \fn  charger_save( char  nomsave[34], t_liste * ordre_action, int * Nb_tours) 
 * \brief Charge les données d'une sauvegarde passée en paramètre  et retourne vrai si erreur
 * \param char nomsave[10] : le ,nom de la sauvegarde, t_liste * ordre_action : la liste triés des personnages par ordre de jeu,int Nb_tours : le nombre de tours actuel de la sauvegarde
 */
	int i;
	FILE * fic = NULL;
	char  dirsave[100];
	t_personnage persoc;
	t_save save;

	sprintf(dirsave, "./Saves/%s", nomsave);
	/*ouverture de la sauvegarde */
	fic = fopen( dirsave,"r" );

	if( fic != NULL ) {
		/*lecture des données de la sauvegarde */
		fread(&save, sizeof(save) , 1 , fic);

		/*récupération des données de la sauvegarde */
		vider(ordre_action);
		for(i = 0; i < save.nbpersos; i++){
			en_queue(ordre_action);
			ajout_droit(ordre_action, save.tab_action[i]);
		}
		*Nb_tours = save.nbtours;

		en_tete(ordre_action);
		valeur_elt(ordre_action, &persoc);
		while( (persoc.x != save.persoc.x) || (persoc.y != save.persoc.y) ){
			suivant(ordre_action);
			valeur_elt(ordre_action, &persoc);
		}

		fclose(fic);
		return faux;
	} else return vrai;
}

int enregistrer_save( char  nomsave[34], t_liste * ordre_action, int Nb_tours) {
/**
 * \fn  enregistrer_save( char  nomsave[34], t_liste * ordre_action, int Nb_tours))
 * \brief Sauvegarde les données d'une parite dans un fichier dont le nom passée en paramètre
 * \param char nomsave[10] : le ,nom de la sauvegarde, t_liste * ordre_action : la liste triés des personnages par ordre de jeu,int Nb_tours: le nombre de tours actuel de la sauvegarde
 */
	FILE * fic = NULL;
	char  dirsave[100];
	int nbpersos, i;
	t_personnage persoc;
	t_save save;

	sprintf(dirsave, "./Saves/%s", nomsave);
	/*ouverture de la sauvegarde */
	fic = fopen( dirsave,"w" );

	if( fic != NULL ) {
		/*enregistrement des données de la sauvegarde */
		valeur_elt(ordre_action, &persoc);
		save.persoc = persoc;

		compter_elts(ordre_action, &nbpersos);
		t_personnage tab_action[nbpersos];

		en_tete(ordre_action);

		for(i = 0; i < nbpersos; i++) {
			valeur_elt(ordre_action, &persoc);
			save.tab_action[i] = persoc;
			suivant(ordre_action);
		}
		save.nbpersos = nbpersos;
		save.nbtours = Nb_tours;

		/*écriture des données de la sauvegarde */
		fwrite(&save , sizeof(save) , 1 , fic);

		fclose(fic);
		return faux;
	} else return vrai;
}

void charger_partie(char mbilan[100]) {
/**
 * \fn charger_partie()
 * \brief Demande au joueur s’il souhaite vraiment charger une sauvegarde puis lui fait choisir celle qu’il souhaite. 
 * \param mbilan[100] permet de stocker le message de fin.
 */

	int choix = -1, nb_saves, i, erreur = faux;
	int Nb_tours=1;
	int gagnant = 0; 
	char mretour[100] = "\n";
	char dirsave[34];
	char chaine[30];
	char* fin = NULL;

	t_liste equipe1;
	t_liste equipe2;
	t_liste ordre_action;

	init_liste(&ordre_action);

	t_map carte=creerMat();
	
	FILE * fic = NULL;
	
	struct dirent * ent;

	do {

		clearScreen();

		nb_saves = 0;
		i =0;

		printf("Choisissez une sauvegarde : \n");
		if(erreur) {
			couleur("31");
			printf("%s",mretour);
			couleur("0");
		} else {
			couleur("32");
			printf("%s",mretour);
			couleur("0");
		}
		erreur = faux;
		strcpy(mretour,"\n");

		DIR * rep = opendir("./Saves/");
	     
		if (rep != NULL) {
			while ((ent = readdir(rep)) != NULL) {
				if (strcmp(ent->d_name, ".") != 0 && /* Si le fichier lu n'est pas . */
		 		strcmp(ent->d_name, "..") != 0) { /*  Et n'est pas .. non plus */
					nb_saves++;
				}
			}

			char tab_saves[nb_saves][34];
			rewinddir(rep);

			while ((ent = readdir(rep)) != NULL) {
				if (strcmp(ent->d_name, ".") != 0 && /* Si le fichier lu n'est pas . */
		 		strcmp(ent->d_name, "..") != 0) { /*  Et n'est pas .. non plus */
					i++;
					printf(" %i- %s\n", i, ent->d_name);
					strcpy(tab_saves[i],ent->d_name);
				}
			}

			closedir(rep);

			printf("\n %i- Annuler\n", nb_saves+1);
			printf("\nVotre choix : ");

			scanclav(chaine, 30);
			choix = strtol(chaine, &fin, 10);

			printf("\n%i\n",choix);

			/*Traitement du choix de l'utilisateur */
			if(choix > 0 && choix < nb_saves+1) {

				erreur = charger_save(tab_saves[choix], &ordre_action, &Nb_tours);

				if(erreur) {
					strcpy(mretour, "\tCette sauvegarde n'existe pas.\n");
				} else {
					carte=actumap(&ordre_action, carte);
					afficherMat(carte);
					while (gagnant == 0){
						gestion_tour(&ordre_action, &Nb_tours, &carte, &gagnant);
					}
					choix = nb_saves+1;
				}

			} else if (choix > nb_saves+1 || choix < 1) {
				sprintf(mretour, "\tVotre choix doit être compris entre  1 et %i.\n", nb_saves+1);
				erreur = vrai;
			}
		} else {
			strcpy(mretour, "\tLe dossier de sauvegardes n'existe pas.\n");
			erreur = vrai;
			break;
		}
	} while(choix != nb_saves+1);

	supprimer(&ordre_action);
	free_map(carte);
}

void nouvelle_partie(char mbilan[100]) {
/**
 * \fn charger_partie()
 * \brief Initialisation des structures et libérations de la mémoire en fin de partie.
 * \param mbilan[100] permet de stocker le message de fin.
 */
	t_liste equipe1;
	t_liste equipe2;
	t_liste ordre_action;
	t_map carte=creerMat();

	int choix, erreur = faux;
	int NbTour=1;
	int gagnant=0; 
	int PE1 = 10;
	int PE2 = 10;
	char mretour[100] = "\n";

	char chaine[30];
	char* fin = NULL;

	/* Initialisation des listes de personnages */
	init_liste(&equipe1);
	init_liste(&equipe2);
	init_liste(&ordre_action);
	
	/* Affichage du menu et saisie du choix */
	do  {
		gagnant=0;
		clearScreen();
		printf("Menu :\n");
		if(erreur) {
			couleur("31");
			printf("%s", mretour);
			couleur("0");
		}else printf("\n");
		erreur = faux;
		printf(" 1- Editer equipe 1.\t\t");
		if(!liste_vide(&equipe1)) {
			printf("L'equipe 1 est constituée de : ");
			couleur("34;1");
			afficher(&equipe1);
			couleur("0");
			printf("(%iPE restant)\n", PE1);
		}else {
			printf("L'equipe 1 est vide.");		
			printf("(%iPE restant)\n", PE1);
		}

		printf(" 2- Editer equipe 2.\t\t");
		if(!liste_vide(&equipe2)) {
			printf("L'equipe 2 est constituée de : ");
			couleur("31;1");
			afficher(&equipe2);
			couleur("0");
			printf("(%iPE restant)\n", PE2);
		}else {
			printf("L'equipe 2 est vide.");		
			printf("(%iPE restant)\n", PE2);
		}

		printf(" 3- Lancer Partie.\n");
		printf("\n 4- Retour.\n");
		printf("\nVotre choix : ");
		scanclav(chaine, 30);
		choix = strtol(chaine, &fin, 10);

		/* Traitement du choix de l'utilisateur */
		switch(choix) {
			case 1: init_equipe(&equipe1, 1, &PE1); break;
			case 2: init_equipe(&equipe2, 2, &PE2);  break;
			case 3: if(liste_vide(&equipe1) || liste_vide(&equipe2) ){
					strcpy(mretour, "\tLes deux equipes ne doivent pas etre vides\n");
					erreur = vrai;
					break;
				}
				else{			
					init_partie(&equipe1,&equipe2,&ordre_action);
					placer(&ordre_action, &carte);
					carte=actumap(&ordre_action, carte);
					afficherMat(carte);
					en_tete(&ordre_action);
					while (gagnant == 0){
						gestion_tour(&ordre_action,&NbTour, &carte,&gagnant);
					}
					if(gagnant == -1) sprintf(mbilan, "\tLa partie bien a été enregistrée.\n");
					else if(gagnant != -2) sprintf(mbilan, "\tLe joueur %i a gagné en %i tours\n", gagnant, NbTour);
					choix = 4;
					break;
				}
			default: strcpy(mretour, "\tVotre choix doit être compris entre 1 et 4\n"); erreur = vrai;
		}
	}while(choix!=4);
	
	supprimer(&equipe1); /* comme ordre_action = equipe 1, pas besoin de free ordre_action */
	supprimer(&equipe2);
	free_map(carte);
}

void quitter_partie(t_liste * ordre_action, int Nb_tours, int *gagnant) {
/**
 * \fn  quitter_partie(t_liste * ordre_action, int Nb_tours)
 * \brief Demande au joueur si il souhaite vraiment quittez et si il souhaite sauvegarder la partie en cours.
 * \param t_liste * ordre_action : la liste triés des personnages par ordre de jeu, int Nb_tours, le nombre de tours actuel de la sauvegarde, int *gagnant permet de stocker l'état de la partie.
 */

	int choix = -1, erreur= faux, nb_saves;
	char mretour[100] = "\n", new_save[34];
	int i =0;

	char chaine[30];
	char* fin = NULL;

	FILE * fic = NULL;
	struct dirent * ent;

	do{
		clearScreen();
		printf("Voulez-vous sauvegarder la partie avant de quitter ?\n");
		if(erreur) {
			couleur("31");
			printf("%s", mretour);
			couleur("0");
		}else printf("\n");
		erreur = faux;

		printf(" 1- Sauvegarder et quitter.\n");
		printf(" 2- Quitter sans sauvegarder.\n");
		printf("\n 3- Annuler.\n"); 
		printf("\nVotre choix : ");
		scanclav(chaine, 30);
		choix = strtol(chaine, &fin, 10);

		/* Traitement du choix de l'utilisateur */
		switch(choix) {
			case 1:
				do {
					clearScreen();

					nb_saves = 0;
					i =0;

					printf("Choisissez une sauvegarde : \n");
					if(erreur) {
						couleur("31");
						printf("%s",mretour);
						couleur("0");
					} else {
						couleur("32");
						printf("%s",mretour);
						couleur("0");
					}
					erreur = faux;
					strcpy(mretour,"\n");
					DIR * rep = opendir("./Saves/");
				     
					if (rep != NULL) {
						while ((ent = readdir(rep)) != NULL) {
							if (strcmp(ent->d_name, ".") != 0 && /* Si le fichier lu n'est pas . */
					 		strcmp(ent->d_name, "..") != 0) { /*  Et n'est pas .. non plus */
								nb_saves++;
							}
						}

						char tab_saves[nb_saves][34];
						rewinddir(rep);
						while ((ent = readdir(rep)) != NULL) {
							if (strcmp(ent->d_name, ".") != 0 && /* Si le fichier lu n'est pas . */
					 		strcmp(ent->d_name, "..") != 0) { /*  Et n'est pas .. non plus */
								strcpy(tab_saves[i],ent->d_name);
								i++;
								printf(" %i- %s\n", i, ent->d_name);
							}
						}
						closedir(rep);
						
						printf(" %i- Nouvelle Sauvegarde.\n", nb_saves+1);
						printf("\n %i- Annuler\n", nb_saves+2);
						printf("\nVotre choix : ");

						scanclav(chaine, 30);
						choix = strtol(chaine, &fin, 10);

						/*Traitement du choix de l'utilisateur */
						if(choix > 0 && choix < nb_saves+1) {
							erreur = enregistrer_save(tab_saves[choix-1], ordre_action, Nb_tours);
							if(erreur) {
								strcpy(mretour, "\tCette sauvegarde n'existe pas.\n");
							} else {
								*gagnant = -1;
								choix = nb_saves+2;
							}

						} else if (choix > nb_saves+2 || choix < 1) {

							sprintf(mretour, "\tVotre choix doit être compris entre  1 et %i.\n", nb_saves+2);
							erreur = vrai;

						} else if (choix == nb_saves+1) {

							clearScreen();
							printf("Entrez le nom de votre sauvegarde (30 caractères max) :\n\t");
							scanclav(new_save, 30);
							strcat(new_save,".bin");

							erreur = enregistrer_save(new_save, ordre_action, Nb_tours);

							if(erreur) {
								strcpy(mretour, "\tLa sauvegarde n'a pas pu être créée.\n");
							} else {
								printf("%i\n" , *gagnant);
								*gagnant = -1;
								choix = nb_saves+2;
							}
						}

					} else {
						strcpy(mretour, "\tLe dossier de sauvegardes n'existe pas.\n");
						erreur = vrai;
						break;
					}
				} while(choix != nb_saves+2);
				choix = 3;
				break;
			case 2:  *gagnant = -2; choix = 3; break;
			case 3:  break;
			default: strcpy(mretour, "\tVotre choix doit être compris entre 1 et 3\n"); erreur = vrai;
		}
	} while (choix != 3);
}

void gerer_save(char mbilan[100]) {
/**
 * \fn gerer_save(char mbilan[100])
 * \brief Suppression ou renommage des sauvegardes.
 * \param mbilan[100] permet de stocker le message de fin.
 */
	int choix = -1, erreur= faux, nb_saves;
	char mretour[100] = "\n", new_name[34], new_dir[34];
	char  dirsave[100];
	int i =0;
	char chaine[30];
	char* fin = NULL;

	FILE * fic = NULL;
	struct dirent * ent;

	do{
		clearScreen();
		/* Traitement du choix de l'utilisateur */
		printf("Gestion des sauvegardes :\n");
		printf("Choisissez une sauvegarde : \n");

		nb_saves = 0;
		i = 0;

		if(erreur) {
			couleur("31");
			printf("%s",mretour);
			couleur("0");
		} else {
			couleur("32");
			printf("%s",mretour);
			couleur("0");
		}
		erreur = faux;
		strcpy(mretour,"\n");
			DIR * rep = opendir("./Saves/");
		     
		if (rep != NULL) {
			while ((ent = readdir(rep)) != NULL) {
				if (strcmp(ent->d_name, ".") != 0 && /* Si le fichier lu n'est pas . */
		 		strcmp(ent->d_name, "..") != 0) { /*  Et n'est pas .. non plus */
					nb_saves++;
				}
			}

			char tab_saves[nb_saves][34];
			rewinddir(rep);

			while ((ent = readdir(rep)) != NULL) {
				if (strcmp(ent->d_name, ".") != 0 && /* Si le fichier lu n'est pas . */
		 		strcmp(ent->d_name, "..") != 0) { /*  Et n'est pas .. non plus */
					i++;
					printf(" %i- %s\n", i, ent->d_name);
					strcpy(tab_saves[i],ent->d_name);
				}
			}
			printf("\n %i- Annuler\n", nb_saves+1);
			printf("\nVotre choix : ");
			scanclav(chaine, 30);
			choix = strtol(chaine, &fin, 10);

			if(choix > 0 && choix < nb_saves +1) {
				
				sprintf(dirsave, "./Saves/%s", tab_saves[choix]);
					
				if( (fic = fopen(dirsave, "r")) == NULL) {
					strcpy(mretour, "\tCette sauvegarde n'existe pas.\n");
				} else {
				
					do {
						clearScreen();
						/* Traitement du choix de l'utilisateur */
						printf("Gestion des sauvegardes :\n");
						printf("Que souhaitez vous faire ?\n");


						if(erreur) {
							couleur("31");
							printf("%s",mretour);
							couleur("0");
						} else {
							couleur("32");
							printf("%s",mretour);
							couleur("0");
						}
						erreur = faux;
						strcpy(mretour,"\n");				

						printf(" 1- Renommer sauvegarde.\n");
						printf(" 2- Supprimer sauvegarde.\n");
						printf(" 3- Retour.\n");
						
						printf("\nVotre choix : ");
						scanclav(chaine, 30);
						choix = strtol(chaine, &fin, 10);
							
						switch(choix) {
							case 1:
								clearScreen();
								printf("Entrez le nom de votre sauvegarde (30 caractères max) :\n\t");

								scanclav(new_name, 30);
							
								strcat(new_name,".bin");
								sprintf(new_dir, "./Saves/%s", new_name);

								printf("%s\n", new_dir);

								rename(dirsave, new_dir);
								choix = 3;
								break;
							case 2: remove(dirsave); choix = 3; break;
							case 3:break;
							default : strcpy(mretour, "\tVotre choix doit être compris entre  1 et 3.\n"); erreur = vrai;
						}
					} while (choix != 3);
					fclose(fic);
					closedir(rep);
					choix = -1;
				}
			} else if (choix > nb_saves+1 || choix < 1) {
				sprintf(mretour, "\tVotre choix doit être compris entre  1 et %i.\n", nb_saves+2);
				erreur = vrai;
			}
		} else {
			strcpy(mretour, "\tLe dossier de sauvegardes n'existe pas.\n");
			erreur = vrai;
			break;
		}
	} while(choix != nb_saves+1);
}