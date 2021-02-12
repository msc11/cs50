-- list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.
SELECT title
FROM (SELECT M.id, M.title
    FROM movies AS M
    JOIN stars AS S
      ON S.movie_id = M.id
    JOIN people AS P
      ON P.id = S.person_id
    WHERE P.name = "Johnny Depp") as JD
JOIN stars AS S
  ON S.movie_id = JD.id
JOIN people AS P
  ON P.id = S.person_id
WHERE P.name = "Helena Bonham Carter";