CREATE TABLE users (
  user_id INTEGER PRIMARY KEY,
  age INTEGER,
  country TEXT,
  account_age_days INTEGER
);
CREATE TABLE items (
  item_id INTEGER PRIMARY KEY,
  category TEXT,
  price REAL
);
CREATE TABLE interactions (
  interaction_id INTEGER PRIMARY KEY,
  user_id INTEGER,
  item_id INTEGER,
  timestamp INTEGER,
  clicked INTEGER,
  FOREIGN KEY (user_id) REFERENCES users(user_id),
  FOREIGN KEY (item_id) REFERENCES items(item_id)
);
