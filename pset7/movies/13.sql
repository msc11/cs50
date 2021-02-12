-- list the names of all people who starred in a movie in which Kevin Bacon also starred.
SELECT DISTINCT P.name
FROM
    (
    SELECT M.id
    FROM movies AS M
    JOIN stars AS S
      ON S.movie_id = M.id
    JOIN people AS P
      ON P.id = S.person_id
    WHERE P.name = "Kevin Bacon"
    ) as KB
JOIN stars AS S
  ON S.movie_id = KB.id
JOIN people AS P
  ON P.id = S.person_id
WHERE P.name != "Kevin Bacon";