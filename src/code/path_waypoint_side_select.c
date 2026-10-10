/* SPAN 0x8007F774 */
/* RODATA_VRAM 0x80071150 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Node { u16 pad0, id; float key; struct Node *next; } Node;
typedef struct { Node *head; u16 pad4, count, pad8, current; } Path;
typedef struct { char pad0[0xF0]; Path path; char padFC[0xC]; int mode; } Entity;
typedef struct { u8 type; char pad1[0x13]; u8 subtype; char pad15; u16 link; } Waypoint;

extern char *D_80114680;
Waypoint *func_8007DA5C(int, u16);
u16 func_8007ED68(Entity *, int, int);
u16 func_8007D884(Node **, int);
void func_80080110(Entity *, Node *);
void func_80080818(Entity *, int);
float func_8007F774(Entity *, Node *);
void func_8007F128(Entity *);

int func_8007F59C(Entity *entity) {
    u16 current = entity->path.current;
    u16 id = current;
    Path *path = &entity->path;
    Waypoint *waypoint;
    Node *node;
    Node **link;
    int result;
    float key;

    if (id == 0 || *(int *)(D_80114680 + 0xC000) == 0) goto out;
    waypoint = func_8007DA5C(id, current);
    switch (waypoint->type) {
    case 2:
        id = func_8007ED68(entity, id, entity->mode);
        result = id;
        entity->path.current = id;
        goto check;
    case 3:
        switch (waypoint->subtype) {
        case 1: case 2: case 17: case 18:
            id = func_8007ED68(entity, current, waypoint->subtype);
            result = id;
            path->current = id;
        check:
            if (result != 0) return 1;
        out:
            return 0;
        case 0:
            id = func_8007D884(&node, 2);
            waypoint->link = id;
            node->id = current;
            path->count++;
            func_80080110(entity, node);
            func_80080818(entity, id);
            func_8007F128(entity);
            return 1;
        case 3:
            waypoint->link = func_8007D884(&node, 4);
            node->id = current;
            path->count++;
            key = func_8007F774(entity, node);
            for (link = &path->head; *link != 0; link = &(*link)->next) {
                if (key < (*link)->key) break;
            }
            node->next = *link;
            *link = node;
            node->key = key;
            func_8007F128(entity);
            return 1;
        default:
            return 1;
        }
        break;
    default:
        return 1;
    }
    return 0;
}
