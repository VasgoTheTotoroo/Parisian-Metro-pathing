#include "fonctionsTP3.h"

#define USAGE "lit un graphe dans le fichier <filename> et affiche le chemin le plus court entre le sommet renseigné et tous les autres sommets"

/* ====================================================================== */
int main(int argc, char **argv)
/* ====================================================================== */
{
  graphe *g;
  pcell p;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s <filename>\n%s\n", argv[0], USAGE);
    exit(0);
  }

  g = ReadGraphe(argv[1]);             /* lit le graphe a partir du fichier */
  Dijkstra(g, atoi(argv[2]));
  AfficheValeursSommets(g);
  TermineGraphe(g);

  return 0;
} /* main() */