-- list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated.
SELECT M.title
FROM movies AS M
JOIN stars AS S
  ON S.movie_id = M.id
JOIN ratings AS R
  ON R.movie_id = M.id
JOIN people AS P
  ON P.id = S.person_id
WHERE P.name = "Chadwick Boseman"
ORDER BY R.rating DESC
LIMIT 5;