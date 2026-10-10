import importlib.util
from pathlib import Path
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location("verify_typed_rodata", ROOT / "tools/verify_typed_rodata.py")
MODULE = importlib.util.module_from_spec(SPEC)
assert SPEC.loader is not None
SPEC.loader.exec_module(MODULE)


class TypedRodataTests(unittest.TestCase):
    def test_vram_to_rom_mapping(self):
        self.assertEqual(0x800725DC - MODULE.VRAM_ROM_DELTA, 0x25DC)

    def test_manifest_parses_hex_fields(self):
        with tempfile.TemporaryDirectory() as directory:
            manifest = Path(directory) / "units.tsv"
            manifest.write_text("unit\taddress\tsize\nexample\t0x800725DC\t0x30\n")
            self.assertEqual(MODULE.load_manifest(manifest), [("example", 0x800725DC, 0x30)])


if __name__ == "__main__":
    unittest.main()
