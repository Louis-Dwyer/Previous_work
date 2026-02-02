import torch
from torch.utils.data import Dataset
import sqlite3

class RecDataset(Dataset):
  def __init__(self, db):
    conn = sqlite3.connect(db)
    rows = conn.execute("SELECT * FROM training_data").fetchall()
    conn.close()
    
    self.users = torch.tensor([r[0] for r in rows])
    self.items = torch.tensor([r[1] for r in rows])
    self.numeric = torch.tensor([
      [r[2], r[3], r[4], r[5], r[6], r[7]] for r in rows
    ], dtype=torch.float32)
    self.labels = torch.tensor([r[8] for r in rows])
  
  def __len__(self):
    return len(self.labels)
    
  def __getitem__(self, idx):
    return self.users[idx], self.items[idx], self.numeric[idx],
self.labels[idx]
