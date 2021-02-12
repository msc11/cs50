-- Keep a log of any SQL queries you execute as you solve the mystery.

-- look at the crime report of July 28, 2020 regarding Chamberlin Street.
SELECT id, description FROM crime_scene_reports WHERE year = 2020 AND month = 7 AND day = 28 AND street = "Chamberlin Street";

-- look at the interview records from July 28, 2020
SELECT name, transcript FROM interviews WHERE year = 2020 AND month = 7 AND day = 28;
-- The three witnesses are: Ruth, Eugene, Raymond


-- 1. Following the clue from Eugene, search about ATM on Fifer Street before the crime time that day
SELECT id, account_number, amount FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type="withdraw"; -- 8 transactions
-- look up people list using the bank accounts list regarding the transactions
SELECT *
FROM bank_accounts AS B
JOIN people AS P
  ON P.id = B.person_id
WHERE B.account_number IN (28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199);
--> So, Ernest, Russell, Roy, Bobby, Elizabeth, Danielle, Madison and Victoria are the culprits


-- 2. Following the clue from Ruth, look at the courthouse security log of the day at 10:00-11:00am since witnesses mentioned about the courthouse and the time
SELECT * FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND activity = "exit";
-- look up people using the license plates information
SELECT P.id, P.name, C.minute
FROM courthouse_security_logs AS C
JOIN people AS P
  ON P.license_plate = C.license_plate
WHERE C.year = 2020 AND C.month = 7 AND C.day = 28 AND C.hour = 10 AND C.activity = "exit"
ORDER BY minute;
--> So, Ernest (18m), Danielle (20m), Elizabeth (23m), Russell (23m), and Madison (35m) are culprits


-- 3. Following Raymond's clue, look at phone call records of that day
SELECT caller, receiver, duration FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60;
-- find the people using the caller/receiver information
SELECT P1.name caller_name, P2.name receiver_name
FROM phone_calls AS PC
JOIN people AS P1
  ON PC.caller = P1.phone_number
JOIN people AS P2
  ON PC.receiver = P2.phone_number
WHERE PC.year = 2020 AND PC.month = 7 AND PC.day = 28 AND PC.duration < 60;
--> So, (Ernest, Berthold), (Russell, Philip), or (Madison, James) are culprits pairs.


-- 4. Following the clue from Raymond, look at flights on the earliest flight next day from Fiftyville
SELECT flights.id, destination_airport_id
FROM flights
JOIN airports
  ON airports.id = flights.origin_airport_id
WHERE airports.city ="Fiftyville" AND year = 2020 AND month = 7 AND day = 29
ORDER BY hour ASC, minute ASC
LIMIT 1;
-- the flight id is 36 and destination airport id is 4
SELECT * FROM passengers WHERE flight_id = 36; -- there were 8 passengers
SELECT * FROM airports WHERE id = 4; -- it went Heathrow Airport, London
-- look up people list using the passengers info
SELECT P.id, P.name
FROM passengers as Pa
JOIN people as P
  ON P.passport_number = Pa.passport_number
WHERE Pa.flight_id = 36;
--> So, Ernest and Madison are culprits


--> Considering the time Ernest and Madison left in the parking lot, Ernest is the thief and Berthold is the accomplice
