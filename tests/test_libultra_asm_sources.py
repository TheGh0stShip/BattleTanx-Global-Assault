import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class LibultraAssemblySourceTests(unittest.TestCase):
    def test_exception_handler_uses_authentic_source_unit(self):
        source = (ROOT / "src/libultra/exceptasm.s").read_text()
        self.assertIn("LEAF(__osException)", source)
        self.assertIn("LEAF(__osDispatchThread)", source)
        self.assertIn("__osIntOffTable:", source)
        self.assertIn("__osIntTable:", source)
        self.assertNotIn(".incbin", source)
        self.assertNotIn("GLOBAL_ASM", source)

        splat = (ROOT / "config/us/splat.yaml").read_text()
        self.assertIn("[0x94FA0, hasm, libultra/exceptasm]", splat)
        rows = (ROOT / "config/us/unit_rodata.tsv").read_text()
        self.assertIn("libultra/exceptasm\t0x80077880\t0x44", rows)
        data = (ROOT / "config/us/unit_data.tsv").read_text()
        self.assertIn("libultra/exceptasm\t0x80126E30\t0x20", data)

    def test_build_pins_ultralib_and_marks_ido_mips3_as_o32(self):
        bootstrap = (ROOT / "tools/bootstrap_ultralib.sh").read_text()
        self.assertIn("e24c836796df4bf520ff8b11a5c9d2cea3a66cbd", bootstrap)
        build = (ROOT / "tools/build_code.sh").read_text()
        self.assertIn("exceptasm.s.o --mips3-32", build)
        placement = (ROOT / "tools/place_unit_rodata.py").read_text()
        self.assertIn('unit == "libultra/exceptasm"', placement)


if __name__ == "__main__":
    unittest.main()
