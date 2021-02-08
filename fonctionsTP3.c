/*! \file fonctionsTP3.c
    \brief fonctions utilisées pour le TP3
	*/

#include "fonctionsTP3.h"

/* ====================================================================== */
/*! \fn void Dijkstra(graphe * g, int x)
    \param g (entrée) : a network. The length of each arc must be stored in
                        the field v_arc of the structure cell.
    \param x (entrée) : a vertex of g.
    \brief compute, for each vertex y of g, the length Dx(y)of a shortest path
           from x to y. This length is stored in the field 
           v_sommets of the structure g.
*/
void Dijkstra(graphe * g, int x)
/* ====================================================================== */
{
    int i, k, mu, n, y_star, y;
    boolean * S;
    TYP_VSOM * Lx;
    pcell p;
    long mini;
    
    n = g->nsom;
    Lx = g->v_sommets;
    S = EnsembleVide(n);
    
    //initialiser les longueurs à LONG_MAX et S à E
    for(y=0;y<n;y++)
    {
        Lx[y] = LONG_MAX;
        S[y] = 1;
    }
    
    Lx[x] = 0;
    k = 0;
    mu = 0;
    
    while(k < n && mu != LONG_MAX)
    {
        mini = LONG_MAX;
        y_star = -1; //pour créer une erreur si il ne trouve aucun minimum mais normalement impossible

        //trouver le minimum dans Lx
        for(i=0;i<n;i++)
        {
            if (Lx[i] < mini && S[i])
            {
                y_star = i;
                mini = Lx[i];
            }//if (Lx[i] < Lx[y_star])
            
        }//for i
        
        S[y_star] = 0;
        k++;
        mu = Lx[y_star];
        
        for (p = g->gamma[y_star]; p != NULL; p = p->next)
        {
            y = p->som;  /* pour tout y, successeur de y* */
            if (S[y])
            {
                Lx[y] = min(Lx[y], Lx[y_star] + p->v_arc);
                
            }//if (S[y])
            
        }//for p
        
    }//while(k < n && mu != LONG_MAX)
        
}/* Dijkstra() */

/* ====================================================================== */
/*! \fn graphe * SP(graphe * g, int x, int y)
    \param g (entrée) : a network. The length of each arc must be stored in
                        the field v_arc of the structure cell.
    \param x (entrée) : a vertex of g (start).
    \param y (entrée) : a vertex of g (arrival).
    \return a shortest path from x to y in g.
    \brief returns a shortest path from x to y in g.
*/
graphe * SP(graphe * g, int x, int a)
/* ====================================================================== */
{
    //avec l'aide de https://fr.wikipedia.org/wiki/Algorithme_de_Dijkstra
    int i, k, mu, n, y_star, y, n_arcs;
    boolean * S;
    TYP_VSOM * Lx;
    pcell p;
    graphe * shortest_path;
    int * predes;
    long mini;
    
    n = g->nsom;
    n_arcs = g->narc;
    Lx = g->v_sommets;
    S = EnsembleVide(n);
    shortest_path = InitGraphe(n, n_arcs);
    predes = (int *) malloc(n * sizeof(int));
    
    shortest_path->x = g->x;
    shortest_path->y = g->y;
    
    //initialiser les longueurs à LONG_MAX et S à E
    for(y=0;y<n;y++)
    {
        Lx[y] = LONG_MAX;
        S[y] = 1;
    }
    
    Lx[x] = 0;
    k = 0;
    mu = 0;
    
    while(k < n && mu != LONG_MAX)
    {
        
        mini = LONG_MAX;
        y_star = -1; //pour créer une erreur si il ne trouve aucun minimum mais normalement impossible

        //trouver le minimum dans Lx
        for(i=0;i<n;i++)
        {
            if (Lx[i] < mini && S[i])
            {
                y_star = i;
                mini = Lx[i];
            }//if (Lx[i] < Lx[y_star])
            
        }//for i
        
        if(y_star == a)
        {            
            while(y_star != x)
            {
                AjouteArc(shortest_path, predes[y_star], y_star);
                y_star = predes[y_star];
                
            }//while(y_star != x)
            
            return shortest_path;
            
        }//if(y_star == a)
        
        S[y_star] = 0;
        
        k++;
        mu = Lx[y_star];
        
        for (p = g->gamma[y_star]; p != NULL; p = p->next)
        {
            y = p->som;  /* pour tout y, successeur de y* */
            if (S[y])
            {
                if (Lx[y] > Lx[y_star] + p->v_arc)
                {
                    Lx[y] = Lx[y_star] + p->v_arc;
                    predes[y] = y_star; //note notre chemin d'un sommet à l'autre
                    
                }//if (Lx[y] > Lx[y_star] + p->v_arc)
                
            }//if (S[y])
            
        }//for p
        
    }//while(k < n && mu != LONG_MAX)
    
}/* SP() */

/* ====================================================================== */
/*! \fn void DijkstraBetter(graphe * g, int x)
    \param g (entrée) : a network. The length of each arc must be stored in
                        the field v_arc of the structure cell.
    \param x (entrée) : a vertex of g.
    \brief compute, for each vertex y of g, the length Dx(y)of a shortest path
           from x to y. This length is stored in the field 
           v_sommets of the structure g.
*/
void DijkstraBetter(graphe * g, int x)
/* ====================================================================== */
{
    int i, k, mu, n, y_star, y;
    boolean * S;
    TYP_VSOM * Lx;
    pcell p;
    long mini;
    ListeChainee T = listeVide();
    LCell *el, *el_supr, *prev, *prev_supr;
    
    n = g->nsom;
    Lx = g->v_sommets;
    S = EnsembleVide(n);
    
    //initialiser les longueurs à LONG_MAX et S à E
    for(y=0;y<n;y++)
    {
        Lx[y] = LONG_MAX;
        S[y] = 1;
    }
    
    Lx[x] = 0;
    ajouteElmt(&T, x);
    k = 0;
    mu = 0;
    
    while(k < n && mu != LONG_MAX)
    {

        //trouver le minimum dans Lx
        el = T->next;
        prev = T;
        
        el_supr = T;
        prev_supr = NULL;
        mini = Lx[prev->elmt]; // Lx du premier élément
        
        while(el!=NULL)
        {
            if (Lx[el->elmt] < mini && S[el->elmt])
            {
                prev_supr = prev;
                el_supr = el;
                mini = Lx[el->elmt];
            }//if (Lx[el->elmt] < mini)
            
            el = el->next;
            prev = prev->next;
            
        }//while(el!=NULL)
        
        y_star = supprimeLCell(&T, el_supr, prev_supr);
        S[y_star] = 0;
        k++;
        mu = Lx[y_star];
        
        for (p = g->gamma[y_star]; p != NULL; p = p->next)
        {
            y = p->som;  /* pour tout y, successeur de y* */
            if (S[y])
            {
                Lx[y] = min(Lx[y], Lx[y_star] + p->v_arc);
                ajouteElmt(&T, y);
                
            }//if (S[y])
            
        }//for p
        
    }//while(k < n && mu != LONG_MAX)
        
}/* Dijkstra() */