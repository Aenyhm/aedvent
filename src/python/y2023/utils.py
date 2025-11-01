import pathlib
import statistics
import time

DATA_FOLDER = "../../data/2023/"


def get_file_path(relative_file_path: str) -> pathlib.Path:
    return pathlib.Path(DATA_FOLDER).joinpath(relative_file_path)


def get_file_content(file_path: str) -> str:
    return get_file_path(file_path).read_text().rstrip("\n")


def file_to_lines(file_path: str) -> list[str]:
    return get_file_content(file_path).split("\n")


def profile(func, runs=10):
    def wrapper(*args, **kwargs):
        times = []
        result = None
        for _ in range(runs):
            start = time.perf_counter()
            result = func(*args, **kwargs)
            end = time.perf_counter()
            times.append(end - start)

        mean = statistics.mean(times)
        print(f"{func.__name__}: {round(mean * 10_000) / 10} ms")
        return result

    return wrapper
