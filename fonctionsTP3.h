/*! \file fonctionsTP3.h
    \brief prototypes utilisées pour le TP3
	*/

#include "graphaux.h"
#include "graphes.h"

#define _fonctionsTP3_h

void Dijkstra(graphe * g, int x);
graphe * SP(graphe * g, int x, int a);
void DijkstraBetter(graphe * g, int x);