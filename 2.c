#include "fonctionsTP3.h"

#define USAGE "lit un graphe dans le fichier <filename> et créer un graphe en postscript du chemin le plus court entre les 2 sommets renseignés"

/* ====================================================================== */
int main(int argc, char **argv)
/* ====================================================================== */
{
  graphe *g, *path;
  char buf[256];

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s <filename>\n%s\n", argv[0], USAGE);
    exit(0);
  }

  g = ReadGraphe(argv[1]);             /* lit le graphe a partir du fichier */
  path = SP(g, atoi(argv[2]), atoi(argv[3]));
  AfficheSuccesseurs(path);
  
  sprintf(buf, "%s_from_%d_to_%d.eps", argv[1], atoi(argv[2]), atoi(argv[3])); /* construit le nom du fichier PostScript */
  EPSGraphe(path, buf, 3, 0, 60, 0, 0, 0, 0); /* genere une figure en PostScript */
  
  TermineGraphe(g);
  TermineGraphe(path);
  
  return 0;
} /* main() */