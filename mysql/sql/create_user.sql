-- Create database user and grant privileges
CREATE USER 'thai'@'localhost' IDENTIFIED BY '!code123';
GRANT ALL PRIVILEGES ON company.* TO 'thai'@'localhost';
FLUSH PRIVILEGES;