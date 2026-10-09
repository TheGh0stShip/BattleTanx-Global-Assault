/* SPAN 0x800EC1C0 */
typedef struct { int a, b, k; } Elem;
#define SWAP(x, y) { tmp.b = (x).b; tmp.a = (x).a; tmp.k = (x).k; \
                     (x).b = (y).b; (x).a = (y).a; (x).k = (y).k; \
                     (y).b = tmp.b; (y).a = tmp.a; (y).k = tmp.k; }
void func_800EBF80(Elem *arr, int n) {
    Elem tmp;
    int lo, hi, pivot;

    if (n < 2) return;
    lo = 1;
    if (n == 2) {
        if (arr[0].k > arr[1].k) {
            SWAP(arr[0], arr[1]);
        }
        return;
    }
    hi = n - 1;
    pivot = arr[0].k;
    while (lo < hi) {
        if (arr[lo].k <= pivot) {
            lo++;
        } else if (arr[hi].k >= pivot) {
            hi--;
        } else {
            SWAP(arr[lo], arr[hi]);
        }
    }
    if (arr[lo].k < pivot) {
        Elem *m = &arr[lo];
        SWAP(*m, arr[0]);
        func_800EBF80(arr, lo);
        func_800EBF80(m + 1, n - lo - 1);
    } else {
        Elem *m = &arr[lo];
        SWAP(m[-1], arr[0]);
        func_800EBF80(arr, lo - 1);
        func_800EBF80(m, n - lo);
    }
}
