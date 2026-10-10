import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "rsp/n_aspMain.s"


class RspAudioMicrocodeTests(unittest.TestCase):
    def test_audio_microcode_is_symbolic_source(self):
        text = SOURCE.read_text()
        self.assertIn(".rsp", text)
        self.assertIn("n_aspMainTextStart:", text)
        self.assertIn("n_aspMainTextEnd:", text)
        self.assertIn(".fill 12, 0x00", text)
        self.assertNotIn(".incbin", text)
        self.assertNotRegex(text, r"(?m)^\s*\.(?:word|dw)\b")

    def test_build_has_an_exact_output_fingerprint(self):
        build = (ROOT / "tools/build_code.sh").read_text()
        self.assertIn(
            "9a2c951186b958b5dae6abf3deb48884  build/us/rsp/n_aspMain.text.bin",
            build,
        )
        row = "rsp_microcode_text\t0x800F8DB0\t0x20C0"
        self.assertIn(row, (ROOT / "config/us/unit_data.tsv").read_text())


if __name__ == "__main__":
    unittest.main()
