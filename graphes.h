/*! \file graphes.h
    \brief structures de base pour la manipulation de graphes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define TYP_VARC long
#define TYP_VSOM long
#ifndef HUGE
#define HUGE HUGE_VAL
#endif
#define SHRT_MIN -32767 
#define SHRT_MAX +32767 
#define USHRT_MAX 65535 
#define INT_MIN -32767 
#define INT_MAX +32767 
#define UINT_MAX 65535 
#define LONG_MIN -2147483647 
#define LONG_MAX +2147483647
#define ULONG_MAX 4294967295
#define M_PI	 3.14159265358979323846
#define max(X,Y) ((X)>=(Y)?(X):(Y))
#define min(X,Y) ((X)<=(Y)?(X):(Y))

/* ================================================ */
/* types publics */
/* ================================================ */

/*! \struct cell
    \brief structure de cellule pour les listes chaînees de successeurs.
*/
typedef struct cell {
//! index du sommet
  int som;
//! poids de l'arc pour les graphes pondérés 
  TYP_VARC v_arc; 
//! suite de la liste ou pointeur NULL
  struct cell * next; 
} cell;

/*! \var pcell
    \brief pointeur sur une cellule
*/
typedef cell * pcell; 


/*! \struct graphe
    \brief structure pour la representation des graphes
*/
typedef struct graphe {

  /* informations globales */

//!  nombre de sommets 
  int nsom;         
//!  nombre maximum d'arcs
  int nmaxarc;      
//!  nombre d'arcs
  int narc;         

  /* representation par listes chainees de successeurs (application gamma) */

//!  tableau des cellules en réserve 
  pcell reserve;    
//!  liste des cellules libres gérée en pile lifo 
  pcell libre;      
//!  tableau des listes de successeurs indexé par les sommets 
  pcell * gamma;    

  /* representation par liste d'arcs 
     (vecteurs tete (sommet initial), queue (sommet final)) */

//!  tableau des extremités initiales d'arcs 
  int *tete;        
//!  tableau des extremités finales d'arcs 
  int *queue;       

  /* informations additionelles ajoutees aux arcs */

//!  tableau des valeurs associées aux arcs
  TYP_VARC *v_arcs;

  /* informations additionelles ajoutees aux sommets */

//!  tableau des valeurs associées aux sommets
  TYP_VSOM *v_sommets;

//!  abcisses des sommets 
  double *x;        
//!  ordonnees des sommets 
  double *y;        
//!  noms des sommets 
  char **nomsommet; 
} graphe;

/* ================================================ */
/* prototypes */
/* ================================================ */

extern void AfficheEnsemble(boolean *s, int n);
extern void AfficheListe(pcell p);
extern void AfficheSuccesseurs(graphe * g) ;
extern void AfficheArcs(graphe * g);
extern void AfficheValeursSommets(graphe * g);
extern void PSGraphe(graphe * g, char *filename, double r, double t, double marge);
void EPSGraphe(graphe * g, char *filename, double r, double t, double marge, int noms_sommets, int v_sommets, int col_sommets, int v_arcs);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE PLONGEMENT DE GRAPHES DANS LE PLAN */
/* ====================================================================== */
/* ====================================================================== */

extern void AutoNomsSommets(graphe * g, int mode);
extern void PlongementCirculaire(graphe * g, double r);
extern void PlongementRadial(graphe * g, int c);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS SUR LES LISTES CHAINEES DE SOMMETS */
/* ====================================================================== */
/* ====================================================================== */

extern pcell AlloueCell(pcell * plibre);
extern void LibereCell(pcell * plibre, pcell p);
extern void RetireTete(pcell * plibre, pcell * pliste);
extern void AjouteTete(pcell * plibre, pcell * pliste, int a, TYP_VARC v);
extern int EstDansListe(pcell p, int a);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'ALLOCATION / LIBERATION POUR UN GRAPHE */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * InitGraphe(int nsom, int nmaxarc);
extern void TermineGraphe(graphe * g);
extern graphe * ReadGraphe(char * filename);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE MANIPULATION DES ARCS (APPLICATION GAMMA UNIQUEMENT) */
/* ====================================================================== */
/* ====================================================================== */

extern void AjouteArc(graphe * g, int i, int s);
extern void AjouteArcValue(graphe * g, int i, int s, TYP_VARC v);
extern void RetireArc(graphe * g, int i, int s);
extern int PopSuccesseur(graphe *g, int i);
extern int EstSuccesseur(graphe *g, int i, int s);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE GENERATION DE GRAPHES */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * GrapheAleatoire(int nsom, int narc);

/* ====================================================================== */
/* ====================================================================== */
/* OPERATEURS DE BASE SUR LES GRAPHES */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * Symetrique(graphe * g);
extern graphe * FermetureSymetrique(graphe * g);
extern void CompFortConnexe(graphe * g, graphe *g_1, int a, boolean * Ca);
extern boolean ExisteCircuit(graphe * g, int a);
extern void CompConnexe(graphe * g, graphe *g_1, int a, boolean * Ca);
extern boolean Connexe(graphe * g, graphe *g_1);

/* ====================================================================== */
/* ====================================================================== */
/* ARBRE DE POIDS MAXIMUM */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * Kruskal1(graphe * g, graphe *g_1);
extern graphe * Kruskal2(graphe * g, graphe *g_1);
