-- Create database and employees table
CREATE DATABASE IF NOT EXISTS company;
USE company;

CREATE TABLE IF NOT EXISTS employees (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    position VARCHAR(100) NOT NULL,
    salary DECIMAL(10,2) NOT NULL
);

-- Insert sample data
INSERT INTO employees (name, position, salary) VALUES
('John Doe', 'Software Engineer', 80000.00),
('Jane Smith', 'Data Analyst', 65000.00),
('Bob Johnson', 'Project Manager', 95000.00);