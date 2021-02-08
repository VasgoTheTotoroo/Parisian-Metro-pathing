/*! \file graphaux.h
    \brief structures auxiliaires
*/
#include <stdio.h>
#include <string.h>
/* 
#include <errno.h> 
#include <sys/types.h>
#include <unistd.h> 
*/
#include <stdlib.h>
#include <sys/time.h>

typedef char boolean;

#define TRUE 1
#define FALSE 0

/* ===================================== */
/* GESTION DE PILE */
/* ===================================== */

/*! \struct Lifo
    \brief structure de pile (Last In, First Out).
*/
typedef struct Lifo {
//! taille max de la Lifo
  int Max;
//! index de pile (pointe la 1ere case libre)
  int Sp;
//! tableau pour la pile (re-dimensionné dynamiquement)
  int Pts[1];
} Lifo;


/*! \struct Lifo
    \brief structure de Liste chainee (linked list).
*/
typedef struct LCell {
  int elmt;
  LCell * next;
}LCell;

typedef struct LCell* ListeChainee;

/* prototypes     */
Lifo * CreeLifoVide( int taillemax );
void LifoFlush( Lifo * L );
boolean LifoVide( Lifo * L );
int LifoPop( Lifo * L );
void LifoPush( Lifo * L, int V );
void LifoPrint( Lifo * L );
void LifoPrintLine( Lifo * L );
void LifoTermine( Lifo * L );

ListeChainee listeVide();
void ajouteElmt(ListeChainee *L, int elmt);
int supprimeLCell(ListeChainee *L, LCell *removedCell, LCell *previousCell);
void afficheListe(ListeChainee L);
boolean estListeVide(ListeChainee L);


/* ===================================== */
/* TRI */
/* ===================================== */

#define TypeCle long

/* prototypes     */
void TriRapideStochastique (int * A, TypeCle *T, int p, int r);

/* ===================================== */
/* MESURE DE TEMPS */
/* ===================================== */

#ifdef DEFTIMEVAL
struct timeval {
  unsigned long	tv_sec;		/* seconds */
  long		tv_usec;	/* and microseconds */
};

struct timezone {
  int	tz_minuteswest;	/* minutes west of Greenwich */
  int	tz_dsttime;	/* type of dst correction */
};
#endif

/*! \var chrono
    \brief strucure pour la mesure du temps
*/
typedef struct timeval chrono;

/* prototypes     */
void start_chrono( chrono *tp );
int read_chrono( chrono *tp );

/* ===================================== */
/* GENERATION DE POSTSCRIPT */
/* ===================================== */

/* prototypes     */

void PSHeader(FILE *fd, double figure_width, double figure_height, double line_width, int font_size );
void EPSHeader(FILE *fd, double figure_width, double figure_height, double line_width, int font_size );
void PSMove (FILE *fd, double x, double y);
void PSDraw (FILE *fd, double x, double y);
void PSLine (FILE *fd, double xd, double yd, double xf, double yf);
void PSDrawcircle (FILE *fd, double x, double y, double r);
void PSDrawdisc (FILE *fd, double x, double y, double r);
void PSString (FILE *fd, double x, double y, char *s);
void PSFooter(FILE *fd);

/* ===================================== */
/* ENSEMBLES */
/* ===================================== */

boolean * EnsembleVide(int n);
