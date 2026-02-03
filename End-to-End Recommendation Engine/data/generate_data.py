import sqlite3
import random
import math

DB = "recommender.db"
NUM_USERS = 1000
NUM_ITEMS = 500
NUM_INTERACTIONS = 100_000

random.seed(42)

conn = sqlite3.connect(DB)
c = conn.cursor()

# Create tables
c.executescript(open("End-to-End Recommendation Engine\sql\schema.sql").read())

# Users
for u in range(NUM_USERS):
  age = random.randint(18, 65)
  country = random.choice(["US", "EU", "ASIA"])
  account_age = random.randint(1, 2000)
  c.execute("INSERT INTO users VALUES (?, ?, ?, ?)", (u, age, country,account_age))

# Items
item_popularity = [random.random() ** 2 for _ in range(NUM_ITEMS)]
for i in range(NUM_ITEMS):
  category = random.choice(["tech", "books", "clothes"])
  price = round(random.uniform(5, 200), 2)
  c.execute("INSERT INTO items VALUES (?, ?, ?)", (i, category, price))

# Latent biases
user_bias = [random.gauss(0, 1) for _ in range(NUM_USERS)]
item_bias = [random.gauss(0, 1) for _ in range(NUM_ITEMS)]

# Interactions
for idx in range(NUM_INTERACTIONS):
  user = random.randrange(NUM_USERS)
  item = random.choices(range(NUM_ITEMS), weights=item_popularity)[0]
  logit = -3 + user_bias[user] + item_bias[item]
  prob = 1 / (1 + math.exp(-logit))
  clicked = int(random.random() < prob)
  ts = random.randint(1_600_000_000, 1_700_000_000)
  c.execute(
  "INSERT INTO interactions VALUES (?, ?, ?, ?, ?)",
  (idx, user, item, ts, clicked)
  )
conn.commit()
conn.close()
