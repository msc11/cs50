-- list the names of all people who have directed a movie that received a rating of at least 9.0.
SELECT P.name
FROM movies AS M
JOIN ratings AS R
  ON R.movie_id = M.id
JOIN directors AS D
  ON D.movie_id = M.id
JOIN people AS P
  ON P.id = D.person_id
WHERE R.rating >= 9.0;