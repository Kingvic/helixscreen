#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
Obsolete module - detects and handles unused translation keys.
"""

from pathlib import Path
from typing import Set, Dict, Any

from .extractor import extract_strings_from_directory, extract_strings_from_cpp_directory
from .yaml_manager import load_yaml_file, save_yaml_file


def find_obsolete_keys(
    xml_dir: Path, yaml_dir: Path, base_locale: str = "en", cpp_dir: Path = None
) -> Set[str]:
    """
    Find translation keys that are not used in any XML or C++ file.

    Args:
        xml_dir: Directory containing XML files to scan
        yaml_dir: Directory containing translation YAML files
        base_locale: The base language to check keys from
        cpp_dir: Optional directory containing C++ source files

    Returns:
        Set of obsolete key names
    """
    # Extract all strings used in XML
    used_strings = extract_strings_from_directory(xml_dir, recursive=True)

    # Also extract from C++ if directory provided
    if cpp_dir and cpp_dir.exists():
        cpp_strings = extract_strings_from_cpp_directory(cpp_dir, recursive=True)
        used_strings.update(cpp_strings)

    # Get all keys from base locale YAML
    base_path = yaml_dir / f"{base_locale}.yml"
    if not base_path.exists():
        return set()

    base_data = load_yaml_file(base_path)
    base_translations = base_data.get("translations", {})

    if not base_translations:
        return set()

    # Find keys in YAML that aren't used in XML or C++
    yaml_keys = set(base_translations.keys())
    obsolete = yaml_keys - used_strings

    return obsolete


def report_obsolete_keys(obsolete_keys: Set[str]) -> None:
    """
    Print a report of obsolete keys.

    Args:
        obsolete_keys: Set of obsolete key names
    """
    if not obsolete_keys:
        print("No obsolete keys found.")
        return

    print(f"Found {len(obsolete_keys)} obsolete keys:")
    print()

    for key in sorted(obsolete_keys):
        print(f"  - {key}")


def mark_obsolete_keys(
    yaml_dir: Path, obsolete_keys: Set[str], dry_run: bool = False
) -> int:
    """
    Mark obsolete keys with a DEPRECATED comment in YAML files.

    Args:
        yaml_dir: Directory containing translation YAML files
        obsolete_keys: Set of keys to mark
        dry_run: If True, don't modify files

    Returns:
        Number of keys marked
    """
    if not obsolete_keys:
        return 0

    marked = 0

    for yaml_path in yaml_dir.glob("*.yml"):
        data = load_yaml_file(yaml_path)
        translations = data.get("translations", {})

        if not translations:
            continue

        modified = False

        # For each obsolete key, add DEPRECATED prefix to value
        for key in obsolete_keys:
            if key in translations:
                value = translations[key]
                if not str(value).startswith("[DEPRECATED]"):
                    translations[key] = f"[DEPRECATED] {value}"
                    modified = True
                    marked += 1

        if modified and not dry_run:
            save_yaml_file(yaml_path, data)

    return marked


def delete_obsolete_keys(
    yaml_dir: Path, obsolete_keys: Set[str], dry_run: bool = False
) -> int:
    """
    Delete obsolete keys from all YAML files.

    Args:
        yaml_dir: Directory containing translation YAML files
        obsolete_keys: Set of keys to delete
        dry_run: If True, don't modify files

    Returns:
        Number of keys deleted
    """
    if not obsolete_keys:
        return 0

    deleted = 0

    for yaml_path in yaml_dir.glob("*.yml"):
        data = load_yaml_file(yaml_path)
        translations = data.get("translations", {})

        if not translations:
            continue

        modified = False

        for key in list(translations.keys()):
            if key in obsolete_keys:
                del translations[key]
                modified = True
                deleted += 1

        if modified and not dry_run:
            save_yaml_file(yaml_path, data)

    return deleted
