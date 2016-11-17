#include<stdio.h>


/**
*\file gest_partie.c
*\brief Ce fichier contient les fonctions permettant le lancement et le déroulement de la partie
*\author Victor Morgant
*\version 1.0
*\date 14/11/2016
*/
#include<stdio.h>
#include<stdlib.h>
#include "global.h"
#include "listes_ptr.h"


void init_partie(t_liste *equipe1,t_liste *equipe2){
/**création de  la liste ordre action avec les personnages triés par initiative*/
	t_personnage *tampon;
	t_liste *ordre_action;
	init_liste(ordre_action);
	en_tete(equipe1);
	en_tete(equipe2);
	int bienplace=0;
	while(!hors_liste(equipe1)&& (!hors_liste(equipe2))){
	/**On ajoute les deux équipes dans ordre action en prenant un joueur sur */
		valeur_elt(equipe1,tampon);
		ajout_droit(ordre_action,*tampon);
		valeur_elt(equipe2,tampon);
		ajout_droit(ordre_action,*tampon);
	}
		en_queue(ordre_action);
		while(!hors_liste(ordre_action)){
			valeur_elt(ordre_action,tampon);
			bienplace=(tampon->classe.INI >= ordre_action->ec->pred->personnage.classe.INI);
			if(!bienplace && ordre_action->ec->pred != ordre_action->drapeau){
				oter_elt(ordre_action);
				ajout_gauche(ordre_action,*tampon);
			}
			else
				suivant(ordre_action);	
		}
		
}

void placer(t_liste personnages equipe1,t_liste personnages equipe2,t_map carte){
/**Place les personnages des deux équipes sur la carte.*/
}

int mort(t_liste ordre_action,t_personnage cadavre){
/**Retire le cadavre de la liste ordre_action puis renvoie vrai s'il reste deux équipes.*/
}

void gestion_tour(t_liste ordre_action,NbTour){
/**Joue le tour suivant*/
}


