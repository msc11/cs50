-- list all movies released in 2010 and their ratings, in descending order by rating.
-- For movies with the same rating, order them alphabetically by title.
SELECT M.title, R.rating
FROM movies AS M
JOIN ratings AS R
  ON M.id = R.movie_id
WHERE M.year = 2010
ORDER BY R.rating DESC, M.title ASC;