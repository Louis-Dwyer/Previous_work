CREATE VIEW training_data AS
SELECT
  i.user_id,
  i.item_id,
  COUNT(*) OVER (PARTITION BY i.user_id) AS user_interactions,
  AVG(i.clicked) OVER (PARTITION BY i.user_id) AS user_ctr,
  COUNT(*) OVER (PARTITION BY i.item_id) AS item_interactions,
  AVG(i.clicked) OVER (PARTITION BY i.item_id) AS item_ctr,
  it.price,
  u.account_age_days,
  i.clicked AS label
FROM interactions i
JOIN users u ON i.user_id = u.user_id
JOIN items it ON i.item_id = it.item_id;
