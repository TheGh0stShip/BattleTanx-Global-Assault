#!/usr/bin/env python3
"""Parse the fixed-record sections of a Global Assault world bundle."""

from __future__ import annotations

import struct


POOL_LIMITS = {
    "geometry": 0x3F6EE8 - 0x3013F0,
    "state": 0x3013F0 - 0x2F8070,
    "texture": 0x2F8070 - 0x102C70,
}


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
    groups_raw = _records(data, offsets[1], offsets[2], struct.Struct(">HH6h"))
    if len(groups_raw) != group_count:
        raise WorldError("group count disagrees with the group section")
    placements_raw = _records(data, offsets[2], offsets[3], struct.Struct(">hhhHI"))
    models_raw = _records(data, offsets[4], offsets[5], struct.Struct(">BBH6h"))
    parts_raw = _records(data, offsets[5], offsets[6], struct.Struct(">BBH"))
    refs_raw = _records(data, offsets[6], offsets[7], struct.Struct(">6i"))

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
        }
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
        "definitions": definitions,
        "models": models,
        "parts": parts,
        "references": references,
    }
