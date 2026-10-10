/* Threshold followed by the five-way secondary dispatch in func_800DFA5C.
 * jtbl_80075A18 is the address of branches within this object. */
#define TARGET(address) extern char D_##address[]
TARGET(800E1214); TARGET(800E12CC); TARGET(800E1348); TARGET(800E13D8);

struct EntryScriptThresholdDispatch {
    double threshold;
    const void *branches[5];
};

const struct EntryScriptThresholdDispatch D_80075A10 = {
    262.5,
    { D_800E1214, D_800E12CC, D_800E1348, D_800E13D8, D_800E13D8 },
};
