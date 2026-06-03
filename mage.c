/* ============================================================
 * mage.c — Implémentation des comportements spécifiques
 * au Mage. Ces fonctions connaissent MageData mais sont
 * enregistrées dans la vtable comme pointeurs de fonction
 * génériques (prenant Entity*).
 * ============================================================ */

#include "mage.h"
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------
 * Fonctions statiques — visibles uniquement dans ce fichier.
 * C'est l'équivalent C des méthodes privées.
 * Leur signature correspond aux pointeurs de fonction
 * de EntityVTable.
 * ------------------------------------------------------------ */

static int mage_attack(Entity *self, Entity *target) {
    MageData *data = (MageData *)self->data;

    /* Régénération mana à chaque tour */
    data->mana += data->mana_regen;
    if (data->mana > data->max_mana) data->mana = data->max_mana;

    /* Dégâts basés sur spell_power et level */
    int damage = (BASE_DAMAGE + data->spell_power) * self->level;
    printf("  ✨ %s lance un sort ! (%d dégâts magiques, mana: %d/%d)\n",
           self->name, damage, data->mana, data->max_mana);
    return damage;
}

static int mage_defend(Entity *self, int incoming_damage) {
    /* Le Mage a un bouclier magique passif qui absorbe
     * un pourcentage des dégâts selon son level */
    int shield = self->level * 2;
    printf("  🛡 Bouclier magique absorbe %d dégâts\n", shield);
    return shield;
}

static void mage_special(Entity *self, Entity *target) {
    MageData *data = (MageData *)self->data;

    if (data->mana < MAGE_FIREBALL_COST) {
        printf("  ❌ %s n'a pas assez de mana pour Boule de Feu ! (%d/%d)\n",
               self->name, data->mana, MAGE_FIREBALL_COST);
        /* Attaque normale à la place */
        int dmg = mage_attack(self, target);
        entity_take_damage(target, dmg);
        return;
    }

    data->mana -= MAGE_FIREBALL_COST;
    int damage = MAGE_FIREBALL_DMG * self->level;
    printf("  🔥 %s lance BOULE DE FEU ! (%d dégâts, mana: %d/%d)\n",
           self->name, damage, data->mana, data->max_mana);
    entity_take_damage(target, damage);
}

static void mage_print_stats(const Entity *self) {
    MageData *data = (MageData *)self->data;
    printf("┌─────────────────────────────┐\n");
    printf("│ 🧙 MAGE : %-18s│\n", self->name);
    printf("│ Niveau      : %-14d│\n", self->level);
    printf("│ HP          : %d/%d%*s│\n",
           self->hp, self->max_hp,
           (int)(14 - snprintf(NULL,0,"%d/%d", self->hp, self->max_hp)), "");
    printf("│ Mana        : %d/%d%*s│\n",
           data->mana, data->max_mana,
           (int)(14 - snprintf(NULL,0,"%d/%d", data->mana, data->max_mana)), "");
    printf("│ Puissance   : %-14d│\n", data->spell_power);
    printf("│ Regen mana  : %-14d│\n", data->mana_regen);
    printf("└─────────────────────────────┘\n");
}

static void mage_destroy(Entity *self) {
    /* Libère les données privées puis la structure elle-même */
    free(self->data);
    free(self);
}

/* ------------------------------------------------------------
 * VTable statique du Mage
 * Une seule instance partagée par tous les Mages.
 * ------------------------------------------------------------ */
static EntityVTable mage_vtable = {
    .attack      = mage_attack,
    .defend      = mage_defend,
    .special     = mage_special,
    .print_stats = mage_print_stats,
    .destroy     = mage_destroy,
};

/* ------------------------------------------------------------
 * Constructeur public
 * Alloue et initialise un Entity configuré comme Mage.
 * Retourne Entity* — l'appelant ne voit jamais MageData.
 * ------------------------------------------------------------ */
Entity *mage_create(const char *name, int level) {
    Entity   *e    = malloc(sizeof(Entity));
    MageData *data = malloc(sizeof(MageData));

    if (!e || !data) {
        free(e); free(data);
        return NULL;
    }

    strncpy(e->name, name, MAX_NAME_LEN - 1);
    e->name[MAX_NAME_LEN - 1] = '\0';
    e->level   = level;
    e->max_hp  = MAGE_BASE_HP + (level * 10);
    e->hp      = e->max_hp;
    e->data    = data;
    e->vtable  = &mage_vtable;

    data->max_mana    = MAGE_BASE_MANA + (level * 20);
    data->mana        = data->max_mana;
    data->spell_power = MAGE_BASE_SPELL_PWR + level;
    data->mana_regen  = MAGE_MANA_REGEN;

    return e;
}
