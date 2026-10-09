import struct
import unittest

from tools.btga_world import WorldError, parse_world


def sample_world():
    components = [
        struct.pack(">I", 1),
        struct.pack(">HH6h", 1, 0, -1, -2, -3, 4, 5, 6),
        struct.pack(">hhhHI", 10, 20, 30, 0x4000, 0),
        b"\x25\x01\x02\x03",
        struct.pack(">BBH6h", 1, 0, 0, -7, -8, -9, 10, 11, 12),
        struct.pack(">BBH", 1, 0, 0),
        struct.pack(">6i", 100, 8, 200, 16, -1, 0),
    ]
    offsets = [32]
    for component in components:
        offsets.append(offsets[-1] + len(component))
    return struct.pack(">8I", *offsets) + b"".join(components)


class BtgaWorldTests(unittest.TestCase):
    def test_parses_every_fixed_record_family(self):
        world = parse_world(sample_world())
        self.assertEqual(world["groups"][0]["min_z"], -3)
        self.assertEqual(world["placements"][0], {
            "x": 10, "y": 20, "z": 30, "yaw": 0x4000, "definition_offset": 0
        })
        self.assertEqual(world["definitions"][0], {"offset": 0, "size": 4, "kind": 0x25})
        self.assertEqual(world["models"][0]["max_y"], 11)
        self.assertEqual(world["parts"][0]["reference_count"], 1)
        self.assertEqual(world["references"][0]["texture_offset"], -1)

    def test_rejects_out_of_range_definition(self):
        data = bytearray(sample_world())
        offsets = struct.unpack_from(">8I", data)
        struct.pack_into(">I", data, offsets[2] + 8, 4)
        with self.assertRaises(WorldError):
            parse_world(data)

    def test_rejects_nonzero_reserved_record_byte(self):
        data = bytearray(sample_world())
        offsets = struct.unpack_from(">8I", data)
        data[offsets[4] + 1] = 1
        with self.assertRaises(WorldError):
            parse_world(data)


if __name__ == "__main__":
    unittest.main()
