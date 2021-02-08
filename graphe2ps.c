#include <stdlib.h>
#include <stdio.h>
#include "graphaux.h"
#include "graphes.h"

#define USAGE "lit un graphe dans le fichier <filename> et genere une figure en PostScript dans <filename>.eps"

int main(int argc, char **argv)
{
  graphe *g = NULL;
  char buf[256];

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <filename>\n%s\n", argv[0], USAGE);
    exit(0);
  }

  g = ReadGraphe(argv[1]);                 /* lit le graphe a partir du fichier */
  if (g == NULL)
  {
    fprintf(stderr, "usage: %s file not found %s\n", argv[0], argv[1]);
    exit(0);
  }

  sprintf(buf, "%s.eps", argv[1]);         /* construit le nom du fichier PostScript */
  EPSGraphe(g, buf, 3, 0, 60, 0, 0, 0, 0); /* genere une figure en PostScript */

  TermineGraphe(g);

  return 0;
} /* main() */
