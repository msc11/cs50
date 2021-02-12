-- list the names of all people who starred in a movie released in 2004, ordered by birth year.
SELECT P.name
FROM (movies AS M LEFT JOIN stars AS S ON M.id = S.movie_id) AS MS LEFT JOIN people AS P ON MS.person_id = P.id
WHERE M.year = 2004
ORDER BY P.birth ASC;