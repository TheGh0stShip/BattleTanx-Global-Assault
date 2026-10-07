import importlib.util
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[1]
MODULE_PATH = ROOT / "tools" / "normalize_kmc_gcc_asm.py"
SPEC = importlib.util.spec_from_file_location("normalize_kmc_gcc_asm", MODULE_PATH)
assert SPEC is not None and SPEC.loader is not None
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class KmcPipelineTests(unittest.TestCase):
    def test_preserves_compiler_filled_delay_slot(self) -> None:
        source = (
            "\t.set\tnoreorder\n"
            "\tjal\t_bzero\n"
            "\tli\t$5,0x300\n"
            "\t.set\treorder\n"
        )
        self.assertEqual(
            (
                "\t.set\tnoreorder\n"
                "\tjal\t_bzero\n"
                "\tli\t$5,0x300\n"
                "\t.set\tnoreorder\n"
            ),
            MODULE.normalize(source),
        )

    def test_adds_nop_to_unfilled_reorder_transfer(self) -> None:
        source = "\tsb\t$4,D_80114512\n\tjal\tfunc_800A9F10\n\tlw\t$31,20($sp)\n"
        self.assertEqual(
            "\tsb\t$4,D_80114512\n\tjal\tfunc_800A9F10\n\tnop\n\tlw\t$31,20($sp)\n",
            MODULE.normalize(source),
        )


if __name__ == "__main__":
    unittest.main()
