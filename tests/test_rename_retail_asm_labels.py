import tempfile
import unittest
from pathlib import Path

from tools.rename_retail_asm_labels import manifest_symbols, rewrite_file


class RenameRetailAsmLabelsTests(unittest.TestCase):
    def test_manifest_symbols_and_exact_definition_rewrite(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source_dir = root / "src"
            source_dir.mkdir()
            manifest = root / "units.tsv"
            manifest.write_text(
                "unit\taddress\tsize\n"
                "# ignored\n"
                "pool\t0x80000000\t0x8\n"
            )
            (source_dir / "pool.c").write_text(
                "extern int D_80000000; extern int jtbl_80000004;\n"
            )
            asm = root / "unit.s"
            asm.write_text(
                "glabel D_80000000\n"
                "  lui $v0, %hi(D_80000000)\n"
                "endlabel D_80000000\n"
                "dlabel jtbl_80000004\n"
                "glabel D_80000008\n"
            )

            symbols = manifest_symbols(manifest, source_dir)
            self.assertEqual(symbols, {"D_80000000", "jtbl_80000004"})
            self.assertTrue(rewrite_file(asm, symbols))
            self.assertEqual(
                asm.read_text(),
                "glabel __retail_D_80000000\n"
                "  lui $v0, %hi(D_80000000)\n"
                "endlabel __retail_D_80000000\n"
                "dlabel __retail_jtbl_80000004\n"
                "glabel D_80000008\n",
            )
            self.assertFalse(rewrite_file(asm, symbols))


if __name__ == "__main__":
    unittest.main()
