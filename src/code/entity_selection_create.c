/* SPAN 0x800EDC00 */
typedef struct { float x, y, z; } Vec3f;
typedef struct { char pad[0x4D]; unsigned char flags; } Model;
typedef struct { short pad0; unsigned short entry2, entry4, entry6; unsigned char selection; } Template;
typedef struct { char pad[0xD]; unsigned char kind; char padE[2]; } Entry;
typedef struct { char pad[0x14]; Entry *entries; char pad18[0x20]; } Element;
typedef struct { int pad0; Element *elements; } Table;
typedef struct { char pad[0x14]; unsigned short value14; char pad16[0xA]; unsigned short value20; char pad22[6]; } Record;
typedef struct {
    char pad[0xC]; Model *model; void *entry10, *entry14, *entry18; Vec3f position;
    short record; unsigned short angle; unsigned char team, selection, count, byte2F; int kind;
} Object;

extern Record D_803978E0[];
extern Object *func_800A18D0(int, int);
extern void *func_800DF758(Table *, int, unsigned short);
extern Model *func_800DF558(Table *, void *, Vec3f *, unsigned short, unsigned char, int, int, int);
extern short func_800DF89C(Table *, Vec3f *, unsigned short, unsigned char, int, unsigned short, int, void *);
extern unsigned char func_800B9C68(int);
extern unsigned int func_8009D914(void);

void func_800ED990(Template *template, Table *table, Vec3f *position,
                   unsigned short angle, unsigned char team, int index) {
    Object *object = func_800A18D0(0x1A, 0x34);
    if (object != 0) {
        short record;
        unsigned char selection;
        object->entry10 = func_800DF758(table, index, template->entry2);
        object->entry18 = func_800DF758(table, index, template->entry4);
        object->entry14 = func_800DF758(table, index, template->entry6);
        object->model = func_800DF558(table, object->entry10, position, angle, team, 2, 0xF0, 1);
        record = func_800DF89C(table, position, angle, team, index, template->entry2, 0x8000, object);
        object->record = record;
        if ((unsigned short)record != 0xFFFF) {
            Record *entry = &D_803978E0[(unsigned short)record];
            entry->value20 = entry->value20 + entry->value14 + 20;
        }
        object->angle = angle;
        object->team = team;
        object->position = *position;
        object->count = func_800B9C68(***(int ***)object->entry10);
        if (object->count == 0) object->count = 1;
        selection = template->selection;
        switch (selection) {
        case 0xFE:
            object->selection = 0xFE;
            break;
        default:
            if (selection < object->count) {
                object->selection = selection;
                break;
            }
        case 0xFF:
            object->selection = func_8009D914() % object->count;
        }
        if (object->selection != 0xFE) object->model->flags |= object->selection;
        object->kind = table->elements[index].entries[template->entry4].kind;
        object->byte2F = 0;
    }
}
