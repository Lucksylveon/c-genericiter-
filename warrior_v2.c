/* ============================================================
 * warrior_v2.c — Implémentation du Guerrier
 * ============================================================ */

#include "warrior_v2.h"

static int warrior_attack(Entity *self, Entity *target) {
    (void)target;
    WarriorData *data = (WarriorData *)self->data;
    int damage = (BASE_DAMAGE + data->strength) * self->level;
    printf("  ⚔️  %s frappe violemment ! (%d dégâts)\n",
           self->name, damage);
    return damage;
}

static int warrior_defend(Entity *self, int incoming_damage) {
    (void)incoming_damage;
    WarriorData *data = (WarriorData *)self->data;
    int absorbed = data->armor + self->level;
    data->rage  += WARRIOR_RAGE_PER_HIT;
    if (data->rage > data->max_rage) data->rage = data->max_rage;
    printf("  🛡 Armure absorbe %d dégâts (rage: %d/%d)\n",
           absorbed, data->rage, data->max_rage);
    return absorbed;
}

static void warrior_special(Entity *self, Entity *target) {
    WarriorData *data = (WarriorData *)self->data;
    if (data->rage < WARRIOR_BERSERK_COST) {
        printf("  ❌ Pas assez de rage ! (%d/%d)\n",
               data->rage, WARRIOR_BERSERK_COST);
        int dmg = warrior_attack(self, target);
        entity_take_damage(target, dmg);
        return;
    }
    data->rage -= WARRIOR_BERSERK_COST;
    int damage  = (BASE_DAMAGE + data->strength)
                  * self->level * WARRIOR_BERSERK_MULT;
    printf("  💢 %s BERSERKER ! (%d dégâts, rage: %d/%d)\n",
           self->name, damage, data->rage, data->max_rage);
    entity_take_damage(target, damage);
}

static void warrior_print_stats(const Entity *self) {
    WarriorData *data = (WarriorData *)self->data;
    printf("┌─────────────────────────────┐\n");
    printf("│ ⚔️  GUERRIER : %-15s│\n", self->name);
    printf("│ Niveau      : %-14d│\n", self->level);
    printf("│ HP          : %d/%d%*s│\n", self->hp, self->max_hp,
           (int)(14 - snprintf(NULL,0,"%d/%d",self->hp,self->max_hp)),"");
    printf("│ Armure      : %-14d│\n", data->armor);
    printf("│ Force       : %-14d│\n", data->strength);
    printf("│ Rage        : %d/%d%*s│\n", data->rage, data->max_rage,
           (int)(14 - snprintf(NULL,0,"%d/%d",data->rage,data->max_rage)),"");
    printf("└─────────────────────────────┘\n");
}

static void warrior_destroy(Entity *self) {
    free(self->data);
    free(self);
}

static EntityVTable warrior_vtable = {
    .attack      = warrior_attack,
    .defend      = warrior_defend,
    .special     = warrior_special,
    .print_stats = warrior_print_stats,
    .destroy     = warrior_destroy,
};

Entity *warrior_create(const char *name, int level) {
    Entity      *e    = malloc(sizeof(Entity));
    WarriorData *data = malloc(sizeof(WarriorData));
    if (!e || !data) { free(e); free(data); return NULL; }

    strncpy(e->name, name, MAX_NAME_LEN - 1);
    e->name[MAX_NAME_LEN - 1] = '\0';
    e->level   = level;
    e->type    = ENTITY_TYPE_warrior;  /* type stocké */
    e->max_hp  = WARRIOR_BASE_HP + (level * 20);
    e->hp      = e->max_hp;
    e->data    = data;
    e->vtable  = &warrior_vtable;

    data->armor    = WARRIOR_BASE_ARMOR + level;
    data->strength = WARRIOR_BASE_STRENGTH + level;
    data->rage     = 0;
    data->max_rage = WARRIOR_MAX_RAGE;
    return e;
}
