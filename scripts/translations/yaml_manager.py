#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
YAML Manager module - handles merging new keys into translation YAML files.

Uses ruamel.yaml for round-trip parsing to preserve comments and formatting.
"""

from dataclasses import dataclass, field
from pathlib import Path
from typing import Set, Dict, List, Tuple, Optional, Any

try:
    from ruamel.yaml import YAML
    from ruamel.yaml.comments import CommentedMap

    RUAMEL_AVAILABLE = True
except ImportError:
    RUAMEL_AVAILABLE = False
    import yaml as pyyaml


@dataclass
class MergeResult:
    """Result of a merge operation."""

    keys_added: int = 0
    files_modified: int = 0
    keys_per_file: Dict[str, int] = field(default_factory=dict)


def get_yaml_instance():
    """Get a configured YAML instance."""
    if not RUAMEL_AVAILABLE:
        return None

    yaml = YAML()
    yaml.preserve_quotes = True
    yaml.indent(mapping=2, sequence=2, offset=2)
    yaml.width = 4096  # Don't wrap lines
    return yaml


def load_yaml_file(yaml_path: Path) -> Dict[str, Any]:
    """
    Load a YAML translation file.

    Args:
        yaml_path: Path to the YAML file

    Returns:
        Dict with 'locale' and 'translations' keys
    """
    if RUAMEL_AVAILABLE:
        yaml = get_yaml_instance()
        with open(yaml_path, "r", encoding="utf-8") as f:
            data = yaml.load(f)
    else:
        with open(yaml_path, "r", encoding="utf-8") as f:
            data = pyyaml.safe_load(f)

    return data or {"locale": "", "translations": {}}


def save_yaml_file(yaml_path: Path, data: Dict[str, Any]) -> None:
    """
    Save a YAML translation file, preserving formatting.

    Args:
        yaml_path: Path to the YAML file
        data: Data to save
    """
    if RUAMEL_AVAILABLE:
        yaml = get_yaml_instance()
        with open(yaml_path, "w", encoding="utf-8") as f:
            yaml.dump(data, f)
    else:
        with open(yaml_path, "w", encoding="utf-8") as f:
            pyyaml.dump(data, f, allow_unicode=True, sort_keys=True)


def merge_new_keys(
    yaml_dir: Path, new_keys: Set[str], dry_run: bool = False
) -> MergeResult:
    """
    Merge new translation keys into all YAML files.

    For English: new keys get the key itself as value
    For other languages: new keys get empty string (needs translation)

    Args:
        yaml_dir: Directory containing translation YAML files
        new_keys: Set of new keys to add
        dry_run: If True, don't modify files

    Returns:
        MergeResult with statistics
    """
    result = MergeResult()

    for yaml_path in yaml_dir.glob("*.yml"):
        data = load_yaml_file(yaml_path)
        locale = data.get("locale", yaml_path.stem)
        translations = data.get("translations", {})

        if translations is None:
            translations = {}
            data["translations"] = translations

        keys_added_to_file = 0

        for key in new_keys:
            # Only add if key doesn't exist
            if key not in translations:
                # English gets the key as value, others get empty string
                if locale == "en":
                    translations[key] = key
                else:
                    translations[key] = ""
                keys_added_to_file += 1

        if keys_added_to_file > 0:
            result.keys_added += keys_added_to_file
            result.files_modified += 1
            result.keys_per_file[yaml_path.name] = keys_added_to_file

            # Sort translations alphabetically
            sorted_translations = dict(sorted(translations.items()))
            data["translations"] = sorted_translations

            if not dry_run:
                save_yaml_file(yaml_path, data)

    return result


def merge_new_keys_with_sources(
    yaml_dir: Path,
    new_keys_with_sources: Dict[str, List[Tuple[str, int]]],
    dry_run: bool = False,
) -> MergeResult:
    """
    Merge new translation keys with source file comments.

    Args:
        yaml_dir: Directory containing translation YAML files
        new_keys_with_sources: Dict mapping keys to list of (filename, line) tuples
        dry_run: If True, don't modify files

    Returns:
        MergeResult with statistics
    """
    result = MergeResult()

    for yaml_path in yaml_dir.glob("*.yml"):
        data = load_yaml_file(yaml_path)
        locale = data.get("locale", yaml_path.stem)
        translations = data.get("translations", {})

        if translations is None:
            translations = {}
            data["translations"] = translations

        keys_added_to_file = 0

        for key, sources in new_keys_with_sources.items():
            if key not in translations:
                if locale == "en":
                    translations[key] = key
                else:
                    translations[key] = ""
                keys_added_to_file += 1

                # Add source comment if using ruamel.yaml
                if RUAMEL_AVAILABLE and isinstance(translations, CommentedMap):
                    source_str = ", ".join(f"{f}:{l}" for f, l in sources[:3])
                    if len(sources) > 3:
                        source_str += f" (+{len(sources) - 3} more)"
                    translations.yaml_set_comment_before_after_key(
                        key, before=f" Source: {source_str}"
                    )

        if keys_added_to_file > 0:
            result.keys_added += keys_added_to_file
            result.files_modified += 1
            result.keys_per_file[yaml_path.name] = keys_added_to_file

            sorted_translations = dict(sorted(translations.items()))
            data["translations"] = sorted_translations

            if not dry_run:
                save_yaml_file(yaml_path, data)

    return result
