import torch.nn as nn
class Recommender(nn.Module):
  def __init__(self, num_users, num_items):
    super().__init__()
    
    self.user_emb = nn.Embedding(num_users, 64)
    self.item_emb = nn.Embedding(num_items, 64)
    
    self.mlp = nn.Sequential(
      nn.Linear(64 * 2 + 6, 256),
      nn.ReLU(),
      nn.Linear(256, 128),
      nn.ReLU(),
      nn.Linear(128, 1)
    )

def forward(self, u, i, x):
  ue = self.user_emb(u)
  ie = self.item_emb(i)
  z = torch.cat([ue, ie, x], dim=1)
  return self.mlp(z).squeeze(1)
