#!/usr/bin/env python3
"""Validate DX7 bank media before the read-only hardware CAT."""
import argparse
from pathlib import Path


def validate(path):
    data = path.read_bytes()
    if len(data) != 4104:
        return 'expected 4104 bytes'
    if data[:2] != bytes((0xF0, 0x43)) or data[-1] != 0xF7:
        return 'invalid Yamaha SysEx framing'
    if data[3:6] != bytes((9, 32, 0)):
        return 'invalid bulk format or length'
    if any(byte > 127 for byte in data[6:4103]):
        return 'non MIDI data byte'
    if (-sum(data[6:4102])) & 127 != data[4102]:
        return 'checksum mismatch'
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('root', type=Path)
    args = parser.parse_args()
    errors, count = [], 0
    for number in range(100):
        directory = args.root / str(number)
        if not directory.is_dir():
            continue
        files = sorted(p for p in directory.iterdir()
                       if p.is_file() and not p.name.startswith('.')
                       and p.suffix.lower() == '.syx')
        if len(files) != 1:
            errors.append(f'{directory}: expected exactly one .syx bank')
            continue
        path = files[0]
        error = validate(path)
        if not 1 <= len(path.stem) <= 10:
            error = 'bank filename must have 1–10 characters before extension'
        if error:
            errors.append(f'{path}: {error}')
        else:
            count += 1
    if count == 0:
        errors.append('no valid banks found')
    for error in errors:
        print(error)
    print(f'{count} valid banks; {len(errors)} errors')
    return bool(errors)


if __name__ == '__main__':
    raise SystemExit(main())
