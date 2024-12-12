from pathlib import Path


def get_file_content(file_path: str) -> str:
    return Path(file_path).read_text().rstrip("\n")
