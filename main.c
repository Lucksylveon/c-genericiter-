/* ============================================================
 * main.c — Démonstration du système générique
 *
 * POINT CLÉ : ce fichier ne connaît que Entity*.
 * Il ne sait pas si c'est un Mage ou un Guerrier.
 * Toute la logique passe par les pointeurs de fonction.
 * ============================================================ */

#include "mage.h"
#include "warrior.h"

/* ------------------------------------------------------------
 * Inclusion multiple sécurisée :
 * mage.h    inclut entity.h  (#ifndef ENTITY_H → traité)
 * warrior.h inclut entity.h  (#ifndef ENTITY_H → IGNORÉ !)
 * Sans les gardes, entity.h serait déclaré deux fois → erreur.
 * ------------------------------------------------------------ */

/* ------------------------------------------------------------
 * Macro de compilation conditionnelle
 * Active des affichages de debug si DEBUG est défini.
 * Compiler avec : gcc -DDEBUG ...
 * ------------------------------------------------------------ */
#ifdef DEBUG
    #define DBG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG(fmt, ...) /* rien en production */
#endif

/* ------------------------------------------------------------
 * tournament — Fait combattre un tableau d'Entity*
 * Sans jamais connaître le type de chaque combattant !
 *
 * C'est la beauté du générique en C :
 * void *data + pointeurs de fonction = polymorphisme pur.
 * ------------------------------------------------------------ */
void tournament(Entity **fighters, int count) {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║         TOURNOI DES LÉGENDES         ║\n");
    printf("╚══════════════════════════════════════╝\n\n");

    /* Affiche les stats de tous les combattants */
    printf("── Participants ──────────────────────\n\n");
    for (int i = 0; i < count; i++) {
        ENTITY_STATS(fighters[i]);  /* Macro générique */
        printf("\n");
    }

    /* Chaque combattant affronte le suivant */
    for (int i = 0; i + 1 < count; i += 2) {
        DBG("Combat %d : %s vs %s", i/2+1,
            fighters[i]->name, fighters[i+1]->name);

        /* entity_fight ne sait pas ce que sont les entités */
        entity_fight(fighters[i], fighters[i + 1]);

        /* Recharge les HP du vainqueur pour le prochain round */
        Entity *winner = ENTITY_IS_ALIVE(fighters[i])
                         ? fighters[i] : fighters[i+1];
        winner->hp = winner->max_hp / 2; /* demi vie pour la suite */
    }
}

/* ------------------------------------------------------------
 * reset_entity — Remet une entité à ses HP max
 * Fonctionne sur n'importe quel type grâce à Entity*
 * ------------------------------------------------------------ */
void reset_entity(Entity *e) {
    e->hp = e->max_hp;
}

int main(void) {

    /* --- Création des combattants ---
     * On obtient des Entity* — on ne manipule que ça ensuite */
    Entity *gandalf  = mage_create("Gandalf",   5);
    Entity *saruman  = mage_create("Saruman",   4);
    Entity *aragorn  = warrior_create("Aragorn", 5);
    Entity *sauron   = warrior_create("Sauron",  6);

    if (!gandalf || !saruman || !aragorn || !sauron) {
        fprintf(stderr, "Erreur allocation mémoire\n");
        return 1;
    }

    /* --- Combat 1 : Mage vs Guerrier ---
     * entity_fight reçoit deux Entity* — c'est tout.
     * Pas de if(est_mage), pas de switch. */
    printf("\n═══ COMBAT 1 : Mage vs Guerrier ═══\n");
    entity_fight(gandalf, aragorn);

    /* Reset pour le tournoi */
    reset_entity(gandalf);
    reset_entity(aragorn);

    /* --- Combat 2 : Guerrier vs Guerrier --- */
    printf("\n═══ COMBAT 2 : Guerrier vs Guerrier ═══\n");
    entity_fight(aragorn, sauron);

    reset_entity(aragorn);
    reset_entity(sauron);

    /* --- Combat 3 : Mage vs Mage --- */
    printf("\n═══ COMBAT 3 : Mage vs Mage ═══\n");
    entity_fight(gandalf, saruman);

    reset_entity(gandalf);
    reset_entity(saruman);
    reset_entity(aragorn);
    reset_entity(sauron);

    /* --- Tournoi générique ---
     * Un tableau de Entity* mélangé : mages et guerriers.
     * tournament() ne fait aucune distinction. */
    Entity *fighters[] = { gandalf, sauron, saruman, aragorn };
    tournament(fighters, 4);

    /* --- Libération mémoire via vtable ---
     * destroy() est spécifique à chaque type mais appelé
     * de façon identique grâce aux pointeurs de fonction. */
    ENTITY_DESTROY(gandalf);
    ENTITY_DESTROY(saruman);
    ENTITY_DESTROY(aragorn);
    ENTITY_DESTROY(sauron);

    printf("Mémoire libérée proprement.\n");
    return 0;
}
