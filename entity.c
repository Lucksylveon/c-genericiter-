/* ============================================================
 * entity.c — Fonctions génériques qui fonctionnent sur
 * N'IMPORTE quelle Entity sans connaître son type réel.
 *
 * C'est ici que réside toute la puissance du générique en C :
 * ces fonctions ne savent pas si elles manipulent un Mage
 * ou un Guerrier — elles s'en fichent complètement.
 * ============================================================ */

#include "entity.h"

/* ------------------------------------------------------------
 * entity_take_damage
 * Applique des dégâts à une entité de façon générique.
 * Appelle d'abord la défense spécifique au type via vtable.
 * ------------------------------------------------------------ */
void entity_take_damage(Entity *self, int damage) {
    if (!ENTITY_IS_ALIVE(self)) return;

    /* La défense est spécifique au type (armure guerrier,
     * bouclier magique mage) — on passe par la vtable */
    int absorbed = ENTITY_DEFEND(self, damage);
    int real_damage = damage - absorbed;
    if (real_damage < 1) real_damage = 1; /* minimum 1 dégât */

    self->hp -= real_damage;
    if (self->hp < 0) self->hp = 0;

    printf("  %s reçoit %d dégâts (%d absorbés) — HP: %d/%d\n",
           self->name, real_damage, absorbed,
           self->hp, self->max_hp);
}

/* ------------------------------------------------------------
 * entity_heal
 * Soigne une entité sans dépasser son maximum.
 * ------------------------------------------------------------ */
void entity_heal(Entity *self, int amount) {
    if (!ENTITY_IS_ALIVE(self)) return;
    self->hp += amount;
    if (self->hp > self->max_hp) self->hp = self->max_hp;
    printf("  %s récupère %d HP — HP: %d/%d\n",
           self->name, amount, self->hp, self->max_hp);
}

/* ------------------------------------------------------------
 * entity_print_health
 * Affiche une barre de vie générique.
 * ------------------------------------------------------------ */
void entity_print_health(const Entity *self) {
    int bar_len = 20;
    int filled  = (self->hp * bar_len) / self->max_hp;
    printf("  [");
    for (int i = 0; i < bar_len; i++)
        printf(i < filled ? "#" : "-");
    printf("] %d/%d HP\n", self->hp, self->max_hp);
}

/* ------------------------------------------------------------
 * entity_fight — LE COEUR DU GENERIQUE
 *
 * Cette fonction fait combattre deux entités sans JAMAIS
 * savoir ce qu'elles sont. Mage vs Guerrier, Guerrier vs
 * Guerrier, Mage vs Mage — tout fonctionne identiquement.
 *
 * Paramètres : deux Entity* — c'est tout.
 * Pas de switch(type), pas de if(est_un_mage)...
 * Tout passe par les pointeurs de fonction de la vtable.
 * ------------------------------------------------------------ */
void entity_fight(Entity *attacker, Entity *defender) {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║  %-18s  VS  %-12s║\n", attacker->name, defender->name);
    printf("╚══════════════════════════════════════╝\n\n");

    int round = 1;

    while (ENTITY_IS_ALIVE(attacker) && ENTITY_IS_ALIVE(defender)) {
        printf("── Round %d ──────────────────────────\n", round++);

        /* --- Tour de l'attaquant --- */
        printf("[%s attaque]\n", attacker->name);

        /* Utilise parfois la compétence spéciale */
        if (round % 3 == 0) {
            printf("  → Compétence spéciale !\n");
            ENTITY_SPECIAL(attacker, defender);
        } else {
            /* Attaque générique via vtable */
            int dmg = ENTITY_ATTACK(attacker, defender);
            entity_take_damage(defender, dmg);
        }

        if (!ENTITY_IS_ALIVE(defender)) break;

        /* --- Tour du défenseur --- */
        printf("[%s contre-attaque]\n", defender->name);
        int dmg = ENTITY_ATTACK(defender, attacker);
        entity_take_damage(attacker, dmg);

        /* Affichage des barres de vie */
        printf("\n");
        printf("  %s : ", attacker->name);
        entity_print_health(attacker);
        printf("  %s : ", defender->name);
        entity_print_health(defender);
        printf("\n");

        /* Sécurité anti-boucle infinie */
        if (round > 50) {
            printf("  [Match nul après 50 rounds !]\n");
            return;
        }
    }

    /* Annonce du vainqueur — toujours générique */
    Entity *winner = ENTITY_IS_ALIVE(attacker) ? attacker : defender;
    Entity *loser  = ENTITY_IS_ALIVE(attacker) ? defender : attacker;
    printf("══════════════════════════════════════\n");
    printf("  🏆 %s remporte le combat !\n", winner->name);
    printf("  💀 %s est vaincu.\n", loser->name);
    printf("══════════════════════════════════════\n\n");
}
