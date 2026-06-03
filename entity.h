#ifndef ENTITY_H
#define ENTITY_H

/* ============================================================
 * entity.h — Interface générique d'une entité de combat
 *
 * Technique : garde d'inclusion (#ifndef / #define / #endif)
 * Permet d'inclure ce fichier plusieurs fois sans double
 * déclaration. Le préprocesseur ignore le contenu si
 * ENTITY_H est déjà défini.
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------
 * Constantes globales via #define
 * Pas de variable, pas de mémoire allouée — substitution texte
 * pure par le préprocesseur avant compilation.
 * ------------------------------------------------------------ */
#define MAX_NAME_LEN   32
#define BASE_DAMAGE    10
#define CRIT_MULTIPLIER 2

/* ------------------------------------------------------------
 * Déclaration anticipée (forward declaration)
 * Entity est utilisé dans ses propres pointeurs de fonction
 * donc on doit le déclarer avant de le définir.
 * ------------------------------------------------------------ */
typedef struct Entity Entity;

/* ------------------------------------------------------------
 * Table de fonctions virtuelles (vtable)
 * Equivalent C du polymorphisme orienté objet.
 * Chaque type (Mage, Guerrier) aura SA propre vtable
 * avec ses implémentations spécifiques.
 * ------------------------------------------------------------ */
typedef struct {
    /* Attaque : retourne les dégâts infligés */
    int   (*attack)(Entity *self, Entity *target);

    /* Défense : retourne les dégâts absorbés */
    int   (*defend)(Entity *self, int incoming_damage);

    /* Compétence spéciale propre à chaque type */
    void  (*special)(Entity *self, Entity *target);

    /* Affichage des stats — chaque type a son format */
    void  (*print_stats)(const Entity *self);

    /* Destructeur — libère la mémoire des données privées */
    void  (*destroy)(Entity *self);
} EntityVTable;

/* ------------------------------------------------------------
 * Structure générique Entity
 * void *data pointe vers les données spécifiques
 * (MageData ou WarriorData) sans que Entity ait besoin
 * de connaître leur type exact.
 * ------------------------------------------------------------ */
struct Entity {
    char          name[MAX_NAME_LEN];
    int           hp;
    int           max_hp;
    int           level;
    void         *data;       /* données privées du type     */
    EntityVTable *vtable;     /* table de fonctions virtuelles */
};

/* ------------------------------------------------------------
 * Macros utilitaires
 * Le ## colle deux tokens, le # transforme en chaîne.
 * ------------------------------------------------------------ */
#define ENTITY_IS_ALIVE(e)   ((e)->hp > 0)
#define ENTITY_ATTACK(e, t)  ((e)->vtable->attack((e), (t)))
#define ENTITY_DEFEND(e, d)  ((e)->vtable->defend((e), (d)))
#define ENTITY_SPECIAL(e, t) ((e)->vtable->special((e), (t)))
#define ENTITY_STATS(e)      ((e)->vtable->print_stats((e)))
#define ENTITY_DESTROY(e)    ((e)->vtable->destroy((e)))

/* ------------------------------------------------------------
 * Fonctions génériques déclarées ici, définies dans entity.c
 * Elles fonctionnent sur N'IMPORTE quelle Entity grâce
 * aux pointeurs de fonction dans la vtable.
 * ------------------------------------------------------------ */
void entity_take_damage(Entity *self, int damage);
void entity_heal(Entity *self, int amount);
void entity_fight(Entity *attacker, Entity *defender);
void entity_print_health(const Entity *self);

#endif /* ENTITY_H */
