/*! \file graph_algos.c
    \brief algorithmes fondamentaux
*/
/*
      Michel Couprie, septembre 1999
*/
#include "graphaux.h"
#include "graphes.h"
#define INFINITY LONG_MAX

/* ====================================================================== */
/* A RETIRER POUR LE TP 1 */
/* ====================================================================== */
/*! \fn graphe * Symetrique(graphe * g)
    \param g (entrée) : un graphe.
    \return un graphe.
    \brief construit et retourne le graphe g_1 symétrique du graphe g (algorithme linéaire)
    \warning seule la représentation 'gamma' est utilisée. 
*/
graphe * Symetrique(graphe * g)
/* ====================================================================== */
{
  graphe *g_1;
  int nsom, narc, al_arcs, k, i, j;
  pcell p;

  nsom = g->nsom;
  narc = g->narc;
  g_1 = InitGraphe(nsom, narc);
  for (i = 0; i < nsom; i++) /* pour tout i sommet de g */
  {
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout j successeur de i */
      j = p->som;
      AjouteArcValue(g_1, j, i, p->v_arc);
    } // for p
  } // for i

  return g_1;
} /* Symetrique() */

/* ====================================================================== */
/* A RETIRER POUR LE TP 1 */
/* ====================================================================== */
/*! \fn graphe * FermetureSymetrique(graphe * g)
    \param g (entrée) : un graphe.
    \return un graphe.
    \brief construit et retourne la fermeture symétrique du graphe g (algorithme linéaire)
    \warning seule la représentation 'gamma' est utilisée. 
*/
graphe * FermetureSymetrique(graphe * g)
/* ====================================================================== */
{
  graphe *gs;
  int nsom, narc, al_arcs, k, i, j;
  pcell p;

  nsom = g->nsom;
  narc = g->narc;
  gs = InitGraphe(nsom, 2 * narc);
  for (i = 0; i < nsom; i++) /* pour tout i sommet de g */
  {
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout j successeur de i */
      j = p->som;
      AjouteArc(gs, i, j);
      AjouteArc(gs, j, i);
    } // for p
  } // for i

  return gs;
} /* FermetureSymetrique() */

/* ====================================================================== */
/*! \fn void CompFortConnexe(graphe * g, graphe *g_1, int a, boolean * Ca)
    \param g (entrée) : un graphe.
    \param g_1 (entrée) : le graphe symétrique de g.
    \param a (entrée) : un sommet du graphe g.
    \param Ca (sortie) : un sous-ensemble des sommets de g (tableau de booléens).
    \brief retourne dans Ca la composante fortement connexe de g contenant a 
          (sous la forme d'un tableau de booléens). 
    \warning Ca doit avoir été alloué correctement (pas de vérification)
*/
void CompFortConnexe(graphe * g, graphe *g_1, int a, boolean * Ca)
/* ====================================================================== */
{
  boolean * D;   /* pour les "descendants" (successeurs a N niveaux) */
  boolean * A;   /* pour les "ascendants" (predecesseurs a N niveaux) */
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int i, n, s;
  pcell p;

  n = g->nsom;
  T = CreeLifoVide(n);
  A = EnsembleVide(n);
  D = EnsembleVide(n);
  memset(Ca, 0, n);   // Ca := vide

  /* calcule les descendants de a */
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout sommet s successeur de i */
      s = p->som;
      if (!D[s]) 
      {
        D[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  /* calcule les ascendants de a */
  LifoPush(T, a);
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g_1->gamma[i]; p != NULL; p = p->next) 
    { /* pour tout s successeur de i */
      s = p->som;
      if (!A[s]) 
      {
        A[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  /* intersection de D et de A, union { a } */
  for (i = 0; i < n; i++) Ca[i] = (D[i] && A[i]);
  Ca[a] = TRUE;
    
  free(A);
  free(D);
  LifoTermine(T);
} /* CompFortConnexe() */

/* ====================================================================== */
/*! \fn boolean ExisteCircuit(graphe * g, int a)
    \param g (entrée) : un graphe.
    \param a (entrée) : un sommet du graphe g.
    \return booléen
    \brief teste l'existence d'un circuit dans g contenant a 
*/
boolean ExisteCircuit(graphe * g, int a)
/* ====================================================================== */
{
  boolean * D;   /* pour les "descendants" (successeurs a N niveaux) */
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int i, n, s;
  pcell p;

  n = g->nsom;
  T = CreeLifoVide(n);
  D = EnsembleVide(n);

  /* calcule les descendants de a */
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout sommet s successeur de i */
      s = p->som;
      if (s == a) { free(D); LifoTermine(T); return TRUE; }
      if (!D[s]) 
      {
        D[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  free(D);
  LifoTermine(T);
  return FALSE;
} /* ExisteCircuit() */

/* ====================================================================== */
/*! \fn void CompConnexe(graphe * g, graphe *g_1, int a, boolean * Ca)
    \param   g (entrée) : un graphe.
    \param g_1 (entrée) : le graphe symétrique de g.
    \param a (entrée) : un sommet du graphe g.
    \return un sous-ensemble de sommets de g (tableau de booléens).
    \brief retourne la composante connexe de g contenant a
          (sous la forme d'un tableau de booléens). 
*/
void CompConnexe(graphe * g, graphe *g_1, int a, boolean * Ca)
/* ====================================================================== */
{
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int i, n, s;
  pcell p;

  n = g->nsom;
  T = CreeLifoVide(n);
  memset(Ca, 0, n); // Ca := vide

  Ca[a] = TRUE;
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (!Ca[s]) 
      {
        Ca[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
    for (p = g_1->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i dans g_1 */
      s = p->som;
      if (!Ca[s]) 
      {
        Ca[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  LifoTermine(T);
} /* CompConnexe() */

/* ====================================================================== */
/*! \fn int Connexe(graphe * g, graphe *g_1)
    \param g (entrée) : un graphe.
    \param g_1 (entrée) : le graphe symétrique de g.
    \return booléen.
    \brief retourne 1 si le graphe est connexe, 0 sinon. 
*/
boolean Connexe(graphe * g, graphe *g_1)
/* ====================================================================== */
{
  int i;
  boolean ret = TRUE;
  int n = g->nsom;
  boolean * C = EnsembleVide(n);
  CompConnexe(g, g_1, 0, C);
  for (i = 0; i < n; i++) if (!C[i]) { ret = FALSE; break; }
  free(C);
  return ret;
} /* Connexe() */

/* ====================================================================== */
/* ====================================================================== */
/* ARBRE DE POIDS MAXIMUM */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/* A RETIRER POUR LES TP 1, 2 */
/* ====================================================================== */
/*! \fn graphe * Kruskal1(graphe * g, graphe *g_1)
    \param g (entrée) : un graphe pondéré connexe sans boucle.
    \param g_1 (entrée) : le graphe symétrique de g.
    \return un arbre. 
    \brief retourne un arbre de poids maximal pour g.
*/
graphe * Kruskal1(graphe * g, graphe *g_1)
/* ====================================================================== */
{
  int n = g->nsom;
  int m = g->narc;
  graphe * apm;    /* pour le resultat */
  graphe * apm_1;  /* pour la detection de cycles */
  int *A;          /* tableau pour ranger les index des arcs */
  int i, j, t, q;
  boolean *Ct = EnsembleVide(n);

  /* tri des arcs par poids croissant */
  A = (int *)malloc(m * sizeof(int)); /* allocation index */
  if (A == NULL)
  {   fprintf(stderr, "Kruskal1 : malloc failed\n");
      exit(0);
  }  
  for (i = 0; i < m; i++) A[i] = i; /* indexation initiale */
  TriRapideStochastique(A, g->v_arcs, 0, m-1);

  /* construit le graphe resultat, initialement vide */
  apm = InitGraphe(n, n-1);
  apm_1 = InitGraphe(n, n-1);

  /* Boucle sur les arcs pris par ordre decroissant:
       on ajoute un arc dans apm, si cela ne cree pas un cycle dans apm.
     Arret lorsque nb arcs ajoutes = n-1. */

  j = 0; 
  i = m - 1;
  while (j < n-1)
  {
    t = g->tete[A[i]]; q = g->queue[A[i]]; 
    CompConnexe(apm, apm_1, t, Ct);
    if (!Ct[q]) 
    {
      AjouteArc(apm, t, q);
      AjouteArc(apm_1, q, t);
      j++; 
    }
    i--;
    if (i < 0)
    {   fprintf(stderr, "Kruskal1 : graphe d'origine non connexe\n");
        exit(0);
    }  
  } // while (j < n-1)

  if (g->x != NULL) // recopie les coordonnees des sommets pour l'affichage
    for (i = 0; i < n; i++) { apm->x[i] = g->x[i];  apm->y[i] = g->y[i]; }

  free(A);
  free(Ct);
  TermineGraphe(apm_1);
  return apm;
} /* Kruskal1() */

/* ====================================================================== */
/* A RETIRER POUR LES TP 1, 2 */
/* ====================================================================== */
/*! \fn graphe * Kruskal2(graphe * g, graphe *g_1)
    \param g (entrée) : un graphe pondéré connexe sans boucle.
    \param g_1 (entrée) : le graphe symétrique de g.
    \return un arbre. 
    \brief retourne un arbre de poids maximal pour g.
    \warning les données du graphe d'origine seront détruites. 
*/
graphe * Kruskal2(graphe * g, graphe *g_1)
/* ====================================================================== */
{
  int n = g->nsom;
  int msav, m = g->narc;
  graphe * apm;    /* pour le resultat */
  int *A;          /* tableau pour ranger les index des arcs */
  int i, j, t, q;
  msav = m;

  if (!Connexe(g, g_1)) 
  {   fprintf(stderr, "Kruskal2 : graphe d'origine non connexe\n");
      exit(0);
  }  

  /* tri des arcs par poids croissant */
  A = (int *)malloc(m * sizeof(int)); /* allocation index */
  if (A == NULL)
  {   fprintf(stderr, "Kruskal2 : malloc failed\n");
      exit(0);
  }  
  for (i = 0; i < m; i++) A[i] = i; /* indexation initiale */
  TriRapideStochastique(A, g->v_arcs, 0, m-1);

  /* Boucle sur les arcs pris par ordre croissant:
       on enleve un arc, si cela deconnecte le graphe on le remet.
     Arret lorsque m = n-1. */
  i = 0;
  while (m > n-1)
  {
    t = g->tete[A[i]]; q = g->queue[A[i]]; 
    RetireArc(g, t, q);
    RetireArc(g_1, q, t);
    if (Connexe(g, g_1)) 
      m--; 
    else 
    {
      AjouteArc(g, t, q);
      AjouteArc(g_1, q, t);
    }
    i++;
  } // while (m > n-1)

  /* construit le graphe resultat */
  apm = InitGraphe(n, m);
  for (i = 0, j = 0; i < msav; i++)
  {
    t = g->tete[i]; q = g->queue[i]; 
    if (EstSuccesseur(g, t, q)) 
    {
      AjouteArc(apm, t, q);
      apm->tete[j] = t; 
      apm->queue[j] = q; 
      apm->v_arcs[j] = g->v_arcs[i]; 
      j++;
    }
  } // for i

  if (g->x)
    for (i = 0; i < n; i++) { apm->x[i] = g->x[i];  apm->y[i] = g->y[i]; }

  free(A);
  return apm;
} /* Kruskal2() */

/* ====================================================================== */
/* ====================================================================== */
/* PROGRAMME DE TEST */
/* ====================================================================== */
/* ====================================================================== */

#ifdef TEST
int main(int argc, char **argv)
{
  graphe * g, *g_1, *a;
  int s1, s2, na, ns;
  boolean *Cs;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s <nombre sommets> <nombre arcs>\n", argv[0]);
    exit(0);
  }

  ns = atoi(argv[1]);
  na = atoi(argv[2]);
  
  g = GrapheAleatoire(ns, na);
  AfficheSuccesseurs(g);
  AfficheArcs(g);
  g_1 = Symetrique(g);
  AfficheSuccesseurs(g_1);
  AfficheArcs(g_1);

  if (Connexe(g, g_1))
  {
    printf("graphe connexe\n");
    a = Kruskal2(g, g_1);
    AfficheSuccesseurs(a);
    AfficheArcs(a);
    TermineGraphe(a);
  }
  else printf("graphe NON connexe\n");

  do
  {
    printf("entrer un sommet : ");
    scanf("%d", &s1);
    if (s1 >= 0) 
    {
      Cs = CompFortConnexe(g, g_1, s1);
      AfficheEnsemble(Cs, g->nsom);
      free(Cs);
    }
  } while (s1 >= 0);

  do
  {
    printf("entrer un sommet : ");
    scanf("%d", &s1);
    if (s1 >= 0) 
    {
      Cs = CompConnexe(g, g_1, s1);
      AfficheEnsemble(Cs, g->nsom);
      free(Cs);
    }
  } while (s1 >= 0);

  TermineGraphe(g);
  TermineGraphe(g_1);

} /* main() */
#endif
