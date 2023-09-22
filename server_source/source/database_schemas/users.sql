DROP TABLE IF EXISTS users;

CREATE TABLE IF NOT EXISTS users (
    username character varying not null unique,
    password character varying not null
);

INSERT INTO users VALUES ('admin', 'md5f6fdffe48c908deb0f4c3bd36c032e72');