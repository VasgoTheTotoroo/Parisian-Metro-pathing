/*! \file graph_basic.c
    \brief fonctions de base pour la manipulation de graphes
*/
/*
      Michel Couprie, septembre 1999
*/
#include "graphaux.h"
#include "graphes.h"

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS SUR LES LISTES CHAINEES DE SOMMETS */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn pcell AlloueCell(pcell * plibre)
    \param plibre (entrée) : pointeur sur une liste chaînee de cellules libres.
    \return pointeur sur une cellule.
    \brief retire la premiere cellule de la liste pointée par plibre et retourne un pointeur sur cette cellule.
*/
pcell AlloueCell(pcell * plibre)
/* ====================================================================== */
{
  pcell p;
  if (*plibre == NULL) 
  {
    fprintf(stderr, "AlloueCell : plus de cellules libres\n");
    exit(0);
  }
  p = *plibre;
  *plibre = (*plibre)->next;
  return p;
} /* AlloueCell() */

/* ====================================================================== */
/*! \fn void LibereCell(pcell * plibre, pcell p)
    \param plibre (entrée) : pointeur sur une liste chaînee de cellules libres.
    \param p (entrée) : pointeur sur une cellule.
    \brief insère la cellule p au début de la liste pointée par 'plibre'. 
*/
void LibereCell(pcell * plibre, pcell p)
/* ====================================================================== */
{
  p->next = *plibre;
  *plibre = p;
} /* LibereCell() */

/* ====================================================================== */
/*! \fn void RetireTete(pcell * plibre, pcell * pliste)
    \param plibre (entrée) : pointeur sur une liste chaînee de cellules libres.
    \param pliste (entrée) : pointeur sur une liste.
    \brief retire la première cellule de la liste 'pliste'. La cellule est chaînee à la liste 'plibre'. 
*/
void RetireTete(pcell * plibre, pcell * pliste)
/* ====================================================================== */
{
  pcell p;
  p = (*pliste)->next;
  LibereCell(plibre, *pliste);
  *pliste = p;
} /* RetireTete() */

/* ====================================================================== */
/*! \fn void AjouteTete(pcell * plibre, pcell * pliste, int a, TYP_VARC v)
    \param plibre (entrée) : pointeur sur une liste chaînee de cellules libres.
    \param pliste (entrée) : pointeur sur une liste.
    \param a (entrée) : un sommet.
    \param v (entrée) : une valeur.
    \brief ajoute une cellule contenant le sommet 'a' et la valeur 'v' en tête de la liste 'pliste'. La cellule est prise dans la liste 'plibre'. 
*/
void AjouteTete(pcell * plibre, pcell * pliste, int a, TYP_VARC v)
/* ====================================================================== */
{
  pcell p;
  p = AlloueCell(plibre);
  p->next = *pliste;
  p->som = a;
  p->v_arc = v;
  *pliste = p;
} /* AjouteTete() */

/* ====================================================================== */
/*! \fn int EstDansListe(pcell p, int a) 
    \param p (entrée) : une liste chaînee de successeurs.
    \param a (entrée) : un sommet.
    \return booléen.
    \brief retourne 1 si le sommet 'a' se trouve dans la liste 'p', 0 sinon. 
*/
int EstDansListe(pcell p, int a) 
/* ====================================================================== */
{
  for (; p != NULL; p = p->next)
    if (p->som == a) return 1;
  return 0;
} /* EstDansListe() */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'ALLOCATION / LIBERATION POUR UN GRAPHE */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn graphe * InitGraphe(int nsom, int nmaxarc)
    \param   nsom (entrée) : nombre de sommets.
    \param nmaxarc (entrée) : nombre maximum d'arcs.
    \return un graphe.
    \brief alloue la memoire nécessaire pour représenter un graphe a 'nsom' sommets,
              possédant au maximum 'nmaxarc' arcs. 
              Retourne un pointeur sur la structure allouée. 
*/
graphe * InitGraphe(int nsom, int nmaxarc)
/* ====================================================================== */
{
  graphe * g;
  int i;
  
  g = (graphe *)malloc(sizeof(graphe));
  if (g == NULL)
  {   fprintf(stderr, "InitGraphe : malloc failed\n");
      exit(0);
  }

  g->reserve = (cell *)malloc(nmaxarc * sizeof(cell));
  if (g->reserve == NULL)
  {   fprintf(stderr, "InitGraphe : malloc failed\n");
      exit(0);
  }

  g->gamma = (pcell *)calloc(nsom, sizeof(pcell)); /* calloc met a 0 la memoire allouee */
  if (g->gamma == NULL)
  {   fprintf(stderr, "InitGraphe : calloc failed\n");
      exit(0);
  }

  g->tete = (int *)malloc(nmaxarc * sizeof(int));
  g->queue = (int *)malloc(nmaxarc * sizeof(int));
  if ((g->tete == NULL) || (g->tete == NULL))
  {   fprintf(stderr, "InitGraphe : malloc failed\n");
      exit(0);
  }

  g->v_arcs = (TYP_VARC *)malloc(nmaxarc * sizeof(TYP_VARC));
  if (g->v_arcs == NULL)
  {   fprintf(stderr, "InitGraphe : malloc failed\n");
      exit(0);
  }

  g->v_sommets = (TYP_VSOM *)malloc(nsom * sizeof(TYP_VSOM));
  if (g->v_sommets == NULL)
  {   fprintf(stderr, "InitGraphe : malloc failed\n");
      exit(0);
  }

  g->x = (double *)malloc(nsom * sizeof(double));
  g->y = (double *)malloc(nsom * sizeof(double));
  if ((g->x == NULL) || (g->y == NULL))
  {   fprintf(stderr, "InitGraphe : malloc failed\n");
      exit(0);
  }

  g->nsom = nsom;
  g->nmaxarc = nmaxarc;
  g->narc = 0;

  /* construit la liste initiale de cellules libres */
  for (i = 0; i < nmaxarc - 1; i++)
    (g->reserve+i)->next = g->reserve+i+1;
  (g->reserve+i)->next = NULL;
  g->libre = g->reserve;  

  g->nomsommet = NULL;
  
  return g;
} /* InitGraphe() */

/* ====================================================================== */
/*! \fn void TermineGraphe(graphe * g)
    \param g (entrée) : un graphe.
    \brief libère la memoire occupée par le graphe g. 
*/
void TermineGraphe(graphe * g)
/* ====================================================================== */
{
  int i, n = g->nsom;
  free(g->reserve);
  if (g->gamma) free(g->gamma);
  if (g->tete) { free(g->tete); free(g->queue); }
  if (g->v_arcs) free(g->v_arcs);
  if (g->v_sommets) free(g->v_sommets);
  if (g->nomsommet)
  {
    for (i = 0; i < n; i++) free(g->nomsommet[i]);
    free(g->nomsommet);
  }
  free(g);
} /* TermineGraphe() */


/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'ENTREE / SORTIE FICHIER POUR UN GRAPHE */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn graphe * ReadGraphe(char * filename)
    \param   filename (entrée) : nom du fichier graphe.
    \return un graphe.
    \brief Lit les données d'un graphe dans le fichier filename, retourne un pointeur sur la structure graphe construite. 
*/
graphe * ReadGraphe(char * filename)
/* ====================================================================== */
{
#define TAILLEBUF 4096
  graphe * g;
  int i, n, m, t, q;
  char buf[TAILLEBUF];
  char *ret;

  FILE *fd = NULL;

  fd = fopen(filename,"r");
  if (!fd)
  {
    fprintf(stderr, "ReadGraphe: file not found: %s\n", filename);
    return NULL;
  }

  fscanf(fd, "%d %d\n", &n, &m);
  g = InitGraphe(n, m);
  do
  {
    ret = fgets(buf, TAILLEBUF, fd);
    if ((ret != NULL) && (strncmp(buf, "noms sommets", 12) == 0))
    {
      g->nomsommet = (char **)malloc(n * sizeof(char *));
      if (g->nomsommet == NULL)
      {   fprintf(stderr, "ReadGraphe : malloc failed\n");
          exit(0);
      }
      for (i = 0; i < n; i++)
      {
        fscanf(fd, "%d\n", &t);
        fgets(buf, TAILLEBUF, fd);
        g->nomsommet[t] = (char *)malloc((strlen(buf)+1) * sizeof(char));
        if (g->nomsommet[t] == NULL)
        {   fprintf(stderr, "ReadGraphe : malloc failed\n");
            exit(0);
        }
        strcpy(g->nomsommet[t], buf);
      } /* for (i = 0; i < n; i++) */
    } /* if ((ret != NULL) && (strcmp(buf, "noms sommets") == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "coord sommets", 13) == 0))
    {
      double x, y;
      for (i = 0; i < n; i++)  
      {
        fscanf(fd, "%d %lf %lf\n", &t, &x, &y);
        g->x[t] = x; 
        g->y[t] = y; 
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "arcs values", 13) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "arcs values", 11) == 0))
    {
      double v;
      for (i = 0; i < m; i++)  
      {
        fscanf(fd, "%d %d %lf\n", &t, &q, &v);
        AjouteArcValue(g, t, q, (TYP_VARC)v);
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "arcs values", 11) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "arcs", 4) == 0))
    {
      for (i = 0; i < m; i++)  
      {
        fscanf(fd, "%d %d\n", &t, &q);
        AjouteArc(g, t, q);
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "arcs", 4) == 0)) */
  } while (ret != NULL);

  return g;
} /* ReadGraphe() */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE MANIPULATION DES ARCS (APPLICATION GAMMA UNIQUEMENT) */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn void AjouteArc(graphe * g, int i, int s)
    \param g (entrée/sortie) : un graphe.
    \param i (entrée) : extrémité initiale de l'arc.
    \param s (entrée) : extrémité finale de l'arc.
    \brief ajoute l'arc (i,s) au graphe g (application gamma seulement). 
*/
void AjouteArc(graphe * g, int i, int s)
/* ====================================================================== */
{
  AjouteTete(&(g->libre), &(g->gamma[i]), s, 0);
  g->narc++;
} /* AjouteArc() */

/* ====================================================================== */
/*! \fn void AjouteArcValue(graphe * g, int i, int s, TYP_VARC v)
    \param g (entrée/sortie) : un graphe.
    \param i (entrée) : extrémité initiale de l'arc.
    \param s (entrée) : extrémité finale de l'arc.
    \param v (entrée) : une valeur pour l'arc.
    \brief ajoute l'arc (i,s) au graphe g (application gamma seulement). 
*/
void AjouteArcValue(graphe * g, int i, int s, TYP_VARC v)
/* ====================================================================== */
{
  AjouteTete(&(g->libre), &(g->gamma[i]), s, v);
  g->narc++;
} /* AjouteArcValue() */

/* ====================================================================== */
/*! \fn void RetireArc(graphe * g, int i, int s)
    \param g (entrée/sortie) : un graphe.
    \param i (entrée) : un sommet de g.
    \param s (entrée) : un sommet de g.
    \brief retire l'arc (i,s) du graphe g (application gamma seulement),
              si celui-ci etait présent. Sinon, pas d'action. 
*/
void RetireArc(graphe * g, int i, int s)
/* ====================================================================== */
{
  pcell * pliste;
  pliste = g->gamma + i;
  while ((*pliste != NULL) && (((*pliste)->som) != s))
    pliste = &((*pliste)->next);
  if (*pliste != NULL) RetireTete(&(g->libre), pliste);
  g->narc--;
} /* RetireArc() */

/* ====================================================================== */
/*! \fn int PopSuccesseur(graphe * g, int i)
    \param g (entrée/sortie) : un graphe.
    \param i (entrée) : un sommet de g.
    \brief retire un arc (i,s) du graphe g (application gamma seulement),
           et retourne le sommet s
    \warning le sommet i doit avoir au moins un successeur (pas de vérification)
*/
int PopSuccesseur(graphe * g, int i)
/* ====================================================================== */
{
  int s = g->gamma[i]->som;
  RetireTete(&(g->libre), &(g->gamma[i]));
  return s;
} /* PopSuccesseur() */

/* ====================================================================== */
/*! \fn int EstSuccesseur(graphe *g, int i, int s) 
    \param g (entrée) : un graphe.
    \param i (entrée) : un sommet de g.
    \param s (entrée) : un sommet de g.
    \return booléen.
    \brief retourne 1 si le sommet 's' est un successeur du sommet 'i', 0 sinon.
*/
int EstSuccesseur(graphe *g, int i, int s) 
/* ====================================================================== */
{
  return EstDansListe(g->gamma[i], s);
} /* EstSuccesseur() */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE GENERATION DE GRAPHES */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn graphe * GrapheAleatoire(int nsom, int narc)
    \param nsom (entrée) : nombre de sommets.
    \param narc (entrée) : nombre d'arcs.
    \return un graphe.
    \brief retourne un graphe aléatoire à 'nsom' sommets et 'narc' arcs. 
              Le graphe est antisymétrique et sans boucle.
              Le nombre d'arcs doit être <= nsom (nsom - 1) / 2.
              Les arcs sont pondérés (valeur aléatoire entre 0.0 et 1.0).
    \warning la méthode employée ici est naïve, tres inefficace du point de vue
              temps de calcul et ne garantit aucune propriété statistique. 
*/
graphe * GrapheAleatoire(int nsom, int narc)
/* ====================================================================== */
{
  graphe * g;
  int i, j, m;
  double mmax = ((double)nsom * ((double)nsom - 1)) / 2;

  if (narc > mmax)
  {
    fprintf(stderr, "GrapheAleatoire : pas plus de %g arcs pour %d sommets\n", 
                     mmax, nsom);
    exit(0);
  }

  if ((mmax >= (1<<31)) || (narc <= ((int)mmax) / 2))
  {
    g = InitGraphe(nsom, narc);
    while (narc > 0)
    {
      narc--;
      do
      {
        i = rand() % nsom;
        j = rand() % nsom;
      } while ((i == j) || EstSuccesseur(g, i, j) || EstSuccesseur(g, j, i));
      AjouteArc(g, i, j);
      g->tete[narc] = i;
      g->queue[narc] = j;
      if (g->v_arcs) g->v_arcs[narc] = (TYP_VARC)(rand()*100.0);
    }
  }
  else /* on part du graphe complet et on retire des arcs au hasard */
  {
    /* construit un graphe complet antisymetrique sans boucle */
    g = InitGraphe(nsom, (int)mmax);
    for (i = 0; i < nsom ; i++)
      for (j = i+1; j < nsom; j++)
        AjouteArc(g, i, j);         

    /* retire mmax - narc arcs */
    m = (int)mmax;
    while (m > narc)
    {
      m--;
      do
      {
        i = rand() % nsom;
        j = rand() % nsom;
      } while ((i == j) || !EstSuccesseur(g, min(i,j), max(i,j)));
      RetireArc(g, min(i,j), max(i,j));
    }

    /* rajoute la liste des arcs et les poids */
    m = 0;
    for (i = 0; i < nsom ; i++)
      for (j = i+1; j < nsom; j++)
        if (EstSuccesseur(g, i, j))
	{
          g->tete[m] = i;
          g->queue[m] = j;
          if (g->v_arcs) g->v_arcs[narc] = (TYP_VARC)(rand()*100.0);
          m++;
	}
  }

  return g;
} /* GrapheAleatoire() */
