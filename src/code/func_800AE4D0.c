#include "model_draw_internal.h"

void func_800AE4D0(ModelDrawModel *model, void *arg1,
                   ModelDrawObjectTransform *object,
                   ModelDrawFixedTransform *fixed, int arg4, int arg5,
                   u8 mask) {
    int playerCount = D_80219498.nPlayers;

    if (model != 0) {
        if (mask != 0) {
            int view;

            for (view = 0; view < playerCount; view++) {
                if ((mask >> view) & 1) {
                    model_draw_object_for_view(model, arg1, object, fixed,
                                               arg4, arg5, view);
                }
            }
        }
    }
}
