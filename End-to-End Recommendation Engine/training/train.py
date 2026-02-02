import torch
from torch.utils.data import DataLoader
from sklearn.metrics import roc_auc_score

DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")

dataset = RecDataset("recommender.db")
loader = DataLoader(dataset, batch_size=512, shuffle=True)

model = Recommender(1000, 500).to(DEVICE)
opt = torch.optim.AdamW(model.parameters(), lr=3e-4)
loss_fn = nn.BCEWithLogitsLoss()

for epoch in range(5):
  model.train()
  for u, i, x, y in loader:
    u, i, x, y = u.to(DEVICE), i.to(DEVICE), x.to(DEVICE),
y.float().to(DEVICE)
    opt.zero_grad()
    logits = model(u, i, x)
    loss = loss_fn(logits, y)
    loss.backward()
    opt.step()

  model.eval()
  preds, labels = [], []
  with torch.no_grad():
    for u, i, x, y in loader:
      logits = model(u.to(DEVICE), i.to(DEVICE), x.to(DEVICE))
      preds.extend(torch.sigmoid(logits).cpu().tolist())
      labels.extend(y.tolist())
      
  auc = roc_auc_score(labels, preds)
  print(f"Epoch {epoch+1} | ROC-AUC: {auc:.4f}")
