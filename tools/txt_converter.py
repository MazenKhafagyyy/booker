# USAGE:
# python ./tools/txt_converter.py assets/test.txt book_data.h book_data

import sys
from pathlib import Path


def sanitize_name(name: str) -> str:
    out = []
    for ch in name:
        if ch.isalnum():
            out.append(ch.lower())
        else:
            out.append("_")
    s = "".join(out).strip("_")
    while "__" in s:
        s = s.replace("__", "_")
    if not s:
        s = "embedded_text"
    if s[0].isdigit():
        s = "_" + s
    return s


def normalize_text_bytes(data: bytes) -> bytes:
    # Remove UTF-8 BOM if present
    if data.startswith(b"\xEF\xBB\xBF"):
        data = data[3:]

    # Normalize line endings
    data = data.replace(b"\r\n", b"\n")
    data = data.replace(b"\r", b"\n")

    return data


def main():
    if len(sys.argv) != 4:
        print("Usage: python txt_converter.py <input.txt> <output.h> <symbol_name>")
        sys.exit(1)

    input_path = Path(sys.argv[1])
    output_path = Path(sys.argv[2])
    symbol_name = sanitize_name(sys.argv[3])

    if not input_path.exists():
        print(f"Error: input file not found: {input_path}")
        sys.exit(1)

    data = input_path.read_bytes()
    data = normalize_text_bytes(data)

    guard = f"{symbol_name.upper()}_H"

    lines = []
    lines.append(f"#ifndef {guard}")
    lines.append(f"#define {guard}")
    lines.append("")
    lines.append("#include <stddef.h>")
    lines.append("#include <stdint.h>")
    lines.append("")
    lines.append(f"static const uint8_t {symbol_name}[] = {{")

    if len(data) == 0:
        lines.append("    0x00,")
    else:
        for i in range(0, len(data), 12):
            chunk = data[i:i + 12]
            hex_bytes = ", ".join(f"0x{b:02X}" for b in chunk)
            lines.append(f"    {hex_bytes},")

    lines.append("};")
    lines.append("")
    lines.append(f"static const size_t {symbol_name}_len = {len(data)};")
    lines.append("")
    lines.append(f"#endif /* {guard} */")
    lines.append("")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text("\n".join(lines), encoding="utf-8")

    print(f"Generated {output_path} from {input_path}")
    print(f"Symbol: {symbol_name}")
    print(f"Bytes: {len(data)}")


if __name__ == "__main__":
    main()