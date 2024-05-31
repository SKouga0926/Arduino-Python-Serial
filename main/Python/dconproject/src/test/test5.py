import psutil

process = psutil.Process()
print(process.memory_info().rss / 1024 ** 2, "[MB]")

values = [0] * 100_000_000

print(process.memory_info().rss / 1024 ** 2, "[MB]")
