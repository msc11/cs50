-- list the names of all people who starred in Toy Story.
SELECT P.name
FROM (movies AS M LEFT JOIN stars AS S ON M.id = S.movie_id) AS MS LEFT JOIN people AS P ON MS.person_id = P.id
WHERE M.title = "Toy Story";