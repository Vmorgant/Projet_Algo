/**
*\file listes_ptr.c
*\brief Ce fichier contient les définitions des primitives d'acces aux listes
*\author Corentin Petit
*\version 1.0
*\date 09/11/2016
*/

#include<stdio.h>
#include<stdlib.h>
#include "../includes/global.h"


void init_liste(t_liste * p) {
	p->drapeau =  (t_element*)malloc (sizeof(t_element));
	p->drapeau -> pred = p->drapeau;
	p->drapeau -> succ = p->drapeau;
	p->ec = p->drapeau;
}

int liste_vide(t_liste * p) {
	return(p->drapeau->pred == p->drapeau);
}

int hors_liste(t_liste * p) {
	return(p->ec == p->drapeau);
}

void en_tete(t_liste * p) {
	p->ec = p->drapeau -> succ;
}

void en_queue(t_liste * p) {
	p->ec = p->drapeau -> pred;
}

void suivant(t_liste * p) {
	if (!hors_liste(p)){
		p->ec = p->ec -> succ;
	}
}

void precedent(t_liste * p) {
	if (!hors_liste(p)){
		p->ec = p->ec -> pred;
	}
}

void valeur_elt(t_liste * p, t_personnage * v) {
	if(!hors_liste(p))
		*v = p->ec -> personnage;
}

void modif_elt(t_liste * p, t_personnage v) {
	if(!hors_liste(p))
		p->ec -> personnage = v;
}

void oter_elt(t_liste * p) {
	if(!hors_liste(p)) {
		t_element * temp;
		(p->ec->succ)->pred = p->ec->pred;
		(p->ec->pred)->succ = p->ec->succ;
		temp = p->ec;
		p->ec = p->ec->pred;
		free(temp);
	}
}

void ajout_droit(t_liste * p, t_personnage v) {
	if (liste_vide(p) || !hors_liste(p) ) {
		t_element * nouv;
		nouv = malloc (sizeof(t_element));
		nouv -> personnage = v;
		nouv -> succ = p->ec -> succ;
		nouv -> pred = p->ec;
		(p->ec -> succ) -> pred = nouv;
		p->ec -> succ = nouv;
	}
}

void ajout_gauche(t_liste * p, t_personnage v) {
	if (liste_vide(p) || !hors_liste(p) ) {
		t_element * nouv;
		nouv = malloc (sizeof(t_element));
		nouv -> personnage = v;
		nouv -> succ = p->ec;
		nouv -> pred = p->ec -> pred;
		(p->ec -> pred) -> succ = nouv;
		p->ec -> pred = nouv;
	}
}

void afficher(t_liste* p) {
	if(!liste_vide(p)){
		en_tete(p);
		while(!hors_liste(p)){
				printf("%s(%iPE)\t", p->ec->personnage.classe.nom, p->ec->personnage.classe.coutPE);
				suivant(p);
		}
	}
}

void compter_elts(t_liste*p, int*v){
	*v = 0;
	if(!liste_vide(p)){
		en_tete(p);
		while(!hors_liste(p)){
			(*v)++;
			suivant(p);
		}
	}
}

void vider(t_liste* p) {
	if(!liste_vide(p)){
		en_queue(p);
		while(!liste_vide(p)){
				oter_elt(p);
		}
	}
}

void supprimer(t_liste* p) {
	vider(p);
	free(p->drapeau);
}
