typedef unsigned char u8;

typedef struct {
    void *a;
    void *b;
    void *c;
} Part;

typedef struct {
    Part *parts;
    u8 count;
} Lod;

extern u8 D_802194A5;

void *func_800F1900(void *part, int transform);
void *func_8007B0E4(void *commands, int count, void *part);
void func_8007B1F0(void *a, void *c, void *b, void *arg1, int arg4,
                   void *object, void *fixed, int view, int arg3,
                   int relocated);

void func_800ADFC8(Lod *lod, void *arg1, void *object, int arg3, int arg4,
                   u8 mask, void *commands, int commandCount,
                   void *overridePart, int transform) {
    int playerCount = D_802194A5;
    int view;
    int partIndex;
    Part *part;
    void *partData;
    void *relocatedPart;

    for (partIndex = 0; partIndex < lod->count;) {
        part = &lod->parts[partIndex];
        partData = transform != 0 ? func_800F1900(part->c, transform)
                                  : part->c;
        relocatedPart = func_8007B0E4(commands, commandCount, partData);
        if (overridePart != 0) {
            for (view = 0; view < playerCount; view++) {
                if ((mask >> view) & 1) {
                    func_8007B1F0(overridePart, relocatedPart, 0, arg1, arg4,
                                  object, 0, view, arg3, 1);
                }
            }
        } else {
            for (view = 0; view < playerCount; view++) {
                if ((mask >> view) & 1) {
                    func_8007B1F0(part->a, relocatedPart, part->b, arg1, arg4,
                                  object, 0, view, arg3, 1);
                }
            }
        }
        partIndex++;
    }
}
