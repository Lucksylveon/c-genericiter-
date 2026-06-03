#ifndef MAGE_H
#define MAGE_H

/* ============================================================
 * mage.h — Déclarations spécifiques au Mage
 *
 * Ce fichier inclut entity.h — la garde d'inclusion dans
 * entity.h garantit qu'il ne sera pas traité deux fois
 * même si mage.h et warrior.h sont tous deux inclus
 * dans le même fichier source.
 * ============================================================ */

#include "entity.h"

/* ------------------------------------------------------------
 * Données privées du Mage
 * Entity ne connaît que void *data — seul mage.c connaît
 * cette structure. C'est l'encapsulation en C pur.
 * ------------------------------------------------------------ */
typedef struct {
    int mana;
    int max_mana;
    int spell_power;    /* multiplicateur de dégâts magiques  */
    int mana_regen;     /* mana récupéré par tour             */
} MageData;

/* ------------------------------------------------------------
 * Constantes spécifiques au Mage via #define
 * ------------------------------------------------------------ */
#define MAGE_BASE_HP        80
#define MAGE_BASE_MANA      150
#define MAGE_BASE_SPELL_PWR 3
#define MAGE_MANA_REGEN     15
#define MAGE_FIREBALL_COST  30
#define MAGE_FIREBALL_DMG   45

/* ------------------------------------------------------------
 * Constructeur — retourne un Entity* générique
 * L'appelant n'a pas besoin de savoir que c'est un Mage.
 * ------------------------------------------------------------ */
Entity *mage_create(const char *name, int level);

#endif /* MAGE_H */
