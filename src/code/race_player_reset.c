extern signed char D_80117EB0;
extern int D_8011DDF8, D_8011DE18, D_8011DE38, D_8011DE58, D_8011DE68;
extern int D_8011DE00, D_8011DE20, D_8011DE40, D_8011DE5C, D_8011DE6C;
extern int D_8011DE08, D_8011DE28, D_8011DE48, D_8011DE60, D_8011DE70;
extern int D_8011DE10, D_8011DE30, D_8011DE50, D_8011DE64, D_8011DE74;
extern int D_803A5FF4[];
void func_800CA1A8(unsigned short idx) {
    if (idx < D_80117EB0) {
        switch (idx) {
        case 1:
            D_8011DE00 = 0; D_8011DE20 = 0; D_8011DE40 = 0; D_8011DE5C = 0; D_8011DE6C = 0;
            break;
        case 2:
            D_8011DE08 = 0; D_8011DE28 = 0; D_8011DE48 = 0; D_8011DE60 = 0; D_8011DE70 = 0;
            break;
        case 3:
            D_8011DE10 = 0; D_8011DE30 = 0; D_8011DE50 = 0; D_8011DE64 = 0; D_8011DE74 = 0;
            break;
        case 0:
        default:
            D_8011DDF8 = 0; D_8011DE18 = 0; D_8011DE38 = 0; D_8011DE58 = 0; D_8011DE68 = 0;
            break;
        }
        D_803A5FF4[idx * 4] = 0;
    }
}
