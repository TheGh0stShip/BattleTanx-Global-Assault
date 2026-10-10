/* SPAN 0x800EBCA8 */
typedef struct { short position[3]; short flag; int texture; int color; } Vertex;
typedef struct { unsigned int w0, w1; } GfxWords;
typedef struct { unsigned char command; } GfxByte;
typedef union { GfxWords words; GfxByte byte; } Gfx;

extern int func_80079570(void);
extern Gfx *func_8007B190(int);

Gfx *func_800EBA98(Gfx *source, signed char *offsets) {
    int i = 0;
    Gfx *destination;
    int j;
    int count;
    Vertex *destinationVertex;
    Vertex *sourceVertex;

    destination = func_8007B190((unsigned int)func_80079570() >> 3);
    if (destination == 0) return 0;
    while (source[i].words.w0 != 0xDF000000) {
        destination[i] = source[i];
        if (source[i].byte.command == 1) {
            destinationVertex = (Vertex *)(source[i].words.w1 -
                (unsigned int)source + (unsigned int)destination);
            destination[i].words.w1 = (unsigned int)destinationVertex;
            sourceVertex = (Vertex *)source[i].words.w1;
            count = (source[i].words.w0 >> 12) & 0xFF;
            for (j = 0; j < count; j++) {
                destinationVertex[j] = sourceVertex[j];
                switch (destinationVertex[j].position[1]) {
                case 144:
                    destinationVertex[j].position[0] += offsets[0] * 3;
                    destinationVertex[j].position[1] += offsets[1] * 3;
                    break;
                case 288:
                    destinationVertex[j].position[0] += offsets[2] * 3;
                    destinationVertex[j].position[1] += offsets[3] * 3;
                    break;
                case 432:
                    destinationVertex[j].position[0] += offsets[4] * 3;
                    destinationVertex[j].position[1] += offsets[5] * 3;
                    break;
                case 576:
                    destinationVertex[j].position[0] += offsets[6] * 3;
                    destinationVertex[j].position[1] += offsets[7] * 3;
                    break;
                }
            }
        }
        i++;
    }
    destination[i] = source[i];
    return destination;
}
