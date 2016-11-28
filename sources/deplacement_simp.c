/**
*\file deplacement_simp.c
*\brief Ce fichier contient une version simplifiée des fonctions de déplacement sur la carte.
*\author Martin Lebourdais
*\version 1.0
*\date 22/11/2016
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "global.h"
#include "liste_chemin.h"
#include "Init_map.h"

void permuter(t_element_noeud *actuel,t_element_noeud *suivant){
/**
 * \fn  permuter(t_element_noeud *actuel,t_element_noeud *suivant)
 * \brief Intervertit les structures des deux cases.
 * \param t_element_noeud *actuel : La case sur laquelle le personnage se trouve, t_element_noeud *suivant : La case ou l'on souhaite placer le personnage.
 */
	t_element_noeud tampon;
	tampon = *actuel;
	*actuel = *suivant;
	*suivant = tampon;
}

int existinf(t_liste_noeud liste,t_noeud valeur){
/**
 * \fn  existinf(t_liste_noeud liste,t_noeud valeur)
 * \brief Recherche s'il existe une structure noeud avec un cout inférieur au noeud en paramètre dans la liste en paramètre.
 * \param t_liste_noeud liste : La liste dans laquelle on souhaite chercher, t_noeud valeur : Le noeud que l'on souhaite tester 
 */
	en_tete_noeud(&liste);
	t_noeud vcour;
	while(!hors_liste_noeud(&liste)){
		valeur_elt_noeud(&liste,&vcour);
		if ((vcour.x==valeur.x) && (vcour.y==valeur.y) && (vcour.cout<valeur.cout)) return 1;/*On compare les coordonnées pour savoir si le noeud est le même*/
		suivant_noeud(&liste);
	}
	return 0;
}

float distance(int x,int y,int objx,int objy){
/**
 * \fn  distance(int x,int y,int objx,int objy)
 * \brief Calcule le carré de la distance entre le point de départ et l'objectif
 * \param int x : Abscisse de départ, int y : Ordonnée de départ, int xobj : Abscisse cible; int yobj : Ordonnée cible
 */
float distance = sqrt(((float)objx-(float)x)*((float)objx-(float)x)+((float)objy-(float)y)*((float)objy-(float)y));
	return distance;
}


void pathfinding(int x, int y,int objx,int objy,t_liste_noeud *openlist,t_map map){
/**
 * \fn  pathfinding(int x, int y,int objx,int objy)
 * \brief Trouve le chemin le plus court entre le point de départ et l'objectif et retourne ce chemin dans une liste.
 * \param int x : Abscisse de départ, int y : Ordonnée de départ, int xobj : Abscisse cible; int yobj : Ordonnée cible
 */

t_noeud depart = {x,y,0,0};
t_noeud tampon;
t_noeud v1 = {x+1,y,0,0};
t_noeud v2 = {x,y+1,0,0};
t_noeud v3 = {x-1,y,0,0};
t_noeud v4 = {x,y-1,0,0};
while (depart.x != objx || depart.y != objy){
	if (map.cell[v1.x][v1.y] == 0 && (distance(v1.x,v1.y,objx,objy) < distance(depart.x,depart.y,objx,objy))){	
			ajout_droit_noeud(openlist,v1);
			suivant_noeud(openlist);
			tampon = v1;
			if (v1.x != 9) {
				v1.x = tampon.x+1;
				v1.y = tampon.y;
			}
			if (v1.y != 9) {
				v2.x = tampon.x;
				v2.y = tampon.y+1;
			}
			if (v1.x != 0) {
				v3.x = tampon.x-1;
				v3.y = tampon.y;
			}
			if (v1.y != 0) {
				v4.x = tampon.x;
				v4.y = tampon.y-1;
			}
			depart = tampon;
			
			
	}
	else if (map.cell[v2.x][v2.y] == 0 && (distance(v2.x,v2.y,objx,objy) < distance(depart.x,depart.y,objx,objy))){	
			ajout_droit_noeud(openlist,v2);
			suivant_noeud(openlist);
			tampon = v2;
			if (v2.x != 9) {
				v1.x = tampon.x+1;
				v1.y = tampon.y;
			}
			if (v2.y != 9) {
				v2.x = tampon.x;
				v2.y = tampon.y+1;
			}
			if (v2.x != 0) {
				v3.x = tampon.x-1;
				v3.y = tampon.y;
			}
			if (v2.y != 0) {
				v4.x = tampon.x;
				v4.y = tampon.y-1;
			}
			depart = tampon;
			
	}
	else if (map.cell[v3.x][v3.y] == 0 && (distance(v3.x,v3.y,objx,objy)<distance(depart.x,depart.y,objx,objy))){	
			ajout_droit_noeud(openlist,v3);
			suivant_noeud(openlist);
			tampon = v3;
			if (v3.x != 9) {
				v1.x = tampon.x+1;
				v1.y = tampon.y;
			}
			if (v3.y != 9) {
				v2.x = tampon.x;
				v2.y = tampon.y+1;
			}
			if (v3.x != 0) {
				v3.x = tampon.x-1;
				v3.y = tampon.y;
			}
			if (v3.y != 0) {
				v4.x = tampon.x;
				v4.y = tampon.y-1;
			}
			depart = tampon;
			
	}
	else if (map.cell[v4.x][v4.y] == 0 && (distance(v4.x,v4.y,objx,objy)<distance(depart.x,depart.y,objx,objy))){	
			ajout_droit_noeud(openlist,v4);
			suivant_noeud(openlist);
			tampon = v4;
			if (v4.x != 9) {
				v1.x = tampon.x+1;
				v1.y = tampon.y;
			}
			if (v4.y != 9) {
				v2.x = tampon.x;
				v2.y = tampon.y+1;
			}
			if (v4.x != 0) {
				v3.x = tampon.x-1;
				v3.y = tampon.y;
			}
			if (v4.y != 0){
				v4.x = tampon.x;
				v4.y = tampon.y-1;
			}
			depart = tampon;
			
	}
}

}

int cout_dep(t_liste * ordre_action){
	int cout;
	cout = 0.1 * ordre_action->ec->personnage.classe.poids;
	return cout;
}

int nb_case(t_liste_noeud *liste){
	int i=0;
	
	en_tete_noeud(liste);
	
	while(!hors_liste_noeud(liste)){    
		i++;
		suivant_noeud(liste);
	} 
	return i;   
}

void deplacement_simp(t_liste *ordre_action,t_map map){
/**
 * \fn  deplacement_simp(t_liste *ordre_action,t_map map)
 * \brief Version simplifiée de la fonction déplacement.
 * \param t_liste *ordre_action : ordre de jeu des personnages (util pour connaître le personnage qui se déplace),t_map map : Plateau de jeu
 */
    int xobj;
    int yobj;
    int sortie;
    char choix;
    
    t_liste_noeud openlist;				
    t_liste_noeud closedlist;
    init_liste_noeud(&openlist);
    init_liste_noeud(&closedlist);
	printf("Rentrez un x pour le déplacement :\n");
	scanf("%i",&xobj);
	printf("Rentrez un y pour le déplacement :\n");
	scanf("%i",&yobj);
	printf("\n");
	pathfinding(ordre_action->ec->personnage.x,ordre_action->ec->personnage.y,xobj,xobj, &openlist,map);
	
	en_tete_noeud(&openlist);
	
	
	int cout = cout_dep(ordre_action) * nb_case(&openlist);
	printf("%i PA\n", cout);
	
	
			while (xobj > 9 || yobj > 9 || xobj < 0 || yobj < 0 || map.cell[xobj][yobj]!=0 || cout > ordre_action->ec->personnage.pa || sortie == 1){
				if (map.cell[xobj][yobj] != 0){															//On teste si la case est vide
						printf("La case est déjà occupée\n");
						printf("Rentrez un x pour le déplacement :\n");
						scanf("%i",&xobj);
						printf("Rentrez un y pour le déplacement :\n");
						scanf("%i",&yobj);
						printf("\n");
						pathfinding(ordre_action->ec->personnage.x,ordre_action->ec->personnage.y,xobj,xobj, &openlist,map);
						cout = cout_dep(ordre_action)*nb_case(&openlist);
					}
				if (xobj > 9 || yobj > 9 || xobj < 0 || yobj < 0){
						printf("Les coordonnées doivent-être des entiers compris entre 0 et 9\n");
						printf("Rentrez un x pour le déplacement :\n");
						scanf("%i",&xobj);
						printf("Rentrez un y pour le déplacement :\n");
						scanf("%i",&yobj);
						printf("\n");
						pathfinding(ordre_action->ec->personnage.x,ordre_action->ec->personnage.y,xobj,xobj, &openlist,map);
						cout = cout_dep(ordre_action)*nb_case(&openlist);
					}
				else{
						printf("Vous n'avez pas assez de PA\n");
						printf("Voulez vous toujours vous déplacer\n");
						printf("o/n : ");
						scanf("%c",&choix);
						printf("\n");
						switch (choix){
							case 'o':
								printf("Rentrez un x pour le déplacement :\n");
								scanf("%i",&xobj);
								printf("Rentrez un y pour le déplacement :\n");
								scanf("%i",&yobj);
								printf("\n");
								pathfinding(ordre_action->ec->personnage.x,ordre_action->ec->personnage.y,xobj,xobj, &openlist,map);	
								cout = cout_dep(ordre_action)*nb_case(&openlist);
								printf("%i PA\n", cout);
								break ;
							case 'n' : sortie = 1 ;break;
						}
						
				}
			}
			map.cell[ordre_action->ec->personnage.x][ordre_action->ec->personnage.y] = 0;
			ordre_action->ec->personnage.x = xobj;
			ordre_action->ec->personnage.y = yobj;
			map=actumap(ordre_action, map);
			afficherMat(map);

}
