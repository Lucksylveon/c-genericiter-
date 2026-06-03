#ifndef WARRIOR_H
#define WARRIOR_H

/* ============================================================
 * warrior.h — Déclarations spécifiques au Guerrier
 * ============================================================ */

#include "entity.h"

/* ------------------------------------------------------------
 * Données privées du Guerrier
 * ------------------------------------------------------------ */
typedef struct {
    int armor;          /* réduit les dégâts reçus            */
    int rage;           /* s'accumule en prenant des coups    */
    int max_rage;
    int strength;       /* bonus aux dégâts physiques         */
} WarriorData;

/* ------------------------------------------------------------
 * Constantes spécifiques au Guerrier
 * ------------------------------------------------------------ */
#define WARRIOR_BASE_HP       150
#define WARRIOR_BASE_ARMOR    8
#define WARRIOR_BASE_STRENGTH 5
#define WARRIOR_MAX_RAGE      100
#define WARRIOR_RAGE_PER_HIT  20
#define WARRIOR_BERSERK_COST  60   /* rage nécessaire pour berserk */
#define WARRIOR_BERSERK_MULT  3    /* multiplicateur dégâts berserk */

/* ------------------------------------------------------------
 * Constructeur — retourne un Entity* générique
 * ------------------------------------------------------------ */
Entity *warrior_create(const char *name, int level);

#endif /* WARRIOR_H */
