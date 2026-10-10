typedef unsigned char u8;
typedef unsigned int u32;

typedef struct {
    u32 word0;
    u32 word1;
} DisplayCommand;

typedef struct {
    char pad00[4];
    DisplayCommand *display_list;
    char pad08[4];
    int unk0C;
    int unk10;
} Mesh;

typedef struct {
    Mesh *mesh;
    u8 kind;
} ModelObject;

extern u8 D_80219590[][3];
extern int D_802195A0[];

void func_800F80E8(int id, int *a, int *b, u8 *color);

static inline void apply_color(DisplayCommand *command, u8 *color) {
    int count = 0;
    u32 end_opcode = 0xDF000000;
    u32 color_opcode = 0xFB000000;
    DisplayCommand *current = command;

    do {
        u32 opcode = current->word0;

        if (opcode == end_opcode) {
            break;
        }
        if (opcode == color_opcode) {
            current->word0 = opcode;
            current->word1 =
                (color[0] << 24) | (color[1] << 16) |
                (color[2] << 8) | 0xFF;
        }
        count++;
        current++;
    } while (count < 1000);
}

void func_800B9AB0(ModelObject **object_ptr, int player) {
    unsigned long color = player * 3;
    ModelObject *object = *object_ptr;
    Mesh *mesh;
    int a;
    int b;

    color += (unsigned long)D_80219590;
    apply_color(object->mesh->display_list, (u8 *)color);
    if (object->kind == 2) {
        mesh = object->mesh;
        func_800F80E8(D_802195A0[player], &a, &b, (u8 *)color);
        if (a != 0) {
            mesh->unk10 = a;
            mesh->unk0C = b;
        }
    }
}
