#include <stdlib.h>
#include <stdio.h>
#include "graphaux.h"
#include "graphes.h"

int main()
{
  ListeChainee L = listeVide();
  LCell *p, *el;
  int val;

  afficheListe(L);  
    
  ajouteElmt(&L, 2);
  ajouteElmt(&L, 1);
  ajouteElmt(&L, 0);
  ajouteElmt(&L, 4);
  ajouteElmt(&L, 3);
  ajouteElmt(&L, 5);

  afficheListe(L);

  // supprime le premier element de la liste (suppress the first element of the list)
  el = L; /* el est le premier element de la liste */
  supprimeLCell(&L, el, NULL);
  
  afficheListe(L);

  // supprime le deuxieme element de la liste (suppress the second element of the list)
  p = L; /* p est le premier element de la liste (p is then the first element of the list) */
  el = p->next;  /* el est donc le deuxieme element de la liste (el is then the second element of the list) */
  val = supprimeLCell(&L, el, p);
 
  afficheListe(L);

  /* Supprime tous les éléments de la liste (suppress all the elements of the list) */
  
  while(!estListeVide(L)) {
    el = L;
    supprimeLCell(&L, el, NULL);
    afficheListe(L);
  }

  return 0;
} /* main() */
