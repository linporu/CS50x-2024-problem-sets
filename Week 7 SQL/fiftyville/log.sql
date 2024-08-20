-- Keep a log of any SQL queries you execute as you solve the mystery.


--.schema
CREATE TABLE crime_scene_reports (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    street TEXT,
    description TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE interviews (
    id INTEGER,
    name TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    transcript TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE atm_transactions (
    id INTEGER,
    account_number INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    atm_location TEXT,
    transaction_type TEXT,
    amount INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE bank_accounts (
    account_number INTEGER,
    person_id INTEGER,
    creation_year INTEGER,
    FOREIGN KEY(person_id) REFERENCES people(id)
);
CREATE TABLE airports (
    id INTEGER,
    abbreviation TEXT,
    full_name TEXT,
    city TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE flights (
    id INTEGER,
    origin_airport_id INTEGER,
    destination_airport_id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
    FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
);
CREATE TABLE passengers (
    flight_id INTEGER,
    passport_number INTEGER,
    seat TEXT,
    FOREIGN KEY(flight_id) REFERENCES flights(id)
);
CREATE TABLE phone_calls (
    id INTEGER,
    caller TEXT,
    receiver TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    duration INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE people (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number INTEGER,
    license_plate TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE bakery_security_logs (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    activity TEXT,
    license_plate TEXT,
    PRIMARY KEY(id)
);

-- Check crime_scene_reports
SELECT description
FROM crime_scene_reports
WHERE month = 7
AND day = 28
AND street = 'Humphrey Street';


-- Check interviews
SELECT transcript
FROM interviews
WHERE month = 7
AND day = 28
AND transcript LIKE '%bakery%';


-- INDEX
CREATE INDEX name_index ON people(name);
CREATE INDEX phone_number_index ON people(phone_number);
CREATE INDEX license_plate_index ON people(license_plate);
CREATE INDEX passport_number_index ON people(passport_number);
CREATE INDEX bank_accounts_index ON bank_accounts(account_number)


-- The THIEF is: Bruce
SELECT DISTINCT p1.name
FROM people p1
JOIN bakery_security_logs bakery ON p1.license_plate = bakery.license_plate
JOIN phone_calls phone ON p1.phone_number = phone.caller
JOIN people p2 ON p2.phone_number = phone.receiver
JOIN bank_accounts bank ON p1.id = bank.person_id
JOIN atm_transactions atm ON atm.account_number = bank.account_number
JOIN passengers pass ON p1.passport_number = pass.passport_number
JOIN flights f ON f.id = pass.flight_id
WHERE  (bakery.month = 7 -- 擁有案發十分鐘內離場的車牌號碼
    AND bakery.day = 28
    AND bakery.activity = 'exit'
    AND bakery.hour = 10
    AND bakery.minute >= 15
    AND bakery.minute <= 25
)
AND    (atm.month = 7 -- 案發當天在 Leggett Street ATM 領錢
    AND atm.day = 28
    AND atm.atm_location = 'Leggett Street'
    AND atm.transaction_type = 'withdraw'
)
AND (phone.month = 7 -- 案發當天打了低於一分鐘的電話
    AND phone.day = 28
    AND phone.duration < 60
)
AND ( -- 護照號碼出現在隔天一早離開 Fiftyville 飛機的乘客名單中
    pass.flight_id IN (
        SELECT f.id
        FROM flights f
        JOIN airports a ON a.id = f.origin_airport_id
        WHERE city = 'Fiftyville'
        AND f.year = 2023
        AND f.month = 7
        AND f.day = 29
        AND f.hour = (
            SELECT MIN(hour)
            FROM flights
            WHERE month = 7 AND day = 29
        )
    )
);


-- The city the thief ESCAPED TO: New York City
SELECT ad.city
FROM airports AS ad
JOIN flights AS f ON f.destination_airport_id = ad.id
JOIN airports AS ao ON f.origin_airport_id = ao.id
WHERE ao.city = 'Fiftyville' -- 由 Fiftyville 啟程
AND (f.month = 7 -- 7/29 最早班機
    AND f.day = 29
    AND f.hour = (
        SELECT MIN(hour)
        FROM flights
        WHERE month = 7 AND day = 29
    )
);


-- The ACCOMPLICE is: Robin
SELECT DISTINCT p2.name
FROM people p2
JOIN phone_calls phone ON phone.receiver = p2.phone_number
JOIN people p1 ON p1.phone_number = phone.caller
JOIN passengers pass ON p1.passport_number = pass.passport_number
JOIN flights f ON f.id = pass.flight_id
WHERE p1.name = 'Bruce' -- 和 Bruce 通話
AND (
    phone.month = 7 -- 案發當天打了低於一分鐘的電話
    AND phone.day = 28
    AND phone.duration < 60
);
