# config.py
DB_PATH = "recommender.db"
NUM_USERS = 1000
NUM_ITEMS = 500
NUM_INTERACTIONS = 100000
BATCH_SIZE = 512
LR = 3e-4
EPOCHS = 5
EMBED_DIM = 64
HIDDEN_DIMS = [256, 128]
DEVICE = "cuda" if __import__('torch').cuda.is_available() else "cpu"
