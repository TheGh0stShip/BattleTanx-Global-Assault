#!/usr/bin/env python3
"""Parse the fixed-record sections of a Global Assault world bundle."""

from __future__ import annotations

import struct


POOL_LIMITS = {
    "geometry": 0x3F6EE8 - 0x3013F0,
    "state": 0x3013F0 - 0x2F8070,
    "texture": 0x2F8070 - 0x102C70,
}

GROUP = struct.Struct(">HH6h")
PLACEMENT = struct.Struct(">hhhHI")
MODEL = struct.Struct(">BBH6h")
PART = struct.Struct(">BBH")
REFERENCE = struct.Struct(">6i")

# Field offsets are established by the matching creation handlers. Names stay
# deliberately functional where the original game terminology is not proven.
# The raw span remains authoritative for bytes outside these schemas.
DEFINITION_SCHEMAS = {
    0: (("model_index", 2, "H"),),
    1: (("model_index", 2, "H"), ("height_mode", 4, "B")),
    2: (("model_index", 2, "H"),),
    3: tuple((f"model_index_{i}", 2 + i * 2, "H") for i in range(8))
    + (("variant", 18, "B"), ("selection", 19, "B")),
    4: tuple((f"model_index_{i}", 2 + i * 2, "H") for i in range(5))
    + (("selection_from_model", 12, "B"),),
    5: tuple((f"model_index_{i}", 2 + i * 2, "H") for i in range(3)),
    6: (("row", 1, "B"), ("column", 2, "B")),
    7: (("mode", 1, "B"), ("value", 2, "B")),
    8: (("first_offset", 4, "i"), ("second_offset", 8, "i")),
    10: tuple((f"model_index_{i}", 2 + i * 2, "H") for i in range(3))
    + (("variant", 8, "B"), ("selection_from_model", 9, "B")),
    11: (("model_index", 2, "H"),),
    12: (("mode", 1, "B"), ("player", 2, "B"), ("model_index", 4, "H")),
    13: (
        ("variant", 1, "B"),
        ("player", 2, "B"),
        ("effect", 3, "B"),
        ("model_index", 4, "H"),
        ("model_index_1", 6, "H"),
        ("parameter", 8, "B"),
        ("aux_model_index", 12, "H"),
        ("aux_parameter", 14, "H"),
    ),
    14: (("model_index", 2, "H"), ("height_mode", 4, "B")),
    15: (
        ("model_index", 2, "H"),
        ("model_index_1", 4, "H"),
        ("model_index_2", 6, "H"),
        ("variant", 8, "B"),
        ("selection", 9, "B"),
        ("model_index_3", 10, "H"),
    ),
    16: (("radius_x", 1, "B"), ("radius_y", 2, "B"), ("variant", 3, "B")),
    17: (("slot", 1, "B"), ("scale", 2, "H")),
    20: (("model_index", 2, "H"),),
    21: tuple((f"model_index_{i}", 2 + i * 2, "H") for i in range(5))
    + (
        ("variant", 12, "B"),
        ("conditional_offset", 16, "i"),
        ("mission_type", 20, "B"),
    ),
    22: (("model_index", 2, "H"),),
    24: tuple((f"model_index_{i}", 2 + i * 2, "H") for i in range(3))
    + (("selection", 8, "B"),),
    26: (
        ("variant", 1, "B"),
        ("first_offset", 4, "i"),
        ("second_offset", 8, "i"),
        ("parameter_0", 12, "H"),
        ("parameter_1", 14, "H"),
    )
    + tuple((f"segment_offset_{i}", 16 + i * 4, "i") for i in range(6)),
    27: (("enabled", 1, "B"),),
    28: tuple((f"model_index_{i}", 2 + i * 2, "H") for i in range(3)),
    29: (("model_index", 2, "H"),),
    30: (("mode", 1, "B"),)
    + tuple((f"bound_{i}", 2 + i * 2, "h") for i in range(6)),
    31: (
        ("team", 1, "B"),
        ("actor_model_index", 2, "H"),
        ("structure_definition_offset", 4, "i"),
    ),
    32: (("variant", 1, "B"),)
    + tuple((f"model_index_{i}", 2 + i * 2, "H") for i in range(7)),
    34: (("model_index", 2, "H"),),
    35: (
        ("model_index", 2, "H"),
        ("colour_index", 5, "B"),
        ("collision_mode", 6, "B"),
    ),
    36: (("model_index", 2, "H"), ("model_index_1", 4, "H")),
    37: tuple(
        (f"colour_{colour}_{channel}", 1 + colour * 3 + channel_index, "B")
        for colour in range(7)
        for channel_index, channel in enumerate(("r", "g", "b"))
    ),
    38: (("red", 1, "B"), ("green", 2, "B"), ("blue", 3, "B")),
    39: (("condition", 1, "B"), ("flag", 2, "H"), ("target_offset", 4, "I")),
    40: (("model_index", 2, "H"),),
    42: (("model_index", 2, "H"),),
    43: (
        ("model_index", 2, "H"),
        ("height_mode", 4, "B"),
        ("collision_mode", 5, "B"),
    ),
    44: (("model_index", 2, "H"),),
    45: (("flag", 1, "B"), ("player", 2, "B")),
}

# The dispatcher's default arm returns without reading these records. Kind 41
# does invoke its placement handler, but that handler does not inspect the
# definition payload. Preserve all of their bytes without inventing layouts.
IGNORED_DEFINITION_KINDS = {9, 25, 33}
PAYLOAD_UNUSED_DEFINITION_KINDS = {41}

DEFINITION_HANDLERS = {
    0: "inline_model_create",
    1: "inline_model_collision_create",
    2: "inline_model_barrier_create",
    3: "func_800DD3A0",
    4: "func_800E2308",
    5: "func_800E1540",
    6: "func_800B03F4",
    7: "func_800D8C60",
    8: "func_800E56D0",
    9: "none",
    10: "func_800ED320",
    11: "inline_model_collision_create",
    12: "func_800E2AEC",
    13: "func_800DE930",
    14: "inline_model_collision_create",
    15: "func_800EAC30",
    16: "func_800E9B50",
    17: "inline_effect_endpoint_create",
    20: "func_800E6E5C",
    21: "func_800E9E80",
    22: "inline_model_collision_create",
    23: "__dummy",
    24: "func_800ED990",
    25: "none",
    26: "func_800EF770",
    27: "func_800F1B30",
    28: "func_800F2BC0",
    29: "func_800F33C0",
    30: "inline_bounds_or_collision_create",
    31: "func_800E80F4",
    32: "func_800F5298",
    33: "none",
    34: "inline_model_create",
    35: "inline_coloured_model_create",
    36: "func_800F7C30",
    37: "func_800B05F4",
    38: "func_800B06A8",
    39: "conditional_definition",
    40: "func_800F7EC0",
    41: "func_800F85F0",
    42: "inline_collision_create",
    43: "inline_model_collision_create",
    44: "inline_model_create",
    45: "func_800D7DE0",
}


def _decode_definition_fields(kind: int, payload: bytes) -> dict:
    fields = {}
    for name, offset, code in DEFINITION_SCHEMAS.get(kind, ()):
        value_size = struct.calcsize(">" + code)
        if offset + value_size > len(payload):
            raise WorldError(f"kind-{kind} definition is shorter than field {name}")
        fields[name] = struct.unpack_from(">" + code, payload, offset)[0]
    return fields


def _encode_definition_fields(row: dict, payload: bytes) -> bytes:
    result = bytearray(payload)
    fields = row.get("fields", {})
    schema = DEFINITION_SCHEMAS.get(row["kind"], ())
    expected = {name for name, _offset, _code in schema}
    unknown = set(fields) - expected
    if unknown:
        raise WorldError(f"unknown kind-{row['kind']} fields: {sorted(unknown)}")
    for name, offset, code in schema:
        if name in fields:
            struct.pack_into(">" + code, result, offset, fields[name])
    return bytes(result)


class WorldError(ValueError):
    """Raised when a decoded world violates its loader-established layout."""


def _records(data: bytes, start: int, end: int, record: struct.Struct):
    if (end - start) % record.size:
        raise WorldError(f"section size {end - start} is not divisible by {record.size}")
    return [record.unpack_from(data, offset) for offset in range(start, end, record.size)]


def parse_world(data: bytes) -> dict:
    if len(data) < 32:
        raise WorldError("world is shorter than its eight-offset header")
    offsets = struct.unpack_from(">8I", data)
    if offsets[0] != 32 or offsets[-1] != len(data):
        raise WorldError("world offsets do not cover the decoded payload")
    if any(left > right for left, right in zip(offsets, offsets[1:])):
        raise WorldError("world offsets are not monotonic")

    group_count = struct.unpack_from(">I", data, offsets[0])[0]
    groups_raw = _records(data, offsets[1], offsets[2], GROUP)
    if len(groups_raw) != group_count:
        raise WorldError("group count disagrees with the group section")
    placements_raw = _records(data, offsets[2], offsets[3], PLACEMENT)
    models_raw = _records(data, offsets[4], offsets[5], MODEL)
    parts_raw = _records(data, offsets[5], offsets[6], PART)
    refs_raw = _records(data, offsets[6], offsets[7], REFERENCE)

    for count, first, *_ in groups_raw:
        if first + count > len(placements_raw):
            raise WorldError("group placement range exceeds the placement section")
    for count, zero, first, *_ in models_raw:
        if zero != 0 or first + count > len(parts_raw):
            raise WorldError("model part range exceeds the part section")
    for count, zero, first in parts_raw:
        if zero != 0 or first + count > len(refs_raw):
            raise WorldError("part pool-reference range exceeds the reference section")
    for row in refs_raw:
        for index, name in enumerate(("geometry", "state", "texture")):
            offset, size = row[index * 2 : index * 2 + 2]
            if name == "texture" and (offset, size) == (-1, -1):
                continue
            if offset < 0 or size < 0 or offset + size > POOL_LIMITS[name]:
                raise WorldError(f"{name} reference exceeds its raw ROM pool")

    definition_size = offsets[4] - offsets[3]
    placement_definition_offsets = [item[4] for item in placements_raw]
    definition_offsets = set(placement_definition_offsets)
    if any(offset >= definition_size for offset in definition_offsets):
        raise WorldError("placement points outside the object-definition section")
    # Kind 39 is a conditional indirection: byte condition, u16 flag, then a
    # component-relative u32 definition offset. Follow the graph so definitions
    # reachable only through an include are not hidden from the inventory.
    pending = list(definition_offsets)
    while pending:
        start = pending.pop()
        absolute = offsets[3] + start
        if data[absolute] != 39:
            continue
        if start + 8 > definition_size:
            raise WorldError("conditional definition is truncated")
        target = struct.unpack_from(">I", data, absolute + 4)[0]
        if target >= definition_size:
            raise WorldError("conditional definition points outside its section")
        if target not in definition_offsets:
            definition_offsets.add(target)
            pending.append(target)
    definition_offsets = sorted(definition_offsets)
    first_definition = definition_offsets[0] if definition_offsets else definition_size
    definition_prefix = data[offsets[3] : offsets[3] + first_definition]
    definition_ends = definition_offsets[1:] + [definition_size]
    definitions = []
    for start, end in zip(definition_offsets, definition_ends):
        if end <= start:
            raise WorldError("object-definition offsets overlap")
        payload = data[offsets[3] + start : offsets[3] + end]
        definition = {
            "offset": start,
            "size": len(payload),
            "kind": payload[0],
            "placement_references": placement_definition_offsets.count(start),
            # Dispatcher-proven starts divide the whole section into lossless
            # spans. Some spans can still contain trailing unknown records or
            # padding until every kind's precise schema is established.
            "raw_hex": payload.hex(),
        }
        fields = _decode_definition_fields(payload[0], payload)
        if fields:
            definition["fields"] = fields
        if payload[0] in DEFINITION_HANDLERS:
            definition["handler"] = DEFINITION_HANDLERS[payload[0]]
        if payload[0] in IGNORED_DEFINITION_KINDS:
            definition["dispatch"] = "ignored"
        elif payload[0] in PAYLOAD_UNUSED_DEFINITION_KINDS:
            definition["dispatch"] = "payload_unused"
        if payload[0] == 39:
            definition.update(
                {
                    "condition": payload[1],
                    "flag": struct.unpack_from(">H", payload, 2)[0],
                    "target_offset": struct.unpack_from(">I", payload, 4)[0],
                }
            )
        definitions.append(definition)

    bounds = ("min_x", "min_y", "min_z", "max_x", "max_y", "max_z")
    groups = [
        {"placement_count": row[0], "first_placement": row[1], **dict(zip(bounds, row[2:]))}
        for row in groups_raw
    ]
    placements = [
        {"x": x, "y": y, "z": z, "yaw": yaw, "definition_offset": definition}
        for x, y, z, yaw, definition in placements_raw
    ]
    models = [
        {"part_count": row[0], "first_part": row[2], **dict(zip(bounds, row[3:]))}
        for row in models_raw
    ]
    parts = [
        {"reference_count": count, "first_reference": first}
        for count, _zero, first in parts_raw
    ]
    references = [
        {
            "geometry_offset": row[0],
            "geometry_size": row[1],
            "state_offset": row[2],
            "state_size": row[3],
            "texture_offset": row[4],
            "texture_size": row[5],
        }
        for row in refs_raw
    ]
    return {
        "offsets": list(offsets),
        "groups": groups,
        "placements": placements,
        "definition_prefix_hex": definition_prefix.hex(),
        "definitions": definitions,
        "models": models,
        "parts": parts,
        "references": references,
    }


def build_world(world: dict) -> bytes:
    """Rebuild a decoded seven-component world from ``parse_world`` JSON."""

    bounds = ("min_x", "min_y", "min_z", "max_x", "max_y", "max_z")
    groups = b"".join(
        GROUP.pack(
            row["placement_count"],
            row["first_placement"],
            *(row[name] for name in bounds),
        )
        for row in world["groups"]
    )
    placements = b"".join(
        PLACEMENT.pack(
            row["x"], row["y"], row["z"], row["yaw"], row["definition_offset"]
        )
        for row in world["placements"]
    )

    definitions = bytearray.fromhex(world.get("definition_prefix_hex", ""))
    for row in sorted(world["definitions"], key=lambda item: item["offset"]):
        if row["offset"] != len(definitions):
            raise WorldError(
                f"object-definition spans are not contiguous at 0x{len(definitions):X}"
            )
        payload = _encode_definition_fields(row, bytes.fromhex(row["raw_hex"]))
        if len(payload) != row["size"] or not payload or payload[0] != row["kind"]:
            raise WorldError(
                f"object-definition span at 0x{row['offset']:X} disagrees with metadata"
            )
        definitions.extend(payload)

    models = b"".join(
        MODEL.pack(
            row["part_count"],
            0,
            row["first_part"],
            *(row[name] for name in bounds),
        )
        for row in world["models"]
    )
    parts = b"".join(
        PART.pack(row["reference_count"], 0, row["first_reference"])
        for row in world["parts"]
    )
    references = b"".join(
        REFERENCE.pack(
            row["geometry_offset"],
            row["geometry_size"],
            row["state_offset"],
            row["state_size"],
            row["texture_offset"],
            row["texture_size"],
        )
        for row in world["references"]
    )
    components = (
        struct.pack(">I", len(world["groups"])),
        groups,
        placements,
        bytes(definitions),
        models,
        parts,
        references,
    )
    offsets = [32]
    for component in components:
        offsets.append(offsets[-1] + len(component))
    rebuilt = struct.pack(">8I", *offsets) + b"".join(components)
    parse_world(rebuilt)
    return rebuilt
