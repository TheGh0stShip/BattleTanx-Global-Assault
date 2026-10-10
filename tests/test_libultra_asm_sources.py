import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class LibultraAssemblySourceTests(unittest.TestCase):
    def test_boot_exception_monitor_uses_readable_source_units(self):
        units = {
            "exception_preamble": "__bootExceptionPreamble",
            "exception_handler": "__bootException",
            "exception_vector_install": "func_8007919C",
            "exception_context_save": "func_80079260",
        }
        splat = (ROOT / "config/us/splat.yaml").read_text()
        build = (ROOT / "tools/build_code.sh").read_text()

        for unit, entry in units.items():
            source = (ROOT / f"src/boot/{unit}.s").read_text()
            self.assertIn(entry, source)
            self.assertNotIn(".incbin", source)
            self.assertNotIn("GLOBAL_ASM", source)
            self.assertIn(f"hasm, boot/{unit}]", splat)
            self.assertIn(f"boot/{unit}.s.o", build)

        handler = (ROOT / "src/boot/exception_handler.s").read_text()
        self.assertIn(".word   D_80079208", handler)
        self.assertIn("bgezal  $zero, func_8007919C", handler)
        context = (ROOT / "src/boot/exception_context_save.s").read_text()
        self.assertIn("SAVE_CP0 $31, 0x210", context)
        self.assertIn("SAVE_ODD_FPR $f31, 0x310", context)

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
        self.assertIn('"libultra/exceptasm", "libultra/setintmask"', placement)

    def test_handwritten_libultra_units_use_authentic_sources(self):
        units = {
            "bcopy", "bzero", "getcount", "getsr", "interrupt",
            "invaldcache", "invalicache", "maptlbrdb", "setfpccsr",
            "setintmask", "setsr", "sqrtf", "setcompare", "probetlb",
            "writebackdcache", "writebackdcacheall",
        }
        splat = (ROOT / "config/us/splat.yaml").read_text()
        build = (ROOT / "tools/build_code.sh").read_text()
        for unit in units:
            source = (ROOT / f"src/libultra/{unit}.s").read_text()
            self.assertNotIn(".incbin", source)
            self.assertIn(f"hasm, libultra/{unit}]", splat)
            self.assertIn(f"{unit} ", build)

        rows = (ROOT / "config/us/unit_rodata.tsv").read_text()
        self.assertIn("libultra/setintmask\t0x80077A00\t0x80", rows)
        self.assertIn("-D__osGetSR=__osGetSR_80105DA0", build)


if __name__ == "__main__":
    unittest.main()
