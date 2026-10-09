import importlib.util
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[1]
MODULE_PATH = ROOT / "tools" / "normalize_libgcc_asm.py"
SPEC = importlib.util.spec_from_file_location("normalize_libgcc_asm", MODULE_PATH)
assert SPEC is not None and SPEC.loader is not None
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class LibgccPipelineTests(unittest.TestCase):
    def test_udivmoddi4_return_keeps_empty_delay_slot(self) -> None:
        source = (
            "__udivmoddi4:\n"
            "\tlw\t$16,0($sp)\n"
            "\taddu\t$sp,$sp,8\n"
            "\tj\t$31\n"
            "\t.end\t__udivmoddi4\n"
        )
        normalized = MODULE.normalize(source)
        self.assertIn(
            "\taddu\t$sp,$sp,8\n\t.set\tnoreorder\n"
            "\tj\t$31\n\tnop\n\t.set\treorder\n",
            normalized,
        )

    def test_udivmoddi4_return_requires_one_fire(self) -> None:
        with self.assertRaisesRegex(RuntimeError, "fired 0 times"):
            MODULE.normalize("__udivmoddi4:\n\tnop\n")


if __name__ == "__main__":
    unittest.main()
