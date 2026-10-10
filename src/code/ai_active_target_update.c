typedef struct {
    char pad[0x18];
    int kind;
    char pad2[0x6E - 0x1C];
    unsigned char active;
} Controller;

typedef struct {
    char pad[0x170];
    int state;
    char pad2[0x1D0 - 0x174];
    Controller *controller;
} Object;

extern int func_80083230(Object *);
extern void func_80086208(Object *, int);
extern void func_80088854(Object *);
extern void func_80088B18(Object *, int, float, int);
extern void func_80089008(Object *);
extern void func_80089094(Object *, int);

void func_80083464(Object *object) {
    int target;
    float effectStrength = 400.0f;

    switch (object->state) {
    case 1:
        if (object->controller->kind == 7 && object->controller->active != 0) {
            target = func_80083230(object);
            if (target != 0) {
                func_80088B18(object, target, effectStrength, 0);
                func_80089094(object, target);
                object->state = 2;
            }
        } else {
            func_80088854(object);
        }
        break;
    case 2:
        if (object->controller->kind != 7) {
            func_80086208(object, 6);
        }
        if (object->controller->active == 0) {
            func_80088854(object);
            func_80089008(object);
            object->state = 1;
        }
        break;
    }
}
