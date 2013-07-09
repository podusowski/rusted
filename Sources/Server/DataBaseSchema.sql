CREATE TABLE users(id INT PRIMARY KEY, login TEXT, password TEXT);

CREATE TABLE objects(
    id INT PRIMARY KEY,
    type TEXT,
    class INT,
    x INT,
    y INT,
    z INT,
    owner INT,
    integrity INT,
    carbon INT,
    helium INT
);

