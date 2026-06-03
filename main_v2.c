/* ============================================================
 * main_v2.c — Démonstration de la généricité améliorée
 *
 * AMÉLIORATION CLÉ vs v1 :
 * Ce fichier inclut UNIQUEMENT entity_v2.h
 * Il ne connaît pas mage_v2.h ni warrior_v2.h
 * Il n'appelle jamais mage_create() ni warrior_create()
 * Il utilise entity_create() pour tout — une seule fonction !
 * ============================================================ */

#include "entity_v2.h"

/* Notez l'absence de :
 * #include "mage_v2.h"      ← absent !
 * #include "warrior_v2.h"   ← absent !
 *
 * main.c ne sait pas que Mage et Warrior existent en tant
 * que types C — il ne voit que des Entity*. */

int main(void) {

    printf("══════════════════════════════════════\n");
    printf("  DÉMONSTRATION GÉNÉRICITÉ v2\n");
    printf("  (main.c ne connaît que entity_v2.h)\n");
    printf("══════════════════════════════════════\n\n");

    /* ----------------------------------------------------------
     * Création via entity_create() unique
     * Même appel pour tous les types — seul l'enum change.
     * ---------------------------------------------------------- */
    Entity *gandalf = entity_create(ENTITY_TYPE_mage,    "Gandalf", 5);
    Entity *saruman = entity_create(ENTITY_TYPE_mage,    "Saruman",  4);
    Entity *aragorn = entity_create(ENTITY_TYPE_warrior, "Aragorn",  5);
    Entity *sauron  = entity_create(ENTITY_TYPE_warrior, "Sauron",   6);

    if (!gandalf || !saruman || !aragorn || !sauron) {
        fprintf(stderr, "Erreur allocation\n");
        return 1;
    }

    /* ----------------------------------------------------------
     * Affichage du type via entity_print_type()
     * entity_v2.h expose le tableau entity_type_labels[]
     * généré depuis ENTITY_TYPES — aucun if/switch ici.
     * ---------------------------------------------------------- */
    printf("── Types des combattants ─────────────\n");
    entity_print_type(gandalf);
    entity_print_type(aragorn);
    printf("\n");

    /* ----------------------------------------------------------
     * Stats — appel identique peu importe le type
     * ---------------------------------------------------------- */
    printf("── Stats ─────────────────────────────\n");
    Entity *all[] = { gandalf, saruman, aragorn, sauron };
    int count = 4;
    for (int i = 0; i < count; i++) {
        ENTITY_STATS(all[i]);
        printf("\n");
    }

    /* ----------------------------------------------------------
     * Combats — entity_fight() ne sait toujours pas
     * ce que sont les entités, comme en v1.
     * ---------------------------------------------------------- */
    printf("── Combat : Mage vs Guerrier ─────────\n");
    entity_fight(gandalf, aragorn);

    gandalf->hp = gandalf->max_hp;
    aragorn->hp = aragorn->max_hp;

    printf("── Combat : Guerrier vs Guerrier ─────\n");
    entity_fight(aragorn, sauron);

    saruman->hp = saruman->max_hp;
    sauron->hp  = sauron->max_hp;

    printf("── Combat : Mage vs Mage ─────────────\n");
    entity_fight(gandalf, saruman);

    /* ----------------------------------------------------------
     * Démonstration : tableau générique mixte
     * On ne sait pas ce que contient le tableau — Entity* !
     * ---------------------------------------------------------- */
    printf("── Tournoi générique ─────────────────\n");
    for (int i = 0; i < count; i++) {
        all[i]->hp = all[i]->max_hp;
    }
    for (int i = 0; i + 1 < count; i += 2) {
        entity_fight(all[i], all[i + 1]);
        Entity *winner = ENTITY_IS_ALIVE(all[i]) ? all[i] : all[i+1];
        winner->hp = winner->max_hp / 2;
    }

    /* ----------------------------------------------------------
     * Libération — toujours via vtable, toujours générique
     * ---------------------------------------------------------- */
    for (int i = 0; i < count; i++)
        ENTITY_DESTROY(all[i]);

    printf("Mémoire libérée proprement.\n");
    return 0;
}
