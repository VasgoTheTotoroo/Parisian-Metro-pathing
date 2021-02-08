/*! \file graph_print.c
    \brief fonctions pour l'affichage et l'impression de graphes
*/
/*
      Michel Couprie, septembre 1999
*/
#include "graphaux.h"
#include "graphes.h"

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'AFFICHAGE */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn void AfficheEnsemble(boolean *s, int n)
    \brief affiche à l'écran l'ensemble representé par le tableau de booléens s.
    \param s (entrée) : un tableau de valeurs booléennes.
    \param n (entrée) : la taille du tableau. 
*/
void AfficheEnsemble(boolean *s, int n)
/* ====================================================================== */
{
  int i;
  for (i = 0; i < n; i++) if (s[i]) printf("%d ", i);
  printf("\n");
} /* AfficheEnsemble() */

/* ====================================================================== */
/*! \fn void AfficheListe(pcell p) 
    \param p (entrée) : une liste chaînee de successeurs.
    \brief affiche le contenu de la liste p. */
void AfficheListe(pcell p) 
/* ====================================================================== */
{
  for (; p != NULL; p = p->next) printf("%d ", p->som);
  printf("\n");
} /* AfficheListe() */

/* ====================================================================== */
/*! \fn void AfficheSuccesseurs(graphe * g) 
    \param   g (entrée) : un graphe.
    \brief affiche le graphe dans sa représentation "listes de successeurs". 
*/
void AfficheSuccesseurs(graphe * g) 
/* ====================================================================== */
{
  int i;
  
  if (g->gamma)
  {
    for (i = 0; i < g->nsom; i++)
    {
      printf("[%d] ", i);
      AfficheListe(g->gamma[i]);
    }
    printf("\n");
  }
  else fprintf(stderr, "representation successeurs absente\n");
} /* AfficheSuccesseurs() */

/* ====================================================================== */
/*! \fn void AfficheArcs(graphe * g)
    \param   g (entrée) : un graphe.
    \brief affiche le graphe dans sa représentation "listes d'arcs". 
*/
void AfficheArcs(graphe * g)
/* ====================================================================== */
{
  int i;

  if (g->tete && g->v_arcs)
  {
    for (i = 0; i < g->narc; i++)
      printf("[%d, %d] %g\n", g->tete[i], g->queue[i], (double)(g->v_arcs[i]));
    printf("\n");
  }
  else if (g->tete)
  {
    for (i = 0; i < g->narc; i++)
      printf("[%d, %d]\n", g->tete[i], g->queue[i]);
    printf("\n");
  }
  else fprintf(stderr, "representation arcs absente\n");
} /* AfficheArcs() */

/* ====================================================================== */
/*! \fn void AfficheValeursSommets(graphe * g)
    \param   g (entrée) : un graphe.
    \brief affiche les valeurs associées aux sommets. 
*/
void AfficheValeursSommets(graphe * g)
/* ====================================================================== */
{
  int i;

  if (g->v_sommets)
  {
    for (i = 0; i < g->nsom; i++)
      printf("sommet %d : valeur %g\n", i, (double)(g->v_sommets[i]));
    printf("\n");
  }
  else fprintf(stderr, "valeurs sommets absentes\n");
} /* AfficheValeursSommets() */

/* ====================================================================== */
/*! \fn void PSGraphe(graphe * g, char *filename, double r, double t, double marge) 
    \param g (entrée) : un graphe.
    \param filename (entrée) : nom du fichier postscript à générer.
    \param r (entrée) : rayon des cercles qui représentent les sommets (0 pour ne pas les dessiner).
    \param t (entrée) : taille (demi-longueur) des flèches pour les arcs (0 pour ne pas les dessiner).
    \param marge (entrée) : marge en x et en y.
    \brief génère une figure PostScript d'après la représentation "successeurs" du graphe g. 
*/
void PSGraphe(graphe * g, char *filename, double r, double t, double marge) 
/* ====================================================================== */
{
  int i, j, n = g->nsom;
  double xmin, xmax, ymin, ymax;
  double x1, y1, x2, y2, x3, y3, x, y, a, b, d;
  pcell p;
  FILE * fd = NULL;
  
  if (g->gamma == NULL) 
  {  fprintf(stderr, "PSGraphe: representation successeurs absente\n");
     return;
  }
  
  if (g->x == NULL) 
  {  fprintf(stderr, "PSGraphe: coordonnees des sommets absentes\n");
     return;
  }

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "PSGraphe: cannot open file: %s\n", filename);
    return;
  }

  /* determine le rectangle englobant et genere le header */
  xmin = xmax = g->x[0];
  ymin = ymax = g->y[0];
  for (i = 1; i < n; i++) 
  {
    if (g->x[i] < xmin) xmin = g->x[i]; else if (g->x[i] > xmax) xmax = g->x[i];
    if (g->y[i] < ymin) ymin = g->y[i]; else if (g->y[i] > ymax) ymax = g->y[i];
  }
  EPSHeader(fd, xmax - xmin + 2.0 * marge, ymax - ymin + 2.0 * marge, 1.0, 14);
  
  /* dessine les sommets */
  for (i = 0; i < n; i++) 
    PSDrawcircle(fd, g->x[i]-xmin+marge, g->y[i]-ymin+marge, r);
  if (g->nomsommet)
    for (i = 0; i < n; i++) 
      PSString(fd, g->x[i]-xmin+marge+2*r, g->y[i]-ymin+marge+2*r, g->nomsommet[i]);

  /* dessine les arcs */
  if (r > 0.0)
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      PSLine(fd, g->x[i]-xmin+marge, g->y[i]-ymin+marge, g->x[j]-xmin+marge, g->y[j]-ymin+marge);
    }

  /* dessine les fleches sur les arcs */
  if (t > 0.0)
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      x1 = g->x[i]-xmin+marge;
      y1 = g->y[i]-ymin+marge;
      x2 = g->x[j]-xmin+marge;
      y2 = g->y[j]-ymin+marge;
      x = (x2 + x1) / 2;
      y = (y2 + y1) / 2;
      a = x2 - x1;
      b = y2 - y1;             /* (a,b) est un vecteur directeur de l'arc */
      d = sqrt(a * a + b * b); /* longueur de l'arc */
      if (d > 1) // sinon on ne dessine rien
      { 
        a /= d; b /= d;          /* norme le vecteur */
        x1 = x + 2 * t * a;
        y1 = y + 2 * t * b;      /* pointe de la fleche */
        x2 = x - 2 * t * a;
        y2 = y - 2 * t * b;      /* base de la fleche */
        x3 = x2 + t * -b;        /* (-b,a) est normal a (a,b) */
        y3 = y2 + t * a;
        x2 = x2 - t * -b;
        y2 = y2 - t * a;
        PSLine(fd, x1, y1, x2, y2);
        PSLine(fd, x2, y2, x3, y3);
        PSLine(fd, x3, y3, x1, y1);
      }
    }
  
  PSFooter(fd);
  fclose(fd);
} /* PSGraphe() */

/* ====================================================================== */
/*! \fn void EPSGraphe(graphe * g, char *filename, double r, double t, double marge, int noms_sommets, int v_sommets, int col_sommets, int v_arcs ) 
    \param g (entrée) : un graphe.
    \param filename (entrée) : nom du fichier postscript à générer.
    \param r (entrée) : rayon des cercles qui représentent les sommets (0 pour ne pas les dessiner).
    \param t (entrée) : taille (demi-longueur) des flèches pour les arcs (0 pour ne pas les dessiner).
    \param marge (entrée) : marge en x et en y.
    \param noms_sommets (entrée) : booléen indiquant s'il faut écrire les noms des sommets.
    \param v_sommets (entrée) : booléen indiquant s'il faut écrire les valeurs des sommets.
    \param col_sommets (entrée) : booléen indiquant s'il faut colorier les sommets.
    \param v_arcs (entrée) : booléen indiquant s'il faut écrire les valeurs des arcs.
    \brief génère une figure PostScript d'après la représentation "successeurs" du graphe g. 
*/
void EPSGraphe(graphe * g, char *filename, double r, double t, double marge, int noms_sommets, int v_sommets, int col_sommets, int v_arcs) 
/* ====================================================================== */
{
  int i, j, n = g->nsom;
  double xmin, xmax, ymin, ymax;
  double x1, y1, x2, y2, x3, y3, x, y, a, b, d;
  pcell p;
  FILE * fd = NULL;
  char buf[80];
  
  if (g->gamma == NULL) 
  {  fprintf(stderr, "PSGraphe: representation successeurs absente\n");
     return;
  }
  
  if (g->x == NULL) 
  {  fprintf(stderr, "PSGraphe: coordonnees des sommets absentes\n");
     return;
  }

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "PSGraphe: cannot open file: %s\n", filename);
    return;
  }

  /* determine le rectangle englobant et genere le header */
  xmin = xmax = g->x[0];
  ymin = ymax = g->y[0];
  for (i = 1; i < n; i++) 
  {
    if (g->x[i] < xmin) xmin = g->x[i]; else if (g->x[i] > xmax) xmax = g->x[i];
    if (g->y[i] < ymin) ymin = g->y[i]; else if (g->y[i] > ymax) ymax = g->y[i];
  }
  EPSHeader(fd, xmax - xmin + 2.0 * marge, ymax - ymin + 2.0 * marge, 1.0, 14);
  
  /* dessine les sommets */
  for (i = 0; i < n; i++) 
    if (col_sommets && (g->v_sommets[i] != 0)) 
      PSDrawdisc(fd, g->x[i]-xmin+marge, g->y[i]-ymin+marge, r);
    else
      PSDrawcircle(fd, g->x[i]-xmin+marge, g->y[i]-ymin+marge, r);

  if (noms_sommets && g->nomsommet)
    for (i = 0; i < n; i++) 
      PSString(fd, g->x[i]-xmin+marge+2*r, g->y[i]-ymin+marge-2*r, g->nomsommet[i]);
  if (v_sommets)
    for (i = 0; i < n; i++) 
    {
      sprintf(buf, "%g", (double)(g->v_sommets[i]));      
      PSString(fd, g->x[i]-xmin+marge+2*r, g->y[i]-ymin+marge+2*r, buf);
    }

  /* dessine les arcs */
  if (r > 0.0)
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      PSLine(fd, g->x[i]-xmin+marge, g->y[i]-ymin+marge, g->x[j]-xmin+marge, g->y[j]-ymin+marge);
    }

  /* dessine les fleches sur les arcs */
  if (t > 0.0)
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      x1 = g->x[i]-xmin+marge;
      y1 = g->y[i]-ymin+marge;
      x2 = g->x[j]-xmin+marge;
      y2 = g->y[j]-ymin+marge;
      x = (x2 + x1) / 2; // milieu de l'arc
      y = (y2 + y1) / 2;
      if (v_arcs)
      {
        sprintf(buf, "%g", (double)(p->v_arc));      
        PSString(fd, x + r, y, buf);
      }
      a = x2 - x1;
      b = y2 - y1;             /* (a,b) est un vecteur directeur de l'arc */
      d = sqrt(a * a + b * b); /* longueur de l'arc */
      if (d > 1) // sinon on ne dessine pas la fleche
      { 
        a /= d; b /= d;          /* norme le vecteur */
        x1 = x + 2 * t * a;
        y1 = y + 2 * t * b;      /* pointe de la fleche */
        x2 = x - 2 * t * a;
        y2 = y - 2 * t * b;      /* base de la fleche */
        x3 = x2 + t * -b;        /* (-b,a) est normal a (a,b) */
        y3 = y2 + t * a;
        x2 = x2 - t * -b;
        y2 = y2 - t * a;
        PSLine(fd, x1, y1, x2, y2);
        PSLine(fd, x2, y2, x3, y3);
        PSLine(fd, x3, y3, x1, y1);
      }
    }
  
  PSFooter(fd);
  fclose(fd);
} /* EPSGraphe() */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE PLONGEMENT DE GRAPHES DANS LE PLAN */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn void AutoNomsSommets(graphe * g, int mode)
    \param g (entrée) : un graphe.
    \param mode (entrée) : 1 pour que les noms soient les indices des sommets,
                           2 pour que les noms soient les valeurs associées aux sommets,
                           3 pour des noms composes de l'indice et de la valeur.
    \brief génère automatiquement des noms pour les sommets du graphe g. 
*/
void AutoNomsSommets(graphe * g, int mode)
/* ====================================================================== */
{
  int len, i, n = g->nsom;
  char buf[80];

  g->nomsommet = (char **)malloc(n * sizeof(char *));
  if (g->nomsommet == NULL)
  {   fprintf(stderr, "AutoNomsSommets : malloc failed\n");
      exit(0);
  }

  if ((mode < 1) || (mode > 3))
  {   fprintf(stderr, "AutoNomsSommets : bad mode : %d\n", mode);
      exit(0);
  }

  for (i = 0; i < n; i++)
  {
    if (mode == 1)
      sprintf(buf, "%d", i);
    else if (mode == 2)
      sprintf(buf, "%g", (double)(g->v_sommets[i]));
    else
      sprintf(buf, "%d [%g]", i, (double)(g->v_sommets[i]));
    len = strlen(buf);
    g->nomsommet[i] = (char *)malloc((len+1) * sizeof(char));
    if (g->nomsommet[i] == NULL)
    {   fprintf(stderr, "AutoNomsSommets : malloc failed\n");
        exit(0);
    }
    strcpy(g->nomsommet[i], buf);
  }
} /* AutoNomsSommets() */

/* ====================================================================== */
/*! \fn void PlongementCirculaire(graphe * g, double r)
    \param g (entrée/sortie) : un graphe.
    \param r (entrée) : le rayon du cercle.
    \brief affecte à chaque sommet des coordonnées (x,y) régulierement réparties sur un cercle. 
*/
void PlongementCirculaire(graphe * g, double r)
/* ====================================================================== */
{
  int i, n = g->nsom;
  double alpha = 2 * M_PI / n;
  
  if (g->x == NULL) 
  {  fprintf(stderr, "PlongementCirculaire: coordonnees des sommets non allouees\n");
     return;
  }

  for (i = 0; i < n; i++) 
  {
    g->x[i] = cos(alpha * i) * r;
    g->y[i] = sin(alpha * i) * r;
  }
} /* PlongementCirculaire() */

/* ====================================================================== */
/*! \fn void PlongementRadial(graphe * g, int c)
    \param g (entrée/sortie) : un graphe.
    \param c (entrée) : le sommet à placer au centre.
    \brief répartit les sommets de g sur des cercles concentriques en fonction de leur rang, avec le sommet c au centre. 
*/
void PlongementRadial(graphe * g, int c)
/* ====================================================================== */
{
  int i, k, n = g->nsom;
  int *l, *t, *tt;    /* listes pour un parcours en largeur d'abord */
  int size_l, size_t; /* tailles des listes */
  int *r;             /* pour les rangs des sommets */
  int currank;        /* rang courant */
  double *beg_sector; /* angles associes aux sommets : debut secteur */
  double *end_sector; /* angles associes aux sommets : fin secteur */
  int v, nv, nl;
  pcell p;
  double theta, theta2;

  l = (int *)malloc(n * sizeof(int));
  t = (int *)malloc(n * sizeof(int));
  r = (int *)malloc(n * sizeof(int));
  beg_sector = (double *)malloc(n * sizeof(double));
  end_sector = (double *)malloc(n * sizeof(double));
  if (!l || !t || !r || !beg_sector || !end_sector)
  {  fprintf(stderr, "PlongementRadial: malloc a echoue\n");
     return;
  }

  if (g->x == NULL) 
  {  fprintf(stderr, "PlongementRadial: coordonnees des sommets non allouees\n");
     return;
  }

  g->x[c] = g->y[c] = 0;             /* place le point c au centre */
  for (i = 0; i < n; i++) r[i] = -1; /* initialise les rangs a -1 */
  r[c] = 0;                          /* c a pour rang 0 */
  currank = 0;                       /* rang courant = rang de c */
  beg_sector[c] = 0;
  end_sector[c] = 2 * M_PI;
  l[0] = c;  
  size_l = 1;
  while (size_l)                     /* boucle sur les niveaux */
  {
    currank += 1;
    size_t = 0;
    for (i = 0; i < size_l; i++)
    {
      v = l[i];                      /* sommet courant */
      nl = 0;                        /* init number of not yet ranked neibourghs */
      for (p = g->gamma[v]; p != NULL; p = p->next)
      {                              /* vertex loop - look for not yet ranked neibourghs */
        nv = p->som;
        if (r[nv] == -1)             /* not yet ranked */
	{
          nl += 1;
          r[nv] = currank;
          t[size_t] = nv;
          size_t += 1;
	}
      } /* for (p = g->gamma[v]; p != NULL; p = p->next) */

      if (nl > 0)
      {
        theta = (end_sector[v] - beg_sector[v]) / nl;
        theta = min(theta, 2 * M_PI / (currank + 1));
        k = 0;
        for (p = g->gamma[v]; p != NULL; p = p->next)
        {                              /* vertex loop - calculate embedding */
          nv = p->som;
          if (r[nv] == currank)
	  {
            beg_sector[nv] = beg_sector[v] + (k * theta);
            end_sector[nv] = beg_sector[nv] + theta;
            theta2 = beg_sector[nv] + ((end_sector[nv] - beg_sector[nv]) / 2);
            g->x[nv] = currank * 50 * cos(theta2);
            g->y[nv] = currank * 50 * sin(theta2);
            r[nv] = 0;
            k = k + 1;
	  } /* if (r[nv] == currank) */
        } /* for (p = g->gamma[v]; p != NULL; p = p->next) */
      } /* if (nl > 0) */
    } /* for (i = 0; i < size_l; i++) */

    tt = l; l = t; t = tt;
    size_l = size_t;
  } /* while (size_l) */

  free(l);
  free(t);
  free(r);
  free(beg_sector);
  free(end_sector);
} /* PlongementRadial() */
