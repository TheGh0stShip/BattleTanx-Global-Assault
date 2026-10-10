typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    s8 values[3];
} Byte3;

typedef struct {
    Byte3 color;
    char pad3;
    Byte3 colorCopy;
    char pad7;
    Byte3 direction;
    char padB;
    char padC[4];
} Light;

typedef struct {
    Byte3 color;
    Byte3 direction;
} LightSource;

typedef struct {
    Byte3 color;
    char pad3[0x101];
    Gfx *displayList;
    void *ambient;
} AreaLight;

typedef struct {
    u8 count;
    LightSource lights[2];
    AreaLight areas[1];
} LightSet;

extern LightSet D_80236B20;

void *func_8007B190(int size);
void func_8007ACF8(Gfx *displayList);

void func_800A9D78(void) {
    int count = D_80236B20.count;
    Light *lights;
    Gfx *displayList;
    int i;

    if (count != 0) {
        lights = func_8007B190(count * 2);
        if (lights != 0) {
            displayList = func_8007B190(count * 2);
            if (displayList != 0) {
                for (i = 0; i < count; i++) {
                    u32 command = 0xDC08000A;
                    Byte3 *colors = &D_80236B20.lights[0].color;
                    Byte3 *directions = &D_80236B20.lights[0].direction;
                    Gfx *entry = &displayList[i];

                    entry->w0 = command | ((((i + 1) * 3 + 3) & 0xFF) << 8);
                    entry->w1 = (u32)&lights[i];
                    lights[i].color = *(Byte3 *)((u8 *)colors + i * 6);
                    lights[i].colorCopy = *(Byte3 *)((u8 *)colors + i * 6);
                    lights[i].direction = *(Byte3 *)((u8 *)directions + i * 6);
                }
                {
                    Gfx *end = &displayList[count];

                    end->w0 = 0xDF000000;
                    end->w1 = 0;
                }
                func_8007ACF8(displayList);
            }
        }
    }
}
