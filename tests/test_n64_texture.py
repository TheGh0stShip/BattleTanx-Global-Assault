import struct
import unittest
import zlib

from tools.n64_texture import TextureError, decode_texture, encode_png_rgba, rgba16


class N64TextureTests(unittest.TestCase):
    def test_rgba16(self):
        self.assertEqual(rgba16(0xF801), bytes((255, 0, 0, 255)))
        self.assertEqual(rgba16(0x07C0), bytes((0, 255, 0, 0)))

    def test_ci4_palette_precedes_indices(self):
        palette = struct.pack(">16H", 0xF801, 0x07C1, *([0] * 14))
        self.assertEqual(
            decode_texture(palette + b"\x01", 2, 0, 2, 1),
            bytes((255, 0, 0, 255, 0, 255, 0, 255)),
        )

    def test_ci4_ignores_complete_storage_rows_beyond_visible_height(self):
        palette = struct.pack(">16H", 0xF801, *([0] * 15))
        self.assertEqual(
            decode_texture(palette + b"\x00\x00", 2, 0, 2, 1),
            bytes((255, 0, 0, 255)) * 2,
        )

    def test_ci8_palette_precedes_indices(self):
        palette = struct.pack(">256H", 0x003F, 0xFFFF, *([0] * 254))
        self.assertEqual(
            decode_texture(palette + b"\x00\x01", 2, 1, 2, 1),
            bytes((0, 0, 255, 255, 255, 255, 255, 255)),
        )

    def test_intensity_alpha_formats(self):
        self.assertEqual(decode_texture(b"\x8F", 3, 1, 1, 1), bytes((136, 136, 136, 255)))
        self.assertEqual(decode_texture(b"\x12\x34", 3, 2, 1, 1), bytes((18, 18, 18, 52)))

    def test_intensity_formats(self):
        self.assertEqual(
            decode_texture(bytes((0x1F,)), 4, 0, 2, 1),
            bytes((0x11, 0x11, 0x11, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF)),
        )
        self.assertEqual(
            decode_texture(bytes((0x12, 0xA0)), 4, 1, 2, 1),
            bytes((0x12, 0x12, 0x12, 0xFF, 0xA0, 0xA0, 0xA0, 0xFF)),
        )

    def test_rgba32(self):
        pixel = bytes((1, 2, 3, 4))
        self.assertEqual(decode_texture(pixel, 0, 3, 1, 1), pixel)

    def test_rejects_wrong_size(self):
        with self.assertRaises(TextureError):
            decode_texture(b"\0", 0, 2, 1, 1)

    def test_png_is_rgba_and_contains_exact_scanline(self):
        pixels = bytes((1, 2, 3, 4, 5, 6, 7, 8))
        png = encode_png_rgba(2, 1, pixels)
        self.assertTrue(png.startswith(b"\x89PNG\r\n\x1a\n"))
        length = struct.unpack_from(">I", png, 8)[0]
        self.assertEqual(png[12:16], b"IHDR")
        self.assertEqual(struct.unpack(">II", png[16:24]), (2, 1))
        offset = 8 + 12 + length
        idat_length = struct.unpack_from(">I", png, offset)[0]
        self.assertEqual(png[offset + 4 : offset + 8], b"IDAT")
        self.assertEqual(zlib.decompress(png[offset + 8 : offset + 8 + idat_length]), b"\0" + pixels)


if __name__ == "__main__":
    unittest.main()
