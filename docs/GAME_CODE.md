# Game-code boundary evidence

The early game-owned region cannot inherit libultra object names or compiler
flags. Boundaries here are established from the ROM's MIPS control flow. A
split is accepted when a `jr $ra` and its delay slot are followed by another
instruction sequence and no conditional branch or absolute jump from the
preceding body crosses the proposed start. Direct callers and coherent global
or structure use provide additional evidence, but are not required because
some routines are reached through function pointers or retained despite being
unused.

Applying that rule to `0x8007A000`-`0x80080000` split the imported aggregates
at `0x8007A75C`, `0x8007A7B4`, `0x8007A9EC`, `0x8007AAA8`, `0x8007AC6C`,
`0x8007ACA4`, `0x8007ADE0`, `0x8007C718`, `0x8007D4BC`, `0x8007D704`,
`0x8007D718`, `0x8007D738`, `0x8007D760`, `0x8007D7AC`, `0x8007D7C4`,
`0x8007DD54`, `0x8007E268`, and `0x8007E778`. Address-derived names remain
in use until call sites and data types justify semantic names.

Three imported functions also included trailing non-code words. The executable
bodies end at `0x8007B01C`, `0x8007D468`, and `0x8007D498`, leaving the words
before the next established function unclaimed. Keeping these gaps explicit
prevents data or alignment from being emitted as C function bodies.

## Proposed translation-unit seams

These are layout hypotheses for future splat subdivision, not recovered source
filenames:

- `0x8007A710` begins a renderer-context and Gfx-command group immediately
  after ASCII data at `0x8007A704`-`0x8007A70F`.
- `0x8007B020` begins a distinct rendering group.
- `0x8007D33C` begins a geometry and animation-helper group.
- The explicit `__dummy` at `0x8007D694` separates the following group that
  starts at `0x8007D69C`.
- `0x8007D998` begins the node-pool subsystem associated with the base pointer
  at `0x80114680`.
- Data at `0x8007E164`-`0x8007E16F` makes `0x8007E170` a firm start for the
  following entity/path-state group.

Do not turn these proposed seams into matching claims without object or ROM
diff evidence. Game compiler flags also remain subject to matching experiments;
SDK compile settings are not evidence for these translation units.

## Compiler-constrained source matches

Some exact C sources use GCC's local register-variable extension where the
retail allocation cannot otherwise be expressed through declaration order.
The paired helpers at `0x800DF89C` and `0x800DF988`, for example, constrain
their eighth parameter to `$a0`; compiling both together then reproduces all
`0x1C0` text bytes without any normalizer rewrite. These constraints remain
source-level matching evidence and must still pass the same object/ROM diff.
