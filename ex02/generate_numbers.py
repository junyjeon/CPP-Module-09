import random

numbers = random.sample(range(1, 3001), 3000)
print(" ".join(map(str, numbers)))