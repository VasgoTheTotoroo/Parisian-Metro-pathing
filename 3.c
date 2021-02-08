#include "fonctionsTP3.h"

#define USAGE "lit un graphe dans le fichier <filename> et affiche le chemin le plus court entre le sommet renseigné et tous les autres sommets"

/* ====================================================================== */
int main(int argc, char **argv)
/* ====================================================================== */
{
  graphe *g;
  chrono Chrono1, Chrono2; /* pour la mesure du temps */
  int temps1, temps2;      /* idem */

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s <filename>\n%s\n", argv[0], USAGE);
    exit(0);
  }

  g = ReadGraphe(argv[1]);             /* lit le graphe a partir du fichier */
  
  start_chrono(&Chrono1);        /* top chrono : debut du traitement */
  Dijkstra(g, atoi(argv[2]));
  temps1 = read_chrono(&Chrono1); /* top chrono : fin du traitement */
  
  AfficheValeursSommets(g);

  g = ReadGraphe(argv[1]);             /* lit le graphe a partir du fichier */
  
  start_chrono(&Chrono2);        /* top chrono : debut du traitement */
  DijkstraBetter(g, atoi(argv[2]));
  temps2 = read_chrono(&Chrono2); /* top chrono : fin du traitement */
  
  AfficheValeursSommets(g);
  TermineGraphe(g);
  
  printf("temps de traitement pour Dijkstra: %g secondes\n", ((double)temps1)/1e6);
  printf("temps de traitement pour DijkstraBetter: %g secondes\n", ((double)temps2)/1e6);
  
  // temps de traitement pour Dijkstra: 0.000393 secondes
  // temps de traitement pour DijkstraBetter: 0.000145 secondes

  return 0;
} /* main() */