-- Create database and employees table
CREATE DATABASE IF NOT EXISTS company;
USE company;

CREATE TABLE IF NOT EXISTS project (
    name VARCHAR(100) NOT NULL,
    code VARCHAR(100) NOT NULL,
    PRIMARY KEY (name, subsystem)
);

